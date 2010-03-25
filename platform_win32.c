#ifdef WIN32
#include <windows.h>
#include <winioctl.h>
#include <fcntl.h>

#include "platform.h"

#define DEVICE	"\\\\.\\PhysicalDrive0"

FileHandle open_disk(const char *path, int *sect_size)
{
    HANDLE handle;
    DISK_GEOMETRY dsk;
    BOOL bResult;
    DWORD junk;

    handle = CreateFileA(path, GENERIC_READ,
                         FILE_SHARE_READ,
                         NULL,
                         OPEN_EXISTING,
                         0, 0 );

    if(handle == INVALID_HANDLE_VALUE)
    {
        return handle;
    }

    bResult = DeviceIoControl(handle,  // device we are querying
                              IOCTL_DISK_GET_DRIVE_GEOMETRY,  // operation to perform
                              NULL, 0, // no input buffer, so pass zero
                              &dsk, sizeof(dsk),  // output buffer
                              &junk, // discard count of bytes returned
                              (LPOVERLAPPED) NULL);  // synchronous I/O

    if((!bResult) || (dsk.BytesPerSector < SECTOR_SIZE))
        *sect_size = SECTOR_SIZE;
    else
        *sect_size = dsk.BytesPerSector;


    return handle;
}

int get_ndisks()
{
    HANDLE hDevice;               // handle to the drive to be examined
    int ndisks = 0;
    char path[20] = {"\\\\.\\PhysicalDrive0"};

    do {
        //TRACE("NDISKS %s", path);
        hDevice = CreateFileA(path, // drive to open
                              GENERIC_READ,
                              FILE_SHARE_READ,  // share mode
                              NULL,    // default security attributes
                              OPEN_EXISTING,  // disposition
                              0,       // file attributes
                              NULL);   // don't copy any file's attributes
        if(hDevice != INVALID_HANDLE_VALUE)
            CloseHandle(hDevice);
        ndisks++;
        path[17] = (char)('0' + ndisks);
    }while(hDevice != INVALID_HANDLE_VALUE);

    return ndisks - 1;
}

void close_disk(FileHandle handle)
{
    CloseHandle(handle);
}

int read_disk(FileHandle hnd, void *ptr, lloff_t sector, int nsects, int sectorsize)
{
    lloff_t offset;
    DWORD rd, len;
    DWORD low;
    LONG high;
    BOOL ret;

    offset = sector * sectorsize;

    low = (DWORD)(offset & 0xFFFFFFFF);
    high = (DWORD)((offset >> 32)& 0xFFFFFFFF);

    low = SetFilePointer(hnd, low, &high, FILE_BEGIN);

    len = nsects * sectorsize;
    ret = ReadFile(hnd, ptr, len, &rd, NULL);
    if(!ret)
        return -1;

    return rd;
}


int write_disk(FileHandle hnd, void *ptr, lloff_t sector, int nsects, int sectorsize)
{
    lloff_t offset;
    DWORD rd, len;
    DWORD low;
    LONG high;
    BOOL ret;

    offset = sector * sectorsize;

    low = (DWORD)(offset & 0xFFFFFFFF);
    high = (DWORD)((offset >> 32)& 0xFFFFFFFF);

    low = SetFilePointer(hnd, low, &high, FILE_BEGIN);

    len = nsects * sectorsize;
    ret = ReadFile(hnd, ptr, len, &rd, NULL);
    if(!ret)
        return -1;

    return rd;
}

int get_nthdevice(char *path, int ndisks)
{
    static int dev = 0;

    if(!path)
        return -1;

    strcpy(path, DEVICE);
    path[17] = dev + '0';
    dev++;

    if(dev >= ndisks)
    {
        dev = 0;
        return -1;
    }

    return 0;
}

#endif
