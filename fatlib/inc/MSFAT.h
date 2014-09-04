#ifndef MSFAT_h
#define MSFAT_h

// For low level read functions
#include <stdint.h>
#include <iostream>
#include "Log.h"

#define FATStartYear 1980

//Helper Structures

namespace MS {


class Time {
private:
	unsigned int time_;
	unsigned int millisec_;
public:
	unsigned int getMiliseconds() const { return ((millisec_ % 10) * 100); };
	unsigned int getSeconds() const { return ((time_ & 0x01f) +
			                                  (millisec_ / 10)); };
	unsigned int getMinutes() const { return ((time_ & 0x7e0) >> 5); };
	unsigned int getHours() const   { return ((time_ & 0xf800) >> 11); };

	Time(unsigned int time, unsigned int millisec) : time_(time),
			                                           millisec_ (millisec){};
};

class Date {
private:
	unsigned int date_;
public:
	unsigned int getDay() const { return (date_ & 0x01f);};
	unsigned int getMonth() const { return ((date_ & 0x1e0) >> 5); };
	unsigned int getYear() const { return (((date_ & 0xfe00) >> 9) + FATStartYear); };
	Date(unsigned int date) : date_(date) {};
};

//Let's turn the packing off
#pragma pack(push, 1)

struct BootSectorBs {
    uint8_t  BS_jmpBoot[3];
    uint8_t  BS_OEMName[8];
    uint16_t BPB_BytesPerSec;
    uint8_t  BPB_SecPerClus;
    uint16_t BPB_RsvdSecCnt;
    uint8_t  BPB_NumFATs;
    uint16_t BPB_RootEntCnt;
    uint16_t BPB_ToSect16;
    uint8_t  BPB_Media;
    uint16_t BPB_FATSz16;
    uint16_t BPB_SecPerTrk;
    uint16_t BPB_NumHeads;
    uint32_t BPB_HiddSec;
    uint32_t BPB_TotSec32;
};

struct BootSectorExtFAT126 {
    uint8_t  BS_DrvNum;
    uint8_t  BS_Reserved1;
    uint8_t  BS_BootSig;
    uint32_t BS_VolID;
    uint8_t  BS_VolLab[11];
    uint8_t  BS_FilSysType[8];
};

struct BootSectorExtFAT32 {
    uint32_t BPB_FATSz32;
    uint16_t BPB_ExtFlags;
    uint16_t BPB_FSVer;
    uint32_t BPB_RootClus;
    uint16_t BPB_FSInfo;
    uint16_t BPB_BkBootSec;
    uint8_t  BPB_Reserved[12];
    uint8_t  BS_DrvNum;
    uint8_t  BS_Reserved1;
    uint8_t  BS_BootSig;
    uint32_t BS_VolID;
    uint8_t  BS_VolLab[11];
    uint8_t BS_FilSysType[8];
};

struct BootSector {
    BootSectorBs base;
    union {
        BootSectorExtFAT126 fat126;
        BootSectorExtFAT32  fat32;
    } f;
    enum FATType { FATv12,
                   FATv16,
                   FATv32 };
    FATType getFATType() const;
};

struct DirectoryEntry
{
	uint8_t DIR_Name[11];
	uint8_t DIR_Attr;
	uint8_t DIR_NTRes;
	uint8_t DIR_CrtTimeTenth;
	uint16_t DIR_CrtTime;
	uint16_t DIR_CrtDate;
	uint16_t DIR_LstAccDate;
	uint16_t DIR_FstClusHI;
	uint16_t DIR_WrtTime;
	uint16_t DIR_WrtDate;
	uint16_t DIR_FstClusLO;
	uint32_t DIR_FileSize;
	bool isFree() const { return ((DIR_Name[0] == 0xe5) || (DIR_Name[0] == 0x00)); };
	bool isNoMore() const { return (DIR_Name[0] == 0x00); };
	bool isKanji() const { return (DIR_Name[0] == 0x05); };

	bool isLongName() const { return ((DIR_Attr & 0x0f) == 0x0f); };

	bool isReadOnly() const { return (!isLongName() &&
			                          ((DIR_Attr & 0x01) > 0)); };
	bool isHidden() const { return (!isLongName() &&
			                        ((DIR_Attr & 0x02) > 0)); };
	bool isSystem() const { return (!isLongName() &&
			                        ((DIR_Attr & 0x04) > 0)); };
	bool isVolumeId() const { return (!isLongName() &&
			                          ((DIR_Attr & 0x08) > 0)); };
	bool isDirectory() const { return (!isLongName() &&
			                           ((DIR_Attr & 0x10) > 0)); };
	bool isArchive() const { return (!isLongName() &&
			                         ((DIR_Attr & 0x20) > 0)); };

	unsigned int firstCluster() const { unsigned int cluster = DIR_FstClusHI;
	                                    cluster<<=16;
	                                    cluster|= DIR_FstClusLO; return cluster; };
	Time getCreateTime() const { return Time(DIR_CrtTime, DIR_CrtTimeTenth); };
	Date getCreateDate() const { return Date(DIR_CrtDate); };
	Time getLastWriteTime() const { return Time(DIR_WrtTime, 0); };
	Date getLastWriteDate() const { return Date(DIR_WrtDate); };
	Date getAccDate() const { return Date(DIR_LstAccDate); };
};

struct DirectoryEntryLong
{
	uint8_t LDIR_Ord;
	uint16_t LDIR_Name1[5];
	uint8_t LDIR_Attr;
	uint8_t LDIR_Type;
	uint8_t LDIR_Chksum;
	uint16_t LDIR_Name2[6];
	uint16_t LDIR_FstClusLo;
	uint16_t LDIR_Name3[2];
};

#pragma pack(pop)

} //END namespace MS

Log operator << (const Log &log, const MS::BootSectorBs& bs);
Log operator << (const Log &log, const MS::BootSectorExtFAT126& bse);
Log operator << (const Log &log, const MS::BootSectorExtFAT32 &bse);
Log operator << (const Log &log, const MS::BootSector &bs);

#endif
