
#ifdef __unix__
#include "platform.h"

FileHandle open_disk(const char *path, struct disk *dsk)
{
}

int get_ndisks()
{

}

void close_disk(FileHandle handle)
{

}

int read_disk(FileHandle handle, void *ptr, lloff_t sector, int nsects, int sectorsize)
{
}


int write_disk(FileHandle handle, void *ptr, lloff_t sector, int nsects, int sectorsize)
{
}

#endif
