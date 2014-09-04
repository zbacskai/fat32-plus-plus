/*
 * FATDisk.cpp
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "FATDisk.h"
#include "FAT32.h"

//TODO: Correct this problem
#include "IFAT.h"
#include "MSFAT.h"
#include "Directory.h"
#include "Log.h"

#include <vector>
#include <sstream>
#include <memory>
#include <cstring>
#include <algorithm>

class LongDir {
private:
	std::vector<MS::DirectoryEntryLong> elements_;
	unsigned char getChkSum(const std::string& name) const;
	bool isChkSumValid(const std::string& name) const;
	void addNameFragWk(const unsigned char *buff,
			           std::string &str,
			           unsigned int len) const;
	void addNameFrag(const MS::DirectoryEntryLong &element, std::string &str) const;
public:
	void addElement(const MS::DirectoryEntryLong *element);
	bool isValid(const std::string& shortName) const;
	void getName(std::string &lName) const;
	void getName(const std::string &sName, std::string &lName) const;
	void clear();
};

unsigned char LongDir::getChkSum(const std::string& name) const
{
	unsigned char chksum = 0;
	for (unsigned int i = 0; i < name.length(); i++)
	{
		chksum = ((chksum & 1) ? 0x80 : 0) + (chksum >> 1) + name[i];
	}
	return chksum;
}

bool LongDir::isChkSumValid(const std::string& name) const
{
	unsigned char chksm = getChkSum(name);

	for (unsigned int i = 0; i < elements_.size(); i++)
		if (static_cast<unsigned char>(elements_[i].LDIR_Chksum) != chksm)
			return false;

	return true;
}

void LongDir::addNameFragWk(const unsigned char* buff,
			                std::string &str,
			                unsigned int len) const
{
	for(unsigned int i = 0; i < len; i =i+2)
	{
		if (!((buff[i] == 0x00) && (buff[i+1] == 0x00)) &&
		    !((buff[i] == 0xff) && (buff[i+1] == 0xff)) )
			str+=buff[i];
		else
			return;
	}
}

void LongDir::addNameFrag(const MS::DirectoryEntryLong &element,
		                  std::string &str) const
{
	addNameFragWk(reinterpret_cast<const unsigned char*>(&element.LDIR_Name1[0]),
	              str,
		          sizeof(element.LDIR_Name1));
	addNameFragWk(reinterpret_cast<const unsigned char*>(&element.LDIR_Name2[0]),
	              str,
		          sizeof(element.LDIR_Name2));
	addNameFragWk(reinterpret_cast<const unsigned char*>(&element.LDIR_Name3[0]),
	              str,
		          sizeof(element.LDIR_Name3));

}

void LongDir::addElement(const MS::DirectoryEntryLong *element)
{
	elements_.push_back(*element);
}

bool LongDir::isValid(const std::string& shortName) const
{
	if (elements_.size() == 0)
		return false;

	return isChkSumValid(shortName);
}

void LongDir::getName(std::string &longName) const
{
	longName.clear();
	for (unsigned int i = elements_.size(); i>0; i--)
	{
		addNameFrag(elements_[i-1], longName);
	}
}

void LongDir::getName(const std::string &sName, std::string &lName) const
{
	if (isValid(sName))
		getName(lName);
	else
		lName = sName;
}

void LongDir::clear()
{
	elements_.clear();
}

///////////////////////////////////////////////////////////////////////////////
//
// FATDisk specific information and functions to be stored here
//

FATDisk::Path::Path(const std::string& path)
{
	std::istringstream ss(path);
	std::string s;
	while(std::getline(ss, s, '/'))
	{
		if (s != "")
			dirNames_.push_back(s);
	}
}

unsigned int FATDisk::Path::numOfElements() const
{
	return dirNames_.size();
}

const std::string& FATDisk::Path::operator [](unsigned int i) const
{
	return dirNames_[i];
}

FATDisk::FATDisk(const char* deviceName)
{
    MS::BootSector bs;
    device_.open(deviceName);

    if (!device_.is_open())
        throw -1;

    device_.read(reinterpret_cast<char*>(&bs), sizeof(bs));

    Log(Log::Debug) << bs;

    if (MS::BootSector::FATv32 == bs.getFATType())
    {
    	fat_ = new FAT32(bs.base.BPB_NumFATs,
    			         bs.f.fat32.BPB_FATSz32 * bs.base.BPB_BytesPerSec,
    			         bs.base.BPB_RsvdSecCnt * bs.base.BPB_BytesPerSec,
    			         bs.base.BPB_SecPerClus * bs.base.BPB_BytesPerSec,
    					 device_);

    	rootCluster_ = bs.f.fat32.BPB_RootClus;
    }
    else
    	fat_= 0;
}

FATDisk::~FATDisk()
{
    device_.close();
    if (fat_)
    	delete fat_;
}

Directory* FATDisk::getRoot()
{
	return (new RDirectory("/", *this, rootCluster_));
}

template <class T>
T* FATDisk::getFileEntry(const Path& path)
{
	Directory* dir = getRoot();
	std::auto_ptr<Directory> root(dir);
	Directory::DirectoryEntries::const_iterator it;

	if (path.numOfElements() == 0)
		return 0;

	for(unsigned int i = 0; i < path.numOfElements(); i++)
	{
		const Directory::DirectoryEntries& ent (dir->list());
		it = ent.find(path[i]);
		if (it != ent.end())
		{
			dir = dynamic_cast<Directory*>(it->second);
			if ((dir == 0) && i < (path.numOfElements() - 1))
				return 0;
		}
		else
			return 0;

	}

	T* entry = dynamic_cast<T*>(it->second);
	if (entry == 0)
		return 0;

	return new T(*entry);
}

IDirectory* FATDisk::getDir(const Path& path)
{
	return getFileEntry<Directory>(path);
}

IDirectory* FATDisk::getDir(const std::string &dirName)
{
	if (dirName == "/")
		return getRoot();

	Path p(dirName);

	return getDir(p);
}

IFile *FATDisk::getFile(const Path& path)
{
	return getFileEntry<File>(path);
}

IFile *FATDisk::getFile(const std::string &fileName)
{
	Path path(fileName);
	return getFileEntry<File>(path);
}

void FATDisk::handleLongDirEntry(LongDir& ldir, const char* entry)
{
	const MS::DirectoryEntryLong *lent =
			reinterpret_cast<const MS::DirectoryEntryLong *>(entry);

	if ((lent->LDIR_Ord & 0x40) > 0)
		ldir.clear();

	ldir.addElement(lent);
}

template <class T>
class CreateEntry
{
	public:
		static T* produce(const char *name,
				          FATDisk& disk, const MS::DirectoryEntry *ent)
		{
			return (new T(name, disk, ent->firstCluster(),
    			    ent->DIR_Attr, Date(ent->getCreateDate().getYear(),
    			    		            ent->getCreateDate().getMonth(),
    			    		            ent->getCreateDate().getDay()),
    			    			   Time(ent->getCreateTime().getHours(),
    			    		            ent->getCreateTime().getMinutes(),
    			    		            ent->getCreateTime().getSeconds(),
    			    		            ent->getCreateTime().getMiliseconds()),
    			    		       Date(ent->getLastWriteDate().getYear(),
    			    		            ent->getLastWriteDate().getMonth(),
    			    		            ent->getLastWriteDate().getDay()),
    			    		       Time(ent->getLastWriteTime().getHours(),
    			    		            ent->getLastWriteTime().getMinutes(),
    			    		            ent->getLastWriteTime().getSeconds(),
    			    		            ent->getLastWriteTime().getMiliseconds()),
    			    		       Date(ent->getAccDate().getYear(),
    	    			                ent->getAccDate().getMonth(),
    	    		 		            ent->getAccDate().getDay()),
    	    		 		            ent->DIR_FileSize));
		};
};

std::string FATDisk::currDirSymbol(".          ");
std::string FATDisk::backDirSymbol("..         ");

std::string& FATDisk::consolidateName(std::string& name)
{
	if (name == currDirSymbol)
		name = ".";

	if (name == backDirSymbol)
		name = "..";

	return name;
}

void FATDisk::addDirEntry(Directory& dir, const LongDir& ldir, const char* entry)
{
    const MS::DirectoryEntry *ent =
    		reinterpret_cast<const MS::DirectoryEntry*>(entry);

    std::string sName(reinterpret_cast<const char*>(&ent->DIR_Name[0]), 11);
    std::string name;

    ldir.getName(sName, name);


    consolidateName(name);

    if (ent->isDirectory())
    	dir.addEntryNSync(CreateEntry<Directory>::produce(name.c_str(), *this, ent));
    else
    	dir.addEntryNSync(CreateEntry<File>::produce(name.c_str(), *this, ent));
}

void FATDisk::list(Directory& dir)
{
	char* cluster = new char[fat_->getClusterSizeInBytes()];
		const unsigned int NumEntriesPerCluster = fat_->getClusterSizeInBytes() /
			                                  sizeof(MS::DirectoryEntry);

	LongDir ldir;
	for (unsigned int i = 0; i < dir.size(); i++)
	{
		fat_->readCluster(dir[i], cluster);
        MS::DirectoryEntry *entry = reinterpret_cast<MS::DirectoryEntry*>(cluster);

		for (unsigned int j = 0; j < NumEntriesPerCluster; j++, entry++)
		  if (!entry->isFree() &&
		      !entry->isKanji() &&
			  !entry->isNoMore() &&
			  !entry->isLongName())
			  	  addDirEntry(dir, ldir, reinterpret_cast<const char*>(entry));
		  else if (entry->isLongName())
			  handleLongDirEntry(ldir, reinterpret_cast<const char*>(entry));
		  else if (entry->isNoMore())
			  break;
	}

	delete [] cluster;
}

unsigned int FATDisk::load(File& file, char* buffer, const unsigned int buffSize)
{
	const unsigned int clusterSize = fat_->getClusterSizeInBytes();
	unsigned int fileLeft = file.getSize();
	unsigned int buffLeft = buffSize;

	char* cluster = new char[clusterSize];

	for (unsigned int i = 0; (i < file.size()) &&
	                         (buffLeft != 0)   &&
	                         (fileLeft != 0); i++)
	{
		fat_->readCluster(file[i], cluster);
		std::memcpy((buffer + i * clusterSize),
				     cluster, std::min(std::min(fileLeft, buffLeft), clusterSize));
		fileLeft = (fileLeft > clusterSize) ? fileLeft - clusterSize : 0;
		buffLeft = (buffLeft > clusterSize) ? buffLeft - clusterSize : 0;
	}

	delete [] cluster;

	return std::min(file.getSize(), buffSize);
}


