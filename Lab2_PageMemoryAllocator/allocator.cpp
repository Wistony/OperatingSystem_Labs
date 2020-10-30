#include "allocator.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

uint8_t Allocator::memory[MEMORY_SIZE];
const size_t Allocator::pageNum;
map<void*, PageHeader> Allocator::pageHeaders;
vector<void*> Allocator::freePages;
map<int, vector<void*>> Allocator::classifiedPages;

Allocator::Allocator() 
{
	initializePageHeaders();
	initializeFreePages();
	initializeClassifiedPages();
}

void Allocator::initializePageHeaders() 
{
	uint8_t* pagePtr = (uint8_t*)memory;

	for (int i = 0; i < pageNum; i++)
	{
		PageHeader header = { Free, 0, 0, NULL };
		pageHeaders.insert({pagePtr, header});
		pagePtr += PAGE_SIZE;
	}
}

void Allocator::initializeFreePages() 
{
	uint8_t* pagePtr = (uint8_t*)memory;

	for (int i = 0; i < pageNum; i++)
	{
		freePages.push_back(pagePtr);
		pagePtr += PAGE_SIZE;
	}
}

void Allocator:: initializeClassifiedPages()
{
	int classSize = 8;

	for (; classSize <= PAGE_SIZE / 2; classSize <<= 1)
	{
		classifiedPages.insert({ classSize, {} });
	}
}

size_t Allocator::roundToPowerOfTwo(size_t size)
{
	size_t powerOfTwo = 1;

	while (powerOfTwo < size) 
	{
		powerOfTwo <<= 1;
	}

	return powerOfTwo;
}

bool Allocator::dividePageIntoBlocks(size_t classSize) 
{
	if (freePages.empty())
	{
		return false;
	}

	uint8_t* pagePtr = (uint8_t*)freePages[0];
	freePages.erase(freePages.begin());

	uint8_t* currPtr = (uint8_t*)pagePtr;

	while (currPtr != pagePtr + PAGE_SIZE) 
	{
		*currPtr = true;
		currPtr += classSize;
	}

	pageHeaders[pagePtr].state = DividedIntoBlocks;
	pageHeaders[pagePtr].classSize = classSize;
	pageHeaders[pagePtr].blocksAmount = PAGE_SIZE / classSize;
	pageHeaders[pagePtr].freeBlockPtr = pagePtr;

	classifiedPages[classSize].push_back(pagePtr);

	return true;
}

void* Allocator::findFreeBlock(uint8_t* pagePtr, size_t blockSize)
{
	uint8_t* currPtr = pagePtr;

	while (currPtr != pagePtr + PAGE_SIZE) 
	{
		if ((bool)*currPtr) 
		{
			return currPtr;
		}

		currPtr += blockSize;
	}

	return NULL;
}

void* Allocator::mem_alloc(size_t size)
{
	void* ptr = NULL;

	if (size <= PAGE_SIZE / 2) 
	{
		//size incremented, because 1 byte needed to header, which consist bool flag(determines block is free or not)
		size_t classSize = roundToPowerOfTwo(++size);

		if (classifiedPages[classSize].empty()) 
		{
			if (!dividePageIntoBlocks(classSize))
			{
				cout << "### All memory is occupied! There is no page with corresponding class! ###";
				return NULL;
			}
		}

		uint8_t* pagePtr = (uint8_t*)classifiedPages[classSize].at(0);
		uint8_t* allocBlock = pageHeaders[pagePtr].freeBlockPtr;

		*allocBlock = false;
		pageHeaders[pagePtr].blocksAmount -= 1;

		if (pageHeaders[pagePtr].blocksAmount > 0) 
		{
			pageHeaders[pagePtr].freeBlockPtr = (uint8_t*)findFreeBlock(pagePtr, classSize);
		}
		else 
		{
			vector<void*> classPagePtr = classifiedPages[classSize];
			auto iterator = find(classPagePtr.begin(), classPagePtr.end(), pagePtr);
			classifiedPages[classSize].erase(iterator);
		}

		ptr = allocBlock;
	}
	else 
	{
		size_t remainderOfDivision = size % PAGE_SIZE;
		size_t neededNumOfPage = size / PAGE_SIZE + (remainderOfDivision == 0 ? 0 : 1);

		if (freePages.size() < neededNumOfPage)
		{
			cout << "### Not enough memory to allocate! ###";
			return NULL;
		}

		ptr = freePages[0];

		for (int i = 1; i <= neededNumOfPage; i++)
		{
			uint8_t* pagePtr = (uint8_t*)freePages[0];
			uint8_t* nextPagePtr = neededNumOfPage == i ? NULL : (uint8_t*)freePages[1];

			pageHeaders[pagePtr].state = MultiPageBlock;
			pageHeaders[pagePtr].classSize = neededNumOfPage * PAGE_SIZE;
			pageHeaders[pagePtr].blocksAmount = neededNumOfPage - i;
			pageHeaders[pagePtr].freeBlockPtr = nextPagePtr;

			freePages.erase(freePages.begin());
		}
	}

	return ptr;
}

void* Allocator::mem_realloc(void* address, size_t size)
{

	return NULL;
}

void Allocator::mem_free(void* address) 
{

}

void Allocator::mem_dump() 
{
	uint8_t* currPage = memory;

	cout << "### mem_dump ###" << endl;
	cout << "============================================" << endl;
	for (int i = 0; i < pageNum; i++)
	{
		PageHeader header = pageHeaders[currPage];

		string state;
		switch (header.state)
		{
		case Free: 
			state = "Free";
			break;
		case DividedIntoBlocks:
			state = "DividedIntoBlocks";
			break;
		case MultiPageBlock:
			state = "MultiPageBlock";
			break;
		}

		int padding = 2 + (i < 9 ? 1 : 0);
		cout << "Page" << setw(padding) << "#" << i + 1;
		cout << ". Address: " << (uint16_t*)currPage;
		cout << ". PageState: " << state;
		if (header.state == DividedIntoBlocks)
		{
			cout << ". ClassSize: " << header.classSize << endl << endl;

			uint8_t* currBlock = currPage;
			for (int j = 0; j < PAGE_SIZE / header.classSize; j++)
			{
				padding = 2 + (j < 9 ? 1 : 0);
				cout << "\tBlock" << setw(padding) << "#" << j + 1;
				cout << ". Address:" << (uint16_t*)currBlock;
				cout << ". IsFree: " << (bool)*currBlock << endl;
				currBlock += header.classSize;
			}
			cout << endl;
		}
		else if (header.state == MultiPageBlock) 
		{
			size_t numOfPage = header.classSize / PAGE_SIZE;
			cout << ". BlockSize: " << header.classSize;
			cout << ". Part#" << numOfPage - header.blocksAmount;
			cout << ". NextPage: " << (uint16_t*)header.freeBlockPtr << endl;
		}
		else 
		{
			cout << ". PageSize: " << PAGE_SIZE << endl;
		}

		currPage += PAGE_SIZE;
	}
	cout << "============================================" << endl;
}