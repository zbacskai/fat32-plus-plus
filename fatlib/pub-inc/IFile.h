/*
 * IFile.h
 *
 *  Created on: 5 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef IFILE_H_
#define IFILE_H_

#include "IFileEntry.h"

class IFile : public virtual IFileEntry
{
public:
	virtual unsigned int getSize() = 0;
	virtual unsigned int load(char* buffer, const unsigned int buffSize) = 0;
};


#endif /* IFILE_H_ */
