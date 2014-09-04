/*
 * Command.cpp
 *
 *  Created on: 4 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "Command.h"
#include "Environment.h"
#include "IDirectory.h"
#include "IDisk.h"
#include "IFile.h"
#include "ITerminal.h"

#include "DeviceManager.h"

#include <sstream>

IDirectory *Command::getDir(Environment &env,
							const std::string path)
{
	std::string currDisk = env.getVar(Environment::currDiskKey_);
	IDisk *disk = env.getDeviceManager().getDisk(currDisk);
	if (!disk)
		return 0;

	return disk->getDir(path);
}

IDirectory *Command::getCurrDir(Environment &env)
{
	std::string currDir = env.getVar(Environment::currDirKey_);
	return getDir(env, currDir);
}

ITerminal* Command::getCurrTerm(Environment &env)
{
	std::string currTerm = env.getVar(Environment::currTerminalKey_);
	return env.getDeviceManager().getTerminal(currTerm);
}

int Dir::execute(Environment &env, const AList &argList) const
{
	IDirectory *dir = Command::getCurrDir(env);
	if (!dir)
		return -1;

	ITerminal &term = *Command::getCurrTerm(env);
	term << '\n';

	IDirectory::DirectoryEntries content = dir->list();
	for (IDirectory::DirectoryEntries::iterator it = content.begin();
	  	it != content.end(); ++it)
	{
		std::string s;
		it->second->asStr(s);
		term << s << '\n';
	}

	delete dir;
	return 0;
}

int PrintEnv::execute(Environment &env, const AList &argList) const
{
	ITerminal &term = *env.getDeviceManager().getTerminal(
									env.getVar(Environment::currTerminalKey_));
	term << '\n';

	const Environment::VarMap allVars = env.getAllVars();
	for (Environment::VarMap::const_iterator it = allVars.begin();
		it != allVars.end(); ++it)
		term << it->first << " = \"" << it->second << "\"\n";

	return 0;
}

int Exit::execute(Environment &env, const AList &argList) const
{
	ITerminal &term = *env.getDeviceManager().getTerminal(
									env.getVar(Environment::currTerminalKey_));
	term.exitPrompt();

	return 0;
}

int Cd::handleEntry(Environment &env, const AList &argList) const
{
	std::string chDir ((argList[1][0] == '/') ?
									 "" : env.getVar(Environment::currDirKey_));
	chDir += '/';
	chDir += argList[1];

	ITerminal &term = *Command::getCurrTerm(env);
	term << '\n';

	IDirectory *dir = Command::getDir(env, chDir);
	if (!dir)
		term << "Invalid entry: \"" << argList[1] << "\"";
	else
		env.setVar(Environment::currDirKey_, chDir);

	delete dir;

	return 0;
}

Cd::Path::Path(const std::string& path)
{
	std::istringstream ss(path);
	std::string s;
	while(std::getline(ss, s, '/'))
	{
		if (s != "")
			dirNames_.push_back(s);
	}
}

std::string Cd::Path::prev() const
{
	if (dirNames_.size() == 0)
		return std::string("/");

	std::string s("/");

	for (unsigned int i = 0; i < dirNames_.size() - 1; i++)
	{
		s+= dirNames_[i];
		s+= '/';
	}

	return s;
}

int Cd::handleBack(Environment &env, const AList &argList) const
{
	std::string currDir = env.getVar(Environment::currDirKey_);
	if (currDir == "/")
		return 0;

	env.setVar(Environment::currDirKey_, Path(currDir).prev());

	return 0;
}

int Cd::execute(Environment &env, const AList &argList) const
{
	// Always return to root
	if (argList[1] == "/")
	{
		env.setVar(Environment::currDirKey_, std::string("/"));
		return 0;
	}

	if (argList[1] == ".")
		return 0;

	if (argList[1] == "..")
		return handleBack(env, argList);

	return handleEntry(env, argList);
}

int Cat::execute(Environment& env, const AList &argList) const
{
	std::string currDisk = env.getVar(Environment::currDiskKey_);
	IDisk *disk = env.getDeviceManager().getDisk(currDisk);
	if (!disk)
		return 0;

	ITerminal &term = *env.getDeviceManager().getTerminal(
										env.getVar(Environment::currTerminalKey_));
	term << '\n';

	std::string fName ((argList[1][0] == '/') ?
									 "" : env.getVar(Environment::currDirKey_));
	fName += '/';
	fName += argList[1];

	IFile *file = disk->getFile(fName);

	if (!file)
	{
		term << "Can not open file: \"" << argList[1] << "\"";
		return -1;
	}

	unsigned int size = file->getSize();
	if (size != 0)
	{
		char * buffer = new char [size+1];
		file->load(buffer, size);
		buffer[size] = 0;
		term << buffer;
		delete [] buffer;
	}
	delete file;

	return 0;
}
