#include <cstdint>
using namespace std;

#define MEMORY_SIZE 4096
#define PAGE_SIZE 256

class Allocator 
{
	static uint8_t memory[MEMORY_SIZE];
	static const size_t pageNum = MEMORY_SIZE / PAGE_SIZE;

public: 
	Allocator();

	void* mem_alloc(size_t);
	void* mem_realloc(void*, size_t);
	void mem_free(void*);
	void mem_dump();
};