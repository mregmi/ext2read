/**
 * Ext2read
 * File: commands.c
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

#include <time.h>

#include "shell.h"
#include "ext2read.h"

struct ext2partition ext2part;

void cmd_help()
{
	printf("ext2readsh 2.0 beta (C) Manish Regmi \n"	\
		"Currently there are 5 commands.\n" \
		"exit   This command exits the program.\n"	\
		"help   This Command shows this help.\n"	\
		"fsls   Analogous to ls UNIX command.\n"	\
		"       eg. fsls /mnt \n"	\
		"           fsls\n"			\
		"fscd   Analogous to cd UNIX Command. \n"	\
		"       eg. fscd /mnt changes current directory to /mnt.\n"	\
		"fsstat Analogous to stat UNIX command\n"	\
		"		eg. fsstat /home/t.txt shows the statistics of the file /home/t.txt \n"
		"All other commands are passed to system shell \n");
}


void cmd_ls(char cmd[][256])
{
	DIRENTRY entry, parent;
	struct ext2partition saved;
	int ret;

	printf("PATH :%s: ", cmd[1]);
	ret = path_to_direntry(cmd[1], &parent);
	if(ret < 0)
		return;

	if(!EXT2_FT_ISDIR(parent.fileType))
	{
		printf("%s\n", parent.fileName);
		return;
	}

	saved = ext2part;
	ext2part.curdir = parent;
	ext2part.curentry = NULL;
	ext2part.lastblock = ext2part.curdir.ino.i_blocks / (ext2part.blocksize/512);
	ext2part.curblock = 0;
	
	while(get_dir_entry(&entry) == 0)
	{
		printf("%s\t", entry.fileName);
	}

	ext2part = saved;
	printf("\n");
}

void cmd_cd(char cmd[][256])
{
	DIRENTRY parent;
	int ret;

	ret = path_to_direntry(cmd[1], &parent);
	if(ret < 0)
		return;

	if(!EXT2_FT_ISDIR(parent.fileType))
	{
		printf("Destination is not a directory\n");
		return;
	}

	ext2_cd(&parent, cmd[1]);
}

void cmd_stat(char cmd[][256])
{
	DIRENTRY parent;
	int ret;

	ret = path_to_direntry(cmd[1], &parent);
	if(ret < 0)
		return;

	printf("file: %s\n", parent.fileName);
	printf("Size: %ld",parent.ino.i_size);
	printf("\tBlocks: %ld",parent.ino.i_blocks);
	printf("\tIO Block: %d",ext2part.blocksize);
	printf("\t\t%s\n",get_type_string(parent.fileType));
	printf("Inode: %ld",parent.inoNum);
	printf("\tLinks: %d\n",parent.ino.i_links_count);
	printf("Access: ( O%o / %s )\n",(parent.ino.i_mode & 0x0fff), get_access(parent.ino.i_mode));
	printf("Access: %s",ctime(&parent.ino.i_atime));
	printf("Modify: %s",ctime(&parent.ino.i_mtime));
	printf("Create: %s",ctime(&parent.ino.i_ctime));

}

