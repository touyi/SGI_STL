#include "Default_alloc.h"
#include<stdlib.h>
#include<memory>
void(*Default_alloc::malloc_fail_hander)() = 0;

void * Default_alloc::Fail_Malloc(size_t n)
{
	void* result = 0;
	while (true)
	{
		if (malloc_fail_hander == 0)throw std::bad_alloc();
		malloc_fail_hander();
		result = malloc(n);
		if (result != 0)return result;
	}
}

void * Default_alloc::Fail_Realloc(void * p , size_t n)
{
	void* result = 0;
	while (true)
	{
		if (malloc_fail_hander == 0)throw std::bad_alloc();
		malloc_fail_hander();
		result = realloc(p, n);
		if (result != 0)return result;
	}
}

void * Default_alloc::allocate(size_t n)
{
	void* result = malloc(n);
	if (0 == result)
	{
		result = Fail_Malloc(n);
	}
	return result;

}

void Default_alloc::deallocate(void * p)
{
	free(p);
}

void Default_alloc::reallocate(void * p, size_t n)
{
	void* result = realloc(p, n);
	if (result == 0)
	{
		result = Fail_Realloc(p, n);
	}
}

void(*Default_alloc::set_fail_alloc_hander(void(*f)()))()
{
	auto old = malloc_fail_hander;
	malloc_fail_hander = f;
	return old;
}
