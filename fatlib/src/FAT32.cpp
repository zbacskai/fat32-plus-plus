/*
 * FAT32.cpp
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "FAT32.h"
#include <stdint.h>

#include <Log.h>

const unsigned int FAT32::ColSize(128);
const unsigned int FAT32::EntryWidth(4);
const unsigned int FAT32::BadSector(0x0ffffff7);
const unsigned int FAT32::ReservedValue(0x0ffffff8);
const unsigned int FAT32::EndOfFileValue(0x0fffffff);
const unsigned int FAT32::FreePlace(0);

//TODO: The second reserved cluster (index2)
//is used to indicate problems and dirtiness
// To be implemented
void FAT32::printFAT() const
{
	disk_.seekg(startOffsOfAllocTableInBytes_);

	char* buffer = new char[sizeOfOneAllocTableInBytes_];

	for (unsigned int k = 0; k < numOfFileAllocTable_; k++)
	{
		disk_.read(buffer, sizeOfOneAllocTableInBytes_);

		Log(Log::Debug) << "FAT copy " << k+1;

		for (unsigned int i = 0; i < sizeOfOneAllocTableInBytes_/EntryWidth; i++)
		{
			uint32_t value =
				*(reinterpret_cast<uint32_t*>(&buffer[i*EntryWidth]));

			if (ReservedValue == value)
				Log(Log::Debug).nLF() << "R";
			else if (EndOfFileValue == value)
				Log(Log::Debug).nLF() << "F";
			else if (FreePlace == value)
				Log(Log::Debug).nLF() << ".";
			else if (BadSector == value)
				Log(Log::Debug).nLF()  << "B";
			else
				Log(Log::Debug).nLF() << "O";

			if ( i % ColSize == ColSize-1)
				Log(Log::Debug).LF();
		}
		Log(Log::Debug).LF();
	}


	delete [] buffer;
}

unsigned int FAT32::readFatEntry(unsigned int clusterNumber) const
{
	// Start Offset points after the first 32 sector.
	// Cluster 0 & Cluster 1 are reserved
	disk_.seekg(startOffsOfAllocTableInBytes_ + clusterNumber * 4);
	uint32_t value;
	disk_.read(reinterpret_cast<char*>(&value), sizeof(value));

	value &= 0x0fffffff;

	return value;
}

void FAT32::writeFatEntry(unsigned int clusterNumber, unsigned int value)
{
	disk_.seekg(startOffsOfAllocTableInBytes_ + clusterNumber * 4);

	value &= 0x0fffffff;

	disk_.write(reinterpret_cast<char*>(&value), sizeof(value));
}

IFAT::NextCluster FAT32::getNextCluster(unsigned int clusterNumber) const
{
	unsigned int value = readFatEntry(clusterNumber);

	bool valid = ((value != FreePlace) && (value != BadSector) &&
			      (value != EndOfFileValue) && (value != ReservedValue));

	return IFAT::NextCluster(value, valid);
}

unsigned int FAT32::findFree(unsigned int minClus,
		                     const unsigned int maxClus) const
{
	for (; minClus < maxClus; ++minClus)
	{
		unsigned int value = readFatEntry(minClus);
		if (value == FreePlace)
			return value;
	}

	return EndOfFileValue;
}

IFAT::NextCluster FAT32::getNextFreeCluster(unsigned int clusterNumber) const
{
	const unsigned int maxClusNum = sizeOfOneAllocTableInBytes_/EntryWidth;
	unsigned int value = findFree(clusterNumber, maxClusNum);

	if (value != FreePlace)
		value =findFree(0, clusterNumber);

	if (value != FreePlace)
		return IFAT::NextCluster(value, false);

	return (IFAT::NextCluster(value, true));
}

void FAT32::readCluster(const int clusterNumber, char* buffer)
{
	if (clusterNumber < 2)
		return;

	disk_.seekg(startOffsOfAllocTableInBytes_+
			    numOfFileAllocTable_ * sizeOfOneAllocTableInBytes_ +
			    (clusterNumber-2) * clusterSizeInBytes_);
	disk_.read(buffer, clusterSizeInBytes_);
}

void FAT32::writeCluster(const int clusterNumber,
			   		     const char *buffer,
			             const unsigned int fatEntry)
{
	if (clusterNumber < 2)
		return;

	unsigned int oldFatEntry = readFatEntry(clusterNumber);

	if ((oldFatEntry != FAT32::FreePlace) && (oldFatEntry != fatEntry))
		return;

	disk_.seekg(startOffsOfAllocTableInBytes_+
			    numOfFileAllocTable_ * sizeOfOneAllocTableInBytes_ +
			    (clusterNumber-2) * clusterSizeInBytes_);

	disk_.write(buffer, clusterSizeInBytes_);

	writeFatEntry(clusterNumber, fatEntry);
}

void FAT32::writeCluster(const int clusterNumber, const char *buffer)
{
	writeCluster(clusterNumber, buffer, FAT32::EndOfFileValue);
}

void FAT32::writeCluster(const int clusterNumber,
		                 const char *buffer, const IFAT::NextCluster &nc)
{
	writeCluster(clusterNumber, buffer, nc);
}

unsigned int FAT32::getClusterSizeInBytes()
{
	return clusterSizeInBytes_;
}

FAT32::~FAT32()
{

}



