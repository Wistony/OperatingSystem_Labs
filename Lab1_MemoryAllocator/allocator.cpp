#include <iostream>
#include "allocator.h"

uint8_t Allocator::heap[HEAP_SIZE];
uint8_t* Allocator::head;
uint8_t* Allocator::end;


Allocator::Allocator()
{
	size_t availableMemory = HEAP_SIZE - sizeof(Header);
	addHeader((Header*)heap, availableMemory, 0, true);

	head = (uint8_t*)(heap + sizeof(Header));
	end = head + HEAP_SIZE;

	/*cout << (uintptr_t*)heap  << endl;
	cout << (uintptr_t*)head << endl;
	Header* test = getHeader(head);
	cout << test->size << endl;*/
}

void Allocator::addHeader(Header* address, size_t blockSize, size_t prevBlockSize, bool isAvailable)
{
	Header header = { blockSize,prevBlockSize,isAvailable };

	*address = header;
}

Header* Allocator::getHeader(uint8_t* currentHead)
{
	return (Header*)(currentHead - sizeof(Header));
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
{

}