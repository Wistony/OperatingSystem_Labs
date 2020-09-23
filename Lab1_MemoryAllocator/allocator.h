#include <cstdint>;

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
	

};

