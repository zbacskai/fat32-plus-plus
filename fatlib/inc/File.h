/*
 * File.h
 *
 *  Created on: May 27, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef FILE_H_
#define FILE_H_

#include "IFile.h"
#include "FileEntry.h"
#include <string>

class File : public FileEntry, public virtual IFile {
public:
	File(const char* name, FATDisk& disk, unsigned int firstCluster,
		 const Attribute &a, const Date &cDate, const Time &cTime,
		                     const Date &wDate, const Time &wTime,
		                     const Date &aDate, unsigned int size);
	File(const File &f);
	unsigned int getSize() { return this->size_; };
	unsigned int load(char* buffer, const unsigned int buffSize);

	virtual ~File() {};
};

#endif /* FILE_H_ */
