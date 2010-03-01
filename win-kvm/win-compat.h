/**
 * Declarations for compatibility. 
 **/

#include <stdint.h>
#include <ddk/ntddk.h>

#ifndef __WIN_COMPAT_H
#define __WIN_COMPAT_H

#define __read_mostly
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))
#define smp_wmb() 

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

//// ARCH DEPENDENT
#ifdef WINKVM_32
typedef  uint32_t size_t;
#define PAGE_SHIFT	12
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
};

struct mm_struct {
};

typedef struct wait_queue {
} wait_queue_head_t;

struct user_return_notifier {
};

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

#define __clear_bit clear_bit
#define __set_bit set_bit

/// other bitmap stuffs
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define BITS_TO_LONGS(nr)       DIV_ROUND_UP(nr, 8 * sizeof(long))
#define DECLARE_BITMAP(name,bits) \
          unsigned long name[BITS_TO_LONGS(bits)]
/// declarations
unsigned long __get_free_page(int flag);
void free_page(unsigned long address);
void spin_lock(spinlock_t *lock);
void spin_unlock(spinlock_t *lock);

void mutex_lock(mutex_t *lock);
void mutex_unlock(mutex_t *lock);

#endif

