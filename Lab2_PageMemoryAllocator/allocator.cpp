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

void* Allocator::mem_alloc(size_t size)
{
	//1 byte needed to header - bool flags(determines block is free or not)
	size += 1;

	size_t degreeOfTwo = log10(size) / log10(2);
	size_t neededSize = pow(2, degreeOfTwo + 1);

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