#include <cstdint>
#include <vector>
#include <map>

using namespace std;

#define MEMORY_SIZE 4096
#define PAGE_SIZE 256

enum PageState
{
	Free,
	DividedIntoBlocks,
	MultiPageBlocks
};

struct PageHeader 
{
	PageState state;
	size_t classSize;
	size_t freeBlockAmount;
	uint8_t* freeBlockPtr;
};

class Allocator 
{
private:
	static uint8_t memory[MEMORY_SIZE];
	static const size_t pageNum = MEMORY_SIZE / PAGE_SIZE;
	static map <void*, PageHeader> pageHeaders;
	static vector <void*> freePages;
	static map <int, vector<void*>> classifiedPages;

	void initializeFreePages();
	void initializePageHeaders();
	void initializeClassifiedPages();

public: 
	Allocator();

	void* mem_alloc(size_t);
	void* mem_realloc(void*, size_t);
	void mem_free(void*);
	void mem_dump();
};