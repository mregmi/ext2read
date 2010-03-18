/**
 * Ext2read
 * File: ext2io.c
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

#include "platform.h"
#include "partition.h"
#include "ext2fs.h"
#include "ext2read.h"

extern struct sys sys;
struct ext2partition ext2part;

#define BLOCK_TO_SECTS (ext2part.blocksize/sys.current_disk->sector_size)

int read_ext2block(int blocknum, void *buffer)
{
	int nsects = BLOCK_TO_SECTS;
	lloff_t sectno = (BLOCK_TO_SECTS * blocknum) + ext2part.part.relSect;

	return read_sector(sys.current_disk, sectno, nsects, buffer);
}

