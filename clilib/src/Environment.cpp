/*
 * Environment.cpp
 *
 *  Created on: 4 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "Environment.h"
#include "DeviceManager.h"

std::string Environment::defaultVal_("");

std::string Environment::currDirKey_("CDIRECTORY");
std::string Environment::currDiskKey_("CDISK");
std::string Environment::currTerminalKey_("CTERMINAL");

const std::string& Environment::getVar(const std::string varName) const
{
	VarMap::const_iterator it = variables_.find(varName);
	if (it != variables_.end())
		return it->second;

	return defaultVal_;
}

void Environment::setVar(const std::string varName,
	            	  	 const std::string varValue)
{
	variables_[varName] = varValue;
}

DeviceManager &Environment::getDeviceManager()
{
	return DeviceManager::instance();
}

