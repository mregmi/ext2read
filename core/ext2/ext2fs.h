/* Declarations for ext2 file system */
/* Ripped from Linux Source. copyright Linus Trovaldis.*/
/*         by RRM       */


#ifndef __EXT2FS_H
#define __EXT2FS_H


#if defined(WIN32)
	#include <windows.h>
	#include "platform.h"
	#pragma pack(1)
#endif

#define EXT2_DEFAULT_PREALLOC_BLOCKS	8

/* Special Inode Numbers  */
#define EXT2_BAD_INO			1
#define EXT2_ROOT_INO			2
#define EXT2_ACL_IDX_INO		3
#define EXT2_ACL_DATA_INO		4
#define EXT2_BOOT_LOADER_INO	5
#define EXT2_UNDEL_DIR_INO		6

#define EXT2_GOOD_OLD_FIRST_INO	11

#define EXT2_SUPER_MAGIC		0xEF53	/* EXT2 Fs Magic Number */

#define EXT2_LINK_MAX			32000	/* Max count of links to the file */

/* Block Size Management */
#define EXT2_MIN_BLOCK_SIZE		1024
#define EXT2_MAX_BLOCK_SIZE		4096
#define EXT2_MIN_BLOCK_LOG_SIZE 10

#define  EXT2_BLOCK_SIZE(s)			(EXT2_MIN_BLOCK_SIZE << (s)->s_log_block_size)
#define  EXT2_ACLE_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / sizeof (struct ext2_acl_entry))
#define  EXT2_ADDR_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / sizeof (WORD))

#define EXT2_INODE_SIZE(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 		EXT2_GOOD_OLD_INODE_SIZE : (s)->s_inode_size)

#define EXT2_FIRST_INO(s)	(((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
				 		EXT2_GOOD_OLD_FIRST_INO :  (s)->s_first_ino)

/* EXT2 Fragment Sizes */
#define EXT2_MIN_FRAG_SIZE		1024
#define EXT2_MAX_FRAG_SIZE		4096
#define EXT2_MIN_FRAG_LOG_SIZE	10

#define EXT2_FRAG_SIZE(s)		(EXT2_MIN_FRAG_SIZE << (s)->s_log_frag_size)
#define EXT2_FRAGS_PER_BLOCK(s)	(EXT2_BLOCK_SIZE(s) / EXT2_FRAG_SIZE(s))

/* Block Group Macros */
# define EXT2_BLOCKS_PER_GROUP(s)	((s)->s_blocks_per_group)
# define EXT2_DESC_PER_BLOCK(s)		(EXT2_BLOCK_SIZE(s) / sizeof (EXT2_GROUP_DESC))
# define EXT2_INODES_PER_GROUP(s)	((s)->s_inodes_per_group)
# define EXT2_INODES_PER_BLOCK(s)	(EXT2_BLOCK_SIZE(s) / sizeof (EXT2_INODE))
# define EXT2_BLOCK_SIZE_BITS(s)	((s)->s_log_block_size + 10)

/* Constants relative to the data blocks  */
#define	EXT2_NDIR_BLOCKS		12
#define	EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS
#define	EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define	EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define	EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)

 /*Inode flags  */
#define	EXT2_SECRM_FL			0x00000001 /* Secure deletion */
#define	EXT2_UNRM_FL				0x00000002 /* Undelete */
#define	EXT2_COMPR_FL			0x00000004 /* Compress file */
#define	EXT2_SYNC_FL				0x00000008 /* Synchronous updates */
#define	EXT2_IMMUTABLE_FL		0x00000010 /* Immutable file */
#define EXT2_APPEND_FL			0x00000020 /* writes to file may only append */
#define EXT2_NODUMP_FL			0x00000040 /* do not dump file */
#define EXT2_NOATIME_FL			0x00000080 /* do not update atime */

/* Reserved for compression usage... */
#define EXT2_DIRTY_FL				0x00000100
#define EXT2_COMPRBLK_FL			0x00000200 /* One or more compressed clusters */
#define EXT2_NOCOMP_FL				0x00000400 /* Don't compress */
#define EXT2_ECOMPR_FL				0x00000800 /* Compression error */

