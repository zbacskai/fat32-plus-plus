/*
 * IDisk.cpp
 *
 *  Created on: 5 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "IDisk.h"
#include "FATDisk.h"

IDisk* IDisk::mount(FileSystem type, const char* deviceName)
{
	switch (type)
	{
	case FsFAT32:
		// Fall through
	case FsAuto:
		// Fall through
	default:
		return new FATDisk(deviceName);
		break;
	}

	return 0;
}

void IDisk::umount(IDisk* disk)
{
	delete disk;
}




