/**
 * Ext2read
 * File: ext2read.h
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

#ifndef __EXT2READ_H
#define __EXT2READ_H

#ifdef __GNUC__
 #include <stdint.h>
#endif

#include "platform.h"
#include "ext2fs.h"

#define FILE_TYPE_PARTITON		0x7E

/* Identifies the type of file by using i_mode of inode */
/* structure as input.									*/
#define EXT2_S_ISDIR(mode)	((mode & 0x0f000) == 0x04000)
#define EXT2_S_ISLINK(mode)	((mode & 0x0f000) == 0x0a000)
#define EXT2_S_ISBLK(mode)	((mode & 0x0f000) == 0x06000)
#define EXT2_S_ISSOCK(mode)	((mode & 0x0f000) == 0x0c000)
#define EXT2_S_ISREG(mode)	((mode & 0x0f000) == 0x08000)
#define EXT2_S_ISCHAR(mode)	((mode & 0x0f000) == 0x02000)
#define EXT2_S_ISFIFO(mode)	((mode & 0x0f000) == 0x01000)

/* Identifies the type of file by using file_type of 	*/
/* directory structure as input.						*/
#define EXT2_FT_ISDIR(mode)		(mode  == 0x2)
#define EXT2_FT_ISLINK(mode)	(mode  == 0x7)
#define EXT2_FT_ISBLK(mode)		(mode  == 0x4)
#define EXT2_FT_ISSOCK(mode)	(mode  == 0x6)
#define EXT2_FT_ISREG(mode)		(mode  == 0x1)
#define EXT2_FT_ISCHAR(mode)	(mode  == 0x3)
#define EXT2_FT_ISFIFO(mode)	(mode  == 0x5)
#define IS_PART(mode)			(mode == FILE_TYPE_PARTITION)

/* Mask values for the access rights. */
/* User */
#define EXT2_S_IRUSR			0x0100
#define EXT2_S_IWUSR			0x0080
#define EXT2_S_IXUSR			0x0040
/* Group */
#define EXT2_S_IRGRP			0x0020
#define EXT2_S_IWGRP			0x0010
#define EXT2_S_IXGRP			0x0008
/* Others */
#define EXT2_S_IROTH			0x0004
#define EXT2_S_IWOTH			0x0002
#define EXT2_S_IXOTH			0x0001


static INLINE char *get_type_string(int type)
{
	switch(type)
	{
		case 0x1:	return "Regular File";
		case 0x2:	return "Directory";
		case 0x3:	return "Character Device";
		case 0x4:	return "Block Device";
		case 0x5:	return "Fifo File";
		case 0x6:	return "Socket File";
		case 0x7:	return "Symbolic Link";
	}

	return "Unknown Type";
}

static INLINE char *get_access(unsigned long mode)
{
	static char acc[9];
	acc[0] = (mode & EXT2_S_IRUSR)? 'r':'-';
	acc[1] = (mode & EXT2_S_IWUSR)? 'w':'-';
	acc[2] = (mode & EXT2_S_IXUSR)? 'x':'-';

	acc[3] = (mode & EXT2_S_IRGRP)? 'r':'-';
	acc[4] = (mode & EXT2_S_IWGRP)? 'w':'-';
	acc[5] = (mode & EXT2_S_IXGRP)? 'x':'-';

	acc[6] = (mode & EXT2_S_IROTH)? 'r':'-';
	acc[7] = (mode & EXT2_S_IWOTH)? 'w':'-';
	acc[8] = (mode & EXT2_S_IXOTH)? 'x':'-';

	acc[9] = '\0';
	return acc;
}

struct partition
{
	uint8_t		systemId;
	lloff_t		totalSectors;
	lloff_t 	relSect;
	uint8_t		disk;       /* disk no */
	uint8_t		flag;		/* a flag to determine partition number */
	char        linux_name[10];
};

struct disk
{
	struct partition *part;
	FileHandle handle;
	int sector_size,
			totalpart;

};

typedef struct tagDIRENTRY {
	EXT2_INODE  ino;
	DWORD 		inoNum;
	char		fileName[EXT2_NAME_LEN];
	WORD		recLen;
	BYTE		fileType;
}DIRENTRY;

struct ext2partition
{
	char *dirbuf;	// buffer to hold directory entries
	struct partition part;
	int blocksize, inodes_per_group, inode_size;
	EXT2_GROUP_DESC *desc;
	DIRENTRY curdir, root;

	EXT2_DIR_ENTRY *curentry;
	uint32_t curblock, lastblock;	// hold block num
	char path[256];
};

struct sys
{
	int ndisks, linuxparts;

	struct disk dsk[5];  // Five disks should be enough
	struct ext2partition *current;
	struct disk *current_disk;
};

#ifndef TRACE
 #include <stdio.h>
 #define TRACE printf
#endif

#ifdef __cplusplus
extern "C"{
#endif

struct partition *ask_for_choice();
void print_ext2();
int mount_partition(const char *path);
void get_choice(const char *partstr, int *disk, int *part);
int read_ext2block(int blocknum, void *buffer);
int enter_new_dir(uint32_t inode, const char *name);
int read_inode(uint32_t iNum, EXT2_INODE *inode);
int mount_ext2(struct partition *part);
int get_dir_entry(DIRENTRY *dir);
void init_root_dir();
void fini_root_dir();
int read_data_block(EXT2_INODE *ino, uint32_t lbn, void *buf);
int get_dentry_inode(DIRENTRY *entry, uint32_t inode);
int save_file(const char *path, uint32_t ino);
int __save_file(const char *path, uint32_t ino);
void free_system();
void namei_init();
void namei_exit();

int log_init();

int ext2explore_log(const char *msg, ...);
int ext2explore_log_err(char *file, char *line, const char *buf, ...);

#ifdef __cplusplus
}
#endif

#define LOG_INFO	ext2explore_log
#define LOG			LOG_INFO
//#define LOG_ERROR	ext2explore_log_err(__FILE__, __LINE__,

#endif  // __EXT2READ_H
