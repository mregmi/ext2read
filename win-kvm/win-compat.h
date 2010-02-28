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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

struct mutex {
};

struct kmem_cache {
};

typedef struct wait_queue {
} wait_queue_head_t;

#endif 
