
#include <ddk/ntddk.h>
#include <string.h>

#define DEVICE_NAME			L"\\Device\KvmWin"
#define DEVICE_NAME_DOS		L"\\DosDevices\KvmWin"

NTSTATUS DDKAPI KvmOpen( PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DDKAPI KvmClose( PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

void DDKAPI KvmUnload(PDRIVER_OBJECT DriverObject)
{
	PDEVICE_OBJECT pDevice = DriverObject->DeviceObject;
	UNICODE_STRING deviceName;

	RtlInitUnicodeString (&deviceName, DEVICE_NAME_DOS);
	IoDeleteSymbolicLink(&deviceName);

	if(pDevice != NULL)
	{
		IoDeleteDevice(pDevice);
	}

}

NTSTATUS DDKAPI KvmIoctl( PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION  irpStack;
    NTSTATUS ntStatus = STATUS_SUCCESS;
	
	irpStack = IoGetCurrentIrpStackLocation(Irp);

	return ntStatus;
}

NTSTATUS DriverEntry(
	PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING  RegistryPath)
{
	NTSTATUS ntStatus;
	UNICODE_STRING deviceName, dosDeviceName;
	PDEVICE_OBJECT deviceObject;

	RtlInitUnicodeString (&deviceName, DEVICE_NAME);
	ntStatus = IoCreateDevice(DriverObject, 0, &deviceName, 
				FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, 
				FALSE, &deviceObject);
	if(!NT_SUCCESS(ntStatus))
	{
		//DBG MSG
		return ntStatus;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = KvmOpen;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = KvmClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = KvmIoctl;
	DriverObject->DriverUnload = KvmUnload;

	RtlInitUnicodeString (&dosDeviceName, DEVICE_NAME_DOS);
	ntStatus = IoCreateSymbolicLink(&dosDeviceName, &deviceName);
	if(!NT_SUCCESS(ntStatus))
	{
		IoDeleteDevice(deviceObject);
	}

	return ntStatus;
}