/* End compression flags --- maybe not all used */
#define EXT2_BTREE_FL				0x00001000 /* btree format dir */
#define EXT2_RESERVED_FL			0x80000000 /* reserved for ext2 lib */

#define EXT2_FL_USER_VISIBLE		0x00001FFF /* User visible flags */
#define EXT2_FL_USER_MODIFIABLE		0x000000FF /* User modifiable flags */

/* Codes for operating systems */
#define EXT2_OS_LINUX		0
#define EXT2_OS_HURD		1
#define EXT2_OS_MASIX		2
#define EXT2_OS_FREEBSD		3
#define EXT2_OS_LITES		4

/* Revision levels  */
#define EXT2_GOOD_OLD_REV	0	/* The good old (original) format */
#define EXT2_DYNAMIC_REV	1 	/* V2 format w/ dynamic inode sizes */

#define EXT2_CURRENT_REV	EXT2_GOOD_OLD_REV
#define EXT2_MAX_SUPP_REV	EXT2_DYNAMIC_REV

#define EXT2_GOOD_OLD_INODE_SIZE 128

/* Default values for user and/or group using reserved blocks */
#define	EXT2_DEF_RESUID		0
#define	EXT2_DEF_RESGID		0

/* Structure of a directory entry */
#define EXT2_NAME_LEN 255

/* In Linux disk is divided into Blocks. These Blocks are divided into Groups. This   */
/* Structure shows different types of groups but it is not implemented. Not Necessary */
/*
typedef struct tagBLOCK_GROUP
{
	1. The SuperBlock
	2. The Group Descriptors
	3. The block Bitmap
	4. The Inode Bitmap
	5. The Inode Table
	6. Data Blocks and Fragments

}BLOCK_GROUP;

*/

/* The Super Block comes first in the block group */
typedef struct tagEXT2_SUPER_BLOCK
{
	DWORD	s_inodes_count;   	/* total no of inodes */
	DWORD	s_blocks_count;   	/* total no of blocks */
	DWORD	s_r_blocks_count; 	/* total no of blocks reserved for exclusive use  of superuser */
	DWORD	s_free_blocks_count;	/* total no of free blocks */
	DWORD	s_free_inodes_count;	/* total no of free inodes */
	DWORD	s_first_data_block;	/* position of the first data block */
	DWORD	s_log_block_size;	/* used to compute logical block size in bytes */
	DWORD	s_log_frag_size;		/* used to compute logical fragment size  */
	DWORD	s_blocks_per_group;	/* total number of blocks contained in the group  */
	DWORD	s_frags_per_group;	/* total number of fragments in a group */
	DWORD	s_inodes_per_group;	/* number of inodes in a group  */
	DWORD	s_mtime;			/* time at which the last mount was performed */
	DWORD	s_wtime;			/* time at which the last write was performed */
	WORD	s_mnt_count;		/* number of time the fs system has been mounted in r/w mode without having checked */
	WORD	s_max_mnt_count;	/* the max no of times the fs can be mounted in r/w mode before a check must be done */
	WORD	s_magic;			/* a number that identifies the fs (eg. 0xef53 for ext2) */
	WORD	s_state;			/* gives the state of fs (eg. 0x001 is Unmounted cleanly) */
	WORD	s_pad;				/* unused */
	WORD    s_minor_rev_level;	/*	*/
	DWORD	s_lastcheck;		/* the time of last check performed */
	DWORD	s_checkinterval;		/* the max possible time between checks on the fs */
	DWORD	s_creator_os;		/* os */
	DWORD	s_rev_level;			/* Revision level */
	WORD	s_def_resuid;		/* default uid for reserved blocks */
	WORD	s_def_regid;		/* default gid for reserved blocks */

	/* for EXT2_DYNAMIC_REV superblocks only */
	DWORD	s_first_ino; 		/* First non-reserved inode */
	WORD    s_inode_size; 		/* size of inode structure */
	WORD	s_block_group_nr; 	/* block group # of this superblock */
	DWORD	s_feature_compat; 	/* compatible feature set */
	DWORD	s_feature_incompat; 	/* incompatible feature set */
	DWORD	s_feature_ro_compat; 	/* readonly-compatible feature set */
	BYTE	s_uuid[16];		/* 128-bit uuid for volume */
	char	s_volume_name[16]; 		/* volume name */
	char	s_last_mounted[64]; 		/* directory where last mounted */
	DWORD	s_algorithm_usage_bitmap; /* For compression */
	BYTE	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
	BYTE	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
	WORD	s_padding1;
	DWORD	s_reserved[204];		/* unused */
}EXT2_SUPER_BLOCK;

