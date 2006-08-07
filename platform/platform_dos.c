#include "platform.h"


#ifdef __msdos__

FileHandle open_disk(const char *path)
{
}

int get_ndisks()
{
	/* Find the no of attached drives (No of Hard Disks). We do it by
	 * simply reading 75'th byte of bios data area..
	 */
	return peekb(0x40,0x75);

}

void close_disk(FileHandle handle)
{
	CloseHandle(handle);
}

int read_disk(void *ptr, lloff_t sector, int nsects, int sectorsize)
{
	lloff_t offset;

	offset = sector * sectorsize;


}


int write_disk(void *ptr, lloff_t sector, int nsects, int sectorsize)
{
}

#endif