/**
 * Ext2read
 * File: print.c
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
#include "parttypes.h"

extern struct sys sys;


void get_mpoint(char *mpoint)
{
 	printf("Please enter the mount point for the partition. \n");
 	printf("We are trying to make the programe behave like unix mounts.\n" \
	 		 "For Dos/Windows typr the non existent drive letter (like e:)\n"	\
			  "In UNIX type the mount point (like /mnt/e2fs).");
	fgets(mpoint, 256, stdin);  
}

struct partition *ask_for_choice(char *mpoint)
{
	char partstr[20];
	char *tmp;
	int disk_idx, part_idx, max;
	struct partition *p;
again:
	print_ext2();
	printf("Please enter your choice in the form (/dev/hda5) or exit to end\n");

	memset(partstr, 0, 20);
	fgets(partstr, 20, stdin);

	get_choice(partstr, &disk_idx, &part_idx);
	printf("Choices are %d %d\n", disk_idx, part_idx);

	if((disk_idx < 0) || (disk_idx >= sys.ndisks))
	{
		printf("invalid choice. please choose again\n");
		goto again;
	}

	max = sys.dsk[disk_idx].totalpart;
	if((part_idx < 0) || (part_idx > max))
	{
		printf("invalid choice. please choose again\n");
		goto again;
	}

	p = &sys.dsk[disk_idx].part[part_idx];
	if(p->systemId != EXT2)
	{
		printf("Not an Ext2 Partition. \n"
			"you can only select Ext2 Partitions");
		goto again;
	}
	
	get_mpoint(mpoint);
	tmp = strchr(mpoint, '\n');
	if(tmp)
		  *tmp = '\0';
		  
	sys.current_disk = &sys.dsk[disk_idx];
	return p;
}
