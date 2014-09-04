/*
 * IDirectory.h
 *
 *  Created on: 5 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef IDIRECTORY_H_
#define IDIRECTORY_H_

#include "IFileEntry.h"

#include <map>

class IDirectory : public virtual IFileEntry {
public:
	typedef std::map<std::string, IFileEntry*> DirectoryEntries;
	virtual void addEntryNSync(IFileEntry* file) = 0;
	virtual const DirectoryEntries& list() = 0;
	virtual ~IDirectory() {};
};


#endif /* IDIRECTORY_H_ */
