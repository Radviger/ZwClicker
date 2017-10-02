#include <ntddk.h>

#define DEVICE_NAME L"\\Device\\ZwClicker"
#define DRIVER_SYMBOLIC_LINK L"\\??\\デバイスドライバ名はみゆきです" //why not? :P

NTSTATUS DriverEntry (DRIVER_OBJECT *DriverObject, PUNICODE_STRING RegistryPath);
VOID     DriverUnload(DRIVER_OBJECT *DriverObject);
NTSTATUS DeviceCreate(DEVICE_OBJECT *DeviceObject, IRP *Irp);
NTSTATUS DeviceClose (DEVICE_OBJECT *DeviceObject, IRP *Irp);
NTSTATUS DeviceWrite (DEVICE_OBJECT *DeviceObject, IRP *Irp);