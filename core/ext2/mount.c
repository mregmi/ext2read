/**
 * Ext2read
 * File: mount.c
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

#include <stdio.h>

#include "platform.h"
#include "ext2read.h"
#include "ext2fs.h"
#include "partition.h"
#include "parttypes.h"

struct ext2partition ext2part;
extern struct sys sys;

static int mounted = 0;

#define BLOCK_TO_SECTS (ext2part.blocksize/sys.current_disk->sector_size)

int mount_ext2(struct partition *part)
{
	EXT2_SUPER_BLOCK sblock;
	int totalGroups, sectsize;
	int gSizes, gSizeb;		/* Size of total group desc in sectors */
	char *tmpbuf;

	if(!part)
		return -1;

	ext2part.part = *part;
	sectsize = sys.current_disk->sector_size;
	read_sector(sys.current_disk, part->relSect + 2, 2, &sblock);	/* read superBlock of root */
	if(sblock.s_magic != EXT2_SUPER_MAGIC)
	{
		LOG("Bad Super Block. The drive is not ext2 formatted.\n");
		return -1;
	}

	ext2part.blocksize = EXT2_BLOCK_SIZE(&sblock);
	ext2part.inodes_per_group = EXT2_INODES_PER_GROUP(&sblock);
	ext2part.inode_size = EXT2_INODE_SIZE(&sblock);
	sys.current = &ext2part;

	totalGroups = (sblock.s_blocks_count)/EXT2_BLOCKS_PER_GROUP(&sblock);
	ext2part.desc = (EXT2_GROUP_DESC *) calloc(totalGroups, sizeof(EXT2_GROUP_DESC));
	if(ext2part.desc == NULL)
	{
		LOG("Not enough Memory: Exiting\n");
		exit(1);
	}
	
	gSizeb = (sizeof(EXT2_GROUP_DESC)*totalGroups);
	gSizes = (gSizeb/sectsize)+1;

	if((tmpbuf = malloc(gSizes*sectsize)) == NULL)
	{
		LOG("Not enough Memory: Exiting\n");
		exit(1);
	}
	
	/* Read all Group descriptors and store in buffer */
	/* I really dont know the official start location of Group Descriptor array */
	if((BLOCK_TO_SECTS) <= 2)
		read_sector(sys.current_disk, (ext2part.part.relSect)+(BLOCK_TO_SECTS + 2), gSizes, tmpbuf);
	else
		read_sector(sys.current_disk, (ext2part.part.relSect)+(BLOCK_TO_SECTS), gSizes, tmpbuf);
	
	memcpy(ext2part.desc, tmpbuf, gSizeb);

	free(tmpbuf);
	
	init_root_dir();
	namei_init();
	strcpy(ext2part.path, "/");
	mounted = 0;

	return 0;
}

struct partition* validate_choice(int disk, int part)
{
	struct partition *p;
	int max;

	if((disk < 0) || (disk >= sys.ndisks))
	{
		printf("invalid choice. please choose again\n");
		return NULL;
	}

	max = sys.dsk[disk].totalpart;
	if((part < 0) || (part > max))
	{
		printf("invalid choice. please choose again\n");
		return NULL;
	}

	p = &sys.dsk[disk].part[part];
 	if(p->systemId != EXT2)
	{
		printf("Not an Ext2 Partition. \n"
			"you can only select Ext2 Partitions");
		return NULL;
	}

	sys.current_disk = &sys.dsk[disk];

	return p;
}


int mount_partition(const char *path)
{
	struct partition *p;
	int disk, part;
	
	if(mounted == 1)
		return 0;

	get_choice(path, &disk, &part);
	p = validate_choice(disk, part);
	mount_ext2(p);

	return 1;
}
