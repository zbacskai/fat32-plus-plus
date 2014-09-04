/*
 * FileEntry.cpp
 *
 *  Created on: 5 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "FileEntry.h"

FileEntry::FileEntry(const char* name,
		   FATDisk& disk,
		   unsigned int firstCluster) :
		   	   ClusterChain(disk, firstCluster), name_(name),
			   attr_(Attribute(0)), cDate_(Date(0,0,0)), cTime_(Time(0,0,0,0)),
		  	   wDate_(Date(0,0,0)), wTime_(Time(0,0,0,0)),
		  	   aDate_(Date(0,0,0)), size_(0)
{

}

FileEntry::FileEntry(const char* name, FATDisk& disk, unsigned int firstCluster,
		   const Attribute &a, const Date &cDate, const Time &cTime,
		  	   	  	  	  	   const Date &wDate, const Time &wTime,
		  	  	  	  	  	   const Date &aDate, unsigned int size) :
		  	  	  	  	  	   	  ClusterChain(disk, firstCluster), name_(name),
		  	  	  	  	  	   	  attr_(a), cDate_(cDate), cTime_(cTime),
		  	  	  	  	  	   	  	  	    wDate_(wDate), wTime_(wTime),
		  	  	  	  	  	   	  	  	    aDate_(aDate), size_(size)
{

}

std::string& FileEntry::getName()
{
	return name_;
}

void FileEntry::asStr(std::string& s)
{
	s += attr_;
	s += ' ';
	s += name_;
	s += ' ';
	s += cDate_;
	s += ' ';
	s += cTime_;
	s += ' ';
	s += wDate_;
	s += ' ';
	s += wTime_;
	s += ' ';
	s += aDate_;
	s += ' ';
	s += size_;
}




