/**
 * Ext2read
 * File: lvm.h
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

#ifndef __LVM_H
#define __LVM_H

#include <stdint.h>
#include <QString>

#include "ext2read.h"

#define LVM_SIGLEN	8
#define LVM_MAGIC_LEN	8
#define UUID_LEN	32

/* Structure to hold Physical Volumes (PV) label*/
typedef struct pv_label_header {
        char        pv_name[LVM_SIGLEN];   // Physical volume signature
        uint64_t    pv_sector_xl;          // sector number of this label
        uint32_t    pv_crc;                // CRC value
        uint32_t    pv_offset_xl;          // Offset from start of struct to contents
        char        pv_vermagic[LVM_MAGIC_LEN]; // Physical Volume version "LVM2 001"
        char        pv_uuid[UUID_LEN];
        uint64_t    pv_unknown1[5];             // documentation lacks for lvm
        uint64_t    pv_labeloffset;             // location of the label
} __attribute__ ((__packed__)) PV_LABEL_HEADER;

typedef struct pv_label {
    uint32_t        pv_magic;
    char            pv_sig[4];          // signature
    uint64_t        unknown1[2];
    uint64_t        pv_offset_low;
    uint64_t        unknown2;
    uint64_t        pv_offset_high;
    uint64_t        pv_length;
} __attribute__ ((__packed__)) PV_LABEL;

class VolumeGroup;

class LVM {
private:
    FileHandle pv_handle;
    lloff_t pv_offset;
    char uuid[UUID_LEN + 1];
    QString pv_metadata;
    Ext2Read *ext2read;
public:
    LVM(FileHandle handle, lloff_t offset, Ext2Read *rd);
    ~LVM();

    int scan_pv();
    int parse_metadata();
    VolumeGroup *find_volgroup(QString &uuid);
    VolumeGroup *add_volgroup(QString &uuid, QString &name, int seq, int size);
};


class PhysicalVolume {
    FileHandle handle;
    lloff_t dev_size;
    uint32_t pe_start, pe_count;
public:
    QString uuid;
    PhysicalVolume(QString &id, lloff_t devsize, uint32_t start, uint32_t count, FileHandle file);
};

// Multiple stripes NOT Implemented: we only support linear for now.
struct stripe {
    int stripe_pv;
    uint32_t stripe_start_extent;
};

class lv_segment {
public:
    uint32_t start_extent;
    uint32_t extent_count;
    struct stripe *stripe;
    PhysicalVolume *pvolumes;

    lv_segment(uint32_t start, uint32_t count)
    {
        start_extent = start;
        extent_count = count;
    }
};

class LogicalVolume {
    int segment_count;
    VolumeGroup *this_group;
    std::list <PhysicalVolume *> pvolumes;
public:
    QString uuid;
    std::list <lv_segment *> segments;

    LogicalVolume(QString &id, int nsegs);
    ~LogicalVolume();
};

class VolumeGroup {
public:
    QString volname;
    QString uuid;
    int extent_size;
    int seqno;
    int max_lv, max_pv;
    std::list <PhysicalVolume *> pvolumes;
    std::list <LogicalVolume *> lvolumes;

public:
    VolumeGroup(QString &id, QString &name, int seq, int size);
    ~VolumeGroup();
    PhysicalVolume *find_physical_volume(QString &id);
    PhysicalVolume *add_physical_volume(QString &id, lloff_t devsize, uint32_t start, uint32_t count, FileHandle file);
    LogicalVolume *find_logical_volume(QString &id);
    LogicalVolume *add_logical_volume(QString &id, int count);
};

#ifdef __cplusplus
extern "C"{
#endif
int scan_lvm(FileHandle handle, lloff_t offset);

#ifdef __cplusplus
}
#endif

#endif // __LVM_H
