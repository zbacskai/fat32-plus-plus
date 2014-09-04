/*
 * FAT32.h
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef FAT32_H_
#define FAT32_H_

#include "IFAT.h"

class FAT32 : public IFAT {
private:
	static const unsigned int ColSize;
	static const unsigned int EntryWidth;
	static const unsigned int BadSector;
	static const unsigned int ReservedValue;
	static const unsigned int EndOfFileValue;
	static const unsigned int FreePlace;
protected:
	const unsigned int numOfFileAllocTable_;
	const unsigned int sizeOfOneAllocTableInBytes_;
	const unsigned int startOffsOfAllocTableInBytes_;
	const unsigned int clusterSizeInBytes_;
	unsigned int readFatEntry(unsigned int clusterNumber) const;
	void writeFatEntry(unsigned int clusterNumber, unsigned int value);
	unsigned int findFree(unsigned int minClus,
			              const unsigned int maxClus) const;
	void writeCluster(const int clusterNumber,
					  const char *buffer,
			          const unsigned int fatEntry);
public:
	FAT32(const unsigned int numOfFileAllocTable,
		  const unsigned int sizeOfOneAllocTableInBytes_,
	      const unsigned int startOffsOfAllocTableInBytes_,
	      const unsigned int clusterSizeInBytes_,
	      std::fstream &disk) : IFAT (disk),
		  	  	 numOfFileAllocTable_(numOfFileAllocTable),
		  	  	 sizeOfOneAllocTableInBytes_(sizeOfOneAllocTableInBytes_),
		         startOffsOfAllocTableInBytes_(startOffsOfAllocTableInBytes_),
		         clusterSizeInBytes_(clusterSizeInBytes_){};

	void printFAT() const;
	NextCluster getNextCluster(unsigned int clusterNumber) const;
	NextCluster getNextFreeCluster(unsigned int clusterNumber) const;
	void readCluster(const int clusterNumber, char* buffer);
	void writeCluster(const int clusterNumber, const char *buffer);
	void writeCluster(const int clusterNumber,
			          const char *buffer, const NextCluster &nc);
	unsigned int getClusterSizeInBytes();
	~FAT32();
};

#endif /* FAT32_H_ */
