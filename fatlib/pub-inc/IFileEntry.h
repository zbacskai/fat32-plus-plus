/*
 * IFileEntry.h
 *
 *  Created on: 5 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef IFILEENTRY_H_
#define IFILEENTRY_H_

#include <string>

class IFileEntry {
public:
	virtual std::string& getName() = 0;
	virtual void asStr(std::string& s) = 0;
	virtual ~IFileEntry() {};
};


#endif /* IFILEENTRY_H_ */
