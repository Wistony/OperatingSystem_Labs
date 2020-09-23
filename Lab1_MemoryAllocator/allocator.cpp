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

size_t Allocator::align(size_t size)
{
	return size + ((4 - (size % 4)) % 4);
}

void* Allocator::mem_alloc(size_t size)
{
	uint8_t* currPtr = heap + sizeof(Header);
	size_t alignedSize = align(size);
	size_t neededSize = alignedSize + sizeof(Header);

	uint8_t* newPtr = nullptr;

	size_t occupiedSize = 0;
	while (occupiedSize < HEAP_SIZE) 
	{
		Header* block = getHeader(currPtr);
		occupiedSize = occupiedSize + block->size + sizeof(Header);

		if (block->isAvailable && block->size >= neededSize)
		{
			size_t availableSize = block->size;
			
			block->isAvailable = false;
			block->size = alignedSize;

			uint8_t* nextBlock = currPtr + alignedSize;
			addHeader((Header*)nextBlock, availableSize - neededSize, alignedSize, true);
			head = nextBlock + sizeof(Header);
			newPtr = head;
		}

		currPtr = currPtr + block->size + sizeof(Header);
	}
	return newPtr;
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
	uint8_t* currPtr = heap + sizeof(Header);

	size_t occupiedSize = 0;
	int i = 0;

	cout << "===== Memory Dump =====" << endl;
	while (occupiedSize < HEAP_SIZE)
	{
		i++;
		Header* block = getHeader(currPtr);
		occupiedSize = occupiedSize + block->size + sizeof(Header);

		cout << "Block#" << i << endl;
		cout << "Address: " << (uintptr_t*)currPtr << endl;
		cout << "Size: " << block->size << (block->size > 1 ? " bytes" : " byte") << endl;
		cout << "Previous Size: " << block->prevSize << endl;
		cout << "-----------------------" << endl;

		currPtr = currPtr + block->size + sizeof(Header);
	}
}