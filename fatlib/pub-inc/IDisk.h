/*
 * IDisk.h
 *
 *  Created on: 5 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef IDISK_H_
#define IDISK_H_

class IDirectory;
class IFile;

#include <string>

class IDisk {
public:
	typedef enum {
		FsFAT32,
		FsAuto,
	} FileSystem;
	virtual IDirectory *getDir(const std::string &dirName) = 0;
	virtual IFile      *getFile(const std::string &fileName) = 0;
	virtual ~IDisk() {};
//TODO: Change this to create/dispose. This is not a valid mount
	static IDisk* mount(FileSystem type, const char* deviceName);
	static void umount(IDisk *disk);
};

#endif /* IDISK_H_ */
