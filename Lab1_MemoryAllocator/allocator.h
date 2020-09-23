#include <cstdint>

using namespace std;

#define HEAP_SIZE 256

//Header size is 8 bytes
struct Header
{
	uint16_t size;
	uint16_t prevSize;
	bool isAvailable;
	uint8_t alignment[3];
};

class Allocator
{
	static uint8_t heap[HEAP_SIZE];
	static uintptr_t* head;
	static uintptr_t* end;

public:
	Allocator();

	void* mem_alloc(size_t);
	void* mem_realloc(void*, size_t);
	void mem_free(void*);
	void mem_dump();

};

uint8_t Allocator::heap[HEAP_SIZE];
uintptr_t* Allocator::head;
uintptr_t* Allocator::end;




