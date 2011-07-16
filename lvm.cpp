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

VolumeGroup *LVM::find_volgroup(QString &uuid)
{
    VolumeGroup *grp;
    list<VolumeGroup *>::iterator i;
    list <VolumeGroup *> grouplist = ext2read->get_volgroups();


    for(i = grouplist.begin(); i != grouplist.end(); i++)
    {
        grp = (*i);
        if(grp->uuid.compare(uuid) == 0)
        {
            return grp;
        }
    }

    return NULL;
}

VolumeGroup *LVM::add_volgroup(QString &uuid, QString &name, int seq, int size)
{
    VolumeGroup *vol;
    //list <VolumeGroup *> grouplist = ext2read->get_volgroups();

    vol = new VolumeGroup(uuid, name, seq, size);
    if(!vol)
        return NULL;

    ext2read->groups.push_back(vol);
    LOG("Volgroup added \n");
    return vol;
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

    for(int i = 0; i < 4; i++)
    {
        sector = pv_offset + i;
        ret = read_disk(pv_handle, buffer, sector, 1, SECTOR_SIZE);

        header = (PV_LABEL_HEADER *) &buffer[0];
        if(strncmp(header->pv_name, "LABELONE", LVM_SIGLEN) != 0)
        {
            continue;
            //LOG("Invalid label. The partition is not LVM2 volume\n");
            //return -1;
        }

        LOG("PV Metadata: %s %UUID=%s offset %d \n",header->pv_name, header->pv_uuid, header->pv_labeloffset);
        strncpy(uuid, header->pv_uuid, UUID_LEN);
        uuid[UUID_LEN] = '\0';
        sector = (header->pv_labeloffset/SECTOR_SIZE) + pv_offset;
        read_disk(pv_handle, buffer, sector, 1, SECTOR_SIZE);
        label = (PV_LABEL *) &buffer[0];

        sector = pv_offset + ((label->pv_offset_low + label->pv_offset_high)/SECTOR_SIZE);
        length = (label->pv_length + SECTOR_SIZE - 1)/SECTOR_SIZE;
        metadata = new char[length * SECTOR_SIZE];
        read_disk(pv_handle, metadata, sector, length, SECTOR_SIZE);

        metadata[label->pv_length] = 0;
        LOG("\n%s", metadata);
        pv_metadata = QString::fromAscii(metadata, label->pv_length);
        parse_metadata();
        delete [] metadata;
        break;
    }
    return 0;
}

