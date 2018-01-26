#pragma once
enum { _ALIGN = 8};
enum {_MAX_BYTES = 128};
enum {_NFREE = _MAX_BYTES/_ALIGN};
class Dym_alloc
{
private:
	union obj
	{
		obj* next;
	};
	static size_t ROUND_UP(size_t n)
	{
		return ((n + _ALIGN - 1) / _ALIGN) * _ALIGN;
	}
	static obj* freeList[_NFREE];
	static size_t FreeListIndex(size_t bytes)
	{
		return (bytes + _ALIGN - 1) / _ALIGN - 1;
	}
	static void* refill(size_t n);
	static char*chunk_allock(size_t size, size_t&nobj);

	// memery pool
	static char* start_free;
	static char* end_free;
	static int heap_size;

public:
	static void* allocate(size_t);
	static void deallocate(void*,size_t);
	static void* reallocate(size_t);


};

