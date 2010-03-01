
#include <ddk/ntddk.h>
#include "win-compat.h"

struct kmem_cache *kmem_cache_create(const char *name, size_t size, size_t offset, 
			unsigned long flag, void (*ctor)(void *))
{
	struct kmem_cache *cache;

	 cache = ExAllocatePoolWithTag(NonPagedPool, sizeof(struct kmem_cache), 'abcd');
	 if(!cache)
		 return cache;

	 ExInitializeNPagedLookasideList(&cache->cache, NULL, NULL, 0, size, 'cbda', 0);

	 return cache;
}

void kmem_cache_destroy(struct kmem_cache *cache)
{
	ExDeleteNPagedLookasideList(&cache->cache);
	ExFreePool(cache);
}
