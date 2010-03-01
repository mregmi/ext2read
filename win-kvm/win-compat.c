
#include <ddk/ntddk.h>
#include "win-compat.h"

static inline unsigned long __get_free_page(int flag)
{
	return MmAllocateNonCachedMemory(PAGE_SIZE);
}

static inline void free_page(unsigned long address)
{
	MmFreeNonCachedMemory((void *)address, PAGE_SIZE);
}

static inline void spin_lock(spinlock_t *lock)
{
	KeAcquireSpinLock(lock->lock, lock->irql);
}

static inline void spin_unlock(spinlock_t *lock)
{
	KeReleaseSpinLock(lock->lock, lock->irql);
}

void mutex_lock(mutex_t *lock)
{
	ExAcquireFastMutex(lock->lock);
}

void mutex_unlock(mutex_t *lock)
{
	ExReleaseFastMutex(lock->lock);
}
