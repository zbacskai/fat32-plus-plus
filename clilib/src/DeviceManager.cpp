/*
 * DeviceManager.cpp
 *
 *  Created on: 4 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "DeviceManager.h"

#include <iostream>

#include "IDisk.h"
#include "ITerminal.h"

IDisk* DeviceManager::getDisk(const std::string& diskName)
{
	Disks::iterator it = myDisks_.find(diskName);
	IDisk *disk = 0;

	if (it != myDisks_.end())
		return it->second;

	try
	{
		disk = IDisk::mount(IDisk::FsFAT32, diskName.c_str());
	} catch (int e) {
	    std::cout << "Error opening disk: " << diskName <<
	       		     ". Error code " << e << std::endl;
	} catch (...) {
	    std::cout << "Unknown problem!" << std::cout;
	}
	myDisks_.insert(DisksPair(diskName, disk));

	return disk;
}

ITerminal* DeviceManager::getTerminal(const std::string& terminalName)
{
	Terminals::iterator it = myTerminals_.find(terminalName);

	if (it != myTerminals_.end())
		return it->second;

	ITerminal *terminal = ITerminal::create(terminalName);
	myTerminals_.insert(TerminalsPair(terminalName, terminal));

	return terminal;
}

DeviceManager::~DeviceManager()
{
	for (Disks::iterator it = myDisks_.begin();
		 it != myDisks_.end(); ++it)
		IDisk::umount(it->second);

	for (Terminals::iterator it = myTerminals_.begin();
			it != myTerminals_.end(); ++it)
		ITerminal::dispose(it->second);
}


