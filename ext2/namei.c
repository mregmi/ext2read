/**
 * Ext2read
 * File: namei.c
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

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
//#include <sys/types.h>


#include "platform.h"
#include "partition.h"
#include "ext2fs.h"
#include "ext2read.h"


/*#define MKDIR(path, mode)		\
{								\
#ifdef WIN32					\
	mkdir(path);				\
#else							\
	mkdir(path, mode);			\
#endif							\
}*/

extern struct sys sys;
struct ext2partition ext2part;


struct block_buff{
	uint32_t *ind, *dind, *tind;
	uint32_t indblk, dindblk, tindblk;
} blkbuf = {
	NULL, NULL, NULL,
	0, 0, 0
};

void namei_init()
{
	blkbuf.indblk = blkbuf.dindblk = blkbuf.tindblk = 0;
	blkbuf.ind = (uint32_t *)malloc(ext2part.blocksize);
	blkbuf.dind = (uint32_t *)malloc(ext2part.blocksize);
	blkbuf.tind = (uint32_t *)malloc(ext2part.blocksize);
}

void namei_exit()
{
	if(blkbuf.ind)
		free(blkbuf.ind);

	if(blkbuf.dind)
		free(blkbuf.dind);

	if(blkbuf.tind)
		free(blkbuf.tind);
}

/* Convert file block no to logical block no of partition */
static INLINE uint32_t fileblock_to_logical(EXT2_INODE *ino, uint32_t lbn)
{
	uint32_t block, indlast, dindlast, tindlast;
	uint32_t tmpblk, sz;

	if(lbn < EXT2_NDIR_BLOCKS)
	{
		return ino->i_block[lbn];
	}
	
	sz = ext2part.blocksize/4;
	indlast = sz + EXT2_NDIR_BLOCKS;

	if((lbn >= EXT2_NDIR_BLOCKS) && (lbn < indlast))
	{
		block = ino->i_block[EXT2_IND_BLOCK];
		if(block != blkbuf.indblk)
		{
			blkbuf.indblk = block;
			read_ext2block(block, blkbuf.ind);
		}
		lbn -= EXT2_NDIR_BLOCKS;
		return blkbuf.ind[lbn];
	}

	dindlast = (sz * sz) + indlast;
	if((lbn >= indlast) && (lbn < dindlast))
	{
		block = ino->i_block[EXT2_DIND_BLOCK];
		if(block != blkbuf.dindblk)
		{
			blkbuf.dindblk = block;
			read_ext2block(block, blkbuf.dind);
		}
		
		tmpblk = lbn - indlast;
		block = blkbuf.dind[tmpblk/sz];
		if(block != blkbuf.indblk)
		{
			blkbuf.indblk = block;
			read_ext2block(block, blkbuf.ind);
		}
		lbn = tmpblk%sz;
		return blkbuf.ind[lbn];
	}

	///tindlast = (sz * sz * sz) + dindlast;
	if(lbn >= dindlast)
	{
		block = ino->i_block[EXT2_TIND_BLOCK];
		if(block != blkbuf.tindblk)
		{
			blkbuf.dindblk = block;
			read_ext2block(block, blkbuf.tind);
		}
		
		tmpblk = lbn - dindlast;
		block = blkbuf.tind[tmpblk/(sz * sz)];
		if(block != blkbuf.dindblk)
		{
			blkbuf.dindblk = block;
			read_ext2block(block, blkbuf.dind);
		}
		block = tmpblk / sz;
		lbn = tmpblk % sz;
		block = blkbuf.dind[block];
		if(block != blkbuf.indblk)
		{
			blkbuf.indblk = block;
			read_ext2block(block, blkbuf.ind);
		}
		return blkbuf.ind[lbn];
	}

	return 0;
}

int read_data_block(EXT2_INODE *ino, uint32_t lbn, void *buf)
{
	uint32_t block;

	block = fileblock_to_logical(ino, lbn);

	return read_ext2block(block, buf);
}

static void remove_last_name(char *path)
{
	int len;
	char ch;

	if(!path)
		return;

	len = strlen(path);
	if(len <= 1)
		return;
	len -= 2;
	do{
		ch = path[len];
		len--;
	}while((ch != '/') && (len > 0));

	path[len + 1] = '\0';

}

