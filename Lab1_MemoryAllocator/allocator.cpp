#include <iostream>
#include "allocator.h"

uint8_t Allocator::heap[HEAP_SIZE];
uint8_t* Allocator::head;
uint8_t* Allocator::end;


Allocator::Allocator()
{
	//cout << (uintptr_t*)heap  << endl;

	size_t availableMemory = HEAP_SIZE - sizeof(Header);
	addHeader((Header*)heap, availableMemory, 0, true);

	head = (uint8_t*)heap;
	end = head + HEAP_SIZE;


	cout << "-------" << endl;
	cout << (uintptr_t*)head << endl;
	cout << (uintptr_t*)(head + sizeof(Header)) << endl;

	Header* g = (Header*)head;
	cout << g->size << endl;
}

void Allocator::addHeader(Header* address, size_t blockSize, size_t prevBlockSize, bool isAvailable)
{
	Header header = { blockSize,prevBlockSize,isAvailable };

	*address = header;
}

void* Allocator::mem_alloc(size_t size)
{
	return nullptr;
}

void* Allocator::mem_realloc(void* address, size_t size)
{
	return nullptr;
}

void Allocator::mem_free(void* address)
{

}

void Allocator::mem_dump()
{}