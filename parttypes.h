/*
 * Everest Partition Manager
 * File: parttypes.h
 */
/*
 * Copyright (C) 2005 by Manish Regmi   (regmi.manish@gmail.com)
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
 */

#ifndef __PARTTYPES_H
#define __PARTTYPES_H


#define  PART_TYPE_PRIMARY	0x01
#define  PART_TYPE_LOGICAL	0x02
#define  PART_TYPE_EXTENDED	0x03


#define  EMPTY	0x00
#define  FAT12	0x01
#define  FAT16	0x06
#define  FAT32	0x0c
#define  NTFS	0x07
#define  EXT2	0x83
#define  LVM	0x8e
#define  SWAP	0x82
#define	 RAID	0xfd
#define EXTENDED	0x05

#define IS_FATTYPE(id) ((id == FAT12) || (id == FAT16) ||(id == FAT32) || \
           (id == 0xB)|| (id == 1) || (id == 4) || (id == 6))

#define IS_NTFSTYPE(id)	((id == NTFS))

#define IS_FAT_OLD(id) 	((id == FAT12) || (id == FAT16) || \
           (id == 1) || (id == 4) || (id == 6))

#define IS_FAT_NEW(id) ((id == 0x0B) || (id == FAT32))
/*
#define 00h	// empty
#define 01h	// DOS 12-bit FAT
#define 02h	// XENIX root file system
#define 03h	// XENIX /usr file system (obsolete)
#define 04h	// DOS 16-bit FAT (up to 32M)
#define 05h	// DOS 3.3+ extended partition
#define 06h	// DOS 3.31+ Large File System (16-bit FAT, over 32M)
#define 07h	// QNX
#define 07h	// Windows NT NTFS

#define 08h	// AIX bootable partition, SplitDrive
#define 09h	// AIX data partition
#define 0Ah	// OS/2 Boot Manager
#define 10h	// OPUS
#define 11h	// OS/2 Boot Manager hidden 12-bit FAT partition
#define 12h	// Compaq Diagnostics partition
#define 14h	// OS/2 Boot Manager hidden sub-32M 16-bit FAT partition
#define 16h	// OS/2 Boot Manager hidden over-32M 16-bit FAT partition
#define 17h	// OS/2 Boot Manager hidden HPFS partition
#define 18h	// AST special Windows swap file
#define 24h	// NEC MS-DOS 3.x
#define 3Ch	// PowerQuest PartitionMagic recovery partition
#define 40h	// VENIX 80286
#define 42h	// SFS (Secure File System) by Peter Gutmann
#define 50h	// Disk Manager, read-only partition
#define 51h	// Novell???
#define 52h	// Microport System V/386
#define 56h	// GoldenBow VFeature
#define 61h	// SpeedStor
#define 63h	// Unix SysV/386, 386/ix
#define 64h	// Novell NetWare
#define 65h	// Novell NetWare (3.11)
#define 70h	// DiskSecure Multi-Boot
#define 75h	// PC/IX
#define 80h	// Minix v1.1 - 1.4a
#define 81h	// Minix v1.4b+
#define 81h	// Mitac Advanced Disk Manager
#define 82h	// Linux Swap partition
#define 83h	// Linux native file system (ext2fs/xiafs)
#define 84h	// OS/2-renumbered type 04h partition (related to hiding DOS C: drive)
#define 93h	// Amoeba file system
#define 94h	// Amoeba bad block table
#define A5h	// FreeBSD
#define B7h	// BSDI file system (secondarily swap)
#define B8h	// BSDI swap partition (secondarily file system)
#define C1h	// DR-DOS 6.0 LOGIN.EXE-secured 12-bit FAT partition
#define C4h	// DR-DOS 6.0 LOGIN.EXE-secured 16-bit FAT partition
#define C6h	// DR-DOS 6.0 LOGIN.EXE-secured Huge partition
#define C7h	// Cyrnix Boot
#define DBh	// CP/M, Concurrent CP/M, Concurrent DOS
#define DBh	// CTOS (Convergent Technologies OS)
#define E1h	// SpeedStor 12-bit FAT extended partition
#define E4h	// SpeedStor 16-bit FAT extended partition
#define F2h	// DOS 3.3+ secondary
#define F4h	// SpeedStor
#define FEh	// LANstep
#define FFh	// bad
*/

