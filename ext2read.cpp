/**
 * Ext2read
 * File: ext2read.cpp
 **/
/**
 * Copyright (C) 2005, 2010 by Manish Regmi   (regmi dot manish at gmail.com)
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

#include <dirent.h>

#include "ext2read.h"
#include "platform.h"
#include "partition.h"
#include "parttypes.h"
#include "lvm.h"



Ext2Read::Ext2Read()
{
    scan_system();
}

Ext2Read::~Ext2Read()
{
    clear_partitions();
}

void Ext2Read::scan_system()
{
    char pathname[20];
    int ret;

    ndisks = get_ndisks();
    LOG_INFO("No of disks %d\n", ndisks);

    for(int i = 0; i < ndisks; i++)
    {
        get_nthdevice(pathname, ndisks);
        LOG_INFO("Scanning %s\n", pathname);
        ret = scan_partitions(pathname, i);
        if(ret < 0)
        {
            LOG_INFO("Scanning of %s failed: ", pathname);
            continue;
        }
    }
}

void Ext2Read::clear_partitions()
{
    list<Ext2Partition *>::iterator i;
    for(i = nparts.begin(); i != nparts.end(); i++)
    {
        delete *i;
    }

    nparts.clear();
}

list<Ext2Partition *> Ext2Read::get_partitions()
{
    return nparts;
}


/* Reads The Extended Partitions */
int Ext2Read::scan_ebr(FileHandle handle, lloff_t base, int sectsize, int disk)
{
    unsigned char sector[SECTOR_SIZE];
    struct MBRpartition *part, *part1;
    Ext2Partition *partition;
    int logical = 4, ret;
    lloff_t  ebrBase, nextPart, ebr2=0;

    ebrBase = base;
    nextPart = base;
    while(1)
    {
        ret = read_disk(handle, sector, nextPart, 1, sectsize);
        if(ret < 0)
            return ret;

        if(ret < sectsize)
        {
            LOG("Error Reading the EBR \n");
            return -1;
        }
        part = pt_offset(sector, 0);
        LOG("index %d ID %X size %Ld \n", logical, part->sys_ind, get_nr_sects(part));

        if(part->sys_ind == EXT2)
        {
            partition = new Ext2Partition(get_nr_sects(part), get_start_sect(part)+ ebrBase + ebr2, sectsize, handle);
            if(partition->is_valid)
            {
                partition->set_linux_name("/dev/sd", disk, logical);
                nparts.push_back(partition);
            }
            else
            {
                delete partition;
            }
        }

        if(part->sys_ind == LINUX_LVM)
        {
            LOG("LVM Physical Volume found disk %d partition %d\n", disk, logical);
            LVM lvm(handle, get_start_sect(part)+ ebrBase + ebr2);
            lvm.scan_pv();
        }

        part1 = pt_offset(sector, 1);
        ebr2 = get_start_sect(part1);
        nextPart = (ebr2 + ebrBase);

        logical++;
        if(part1->sys_ind == 0)
            break;
    }
    return logical;
}


/* Scans The partitions */
int Ext2Read::scan_partitions(char *path, int diskno)
{
    unsigned char sector[SECTOR_SIZE];
    struct MBRpartition *part;
    Ext2Partition *partition;
    FileHandle handle;
    int sector_size;
    int ret, i;

    handle = open_disk(path, &sector_size);
    if(handle < 0)
        return -1;

    ret = read_disk(handle,sector, 0, 1, sector_size);
    if(ret < 0)
        return ret;

    if(ret < sector_size)
    {
        LOG("Error Reading the MBR on %s \n", path);
        return -1;
    }
   // part = pt_offset(sector, 0);
    if(!valid_part_table_flag(sector))
    {
        LOG("Partition Table Error on %s\n", path);
        LOG("Invalid End of sector marker");
        return -INVALID_TABLE;
    }

    /* First Scan primary Partitions */
    for(i = 0; i < 4; i++)
    {
        part = pt_offset(sector, i);
        if((part->sys_ind != 0x00) || (get_nr_sects(part) != 0x00))
        {
            LOG("index %d ID %X size %Ld \n", i, part->sys_ind, get_nr_sects(part));

            if(part->sys_ind == EXT2)
            {
                partition = new Ext2Partition(get_nr_sects(part), get_start_sect(part), sector_size, handle);
                if(partition->is_valid)
                {
                    partition->set_linux_name("/dev/sd", diskno, i);
                    nparts.push_back(partition);
                    LOG("Linux Partition found on disk %d partition %d\n", diskno, i);
                }
                else
                {
                    delete partition;
                }
            }

            if(part->sys_ind == LINUX_LVM)
            {
                LOG("LVM Physical Volume found disk %d partition %d\n", diskno, i);
                LVM lvm(handle, get_start_sect(part));
                lvm.scan_pv();
            }
            else if((part->sys_ind == 0x05) || (part->sys_ind == 0x0f))
            {
                scan_ebr(handle, get_start_sect(part), sector_size, diskno);
            }
        }
    }

    return 0;
}

int Ext2Read::add_loopback(const char *file)
{
    int ret, sector_size;
    Ext2Partition *partition;
    FileHandle handle;

    ndisks++;
   ret = scan_partitions((char *)file, ndisks);
   if(ret == -INVALID_TABLE)
   {
       handle = open_disk(file, &sector_size);
       partition = new Ext2Partition(0, 0, sector_size, handle);
       if(partition->is_valid)
       {
            partition->set_image_name(file);
            nparts.push_back(partition);
            LOG("Linux Partition found on disk %d partition %d\n", ndisks, 0);
            return 1;
        }
       else
       {
           delete partition;
       }
   }
   return 0;
}
