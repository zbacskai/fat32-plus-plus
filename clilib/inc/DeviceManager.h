/*
 * DeviceManager.h
 *
 *  Created on: 4 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef DEVICEMANAGER_H_
#define DEVICEMANAGER_H_

#include "Singleton.h"

#include <string>
#include <map>

class IDisk;
class ITerminal;

class DeviceManager : public Singleton<DeviceManager>
{
private:
	typedef std::map<std::string, IDisk*> Disks;
	typedef std::pair<std::string, IDisk*> DisksPair;
	typedef std::map<std::string, ITerminal*> Terminals;
	typedef std::pair<std::string, ITerminal*> TerminalsPair;
	Disks myDisks_;
	Terminals myTerminals_;
	DeviceManager(const DeviceManager &d) {};
public:
	DeviceManager() {};
	IDisk* getDisk(const std::string& diskName);
	ITerminal* getTerminal(const std::string& terminalName);
	~DeviceManager();
};



#endif /* DEVICEMANAGER_H_ */
