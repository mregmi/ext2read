/**
 * Ext2read
 * File: inode.c
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
struct ext2partition ext2part = {
	NULL
};


static char *inbuff = NULL;
static uint32_t last_block = 0;

int read_inode(uint32_t iNum, EXT2_INODE *inode)
{
	uint32_t group, index, blknum;
	int iIndex, ret = 0;

	if(iNum == 0)
		return -1;

	group = (iNum - 1) / ext2part.inodes_per_group;
	index = ((iNum - 1) % ext2part.inodes_per_group) * ext2part.inode_size;

	iIndex = (index % ext2part.blocksize);

//	if((iNum < inode_start) || (iNum > inode_end))
	blknum = ext2part.desc[group].bg_inode_table + (index/ext2part.blocksize);

	if(blknum != last_block)
		ret = read_ext2block(blknum, inbuff);
	
	memcpy(inode, inbuff + iIndex, ext2part.inode_size);
	last_block = blknum;

	return ret;
}

void init_root_dir()
{
	inbuff = malloc(ext2part.blocksize);
	ext2part.dirbuf = malloc(ext2part.blocksize);

	ext2part.curblock = 0;
	ext2part.curentry = NULL;
	
	read_inode(2, &ext2part.curdir.ino);
	ext2part.lastblock = ext2part.curdir.ino.i_blocks / (ext2part.blocksize/512);

	ext2part.curdir.inoNum = 2;
	ext2part.curdir.fileType = FILE_TYPE_DIR;
	strcpy(ext2part.curdir.fileName, "/");

	ext2part.root = ext2part.curdir;
	ext2part.curentry = NULL;
}

void fini_root_dir()
{
	if(inbuff)
		free(inbuff);
	if(ext2part.dirbuf)
		free(ext2part.dirbuf);

}
