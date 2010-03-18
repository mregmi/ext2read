
#include <ddk\ntifs.h>
#include <ddk\ntddk.h>

#include "ext2readifs.h"

NTSTATUS Ext2Create (
	IN PDEVICE_OBJECT	deviceObject,
	IN PIRP			irp
	)
{
	PIO_STACK_LOCATION ioStackPointer;

	ioStackPointer = IoGetCurrentIrpStackLocation(irp);
	if(deviceObject->Size == (unsigned short)sizeof(DEVICE_OBJECT))
	{
		irp->IoStatus.Status = STATUS_SUCCESS;
		irp->IoStatus.Information = FILE_OPENED;
		IoCompleteRequest(irp, IO_NO_INCREMENT);

		return STATUS_SUCCESS;
	}


	if(ioStackPointer->FileObject->FileName.Length == 0)
	{
	}
	else
	{
	}

	return FALSE;
}

NTSTATUS Ext2Close (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	)
{
	return FALSE;
}

NTSTATUS Ext2Read (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	)
{
	return FALSE;
}

NTSTATUS Ext2Write (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	)
{
	return FALSE;
}

NTSTATUS Ext2FileInfo (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	)
{
	return FALSE;
}

NTSTATUS Ext2DirControl (
	IN PDEVICE_OBJECT	driverObject,
	IN PIRP			irp
	)
{
	return FALSE;
}

