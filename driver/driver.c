#include "ZwClicker.h"

NTSTATUS DriverEntry(DRIVER_OBJECT *DriverObject, PUNICODE_STRING RegistryPath)
{
	NTSTATUS status;
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING DeviceName;
	UNICODE_STRING SymbolicLinkName;

	DbgPrintEx(0, 0, "DriverEntry...");

	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DeviceCreate;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DeviceClose;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = DeviceWrite;

	RtlInitUnicodeString(&DeviceName, DEVICE_NAME);
	status = IoCreateDevice(DriverObject, sizeof(PDEVICE_OBJECT), &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status))
		return status;
	DeviceObject->Flags |= DO_BUFFERED_IO;

	DbgPrintEx(0, 0, "IoCreateDevice...");

	RtlInitUnicodeString(&SymbolicLinkName, DRIVER_SYMBOLIC_LINK);
	status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);
	if (!NT_SUCCESS(status))
		return status;

	DbgPrintEx(0, 0, "IoCreateSymbolicLink...");

	return STATUS_SUCCESS;
}

VOID DriverUnload(DRIVER_OBJECT *DriverObject)
{
	UNICODE_STRING SymbolicLinkName;

	DbgPrintEx(0, 0, "DriverUnload...");

	RtlInitUnicodeString(&SymbolicLinkName, DRIVER_SYMBOLIC_LINK);
	IoDeleteSymbolicLink(&SymbolicLinkName);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS DeviceCreate(DEVICE_OBJECT *DeviceObject, IRP *Irp)
{
	DbgPrintEx(0, 0, "DeviceCreate...");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DeviceClose(DEVICE_OBJECT *DeviceObject, IRP *Irp)
{
	DbgPrintEx(0, 0, "DeviceClose...");

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DeviceWrite(DEVICE_OBJECT *DeviceObject, IRP *Irp)
{
	IO_STACK_LOCATION *IrpStack;
	UCHAR KeyCode;

	//DbgPrintEx(0, 0, "DeviceWrite...");
	
	IrpStack = IoGetCurrentIrpStackLocation(Irp);
	KeyCode = *(UCHAR *)Irp->AssociatedIrp.SystemBuffer;

	while ((READ_PORT_UCHAR((PUCHAR)0x64) & 3) != 0);
	WRITE_PORT_UCHAR((PUCHAR)0x64, (UCHAR)0xD2);
	while ((READ_PORT_UCHAR((PUCHAR)0x64) & 2) != 0);
	WRITE_PORT_UCHAR((PUCHAR)0x60, KeyCode);

	KeyCode |= 0x80;

	while ((READ_PORT_UCHAR((PUCHAR)0x64) & 3) != 0);
	WRITE_PORT_UCHAR((PUCHAR)0x64, (UCHAR)0xD2);
	while ((READ_PORT_UCHAR((PUCHAR)0x64) & 2) != 0);
	WRITE_PORT_UCHAR((PUCHAR)0x60, KeyCode);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = IrpStack->Parameters.Write.Length;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}