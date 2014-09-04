/*
 * FATDisk.h
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef FATDISK_H_
#define FATDISK_H_

#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "IDisk.h"

class IFAT;
class Directory;
class File;
class LongDir;

class FATDisk : public IDisk {
private:
    std::fstream device_;
    IFAT *fat_;
    unsigned int rootCluster_;

    void handleLongDirEntry(LongDir& ldir, const char* entry);
    void addDirEntry(Directory& dir, const LongDir& ldir, const char* entry);
    Directory* getRoot();
    static std::string currDirSymbol;
    static std::string backDirSymbol;
    static std::string& consolidateName(std::string &name);

    class Path {
    private:
    	typedef std::vector<std::string> DNames;
    	DNames dirNames_;

    public:
    	Path(const std::string& path);
    	unsigned int numOfElements() const;
    	const std::string& operator[](unsigned int i) const;
    };

    template <class T>
    T* getFileEntry(const Path& path);

    IDirectory *getDir (const Path& path);
    IFile      *getFile(const Path& path);

public:
    FATDisk(const char* deviceName);
    ~FATDisk();

    void list(Directory& dir);
    unsigned int load(File& file, char* buffer, const unsigned int buffSize);

    IDirectory *getDir(const std::string &dirName);
    IFile      *getFile(const std::string &fileName);
    const IFAT *getFAT() const { return fat_; };
    std::fstream& getDevice() { return device_; };
};

#endif /* FATDISK_H_ */
