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

#include "platform.h"

#define LVM_SIGNATURE	('H' +  ('M' << 8))
#define LVM_NAME_LEN	128
#define UUID_LEN		16

//#pragma pack(push, 1)

/* structure to hold the base address and size */
struct lvm_sz {
	uint32_t base, limit;
} PACKED;

/* Structure to hold Physical Volumes (PV) */
struct pv_metadata {
	uint16_t	pv_sig;			// Physical volume signature
	uint16_t	pv_version;		// Physical Volume version
	struct lvm_sz pv_pv;
	struct lvm_sz pv_vg;
	struct lvm_sz pv_pvuuid;
	struct lvm_sz pv_lv;
	struct lvm_sz pv_pe;
	uint8_t		pv_uuid[LVM_NAME_LEN];
	uint8_t		pv_vgname[LVM_NAME_LEN];
	uint8_t		pv_sysid[LVM_NAME_LEN];
	uint32_t pv_major;
	uint32_t pv_number;
	uint32_t pv_status;
	uint32_t pv_allocatable;
	uint32_t pv_size;		/* HM */
	uint32_t lv_cur;
	uint32_t pe_size;
	uint32_t pe_total;
	uint32_t pe_allocated;
} PACKED;

//#pragma pack(pop)

#ifdef __cplusplus
extern "C"{
#endif
int scan_lvm(struct partition *p);

#ifdef __cplusplus
}
#endif

#endif // __LVM_H
