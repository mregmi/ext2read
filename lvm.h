/**
 * Ext2read
 * File: lvm.h
 **/
/**
 * Copyright (C) 2006 by Manish Regmi   (regmi dot manish at gmail.com)
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

#ifndef __LVM_H
#define __LVM_H

#include <stdint.h>

#include "ext2read.h"

#define LVM_SIGLEN	8
#define LVM_MAGIC_LEN	8
#define UUID_LEN	32

//#pragma pack(push, 1)

/* structure to hold the base address and size */
struct lvm_sz {
	uint32_t base, limit;
} PACKED;

/* Structure to hold Physical Volumes (PV) */
struct pv_metadata {
        char        pv_name[LVM_SIGLEN];    // Physical volume signature
        uint64_t    pv_uknown0;
        uint64_t    pv_uknown1;
        char        pv_vermagic[LVM_MAGIC_LEN]; // Physical Volume version
        char        pv_uuid[UUID_LEN];
        char        padding[448];
} __attribute__ ((__packed__));


class LVM {

    struct pv_metadata *pv;

public:
    LVM(FileHandle handle, lloff_t offset);
    ~LVM();
};

#ifdef __cplusplus
extern "C"{
#endif
int scan_lvm(FileHandle handle, lloff_t offset);

#ifdef __cplusplus
}
#endif

#endif // __LVM_H
