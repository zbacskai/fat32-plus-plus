/*
 * Environment.h
 *
 *  Created on: 4 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <map>
#include <string>

class DeviceManager;

class Environment {
public:
	typedef std::map<std::string, std::string> VarMap;
	typedef std::pair<std::string, std::string> VarMapPair;
	static std::string defaultVal_;
	static std::string currDirKey_;
	static std::string currDiskKey_;
	static std::string currTerminalKey_;
	const std::string& getVar(const std::string varName) const;
	void setVar(const std::string varName,
		        const std::string varValue);
	DeviceManager &getDeviceManager();
private:
	VarMap variables_;
public:
	const VarMap& getAllVars() { return variables_; };
};


#endif /* ENVIRONMENT_H_ */
