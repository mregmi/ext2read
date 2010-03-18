/**
 * Ext2read
 * File: platform.h
 **/
/**
 * Copyright (C) 2005 by Manish Regmi   (regmi dot manish at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 **/

#ifndef __PLATFORM_H
#define __PLATFORM_H

#ifdef WIN32
#include <windows.h>

 typedef HANDLE FileHandle;
#ifdef _MSC_VER
 typedef unsigned char uint8_t;
 typedef unsigned short uint16_t;
 typedef unsigned int uint32_t;
#endif
 
 #define FILE_DELIM		"\\"
#else
 typedef int FileHandle;
 #define FILE_DELIM	"/"
#endif

typedef unsigned int uint;
typedef unsigned long ulong;

#define SECTOR_SIZE             512

#if defined(__GNUC__) || defined(HAS_LONG_LONG)
 typedef long long lloff_t;
#elif defined (WIN32)
 typedef __int64 lloff_t;
#else
 typedef long      lloff_t;
#endif

#if defined(__GNUC__)
 #define PACKED __attribute__((packed))
#else
 #define PACKED
#endif

#ifndef INLINE
  #if defined(_MSC_VER)
   #define INLINE __inline
  #elif defined(__GNUC__)
   #define INLINE __inline__
  #endif /* Compiler */
#endif /* INLINE */

#ifdef __cplusplus
extern "C"{
#endif


FileHandle open_disk(const char *, int *);
int get_ndisks();
void close_disk();
int read_disk(FileHandle hnd, void *ptr, lloff_t sector, int nsects, int sectorsize);
int write_disk(FileHandle hnd, void *ptr, lloff_t sector, int nsects, int sectorsize);
int get_nthdevice(char *path, int ndisks);

#ifdef __cplusplus
}
#endif


#endif // __PLATFORM_H
