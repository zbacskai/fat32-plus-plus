/*
 * MSFAT.cpp
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */
#include <MSFAT.h>
#include <fstream>

using namespace MS;

BootSector::FATType BootSector::getFATType() const
{
	unsigned int RootDirSectors = ((base.BPB_RootEntCnt * 32) +
			                       (base.BPB_BytesPerSec -1)) /
			                        base.BPB_BytesPerSec;

	unsigned int FATSz = (base.BPB_FATSz16) ?
			              base.BPB_FATSz16 :
			              f.fat32.BPB_FATSz32;

	unsigned int ToSec = (base.BPB_ToSect16) ?
			              base.BPB_ToSect16 :
			              base.BPB_TotSec32;

	unsigned int DataSec =ToSec - (base.BPB_RsvdSecCnt +
				          (base.BPB_NumFATs * FATSz) +
				           RootDirSectors);

	unsigned int CountOfClusters = DataSec / base.BPB_SecPerClus;

	if (CountOfClusters < 4085)
		return FATv12;
	else if (CountOfClusters < 65525)
		return FATv16;

	return FATv32;
}

Log operator << (const Log &log, const MS::BootSectorBs& bs)
{
	Log::LogLevel lev = log.getLevel();

	Log(lev) << "              Jump instruction Boot Code: "
			 << Log::Hex
			 << "0x" << static_cast<unsigned int>(bs.BS_jmpBoot[0]) << " "
             << "0x" << static_cast<unsigned int>(bs.BS_jmpBoot[1]) << " "
             << "0x" << static_cast<unsigned int>(bs.BS_jmpBoot[2]) << Log::Dec;

	Log(lev) << "                   Operating system Name: "
             << std::string(reinterpret_cast<const char *>(&bs.BS_OEMName[0]), 8);
	Log(lev) << "                        Bytes Per sector: "
			 << static_cast<unsigned int>(bs.BPB_BytesPerSec);
	Log(lev) << "                     Sectors per Cluster: "
			 << static_cast<unsigned int>(bs.BPB_SecPerClus);
	Log(lev) << "              Number of reserved sectors: "
			 << static_cast<unsigned int>(bs.BPB_RsvdSecCnt);
	Log(lev) << "                           Number of FAT: "
			 << static_cast<unsigned int>(bs.BPB_NumFATs);
	Log(lev) << "             Number of Root File Entries: "
			 << static_cast<unsigned int>(bs.BPB_RootEntCnt);
	Log(lev) << "           Number of FAT16 Total Sectors: "
			 << static_cast<unsigned int>(bs.BPB_ToSect16);
	Log(lev) << "                              Media type: 0x" << Log::Hex
			 << static_cast<unsigned int>(bs.BPB_Media) << Log::Dec;
	Log(lev) << "   Count of Sectors occupied by FAT16/12: "
			 << static_cast<unsigned int>(bs.BPB_FATSz16);
	Log(lev) << "          Sectors per track for Int 0x13: "
			 << static_cast<unsigned int>(bs.BPB_SecPerTrk);
	Log(lev) << "            Number of heads for Int 0x13: "
			 << static_cast<unsigned int>(bs.BPB_NumHeads);
	Log(lev) << "Number of Hidden Sectors before FAT 0x13: "
			 << static_cast<unsigned int>(bs.BPB_HiddSec);
	Log(lev) << "       Number of Total Sectors on volume: "
			 << static_cast<unsigned int>(bs.BPB_TotSec32);

    return (Log(log));
}

Log operator << (const Log& log, const MS::BootSectorExtFAT126& bse)
{
	Log::LogLevel lev = log.getLevel();

	Log(lev) << "                            Drive Number: 0x"
			 << Log::Hex << static_cast<unsigned int>(bse.BS_DrvNum) << Log::Dec;
	Log(lev) << "                Reserved (used by WinNT): "
			 << static_cast<unsigned int>(bse.BS_Reserved1);
	Log(lev) << "                          Boot Signature: 0x"
			 << Log::Hex << static_cast<unsigned int>(bse.BS_BootSig) << Log::Dec;
	Log(lev) << "                               Volume ID: "
			 << static_cast<unsigned int>(bse.BS_VolID);
	Log(lev) << "                            Volume Label: "
			 << std::string(reinterpret_cast<const char*>(&bse.BS_VolLab[0]), 11);
	Log(lev) << "                        File System Type: "
			 << std::string(reinterpret_cast<const char*>(&bse.BS_FilSysType), 8);

	return (Log(log));
}

Log operator << (const Log &log, const MS::BootSectorExtFAT32 &bse)
{
	Log::LogLevel lev = log.getLevel();

	Log(lev) << "                   FAT32 size in sectors: "
	         << static_cast<unsigned int>(bse.BPB_FATSz32);
	Log(lev) << "                          External flags: 0x"
	         << Log::Hex << static_cast<unsigned int>(bse.BPB_ExtFlags)
	         << Log::Dec;
	Log(lev) << "              File System Version Number: 0x"
	         << Log::Hex << static_cast<unsigned int>(bse.BPB_FSVer)
	         << Log::Dec;
	Log(lev) << "            Root Directory First Cluster: "
			 << static_cast<unsigned int>(bse.BPB_RootClus);
	Log(lev) << "                        File System Info: "
			 << static_cast<unsigned int>(bse.BPB_FSInfo);
	Log(lev) << "        Backup Boot Sector Sector Number: "
			 << static_cast<unsigned int>(bse.BPB_BkBootSec);
	Log(lev) << "                                Reserved: "
			 << static_cast<unsigned int>(bse.BPB_Reserved[0]);
	Log(lev) << "                            Drive Number: "
			 << static_cast<unsigned int>(bse.BS_DrvNum);
	Log(lev) << "                                Reserved: "
			 << static_cast<unsigned int>(bse.BS_Reserved1);
	Log(lev) << "                          Boot Signature: 0x"
			 << static_cast<unsigned int>(bse.BS_BootSig);
	Log(lev) << "                               Volume Id: "
			 << static_cast<unsigned int>(bse.BS_VolID);
	Log(lev) << "                            Volume Label: "
			 << std::string(
					 reinterpret_cast<const char *>(&bse.BS_VolLab[0]), 11);
	Log(lev) << "                        File System Type: "
			 << std::string(
					 reinterpret_cast<const char *>(&bse.BS_FilSysType[0]), 8);

	return Log(log);
}

Log operator << (const Log &log, const MS::BootSector &bs)
{
	Log::LogLevel lev = log.getLevel();
    Log(lev) << bs.base;
    if (BootSector::FATv32 == bs.getFATType())
    	Log(lev) << bs.f.fat32;
    else
    	Log(lev) << bs.f.fat126;

    return Log(log);
}

