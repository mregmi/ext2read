/*
 * Ext2read
 * File: main.c
 */
/*
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
#include <string.h>

#include "ext2read.h"
#include "platform.h"
#include "partition.h"
#include "shell.h"

extern struct sys sys;

int main(int argc, char **argv)
{
	int i, ret;
	char pathname[256];
	struct partition *p;

    log_init();
	sys.ndisks = get_ndisks();
	

	TRACE("No of disks %d\n", sys.ndisks);
	for(i = 0; i < sys.ndisks; i++)
	{
		get_nthdevice(pathname, sys.ndisks);
		TRACE("Scanning %s\n", pathname);
		ret = scan_partitions(pathname, i);
		if(ret < 0)
		{
			TRACE("Scanning of %s failed: ", pathname);
		}
	}

	p = ask_for_choice(pathname);
	if(!p)
	      return 1;
	      
 	mount_ext2(p);
	start_shell(pathname);
	fflush(stdin);
	getchar();
}