static const char *part_type(unsigned char type)
{
	switch(type)
	{
	 case  0x06:	return "FAT16";
	 case  0x07:	return "NTFS";
	 case  0x0b:	return "FAT32";
	 case  0x0c:	return "FAT32";
	 case  0x0f:	return "Extended";
	 case  0x82:	return "Linux Swap/Solaris";
	 case  0x83:	return "EXT2fs/EXT3fs";
	 case  0x8e:	return "Linux LVM";
	 case  0x00:	return "empty";
	 case  0x01:	return "DOS 12-bit FAT";
	 case  0x04:	return "DOS 16-bit FAT";
	 case  0x05:	return "Extended";
	 case  0x85:	return "Linux Extended";
	 case  0xfd:	return "Linux Raid";

	 case  0x02:	return "XENIX root";
	 case  0x03:	return "XENIX /usr";
	 case  0x08:	return "AIX bootable";
	 case  0x09:	return "AIX data";
	 case  0x0A:	return "OS/2 Boot Manager";
	 case  0x10:	return "OPUS";
	 case  0x11:	return "OS/2 Boot Manager hidden 12-bit FAT";
	 case  0x12:	return "Compaq Diagnostics";
	 case  0x14:	return "OS/2 Boot Manager hidden sub-32M 16-bit FAT";
	 case  0x16:	return "OS/2 Boot Manager hidden over-32M 16-bit FAT";
	 case  0x17:	return "OS/2 Boot Manager hidden HPFS";
	 case  0x18:	return "AST special Windows swap";
	 case  0x24:	return "NEC MS-DOS";
	 case  0x3C:	return "PowerQuest PartitionMagic recovery partition";
	 case  0x40:	return "VENIX 80286";
	 case  0x42:	return "SFS (Secure File System)";
	 case  0x50:	return "Disk Manager (ro)";
	 case  0x51:	return "Novell";
	 case  0x52:	return "Microport System V/386";
	 case  0x56:	return "GoldenBow VFeature";
	 case  0x61:	return "SpeedStor";
	 case  0x63:	return "GNU HURD";
	 case  0x64:	return "Novell NetWare";
	 case  0x65:	return "Novell NetWare";
	 case  0x70:	return "DiskSecure Multi-Boot";
	 case  0x75:	return "PC/IX";
	 case  0x80:	return "Minix";
	 case  0x81:	return "Minix";
	 case  0x84:	return "OS/2-hidden";
	 case  0x93:	return "Amoeba";
	 case  0x94:	return "Amoeba bad";
	 case  0xA5:	return "FreeBSD";
	 case  0xB7:	return "BSDI";
	 case  0xB8:	return "BSDI swap ";
	 case  0xC1:	return "DR-DOS 6.0 LOGIN.EXE-secured 12-bit FAT";
	 case  0xC4:	return "DR-DOS 6.0 LOGIN.EXE-secured 16-bit";
	 case  0xC6:	return "DR-DOS 6.0 LOGIN.EXE-secured Huge";
 	 case  0xC7:	return "Cyrnix Boot";
	 case  0xDB:	return "CP/M";
	 case  0xE1:	return "SpeedStor 12-bit FAT extended";
	 case  0xE4:	return "SpeedStor 16-bit FAT extended";
	 case  0xF2:	return "DOS 3.3+ secondary";
	 case  0xF4:	return "SpeedStor";
	 case  0xFE:	return "LANstep";
	 case  0xFF:	return "bad";
	}

	return "Unknown";
}

#endif //__PARTTYPES_H
