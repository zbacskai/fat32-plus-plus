/*
 * File.cpp
 *
 *  Created on: May 27, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "File.h"
#include "Log.h"
#include "FATDisk.h"

File::File(const char* name, FATDisk& disk, unsigned int firstCluster,
		   const Attribute &a, const Date &cDate, const Time &cTime,
		  	   	  	  	  	   const Date &wDate, const Time &wTime,
		  	  	  	  	  	   const Date &aDate, unsigned int size) :
		  	  	  	  	  	   	  FileEntry(name, disk, firstCluster, a,
		  	  	  	  	  	   			    cDate, cTime, wDate, wTime,
		  	  	  	  	  	   			    aDate, size)
{

}

File::File(const File &f) : FileEntry(f.name_.c_str(),
		                              f.disk_, f.elems_[0], f.attr_,
		                              f.cDate_, f.cTime_, f.wDate_, f.wTime_,
		                              f.aDate_, f.size_)
{

}

unsigned int File::load(char* buffer, const unsigned int buffSize)
{
	disk_.load(*this, buffer, buffSize);
	return ((buffSize < size_) ? buffSize : size_);
}


