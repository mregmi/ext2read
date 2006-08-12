/**
 * Ext2read
 * File: mainui.cpp
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
#include "ext2read.h"
#include "partition.h"
#include "parttypes.h"

#include "mainui.h"

extern "C" struct sys sys;
extern "C" struct ext2partition ext2part;

MainUi::MainUi(QWidget *parent)
	:QMainWindow(parent)
{
	win.setupUi(this);

	list = win.list;
	tree = win.tree;
}

MainUi::~MainUi()
{
//	delete model;
}


void MainUi::ScanSystem()
{
	char pathname[50];
	struct partition *p;
	struct disk *dsk;
	int ret, nitem = 0;
	DIRENTRY *entry;

	sys.ndisks = get_ndisks();

	log_init();

	LOG_INFO("No of disks %d\n", sys.ndisks);

	for(int i = 0; i < sys.ndisks; i++)
	{
		get_nthdevice(pathname, sys.ndisks);
		LOG_INFO("Scanning %s\n", pathname);
		ret = scan_partitions(pathname, i);
		if(ret < 0)
		{
			LOG_INFO("Scanning of %s failed: ", pathname);
			continue;
		}
	}

	print_ext2();
	entry = new DIRENTRY[sys.linuxparts];

	for(int i = 0; i < sys.ndisks; i++)
	{
		dsk = &sys.dsk[i];
		for(int j = 0; j < dsk->totalpart; j++)
		{
			p = &dsk->part[j];
			if(p->systemId != EXT2)
				continue;

			entry[nitem].fileType = FILE_TYPE_PARTITON;
			strcpy(entry[nitem].fileName, p->linux_name);
			nitem++;
		}
	}

	LOG("Calling ViewModel with entry %s len %d\n", entry->fileName, sys.linuxparts);
//	model = new ViewModel(entry, sys.linuxparts);
//	list->setModel(model);
//	tree->setModel(model);
}