/* The Group Descriptors follow the Super Block. */
typedef struct tagEXT2_GROUP_DESC
{
	DWORD	bg_block_bitmap;	/* points to the blocks bitmap for the group */
	DWORD	bg_inode_bitmap;	/* points to the inodes bitmap for the group */
	DWORD	bg_inode_table;		/* points to the inode table first block     */
	WORD	bg_free_blocks_count;	/* number of free blocks in the group 	     */
	WORD	bg_free_inodes_count;	/* number of free inodes in the		     */
	WORD	bg_used_dirs_count;	/* number of inodes allocated to directories */
	WORD	bg_pad;			/* padding */
	DWORD	bg_reserved[3];		/* reserved */
}EXT2_GROUP_DESC;

/* Structure of an inode on the disk  */
typedef struct tagEXT2_INODE
{
	WORD	i_mode;		/* File mode */
	WORD	i_uid;		/* Low 16 bits of Owner Uid */
	DWORD	i_size;		/* Size in bytes */
	DWORD	i_atime;		/* Access time */
	DWORD	i_ctime;		/* Creation time */
	DWORD	i_mtime;		/* Modification time */
	DWORD	i_dtime;		/* Deletion Time */
	WORD	i_gid;		/* Low 16 bits of Group Id */
	WORD	i_links_count;	/* Links count */
	DWORD	i_blocks;		/* Blocks count */
	DWORD	i_flags;		/* File flags */
	union {
		struct {
			DWORD  l_i_reserved1;
		} linux1;
		struct {
			DWORD  h_i_translator;
		} hurd1;
		struct {
			DWORD  m_i_reserved1;
		} masix1;
	} osd1;				/* OS dependent 1 */
	DWORD	i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
	DWORD	i_generation;	/* File version (for NFS) */
	DWORD	i_file_acl;		/* File ACL */
	DWORD	i_dir_acl;		/* Directory ACL */
	DWORD	i_faddr;		/* Fragment address */
	union {
		struct {
			BYTE	l_i_frag;	/* Fragment number */
			BYTE	l_i_fsize;	/* Fragment size */
			WORD	i_pad1;
			WORD	l_i_uid_high;	/* these 2 fields    */
			WORD	l_i_gid_high;	/* were reserved2[0] */
			DWORD	l_i_reserved2;
		} linux2;
		struct {
			BYTE	h_i_frag;	/* Fragment number */
			BYTE	h_i_fsize;	/* Fragment size */
			WORD	h_i_mode_high;
			WORD	h_i_uid_high;
			WORD	h_i_gid_high;
			WORD	h_i_author;
		} hurd2;
		struct {
			BYTE	m_i_frag;	/* Fragment number */
			BYTE	m_i_fsize;	/* Fragment size */
			WORD	m_pad1;
			DWORD	m_i_reserved2[2];
		} masix2;
	} osd2;					/* OS dependent 2 */
} EXT2_INODE;

/* EXT2 directory structure */
typedef struct tagEXT2_DIR_ENTRY {
	uint32_t	inode;			/* Inode number */
	uint16_t	rec_len;		/* Directory entry length */
	uint8_t 	name_len;		/* Name length */
	uint8_t		filetype;		/* File type */
	char	name[EXT2_NAME_LEN];	/* File name */
}EXT2_DIR_ENTRY;


#endif
