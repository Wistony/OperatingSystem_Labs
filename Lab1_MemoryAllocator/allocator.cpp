#include <iostream>
#include "allocator.h"

uint8_t Allocator::heap[HEAP_SIZE];
uint8_t* Allocator::head;
size_t Allocator::totalBlocks;

Allocator::Allocator()
{
	size_t availableMemory = HEAP_SIZE - sizeof(Header);
	addHeader((Header*)heap, availableMemory, 0, true);

	head = (uint8_t*)(heap + sizeof(Header));
	totalBlocks = 1;
}

void Allocator::addHeader(Header* address, size_t blockSize, size_t prevBlockSize, bool isAvailable)
{
	Header header = { blockSize,prevBlockSize,isAvailable };

	*address = header;
	totalBlocks++;
}

Header* Allocator::getHeader(uint8_t* ptr)
{
	return (Header*)(ptr - sizeof(Header));
}

size_t Allocator::align(size_t size)
{
	return size + ((4 - (size % 4)) % 4);
}

void* Allocator::mem_alloc(size_t size)
{
	uint8_t* currPtr = heap + sizeof(Header);
	size_t neededSize = align(size);

	uint8_t* ptr = NULL;

	for(size_t i = 1; i <= totalBlocks; i++)
	{
		Header* block = getHeader(currPtr);
		
		if (block->isAvailable && block->size >= neededSize)
		{
			size_t availableSize = block->size;
			
			block->isAvailable = false;
			if ((availableSize - neededSize) > sizeof(Header))
			{
				block->size = neededSize;
				uint8_t* nextBlock = currPtr + neededSize;
				addHeader((Header*)nextBlock, availableSize - neededSize - sizeof(Header), neededSize, true);
			}
			else
			{
				block->size = availableSize;
			}
			
			ptr = currPtr;

			cout << endl << "mem_alloc(" << size << ") -> Allocating " << neededSize << " bytes, Address^ " << (uintptr_t*)ptr << endl;
			break;
		}

		currPtr = currPtr + block->size + sizeof(Header);
	}

	if (ptr == NULL)
	{
		cout << "### Allocating Error! Out of memory! ###";
	}
	return ptr;
}

void* Allocator::mem_realloc(void* address, size_t size)
{
	if (address == NULL)
	{
		return mem_alloc(size);
	}

	bool validAddress = false;
	uint8_t* currPtr = heap + sizeof(Header);
	for (size_t i = 0; i < totalBlocks; i++)
	{
		Header* block = getHeader(currPtr);
		validAddress = validAddress ? validAddress : currPtr == (uint8_t*)address;
		currPtr = currPtr + block->size + sizeof(Header);
	}

	if (totalBlocks == 1)
	{
		cout << "### Reallocating Error! All available memory is free ###";
		return NULL;
	}

	if (!validAddress)
	{
		cout << "### Reallocating Error! No block with this address! ###";
		return NULL;
	}
	
	uint8_t* reallocBlockPtr = (uint8_t*)address;
	size_t neededBlockSize = align(size);
	uint8_t* newPtr = NULL;

	if (neededBlockSize <= getHeader(reallocBlockPtr)->size)
	{
		newPtr = (uint8_t*)address;
	}
	else
	{
		newPtr = (uint8_t*)mem_alloc(neededBlockSize);

		for(size_t i = 0; i < getHeader(reallocBlockPtr)->size; i++)
		{
			*(newPtr + i) = *(reallocBlockPtr + i);
		}

		mem_free(reallocBlockPtr);
	}
	
	return newPtr;
}

void Allocator::mem_free(void* address)
{
	if (totalBlocks == 1)
	{
		cout << "### Deleting Error! All available memory is free ###";
		return;
	}

	uint8_t* currPtr = heap + sizeof(Header);

	size_t blockNum = 0;
	for (size_t i = 0; i < totalBlocks; i++) 
	{
		Header* block = getHeader(currPtr);

		blockNum = currPtr == (uint8_t*)address ? i + 1 : blockNum;
		currPtr = currPtr + block->size + sizeof(Header);
	}

	if (blockNum == 0)
	{
		cout << "### Deleting Error! No block with this address! ###";
		return;
	}

	currPtr = (uint8_t*)address;
	Header* blockForDeleting = getHeader(currPtr);
	blockForDeleting->isAvailable = true;

	cout << endl << "mem_free(" << (uintptr_t*)currPtr << ")" << " -> Deleting " << blockForDeleting->size << " bytes" << endl;

	if (blockNum < totalBlocks)
	{
		uint8_t* nextPtr = (uint8_t*)(currPtr + blockForDeleting->size + sizeof(Header));
		Header* nextBlock = getHeader(nextPtr);

		if (nextBlock->isAvailable)
		{
			cout << "Merge deleting block with next block" << endl;
			blockForDeleting->size = nextBlock->size + nextBlock->prevSize + sizeof(Header);
			totalBlocks--;
		}
	}

	if (blockNum > 1) 
	{
		uint8_t* prevPtr = currPtr - blockForDeleting->prevSize - sizeof(Header);
		Header* prevBlock = getHeader(prevPtr);
		if (prevBlock->isAvailable)
		{
			cout << "Merge deleting block with previous block" << endl;
			prevBlock->size = blockForDeleting->size + blockForDeleting->prevSize + sizeof(Header);
			totalBlocks--;
		}
	}

	currPtr = NULL;
}

void Allocator::mem_dump()
{
	uint8_t* currPtr = heap + sizeof(Header);

	cout << endl << "===== Memory Dump =====" << endl;

	for (size_t i = 0; i < totalBlocks; i++)
	{
		Header* block = getHeader(currPtr);
		cout << "   ~~~ Block #" << i + 1 << " ~~~" << endl;
		cout << "Address: " << (uintptr_t*)currPtr << endl;

		cout << "Size: " << block->size << " bytes" << endl;
		cout << "Available: " << (block->isAvailable ? "true" : "false") << endl;
		cout << "PrevSize: " << block->prevSize << endl;
		cout << "-----------------------" << endl;

		currPtr = currPtr + block->size + sizeof(Header);
	}
}