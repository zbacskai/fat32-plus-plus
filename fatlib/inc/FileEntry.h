/*
 * FileEntry.h
 *
 *  Created on: 5 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef FILEENTRY_H_
#define FILEENTRY_H_

#include "ClusterChain.h"
#include "IFileEntry.h"
#include "DateTime.h"

class FileEntry : public ClusterChain, public virtual IFileEntry {
public:
	class Attribute {
	private:
			unsigned char attr_;
		public:
			Attribute(unsigned char a) : attr_(a) {};
			operator std::string();
	};
protected:
	std::string name_;
	Attribute attr_;
	Date cDate_;
	Time cTime_;
	Date wDate_;
	Time wTime_;
	Date aDate_;
	unsigned int size_;
public:
	FileEntry(const char* name, FATDisk& disk, unsigned int firstCluster);
	FileEntry(const char* name, FATDisk& disk, unsigned int firstCluster,
	 	       const Attribute &a, const Date &cDate, const Time &cTime,
		                           const Date &wDate, const Time &wTime,
		                           const Date &aDate, unsigned int size);
	std::string& getName();
	void asStr(std::string& s);
};



#endif /* FILEENTRY_H_ */
