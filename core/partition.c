/**
 * Ext2read
 * File: main.c
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
 /**
  * This file contains implementation of scanning and retrieving
  * partition information. For now we only support MBR style partitions.
  **/
#include <stdio.h>
#include <string.h>

#include "ext2read.h"
#include "platform.h"
#include "partition.h"
#include "parttypes.h"
#include "lvm.h"

struct sys sys = {0, 0};

/* Reads The Extended Partitions */
int scan_ebr(struct disk *dsk, int index)
{
	unsigned char sector[SECTOR_SIZE];
	struct MBRpartition *part, *part1;
	struct partition *p;
	int logical = 4;
	lloff_t  ebrBase, nextPart, ebr2=0;

	ebrBase = dsk->part[index].relSect;

	read_sector(dsk, dsk->part[index].relSect, 1, sector);

	do{
		part = pt_offset(sector, 0);
		p = &dsk->part[logical];
		p->systemId = part->sys_ind;
		p->totalSectors = get_nr_sects(part);
		p->flag = logical + 1;
		p->relSect = get_start_sect(part) + ebrBase + ebr2;
//		p->disk = drive;

		LOG("index %d ID %X size %Ld \n", logical, p->systemId, p->totalSectors);
		if(part->sys_ind == LVM)
		{
			scan_lvm(p);
		}

		part1 = pt_offset(sector, 1);
		ebr2 = get_start_sect(part1);
		nextPart = (ebr2 + ebrBase);

		read_sector(dsk, nextPart, 1, sector);
		logical++;
		part1 = pt_offset(sector, 1);
	}while (part1->sys_ind != 0x00);

	part = pt_offset(sector, 0);
	p = &dsk->part[logical];
	p->systemId = part->sys_ind;
	p->totalSectors = get_nr_sects(part);
	p->flag = logical + 1;
	p->relSect = get_start_sect(part) + ebrBase + ebr2;
	logical++;
//	p->disk = drive;

	LOG("index %d ID %X size %Ld \n", logical, p->systemId, p->totalSectors);
	if(part->sys_ind == LVM)
	{
		scan_lvm(p);
	}

	return logical;
}


/* Scans The partitions */
int scan_partitions(char *path, int diskno)
{
	unsigned char sector[SECTOR_SIZE];
	struct disk *dsk;
	struct MBRpartition *part;
	struct partition *p;
	int ret, i, nebr = 4;

	dsk = &sys.dsk[diskno];
	ret = open_disk(path, dsk);
	if(ret < 0)
		return ret;

	ret = read_sector(dsk, 0, 1, sector);
	if(ret < 0)
		return ret;

	if(!valid_part_table_flag(sector))
	{
		LOG("Partition Table Error on %s\n", path);
		LOG("Invalid End of sector marker");
		return -1;
	}

	dsk->part = malloc(64 * sizeof(struct partition));
	memset(dsk->part, 0, 64 * sizeof(struct partition));
	/* First Scan primary Partitions */
	for(i = 0; i < 4; i++)
	{
		part = pt_offset(sector, i);
		p = &dsk->part[i];
		if((part->sys_ind != 0x00) || (get_nr_sects(part) != 0x00))
		{
			p->systemId = part->sys_ind;
			p->totalSectors = get_nr_sects(part);
			p->disk = diskno;
			p->relSect = get_start_sect(part);
			p->flag = i + 1;
			LOG("index %d ID %X size %Ld \n", i, p->systemId, p->totalSectors);

			if(part->sys_ind == LVM)
			{
				scan_lvm(p);
			}
			else if((part->sys_ind == 0x05) || (part->sys_ind == 0x0f))
			{
				nebr = scan_ebr(dsk, i);
			}
		}
	}

	dsk->totalpart = nebr;
	dsk->part = realloc(dsk->part, nebr * sizeof(struct partition));
	return 0;
}

void free_system()
{
	int i;
	struct disk *dsk;

	for(i = 0; i < sys.ndisks; i++)
	{
		dsk = &sys.dsk[i];
		free(dsk->part);
	}

	fini_root_dir();
	namei_exit();
}

void print_ext2()
{
	int i, j;
	char ch[2], num[3];
	struct disk *dsk;
	struct partition *part;

	for(i = 0; i < sys.ndisks; i++)
	{
		dsk = &sys.dsk[i];
		ch[0] = (i + 'a');
		ch[1] = '\0';
		printf("Disk %d: /dev/hd%c \n", i, ch[0]);
		for(j = 0; j < dsk->totalpart; j++)
		{
			part = &dsk->part[j];
			strcpy(part->linux_name, "/dev/hd");
			strcat(part->linux_name, ch);
			sprintf(num, "%d", j + 1);
			strcat(part->linux_name, num);
			printf("  Partition %d: %s  type: %s\n", \
					j, part->linux_name, part_type(part->systemId));

			if(part->systemId == EXT2)
				sys.linuxparts++;
		}
	}
}

void get_choice(const char *partstr, int *disk, int *part)
{
	char num[10];

	if(!strcmp(partstr, "exit"))
		exit(0);

	*disk = (int)(partstr[7] - 'a');
	strcpy(num, &partstr[8]);
	*part = atoi(num) - 1;
}
