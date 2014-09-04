/*
 * Directory.cpp
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "Directory.h"
#include "FATDisk.h"

#include "Log.h"

Directory::Attribute::operator std::string()
{
	std::stringstream ss;
	ss << ((attr_ & 0x10) ? "d" : "-");
	ss << ((attr_ & 0x01) ? "r-" : "rw");
	ss << ((attr_ & 0x02) ? "h" : "-");
	ss << ((attr_ & 0x04) ? "s" : "-");
	ss << ((attr_ & 0x08) ? "v" : "-");
	ss << ((attr_ & 0x20) ? "a" : "-");
	return ss.str();
}

Directory::Directory(const char* name,
		             FATDisk& disk,
		             unsigned int firstCluster) : FileEntry(name, disk, firstCluster)
{

}
Directory::Directory(const char* name, FATDisk& disk, unsigned int firstCluster,
		  	  	  	 const Attribute &a, const Date &cDate, const Time &cTime,
		  	  	  	 	 	 	 	 	 const Date &wDate, const Time &wTime,
		  	  	  	 	 	 	 	 	 const Date &aDate, unsigned int size) :
		  	  	  	  	  		  FileEntry(name, disk, firstCluster, a,
		  	  	  	  	  			   cDate, cTime, wDate, wTime, aDate, size)
{

}

Directory::Directory(const Directory &dir) : FileEntry (dir.name_.c_str(),
		                                                dir.disk_,
		                                                dir.elems_[0], dir.attr_,
		                                                dir.cDate_, dir.cTime_,
		                                                dir.wDate_, dir.wTime_,
		                                                dir.aDate_, dir.size_)
{

}

void Directory::clearEntries()
{
	for (DirectoryEntries::iterator it = entries_.begin();
		 it != entries_.end();
		 ++it)
		delete it->second;

	entries_.clear();
}

void Directory::addEntryNSync(IFileEntry* file)
{
	entries_.insert(DirectoryEntryPair(file->getName(), file));
}

const Directory::DirectoryEntries& Directory::list()
{
	clearEntries();
	disk_.list(*this);
	return entries_;
}

Directory::~Directory()
{
	clearEntries();
}

