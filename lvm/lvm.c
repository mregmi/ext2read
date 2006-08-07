/**
 * Ext2read
 * File: lvm.c
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

#include <stdlib.h>

#include "ext2read.h"
#include "lvm.h"
#include "partition.h"

extern struct sys sys;

int scan_lvm(struct partition *p)
{
	struct pv_metadata *pv;
	char *buf;
	struct disk *dsk;
	int ret;

	buf = malloc(1024);

	dsk = &sys.dsk[p->disk];
	ret = read_sector(dsk, p->relSect, 2, buf);
	if(ret < 0)
	{
		LOG("Error reading LVM");
	}

	pv = buf;
	ret = sizeof(*pv);
	dsk = NULL;
	free(pv);
	
	return 0;
}
