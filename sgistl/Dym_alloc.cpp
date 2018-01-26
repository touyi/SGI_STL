#include "Dym_alloc.h"
#include"Default_alloc.h"
#include<algorithm>
using std::min;
Dym_alloc::obj* Dym_alloc::freeList[_NFREE] = { 0 };
char* Dym_alloc::start_free = 0;
char* Dym_alloc::end_free = 0;
int Dym_alloc::heap_size = 0;
void * Dym_alloc::refill(size_t n)
{
	size_t nobj = 20;
	char* buffer = chunk_allock(n, nobj);
	if (nobj == 1)
	{
		return buffer;
	}
	void* result = buffer;
	auto mlist = freeList + FreeListIndex(n);
	*mlist = (obj*)(buffer + n);
	obj* current = *mlist;
	for (int i = 1; ; i++)
	{
		if (i == nobj - 1)
		{
			current->next = 0;
			break;
		}
		current->next = (obj*)((char*)current + n);
		current = current->next;
	}
	return result;
}

char * Dym_alloc::chunk_allock(size_t size, size_t & nobj)
{
	size_t free_size = end_free - start_free;
	size_t remain_size_count = free_size / size;
	nobj = min(nobj, remain_size_count);
	size_t need_size = nobj * size;

	char* result;
	if (nobj>0)
	{
		result = start_free;
		start_free += need_size;
		return result;
	}
	else 
	{
		nobj = 20;
		if (free_size > 0)
		{
			auto mlist = freeList + FreeListIndex(free_size);
			obj* head = (obj*)start_free;
			head->next = *mlist;
			*mlist = head;
			start_free = end_free = 0;
		}
		size_t new_size = nobj * size + ROUND_UP(heap_size>>4); // ROUND_UP(heap_size>>4)
		start_free = (char*)malloc(new_size);
		if (start_free == 0)
		{
			for (size_t i = size; i<=_MAX_BYTES; i+=_ALIGN)
			{
				auto * mlist = freeList + FreeListIndex(i);
				if (*mlist != 0)
				{
					start_free = (char*)(*mlist);
					*mlist = (*mlist)->next;
					end_free = start_free + i;
					return chunk_allock(size, nobj);
				}
			}
			start_free = (char*)Default_alloc::allocate(new_size);
		}
		end_free = start_free + new_size;
		heap_size += new_size;
		
		return chunk_allock(size, nobj);
	}
}

void * Dym_alloc::allocate(size_t n)
{
	if (n > _MAX_BYTES)
	{
		return Default_alloc::allocate(n);
	}
	auto mlist = freeList + FreeListIndex(n);
	if (*mlist == 0)
	{
		return refill(ROUND_UP(n));
	}
	void* result = *mlist;
	(*mlist) = (*mlist)->next;
	return result;
}
// 2018/1/26 TODO
void Dym_alloc::deallocate(void* p,size_t n)
{
	if (n > _MAX_BYTES)
	{
		Default_alloc::deallocate(p);
		return;
	}
	auto mlist = freeList + FreeListIndex(n);
	obj* nod = static_cast<obj*>(p);
	nod->next = *mlist;
	(*mlist) = nod;
}

void * Dym_alloc::reallocate(size_t)
{
	return nullptr;
}
