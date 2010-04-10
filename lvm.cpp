/**
 * Ext2read
 * File: lvm.cpp
 **/
/**
 * Copyright (C) 2006 2010 by Manish Regmi   (regmi dot manish at gmail.com)
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

#include <stdlib.h>
#include <sstream>
#include <QRegExp>

#include "lvm.h"


LVM::LVM(FileHandle handle, lloff_t offset, Ext2Read *rd)
{
    pv_handle = handle;
    pv_offset = offset;
    ext2read = rd;
}

LVM::~LVM()
{

}

int LVM::scan_pv()
{
    int ret;
    PV_LABEL_HEADER *header;
    PV_LABEL *label;
    lloff_t sector;
    int length;
    char buffer[SECTOR_SIZE];
    char *metadata;

    sector = pv_offset + 1;
    ret = read_disk(pv_handle, buffer, sector, 1, SECTOR_SIZE);

    header = (PV_LABEL_HEADER *) &buffer[0];
    if(strncmp(header->pv_name, "LABELONE", LVM_SIGLEN) != 0)
    {
        LOG("Invalid label. The partition is not LVM2 volume\n");
        return -1;
    }

    LOG("PV Metadata: %s %UUID=%s offset %d \n",header->pv_name, header->pv_uuid, header->pv_labeloffset);

    sector = (header->pv_labeloffset/SECTOR_SIZE) + pv_offset;
    read_disk(pv_handle, buffer, sector, 1, SECTOR_SIZE);
    label = (PV_LABEL *) &buffer[0];

    sector = pv_offset + ((label->pv_offset_low + label->pv_offset_high)/SECTOR_SIZE);
    length = (label->pv_length + SECTOR_SIZE - 1)/SECTOR_SIZE;
    metadata = new char[length * SECTOR_SIZE];
    read_disk(pv_handle, metadata, sector, length, SECTOR_SIZE);

    metadata[label->pv_length] = 0;
    LOG("\n%s", metadata);
    pv_metadata.append(metadata);
    parse_metadata();
    delete [] metadata;
    return 0;
}

// NOTE: Do error checking
int LVM::parse_metadata()
{
    int num, tmpnum;
    QString volname, suuid;
    int seq, size;
    bool ok;
    QByteArray ba;

    num = pv_metadata.indexOf("{");
    volname = pv_metadata.left(num - 1);
    num = pv_metadata.indexOf(QRegExp("[a-zA-Z0-9]*-{1,}[a-zA-Z0-9]*"), 0);
    if(num > 0)
    {
        suuid = pv_metadata.mid(num, 38);
        suuid.replace("-", "");
    }
    num = pv_metadata.indexOf(QRegExp("[0-9]"), num + 38);
    if(num > 0)
    {
        seq = pv_metadata.mid(num, 1).toInt(&ok);
        if(!ok)
        {
            LOG("Cannot Parse LVM Metadata :-( \n");
            return -1;
        }
    }
    num = pv_metadata.indexOf(QRegExp("[0-9]+"), num + 1);
    if(num > 0)
    {
        size = pv_metadata.mid(num, 4).toInt(&ok);
        if(!ok)
        {
            LOG("Cannot Parse LVM Metadata :-( \n");
            return -1;
        }
    }

    /*ba = pv_metadata.toAscii();
    const char *c_str2 = ba.data();
    LOG("Group %s\n", c_str2);

    ba = suuid.toAscii();
    c_str2 = ba.data();
    LOG("UUID %s\n", c_str2);
*/
    return 0;
}

