/**
 * Declarations for compatibility. 
 **/

#include <stdint.h>

#ifndef __WIN_COMPAT_H
#define __WIN_COMPAT_H

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
};

typedef struct spinlock {
}spinlock_t;

struct kmem_cache {
};

typedef struct wait_queue {
} wait_queue_head_t;

#endif