// NOTE: Do error checking
int LVM::parse_metadata()
{
    int num, num2, numbase;
    QString volname, suuid;
    int seq = 0, size = 0;
    bool ok;
    VolumeGroup *grp;
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

    LOG("Volgroup found seq %d, extent_size %d\n",seq, size);
    grp = find_volgroup(suuid);
    if(!grp)
    {
        grp = add_volgroup(suuid, volname, seq, size);
        grp->set_ext2read(ext2read);
    }

    // Parse Physical Volume
    lloff_t dev_size;
    uint32_t pe_start, pe_count;
    num = pv_metadata.indexOf("physical_volumes", 0);
    if(num < 0)
        return -1;

    while((num = pv_metadata.indexOf(QRegExp("pv[0-9\\s\\t]+\\{"), num)) > 0)
    {
        num = pv_metadata.indexOf(QRegExp("[a-zA-Z0-9]*-{1,}[a-zA-Z0-9]*"), num);
        if(num < 0)
            break;

        suuid = pv_metadata.mid(num, 38);
        suuid.replace("-", "");
        num += 38;
        numbase = num;
        num = pv_metadata.indexOf(QRegExp("dev_size"), num);
        num = pv_metadata.indexOf(QRegExp("[0-9]+"), num);
        num2 = pv_metadata.indexOf(QRegExp("\\n"), num);
        dev_size = pv_metadata.mid(num, num2-num).toULongLong(&ok);
        if(!ok)
        {
            LOG("Cannot Parse LVM Metadata (Physical Volume) :-( \n");
            return -1;
        }

        num = pv_metadata.indexOf(QRegExp("pe_start"), numbase);
        num = pv_metadata.indexOf(QRegExp("[0-9]+"), num);
        num2 = pv_metadata.indexOf(QRegExp("\\n"), num);
        pe_start = pv_metadata.mid(num, num2-num).toUInt(&ok);
        if(!ok)
        {
            LOG("Cannot Parse LVM Metadata :-( \n");
            return -1;
        }

        num = pv_metadata.indexOf(QRegExp("pe_count"), numbase);
        num = pv_metadata.indexOf(QRegExp("[0-9]+"), num);
        num2 = pv_metadata.indexOf(QRegExp("\\n"), num);
        pe_count = pv_metadata.mid(num, num2-num).toUInt(&ok);
        if(!ok)
        {
            LOG("Cannot Parse LVM Metadata (Physical Volume) :-( \n");
            return -1;
        }

        LOG("Physical Volume found. start %d, count %d, size %Ld\n", pe_start, pe_count, dev_size);
        PhysicalVolume *pvol;
        pvol = grp->find_physical_volume(suuid);
        num2 = suuid.compare(uuid);
        if(!pvol && (num2 == 0))
        {
            pvol = grp->add_physical_volume(suuid, dev_size, pe_start, pe_count, pv_handle, pv_offset);
        }
    }

    // Parse Logical Volume
    int nsegs;
    num = pv_metadata.indexOf(QRegExp("logical_volumes"), 0);
    if(num < 0)
        return -1;
    num = pv_metadata.indexOf(QRegExp("\\n"), num);
    num += 2;
    QString lvolname = volname;
    while((num = pv_metadata.indexOf(QRegExp("[a-zA-Z_0-9\\s\\t]+\\{"), num)) > 0)
    {
        num2 = pv_metadata.indexOf(QRegExp("[\\s\\t]+\\{"), num);
        lvolname.append("_");
        lvolname.append(pv_metadata.mid(num, num2-num));
        num = pv_metadata.indexOf(QRegExp("[a-zA-Z0-9]*-{1,}[a-zA-Z0-9]*"), num);
        if(num < 0)
            break;

        suuid = pv_metadata.mid(num, 38);
        suuid.replace("-", "");
        num += 38;
        num = pv_metadata.indexOf("flags", num);
        num = pv_metadata.indexOf(QRegExp("[0-9]+"), num);
        num2 = pv_metadata.indexOf(QRegExp("\\n"), num);
        nsegs = pv_metadata.mid(num, num2-num).toInt(&ok);
        if(!ok)
        {
            LOG("Cannot Parse LVM Metadata (Logical Volume) :-( \n");
            return -1;
        }

        LogicalVolume *lvol;
        uint32_t start_extent, extent_count;
        lvol = grp->find_logical_volume(suuid);
        if(!lvol)
            lvol = grp->add_logical_volume(suuid, nsegs, lvolname);
        LOG("Logical Volume found. Name %s, segments %d\n", lvolname.toAscii().data(), nsegs);
        for(int i = 0; i < nsegs; i++)
        {
            num = pv_metadata.indexOf(QRegExp("segment[0-9]+"), num);
            num += 8;
            numbase = num;
            num = pv_metadata.indexOf("start_extent", num);
            num = pv_metadata.indexOf(QRegExp("[0-9]+"), num);
            num2 = pv_metadata.indexOf(QRegExp("\\n"), num);
            start_extent = pv_metadata.mid(num, num2-num).toInt(&ok);

            num = pv_metadata.indexOf("extent_count", numbase);
            num = pv_metadata.indexOf(QRegExp("[0-9]+"), num);
            num2 = pv_metadata.indexOf(QRegExp("\\n"), num);
            extent_count = pv_metadata.mid(num, num2-num).toInt(&ok);

            // Multiple stripes NOT Implemented: we only support linear for now.
            lv_segment *seg = new lv_segment(start_extent, extent_count);
            seg->stripe = new struct stripe;
            seg->stripe->stripe_pv = 0;
            num = pv_metadata.indexOf(QRegExp("pv[0-9]+"), num);
            num += 4;

            num = pv_metadata.indexOf(QRegExp("[0-9]+"), num);
            num2 = pv_metadata.indexOf(QRegExp("\\n"), num);
            seg->stripe->stripe_start_extent = pv_metadata.mid(num, num2-num).toInt(&ok);
            seg->pvolumes = NULL;   // we do the segment -> pv mapping later because this pv might not be found yet
            lvol->segments.push_back(seg);
            LOG("Segment found. start %d, count %d\n", start_extent, extent_count);
        }
    }

    return 0;
}


VolumeGroup::VolumeGroup(QString &id, QString &name, int seq, int size)
{
    uuid = id;
    volname = name;
    seqno = seq;
    extent_size = size;
}

VolumeGroup::~VolumeGroup()
{
    list<PhysicalVolume *>::iterator i;
    list<LogicalVolume *>::iterator j;
    for(i = pvolumes.begin(); i != pvolumes.end(); i++)
    {
        delete (*i);
    }

    for(j = lvolumes.begin(); j != lvolumes.end(); j++)
    {
        delete (*j);
    }
}

