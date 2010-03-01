/**
 * Declarations for compatibility. 
 **/

#ifndef __WIN_COMPAT_H
#define __WIN_COMPAT_H

#include <stdint.h>
#include <ddk/ntddk.h>

// Linux specific preprocessors which is not used in Windows
#define __read_mostly
#define __init
#define __exit

// some  linux specific macros which is not required in Windows
#define smp_wmb() 
#define THIS_MODULE 0
#define EXPORT_SYMBOL(name)
#define EXPORT_SYMBOL_GPL(name)

#define KERN_ERR

// some re-implemented macros (mostly copied from Linux kernel)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
typedef uint64_t __u64;

typedef int8_t __s8;
typedef int16_t __s16;
typedef int32_t __s32;
typedef int64_t __s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef int bool;
typedef unsigned gfp_t;

//// ARCH DEPENDENT
#ifdef WINKVM_32
typedef  uint32_t size_t;
 #define PAGE_SHIFT	12
 #define BITS_PER_LONG 32

#elif WINKVM_64
 #define BITS_PER_LONG 64
#endif

struct mutex {
	FAST_MUTEX  lock;
} mutex_t;

typedef struct spinlock {
	KSPIN_LOCK lock;
	KIRQL irql;
}spinlock_t;

typedef raw_spinlock_t spinlock_t;

struct srcu_struct {
	FAST_MUTEX  lock;
}

typedef struct {
	volatile int counter;
} atomic_t;

struct kmem_cache {
	NPAGED_LOOKASIDE_LIST cache;
};
typedef struct kmem_cache kmem_cache_t;

struct mm_struct {
};

typedef struct wait_queue {
} wait_queue_head_t;

struct page {
};

#define DEFAULT_SEEKS 2 // from linux kernel 
struct shrinker {
         int (*shrink)(int nr_to_scan, gfp_t gfp_mask);
         int seeks;      /* seeks to recreate an obj */
};

struct user_return_notifier {
};

struct module {
};

// cpu related operations in X86
static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
                                unsigned int *ecx, unsigned int *edx)
 {
         /* ecx is often an input as well as an output. */
         asm volatile("cpuid"
             : "=a" (*eax),
               "=b" (*ebx),
               "=c" (*ecx),
               "=d" (*edx)
             : "0" (*eax), "2" (*ecx));
 }

static inline unsigned int cpuid_ecx(unsigned int op)
{
        unsigned int eax, ebx, ecx, edx;

		eax = op;
		ecx = 0;
        native_cpuid(&eax, &ebx, &ecx, &edx);

        return ecx;
}

///// BIT and String operations X86
static inline void set_bit(unsigned int nr, volatile unsigned long *addr)
{
	asm volatile("bts %1,%0"
           : "=m" (*(volatile long *) (addr)) : "Ir" (nr) : "memory");
}

static inline void clear_bit(int nr, volatile unsigned long *addr)
{
	asm volatile("btr %1,%0"
			: "=m" (*(volatile long *) (addr))
			: "Ir" (nr));
}

static inline int test_bit(unsigned int nr, const volatile unsigned long *addr)
{
       return ((1UL << (nr % BITS_PER_LONG)) &
               (((unsigned long *)addr)[nr / BITS_PER_LONG])) != 0;
}

#define __clear_bit clear_bit
#define __set_bit set_bit

/// other bitmap stuffs
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)       DIV_ROUND_UP(nr, 8 * sizeof(long))
#define DECLARE_BITMAP(name,bits) \
          unsigned long name[BITS_TO_LONGS(bits)]

/// declarations
#define __get_free_page(flag) ((unsigned long)MmAllocateNonCachedMemory(PAGE_SIZE))
#define free_page(address) MmFreeNonCachedMemory((void *)address, PAGE_SIZE)

struct kmem_cache *kmem_cache_create(const char *, size_t, size_t,
			unsigned long, void (*)(void *));
void kmem_cache_destroy(struct kmem_cache *);

#define spin_lock(lock) KeAcquireSpinLock(lock->lock, lock->irql)
#define spin_unlock(lock) KeReleaseSpinLock(lock->lock, lock->irql)

#define mutex_lock(lock) ExAcquireFastMutex(lock->lock)
#define mutex_unlock(lock) ExReleaseFastMutex(lock->lock)

#define printk(str, ...) DbgPrint(str, ...)

#endif

