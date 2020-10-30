#include "allocator.h"
#include <iostream>

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
	pageHeaders[pagePtr].freeBlocksAmount = PAGE_SIZE / classSize;
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
	//size incremented, because 1 byte needed to header, which consist bool flag(determines block is free or not)
	size_t neededSize = roundToPowerOfTwo(++size);

	if (neededSize <= PAGE_SIZE / 2) 
	{
		if (classifiedPages[neededSize].empty()) 
		{
			if (!dividePageIntoBlocks(neededSize)) 
			{
				cout << "### All memory is occupied! There is no page with corresponding class! ###";
				return NULL;
			}
		}

		uint8_t* pagePtr = (uint8_t*)classifiedPages[neededSize].at(0);
		uint8_t* allocBlock = pageHeaders[pagePtr].freeBlockPtr;

		*allocBlock = false;
		pageHeaders[pagePtr].freeBlocksAmount -= 1;

		if (pageHeaders[pagePtr].freeBlocksAmount > 0) 
		{
			pageHeaders[pagePtr].freeBlockPtr = (uint8_t*)findFreeBlock(pagePtr, neededSize);
		}
		else 
		{
			vector<void*> classPagePtr = classifiedPages[neededSize];
			auto iterator = find(classPagePtr.begin(), classPagePtr.end(), pagePtr);
			classifiedPages[neededSize].erase(iterator);
		}

		vector<void*> g = classifiedPages[neededSize];

		for (int i = 0; i < g.size(); i++)
		{
			uint8_t* pagePtr = (uint8_t*)g[i];

			uint8_t* currPtr = (uint8_t*)g[i];

			int j = 0;
			while (currPtr != pagePtr + PAGE_SIZE)
			{
				cout << "Block #" << j << " address:" << (uint16_t*)currPtr << " isFree: " << (bool)*currPtr << endl;
				currPtr += neededSize;
				j++;
			}
		}
	}
	else 
	{

	}

	return NULL;
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

}