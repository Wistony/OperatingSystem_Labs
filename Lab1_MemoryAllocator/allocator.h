#include <cstdint>;

using namespace std;

#define HEAP_SIZE 256;

struct Header
{
	uint16_t size;
	uint16_t prevSize;
	bool isAvailable;
	uint8_t alignment[3];
};
