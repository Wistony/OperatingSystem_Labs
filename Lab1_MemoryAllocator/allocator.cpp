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

	for (size_t i = 1; i <= totalBlocks; i++)
	{
		Header* block = getHeader(currPtr);

		if (block->isAvailable && block->size >= neededSize)
		{
			size_t availableSize = block->size;

			block->isAvailable = false;
			if ((availableSize - neededSize) > sizeof(Header))
			{
				block->size = neededSize;
				uint8_t* nextHeader = currPtr + neededSize;
				addHeader((Header*)nextHeader, availableSize - neededSize - sizeof(Header), neededSize, true);
				uint8_t* nextBlock = nextHeader + sizeof(Header);
				if (i < totalBlocks)
				{
					uint8_t* ptr = nextBlock + getHeader(nextBlock)->size + sizeof(Header);
					getHeader(ptr)->prevSize = getHeader(nextBlock)->size;
				}
			}
			else
			{
				block->size = availableSize;
			}

			ptr = currPtr;

			cout << endl << "mem_alloc(" << size << ") -> Allocating " << neededSize << " bytes, Address: " << (uintptr_t*)ptr << endl;
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

	if (totalBlocks == 1)
	{
		cout << "### Reallocating Error! All available memory is free ###";
		return NULL;
	}

	size_t blockNum = 0;
	uint8_t* currPtr = heap + sizeof(Header);
	for (size_t i = 1; i <= totalBlocks; i++)
	{
		Header* block = getHeader(currPtr);
		blockNum = currPtr == (uint8_t*)address ? i : blockNum;
		currPtr = currPtr + block->size + sizeof(Header);
	}

	if (blockNum == 0)
	{
		cout << "### Reallocating Error! No block with this address! ###";
		return NULL;
	}

	uint8_t* reallocBlock = (uint8_t*)address;
	size_t neededBlockSize = align(size);
	size_t blockCount = totalBlocks;
	uint8_t* newPtr = NULL;

	if (neededBlockSize == getHeader(reallocBlock)->size)
	{
		return reallocBlock;
	}

	uint8_t* prevBlock = NULL;
	if (blockNum > 1)
	{
		prevBlock = reallocBlock - getHeader(reallocBlock)->prevSize - sizeof(Header);
	}
	uint8_t* nextBlock = NULL;
	if (blockNum < totalBlocks)
	{
		nextBlock = reallocBlock + getHeader(reallocBlock)->size + sizeof(Header);
	}

	if (neededBlockSize < getHeader(reallocBlock)->size)
	{
		size_t availableSize = getHeader(reallocBlock)->size;

		getHeader(reallocBlock)->size = availableSize;
		if (availableSize - neededBlockSize > sizeof(Header))
		{
			getHeader(reallocBlock)->size = neededBlockSize;
			uint8_t* nextHeader = reallocBlock + neededBlockSize;
			addHeader((Header*)nextHeader, availableSize - neededBlockSize - sizeof(Header), neededBlockSize, true);
			if (blockNum + 1 < blockCount)
			{
				uint8_t* newBlock = nextHeader + sizeof(Header);
				uint8_t* ptr = newBlock + getHeader(newBlock)->size + sizeof(Header);
				getHeader(ptr)->prevSize = getHeader(nextBlock)->size;
			}
		}

		newPtr = reallocBlock;
	}
	else 
	{
		if (prevBlock && nextBlock && getHeader(prevBlock)->isAvailable && getHeader(nextBlock)->isAvailable)
		{
			size_t availableSize = getHeader(prevBlock)->size + getHeader(reallocBlock)->size + getHeader(nextBlock)->size + 2 * sizeof(Header);

			if (availableSize > neededBlockSize)
			{
				totalBlocks -= 2;
				getHeader(prevBlock)->isAvailable = false;
				for (size_t i = 0; i < getHeader(reallocBlock)->size; i++)
				{
					*(prevBlock + i) = *(reallocBlock + i);
				}
				if (availableSize - neededBlockSize > sizeof(Header))
				{
					getHeader(prevBlock)->size = neededBlockSize;
					uint8_t* nextHeader = prevBlock + neededBlockSize;
					addHeader((Header*)nextHeader, availableSize - neededBlockSize - sizeof(Header), neededBlockSize, true);
					if (blockNum + 1 < blockCount)
					{
						uint8_t* newBlock = nextHeader + sizeof(Header);
						uint8_t* ptr = newBlock + getHeader(newBlock)->size + sizeof(Header);
						getHeader(ptr)->prevSize = getHeader(nextBlock)->size;
					}
				}
				else
				{
					getHeader(prevBlock)->size = availableSize;
					if (blockNum + 1 < blockCount)
					{
						uint8_t* ptr = prevBlock + availableSize + sizeof(Header);
						getHeader(ptr)->prevSize = availableSize;
					}
				}
				newPtr = prevBlock;
			}
		}
		else if (prevBlock && getHeader(prevBlock)->isAvailable)
		{
			size_t availableSize = getHeader(prevBlock)->size + getHeader(reallocBlock)->size + sizeof(Header);
			if (availableSize > neededBlockSize)
			{
				totalBlocks--;
				getHeader(prevBlock)->isAvailable = false;
				for (size_t i = 0; i < getHeader(reallocBlock)->size; i++)
				{
					*(prevBlock + i) = *(reallocBlock + i);
				}
				if (availableSize - neededBlockSize > sizeof(Header))
				{
					getHeader(prevBlock)->size = neededBlockSize;
					uint8_t* nextHeader = prevBlock + neededBlockSize;
					addHeader((Header*)nextHeader, availableSize - neededBlockSize - sizeof(Header), neededBlockSize, true);
					if (blockNum + 1 < blockCount)
					{
						uint8_t* newBlock = nextHeader + sizeof(Header);
						uint8_t* ptr = newBlock + getHeader(newBlock)->size + sizeof(Header);
						getHeader(ptr)->prevSize = getHeader(nextBlock)->size;
					}
				}
				else
				{
					getHeader(prevBlock)->size = availableSize;
					if (blockNum + 1 < blockCount)
					{
						uint8_t* ptr = prevBlock + availableSize + sizeof(Header);
						getHeader(ptr)->prevSize = availableSize;
					}
				}

				newPtr = prevBlock;
			}
		}
		else if (nextBlock && getHeader(nextBlock)->isAvailable)
		{
			size_t availableSize = getHeader(reallocBlock)->size + getHeader(nextBlock)->size + sizeof(Header);
			if (availableSize > neededBlockSize)
			{
				totalBlocks--;
				getHeader(reallocBlock)->isAvailable = false;
				if (availableSize - neededBlockSize > sizeof(Header))
				{
					getHeader(reallocBlock)->size = neededBlockSize;
					uint8_t* nextHeader = reallocBlock + neededBlockSize;
					addHeader((Header*)nextHeader, availableSize - neededBlockSize - sizeof(Header), neededBlockSize, true);
					if (blockNum + 1 < blockCount)
					{
						uint8_t* newBlock = nextHeader + sizeof(Header);
						uint8_t* ptr = newBlock + getHeader(newBlock)->size + sizeof(Header);
						getHeader(ptr)->prevSize = getHeader(nextBlock)->size;
					}
				}
				else
				{
					getHeader(reallocBlock)->size = availableSize;
					if (blockNum + 1 < blockCount)
					{
						uint8_t* ptr = reallocBlock + availableSize + sizeof(Header);
						getHeader(ptr)->prevSize = availableSize;
					}
				}

				newPtr = reallocBlock;
			}
		}
		else 
		{
			newPtr = (uint8_t*)mem_alloc(neededBlockSize);
			if (newPtr) 
			{
				getHeader(reallocBlock)->isAvailable = true;
			}
		}
	}
	return newPtr;
}
//uint8_t* reallocBlockPtr = (uint8_t*)address;
//size_t neededBlockSize = align(size);
//uint8_t* newPtr = NULL;
//
//uint8_t* prevBlock;
//if (blockNum > 1)
//{
//	prevBlock = reallocBlockPtr - getHeader(reallocBlockPtr)->prevSize - sizeof(Header);
//}
//uint8_t* nextBlock;
//if (blockNum < totalBlocks)
//{
//	nextBlock = reallocBlockPtr + getHeader(reallocBlockPtr)->size + sizeof(Header);
//}
//
//if (neededBlockSize <= getHeader(reallocBlockPtr)->size)
//{
//	newPtr = (uint8_t*)address;
//}
//else
//{
//	newPtr = (uint8_t*)mem_alloc(neededBlockSize);
//
//	for (size_t i = 0; i < getHeader(reallocBlockPtr)->size; i++)
//	{
//		*(newPtr + i) = *(reallocBlockPtr + i);
//	}
//
//	mem_free(reallocBlockPtr);
//}
//
//return newPtr;

void Allocator::mem_free(void* address)
{
	if (totalBlocks == 1)
	{
		cout << "### Deleting Error! All available memory is free ###";
		return;
	}

	uint8_t* currPtr = heap + sizeof(Header);

	size_t blockNum = 0;
	for (size_t i = 1; i <= totalBlocks; i++) 
	{
		Header* block = getHeader(currPtr);

		blockNum = currPtr == (uint8_t*)address ? i : blockNum;
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