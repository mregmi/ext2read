#ifndef __EXT2READIFS_H
#define __EXT2READIFS_H

#include "ifstypes.h"

#define E2DEVICENAME	L"\\ext2fsdriver"

#define E2_NODE_TYPE_GLOB		0xbaad0
#define E2_NODE_TYPE_VCB		0xbaad1
#define E2_NODE_TYPE_FCB		0xbaad2
#define E2_NODE_TYPE_CCB		0xbaad3

typedef struct _E2NODEID {
	uint32_t type;
	uint32_t size;
}E2NODEID;

typedef struct _EXT2PARTITION {
	E2NODEID			vcbNode;
	ERESOURCE			vcbResource;
}EXT2_VCB, PEXT2_VCB;

typedef struct _EXT2IFS {
	E2NODEID				e2Node;
	ERESOURCE				e2Resource;		// For protection of this structure
	PDRIVER_OBJECT			e2DriverObject;	// Driver object pointer for this driver
	NPAGED_LOOKASIDE_LIST	e2buf;			// For header of lookaside list
	LIST_ENTRY				nextE2part;		// Partitions are linked with a simple linked list
	PDEVICE_OBJECT			e2devObject;	// Device object
}EXT2IFS, *PEXT2IFS;


extern NTSTATUS Ext2Create (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	);

extern NTSTATUS Ext2Close (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	);

extern NTSTATUS Ext2Read (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	);
extern NTSTATUS Ext2Write (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	);
extern NTSTATUS Ext2FileInfo (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	);
extern NTSTATUS Ext2DirControl (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	);

#endif // __EXT2READIFS_H
