/*
 * Command.h
 *
 *  Created on: 4 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <vector>

class Environment;
class IDirectory;
class ITerminal;

class Command {
protected:
	std::string name_;
	static IDirectory *getDir(Environment &env,
			                  const std::string path);
	static IDirectory *getCurrDir(Environment &env);
	static ITerminal* getCurrTerm(Environment &env);
public:
	Command(const std::string &s) : name_(s) {};
	typedef std::vector<std::string> AList;
	virtual int execute(Environment &env,
			            const AList &argList) const = 0;
	const std::string& getName() const { return name_; };
	virtual ~Command() {};
};

class Dir : public Command {
public:
	Dir(const std::string &s) : Command(s) {};
	int execute(Environment &env, const AList &argList) const;
};

class PrintEnv : public Command {
public:
	PrintEnv(const std::string &s) : Command(s) {};
	int execute(Environment &env, const AList &argList) const;
};

class Exit : public Command {
public:
	Exit(const std::string &s) : Command(s) {};
	int execute(Environment &env, const AList &argList) const;

};

class Cd : public Command {
private:
	int handleEntry(Environment &env, const AList &argList) const;
	int handleBack(Environment &env, const AList &argList) const;
	class Path {
	    private:
	    	typedef std::vector<std::string> DNames;
	    	DNames dirNames_;
	    public:
	    	Path(const std::string& path);
	    	std::string prev() const;
    };

public:
	Cd(const std::string &s) : Command(s) {};
	int execute(Environment &env, const AList &argList) const;

};

class Cat : public Command {
public:
	Cat(const std::string &s) : Command(s) {};
	int execute(Environment &env, const AList &argList) const;
};

#endif /* COMMAND_H_ */