PhysicalVolume *VolumeGroup::find_physical_volume(QString &id)
{
    PhysicalVolume *pvol;
    list<PhysicalVolume *>::iterator i;

    for(i = pvolumes.begin(); i != pvolumes.end(); i++)
    {
        pvol = (*i);
        if(pvol->uuid.compare(id) == 0)
        {
            return pvol;
        }
    }

    return NULL;
}

PhysicalVolume *VolumeGroup::add_physical_volume(QString &id, lloff_t devsize, uint32_t start, uint32_t count, FileHandle file, lloff_t dsk_offset)
{
    PhysicalVolume *pvol;

    pvol = new PhysicalVolume(id, devsize, start, count, file, dsk_offset);
    if(!pvol)
        return NULL;

    pvolumes.push_back(pvol);
    return pvol;
}

LogicalVolume *VolumeGroup::find_logical_volume(QString &id)
{
    LogicalVolume *lvol;
    list<LogicalVolume *>::iterator i;

    for(i = lvolumes.begin(); i != lvolumes.end(); i++)
    {
        lvol = (*i);
        if(lvol->uuid.compare(id) == 0)
        {
            return lvol;
        }
    }

    return NULL;
}

LogicalVolume *VolumeGroup::add_logical_volume(QString &id, int count, QString &vname)
{
    LogicalVolume *lvol;

    lvol = new LogicalVolume(id, count, vname, this);
    if(!lvol)
        return NULL;

    lvolumes.push_back(lvol);
    return lvol;
}

void VolumeGroup::logical_mount()
{
    //EXT2_SUPER_BLOCK sblock;
    LogicalVolume *lvol;
    //PhysicalVolume *pvol;
    Ext2Partition *partition;
    lv_segment *seg;
    lv_segment *root = NULL;
    list<LogicalVolume *>::iterator i;
    list<lv_segment *>::iterator j;
    list<PhysicalVolume *>::iterator k;
    lloff_t start;
    int index = 0;

    for(i = lvolumes.begin(); i != lvolumes.end(); i++)
    {
        lvol = (*i);
        for(j = lvol->segments.begin(); j != lvol->segments.end(); j++)
        {
            seg = (*j);
            if(seg->start_extent == 0)
                root = seg;

            index = 0;
            for(k = pvolumes.begin(); k != pvolumes.end(); k++)
            {
                if(seg->stripe->stripe_pv == index)
                {
                    seg->pvolumes = (*k);
                    break;
                }
                index++;
            }
        }

        if(!root)
            continue;

        start = root->pvolumes->pe_start + root->pvolumes->offset + root->stripe->stripe_start_extent;
        partition = new Ext2Partition(root->pvolumes->dev_size, start, SECTOR_SIZE, root->pvolumes->handle, lvol);
        if(partition->is_valid)
        {
            QByteArray ba;
            ba = lvol->volname.toAscii();
            partition->set_image_name(ba.data());
            ext2read->add_partition(partition);
        }
        else
        {
            delete partition;
        }
    }
}

PhysicalVolume::PhysicalVolume(QString &id, lloff_t devsize, uint32_t start, uint32_t count, FileHandle file, lloff_t dsk_offset)
{
    uuid = id;
    dev_size = devsize;
    pe_start = start;
    pe_count = count;
    handle = file;
    offset = dsk_offset;
}

LogicalVolume::LogicalVolume(QString &id, int nsegs, QString &vname, VolumeGroup *vol)
{
    uuid = id;
    segment_count = nsegs;
    this_group = vol;
    volname = vname;
}

LogicalVolume::~LogicalVolume()
{

}

lloff_t LogicalVolume::lvm_mapper(lloff_t sectno)
{
    lv_segment *seg;
    lloff_t sect_mapped = 0;
    uint32_t extent_no, extent_offset;
    list<lv_segment *>::iterator iterate;

    extent_no = sectno / this_group->extent_size;
    extent_offset = sectno % this_group->extent_size;

    for(iterate = segments.begin(); iterate != segments.end(); iterate++)
    {
        seg = (*iterate);
        if((extent_no >= seg->start_extent) && (extent_no < (seg->start_extent + seg->extent_count)))
        {
            sect_mapped = (lloff_t)(extent_no *  this_group->extent_size) + extent_offset;
            sect_mapped += seg->pvolumes->pe_start + seg->pvolumes->offset + seg->stripe->stripe_start_extent;
            break;
        }
    }

    if(sect_mapped == 0)
    {
        LOG("Error in LVM Mapping \n");
    }
    return sect_mapped;
}
