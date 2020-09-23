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
	static uint8_t* head;
	static uint8_t* end;

	void addHeader(Header*, size_t, size_t, bool);

public:
	Allocator();

	void* mem_alloc(size_t);
	void* mem_realloc(void*, size_t);
	void mem_free(void*);
	void mem_dump();

};




