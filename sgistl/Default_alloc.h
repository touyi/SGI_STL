#pragma once
class Default_alloc
{
private:
	static void* Fail_Malloc(size_t);
	static void* Fail_Realloc(void*, size_t);
	static void(*malloc_fail_hander)();
public:
	static void* allocate(size_t);
	static void deallocate(void*);
	static void reallocate(void*, size_t);

	static void(*set_fail_alloc_hander(void(*f)()))();
};

