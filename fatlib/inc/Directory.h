/*
 * Directory.h
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include "File.h"
#include <map>
#include <string>

#include "FATDisk.h"
#include "IDirectory.h"

class Directory : public FileEntry, public virtual IDirectory {
protected:
	typedef std::pair<std::string, IFileEntry*> DirectoryEntryPair;
	void clearEntries();
	Directory(const char* name, FATDisk& disk, unsigned int firstCluster);
public:
	Directory(const char* name, FATDisk& disk, unsigned int firstCluster,
			  const Attribute &a, const Date &cDate, const Time &cTime,
			  	  	  	  	  	  const Date &wDate, const Time &wTime,
			  	  	  	  	  	  const Date &aDate, unsigned int size);
	Directory(const Directory& dir);
	void addEntryNSync(IFileEntry* file);
	const DirectoryEntries& list();
	~Directory();
private:
	DirectoryEntries entries_;
};

class RDirectory : public Directory
{
	public:
		RDirectory(const char* name, FATDisk& disk, unsigned int firstCluster) :
			Directory(name, disk, firstCluster) {};
		~RDirectory() {};
};

#endif /* DIRECTORY_H_ */
