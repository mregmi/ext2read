
#include <ddk\ntddk.h>
#include <ddk\ntifs.h>

#include "ext2readifs.h"

#ifdef ALLOC_PRAGMA
 #pragma alloc_text( init, DriverEntry )
#endif

/**
 * Global structure to store info on this driver
 */
EXT2IFS		e2ifs;

/**
 * Driver Unload Function
 */


/**
 * Driver Entry Point
 */
NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT pDriverObject,
	IN PUNICODE_STRING pRegistryPath
	)
{
	UNICODE_STRING	devName;
	NTSTATUS status;

	DbgPrint("Initializing sample driver\n");
	RtlZeroMemory(&e2ifs, sizeof(e2ifs));

	status = ExInitializeResourceLite(&e2ifs.e2Resource);
	if(!NT_SUCCESS(status))
	{
		DbgPrint("Resource initialize failed\n");
		return status;
	}

	e2ifs.e2DriverObject = pDriverObject;
	InitializeListHead(&e2ifs.nextE2part);

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = Ext2Create;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = Ext2Close;
	pDriverObject->MajorFunction[IRP_MJ_READ] = Ext2Read;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = Ext2Write;
	pDriverObject->MajorFunction[IRP_MJ_QUERY_INFORMATION] = Ext2FileInfo;
	pDriverObject->MajorFunction[IRP_MJ_SET_INFORMATION] = Ext2FileInfo;
	pDriverObject->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = Ext2DirControl;


	RtlInitUnicodeString(&devName, E2DEVICENAME);

	status = IoCreateDevice(
				pDriverObject,
				0,
				&devName,
				FILE_DEVICE_DISK_FILE_SYSTEM,
				0,
				FALSE,
				&e2ifs.e2devObject
				);
	if(!NT_SUCCESS(status))
	{
		DbgPrint("Create Device failed\n");
		ExDeleteResourceLite(&e2ifs.e2Resource);
		return status;
	}

	IoRegisterFileSystem();
/*	status = IoRegisterFileSystem(e2ifs.e2devObject);
	if(!NT_SUCCESS(status))
	{
		DbgPrint("Register Filesystem failed.\n");
		ExDeleteResourceLite(&e2ifs.e2Resource);
		IoDeleteDevice(e2ifs.e2devObject);
		return status;
	}
*/
	return STATUS_SUCCESS;
}


void DriverUnload(
	IN PDRIVER_OBJECT pDriverObject
	)
{
	DbgPrint("Unloading ext2readifs driver\n");
	ExDeleteResourceLite(&e2ifs.e2Resource);
	IoDeleteDevice(e2ifs.e2devObject);
}
