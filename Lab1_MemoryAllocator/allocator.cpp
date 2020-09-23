#include <iostream>
#include "allocator.h"

uint8_t Allocator::heap[HEAP_SIZE];
uint8_t* Allocator::head;

Allocator::Allocator()
{
	size_t availableMemory = HEAP_SIZE - sizeof(Header);
	addHeader((Header*)heap, availableMemory, 0, true);

	head = (uint8_t*)(heap + sizeof(Header));

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
	size_t neededSize = alignedSize;

	uint8_t* ptr = nullptr;

	size_t occupiedSize = 0;
	while (occupiedSize < HEAP_SIZE) 
	{
		Header* block = getHeader(currPtr);
		occupiedSize = occupiedSize + block->size + sizeof(Header);
		
		if (block->isAvailable && block->size >= neededSize)
		{
			cout << endl << "mem_alloc(" << size << ") -> Allocating " << alignedSize << " bytes" << endl;
			size_t availableSize = block->size;
			
			block->isAvailable = false;
			block->size = alignedSize;

			uint8_t* nextBlock = currPtr + alignedSize;
			addHeader((Header*)nextBlock, availableSize - neededSize - sizeof(Header), alignedSize, true);
			head = nextBlock + sizeof(Header);
			ptr = currPtr;
		}

		currPtr = currPtr + block->size + sizeof(Header);
	}

	if (ptr == nullptr) 
	{
		cout << "### Allocating Error! Out of memory! ###";
	}
	return ptr;
}

void* Allocator::mem_realloc(void* address, size_t size)
{
	return nullptr;
}

void Allocator::mem_free(void* address)
{
	uint8_t* currPtr = heap + sizeof(Header);

	size_t occupiedSize = 0;
	bool validAddress = false;
	int blockNum = 0;
	while (occupiedSize < HEAP_SIZE)
	{
		blockNum++;
		Header* block = getHeader(currPtr);
		occupiedSize = occupiedSize + block->size + sizeof(Header);

		validAddress = validAddress ? validAddress : currPtr == (uint8_t*)address;

		currPtr = currPtr + block->size + sizeof(Header);
	}

	if (blockNum == 1)
	{
		cout << "### Deleting Error! All available memory is free ###";
		return;
	}

	if (!validAddress)
	{
		cout << "### Deleting Error! No block with this address! ###";
		return;
	}

	currPtr = (uint8_t*)address;
	Header* blockForDeleting = getHeader(currPtr);
	blockForDeleting->isAvailable = true;

	cout << endl << "mem_free(" << (uintptr_t*)currPtr << ")" << " -> Deleting " << blockForDeleting->size << "bytes" << endl;

	if ((uint8_t*)(currPtr + blockForDeleting->size) < (uint8_t*)(heap + HEAP_SIZE))
	{
		uint8_t* nextPtr = (uint8_t*)(currPtr + blockForDeleting->size + sizeof(Header));
		Header* nextBlock = getHeader(nextPtr);

		if (nextBlock->isAvailable)
		{
			cout << "Merge deleting block with next block" << endl;
			blockForDeleting->size = nextBlock->size + nextBlock->prevSize + sizeof(Header);
		}
	}

	if ((uint8_t*)blockForDeleting != heap) 
	{
		uint8_t* prevPtr = currPtr - blockForDeleting->prevSize - sizeof(Header);
		Header* prevBlock = getHeader(prevPtr);
		if (prevBlock->isAvailable)
		{
			cout << "Merge deleting block with previous block" << endl;
			prevBlock->size = blockForDeleting->size + blockForDeleting->prevSize + sizeof(Header);
		}
	}
}

void Allocator::mem_dump()
{
	uint8_t* currPtr = heap + sizeof(Header);

	size_t occupiedSize = 0;
	int i = 0;

	cout << endl << "===== Memory Dump =====" << endl;
	while (occupiedSize < HEAP_SIZE)
	{
		i++;
		Header* block = getHeader(currPtr);
		occupiedSize = occupiedSize + block->size + sizeof(Header);

		cout << "   ~~~ Block #" << i << " ~~~" << endl;
		cout << "Address: " << (uintptr_t*)currPtr << endl;
		cout << "Size: " << block->size << " bytes" << endl;
		cout << "Available: " << (block->isAvailable ? "true" : "false") << endl;
		cout << "-----------------------" << endl;

		currPtr = currPtr + block->size + sizeof(Header);
	}
}