int enter_new_dir(uint32_t inode, const char *name)
{
	static EXT2_INODE ino;
	int ret;

	ret = read_inode(inode, &ino);
	if(ret < 0)
	{
		return ret;
	}

	if(!S_ISDIR(ino.i_mode))
		return -1;

	ext2part.curdir.ino = ino;
	ext2part.curentry = NULL;
	ext2part.lastblock = ext2part.curdir.ino.i_blocks / (ext2part.blocksize/512);
	ext2part.curblock = 0;
	ext2part.curdir.inoNum = inode;

	if(!strcmp(name, "."))
		return 0;
	else if(!strcmp(name, ".."))
		remove_last_name(ext2part.path);
	else
	{
		strcat(ext2part.path, name);
		strcat(ext2part.path, "/");
	}

	return 0;
}

#define IS_BUFFER_END(p)	((char *)(p)->curentry >= ((p)->dirbuf + (p)->blocksize))

int get_dir_entry(DIRENTRY *dir)
{
again:
	if(!ext2part.curentry || IS_BUFFER_END(&ext2part))
	{
		if(ext2part.curblock > ext2part.lastblock)
			return -1;
		read_data_block(&ext2part.curdir.ino, ext2part.curblock, ext2part.dirbuf);
		ext2part.curentry = (EXT2_DIR_ENTRY *)ext2part.dirbuf;
		ext2part.curblock++;
	}

	memcpy(dir->fileName, ext2part.curentry->name, ext2part.curentry->name_len);
	dir->fileName[ext2part.curentry->name_len] = '\0';
	dir->inoNum = ext2part.curentry->inode;
	dir->recLen = ext2part.curentry->rec_len;
	dir->fileType = ext2part.curentry->filetype;

	if(dir->recLen == 0)
		return -1;

	ext2part.curentry = (EXT2_DIR_ENTRY *)((char *)ext2part.curentry + dir->recLen);
	if(!dir->inoNum || !ext2part.curentry->name_len)
	{
		goto again;
	}

	read_inode(dir->inoNum, &dir->ino);
	
	return 0;
}

int get_dentry_inode(DIRENTRY *entry, uint32_t inode)
{
	int ret;

	ext2part.curentry = NULL;
	ext2part.lastblock = ext2part.curdir.ino.i_blocks / (ext2part.blocksize/512);
	ext2part.curblock = 0;

	ret = get_dir_entry(entry);
	if(ret < 0)
		return ret;

	do{
		if(entry->inoNum == inode)
			return 0;

		ret = get_dir_entry(entry);
	}while(ret == 0);

	return -1;
}

int save_folder(const char *path, EXT2_INODE *inode)
{
	int ret;
	DIRENTRY entry;
	char fullpath[512];

	mkdir(path);

	ext2part.curdir.ino = *inode;
	ext2part.curentry = NULL;
	ext2part.lastblock = ext2part.curdir.ino.i_blocks / (ext2part.blocksize/512);
	ext2part.curblock = 0;
	ext2part.curdir.inoNum = inode;

	while(get_dir_entry(&entry) == 0)
	{
		if(!strcmp(entry.fileName, ".") || !strcmp(entry.fileName, ".."))
			continue;

		strcpy(fullpath, path);
		strcat(fullpath, FILE_DELIM);
		strcat(fullpath, entry.fileName);
		__save_file(fullpath, entry.inoNum);
	}

	return 0;
}

int save_file(const char *path, uint32_t ino)
{
	struct ext2partition saved;

	saved = ext2part;
	__save_file(path, ino);
	ext2part = saved;
}

int __save_file(const char *path, uint32_t ino)
{
	int fd, mode, ret, i;
	uint32_t blocks, extra;
	EXT2_INODE inode;
	char *buf;

	mode = O_RDWR|O_CREAT;

#ifdef WIN32
	mode |= O_BINARY;
#endif

	ret = read_inode(ino, &inode);
	if(ret < 0)
		return ret;

	if(S_ISDIR(inode.i_mode))
	{
		save_folder(path, &inode);
		return 0;
	}
	else if(!S_ISREG(inode.i_mode))
		return -1;

	fd = open(path, mode);
	if(fd < 0)
		return -1;

	buf = malloc(ext2part.blocksize);

	blocks = inode.i_size/ext2part.blocksize;
	for(i = 0; i < blocks; i++)
	{
		ret = read_data_block(&inode, i, buf);
		if(ret < 0)
			goto err;

		write(fd, buf, ext2part.blocksize);
	}

	extra = inode.i_size % ext2part.blocksize;
	if(extra)
	{
		read_data_block(&inode, i, buf);
		write(fd, buf, extra);
	}

err:
	free(buf);
	close(fd);
end:
	return 0;
}
