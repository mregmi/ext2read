
#include <ddk/winddk.h>
#include <string.h>

#define DEVICE_NAME			L"\\Device\KvmWin"
#define DEVICE_NAME_DOS		L"\\DosDevices\KvmWin"

DRIVER_INITIALIZE DriverEntry;
DRIVER_DISPATCH SioctlDeviceControl;
DRIVER_UNLOAD SioctlUnloadDriver;

NTSTATUS KvmOpen( PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS KvmClose( PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS KvmIoctl( PDEVICE_OBJECT DeviceObject, PIRP Irp)
{

}

NTSTATUS DriverEntry(
	__in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING  RegistryPath)
{
	NTSTATUS ntStatus;
	UNICODE_STRING deviceName, dosDeviceName;
	PDEVICE_OBJECT deviceObject;

	RtlInitUnicodeString (&deviceName, DEVICE_NAME);
	ntStatus = IoCreateDevice(DriverObject, 0, &deviceName, 
				FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, 
				FALSE, deviceObject);
	if(!NT_SUCCESS(ntStatus))
	{
		//DBG MSG
		return ntStatus;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = KvmOpen;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = KvmClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = KvmClose;

	RtlInitUnicodeString (&dosDeviceName, DEVICE_NAME_DOS);
	ntStatus = IoCreateSymbolicLink(&dosDeviceName, deviceName);
	if(!NT_SUCCESS(ntStatus))
	{
		IoDeleteDevice(deviceObject);
	}

	return ntStatus;
}
