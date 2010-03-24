/**
 * Ext2read
 * File: partition.cpp
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

#include "ext2read.h"

Ext2Partition::Ext2Partition(lloff_t size, lloff_t offset, int ssize, FileHandle phandle)
{
    total_sectors = size;
    relative_sect = offset;
    handle = phandle;
    sect_size = ssize;
    mount();
    inode_buffer = NULL;
}

Ext2Partition::~Ext2Partition()
{
    free (desc);
}

void Ext2Partition::set_linux_name(const char *name, int disk, int partition)
{
    char dchar = 'a' + disk;
    char pchar = '0' + partition;


    linux_name = name;
    linux_name.append(1, dchar);
    linux_name.append(1, pchar);
}

int Ext2Partition::ext2_readblock(int blocknum, void *buffer)
{
        int nsects = blocksize/sect_size;
        lloff_t sectno = (lloff_t)((blocksize/sect_size) * blocknum) + relative_sect;

        return read_disk(handle, buffer, sectno, nsects, sect_size);
}

int Ext2Partition::mount()
{
    EXT2_SUPER_BLOCK sblock;
    int totalGroups;
    int gSizes, gSizeb;		/* Size of total group desc in sectors */
    char *tmpbuf;

    read_disk(handle, &sblock, relative_sect + 2, 2, sect_size);	/* read superBlock of root */
    if(sblock.s_magic != EXT2_SUPER_MAGIC)
    {
            LOG("Bad Super Block. The drive %s is not ext2 formatted.\n", linux_name.c_str());
            return -1;
    }

    blocksize = EXT2_BLOCK_SIZE(&sblock);
    inodes_per_group = EXT2_INODES_PER_GROUP(&sblock);
    inode_size = EXT2_INODE_SIZE(&sblock);

    totalGroups = (sblock.s_blocks_count)/EXT2_BLOCKS_PER_GROUP(&sblock);
    gSizeb = (sizeof(EXT2_GROUP_DESC) * totalGroups);
    gSizes = (gSizeb / sect_size)+1;

    desc = (EXT2_GROUP_DESC *) calloc(totalGroups, sizeof(EXT2_GROUP_DESC));
    if(desc == NULL)
    {
        LOG("Not enough Memory: mount: desc: Exiting\n");
            exit(1);
    }

    if((tmpbuf = (char *) malloc(gSizes * sect_size)) == NULL)
    {
        LOG("Not enough Memory: mount: tmpbuf: Exiting\n");
            exit(1);
    }

    /* Read all Group descriptors and store in buffer */
    /* I really dont know the official start location of Group Descriptor array */
    if((blocksize/sect_size) <= 2)
            read_disk(handle, tmpbuf, relative_sect + ((blocksize/sect_size) + 2), gSizes, sect_size);
    else
            read_disk(handle, tmpbuf, relative_sect + (blocksize/sect_size), gSizes, sect_size);

    memcpy(desc, tmpbuf, gSizeb);

    free(tmpbuf);

    return 0;
}

EXT2DIRENT *Ext2Partition::open_dir(Ext2File *parent)
{
    EXT2DIRENT *dirent;

    if(!parent)
        return NULL;

    dirent = new EXT2DIRENT;

    return dirent;
}

Ext2File *Ext2Partition::read_inode(uint32_t inum)
{
    uint32_t group, index, blknum;
    int inode_index, ret = 0;
    Ext2File *file;

    if(inum == 0)
        return NULL;

    if(!inode_buffer)
    {
        inode_buffer = new char[blocksize];
    }

    group = (inum - 1) / inodes_per_group;
    index = ((inum - 1) % inodes_per_group) * inode_size;
    inode_index = (index % blocksize);
    blknum = desc[group].bg_inode_table + (index / blocksize);

    if(blknum != last_block)
        ret = ext2_readblock(blknum, inode_buffer);

    file = new Ext2File;
    memcpy(&file->inode, inode_buffer + inode_index, inode_size);
    file->inode_num = inum;
    last_block = blknum;

    return file;
}

int Ext2Partition::read_data_block(EXT2_INODE *ino, uint32_t lbn, void *buf)
{
        uint32_t block;

        block = fileblock_to_logical(ino, lbn);

        return ext2_readblock(block, buf);
}

uint32_t Ext2Partition::fileblock_to_logical(EXT2_INODE *ino, uint32_t lbn)
{
    uint32_t block, indlast, dindlast;
    uint32_t tmpblk, sz;

    if(lbn < EXT2_NDIR_BLOCKS)
    {
            return ino->i_block[lbn];
    }

    sz = blocksize/4;
    indlast = sz + EXT2_NDIR_BLOCKS;

/*    if((lbn >= EXT2_NDIR_BLOCKS) && (lbn < indlast))
    {
            block = ino->i_block[EXT2_IND_BLOCK];
            ext2_readblock(block, blkbuf.ind);

            lbn -= EXT2_NDIR_BLOCKS;
            return blkbuf.ind[lbn];
    }

    dindlast = (sz * sz) + indlast;
    if((lbn >= indlast) && (lbn < dindlast))
    {
            block = ino->i_block[EXT2_DIND_BLOCK];
            read_ext2block(block, blkbuf.dind);
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
*/
    return 0;
}
