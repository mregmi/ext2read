#include "platform.h"


#ifndef __msdos__ && !defined WIN32

FileHandle open_disk(const char *path)
{
}

int get_ndisks()
{

}

void close_disk(FileHandle handle)
{

}

int read_disk(void *ptr, lloff_t sector, int nsects, int sectorsize)
{
}


int write_disk(void *ptr, lloff_t sector, int nsects, int sectorsize)
{
}

#endif
