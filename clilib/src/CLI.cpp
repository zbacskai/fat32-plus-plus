/*
 * CLI.cpp
 *
 *  Created on: 30 May 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "CLI.h"

#include <iostream>
#include <fstream>

CLI::CLI(ITerminal* terminal)
{
	std::string defDisk;
	myEnv_.setVar(Environment::currDirKey_, std::string("/"));
	myEnv_.setVar(Environment::currTerminalKey_, terminal->getName());
	terminal->registerKeystrokeHandler(this);
	try
	{
		readDiskName(defDisk);
	} catch (...) {
		std::cout << "Failed to read disk name!" << std::endl;
	}
	myEnv_.setVar(Environment::currDiskKey_, defDisk);
}

CLI::TokenizerStateMachine::IState*
		CLI::TokenizerStateMachine::TokenQuotation::handleChar(char c,
				                                            ArgList & args,
				                                            bool more)
{
	if (c != '"')
	{
		arg_+=c;
		if (more)
			return this;
	}

	args.push_back(arg_);

	return new 	CLI::TokenizerStateMachine::ReceiveChar();
}

void CLI::readDiskName(std::string &fName)
{
	std::ifstream f;
	char name[16];

	f.open(".disk");
	if (!f.is_open())
		throw -1;

	f.getline(name, 16);

	fName = std::string(name);
	f.close();
}

CLI::TokenizerStateMachine::IState*
		CLI::TokenizerStateMachine::TokenAposthrope::handleChar(char c,
				                                            ArgList & args,
				                                            bool more)
{
	if (c != '\'')
	{
		arg_+=c;
		if (more)
			return this;
	}

	args.push_back(arg_);

	return new 	CLI::TokenizerStateMachine::ReceiveChar();
}

CLI::TokenizerStateMachine::IState*
		CLI::TokenizerStateMachine::TokenSpace::handleChar(char c,
				                                            ArgList & args,
				                                            bool more)
{
	IState *next = this;
	if (c != ' ')
		arg_+=c;
	else
		next = new CLI::TokenizerStateMachine::ReceiveChar();

	if ((c == ' ') || (!more))
		args.push_back(arg_);

	return (next);
}

CLI::TokenizerStateMachine::IState*
		CLI::TokenizerStateMachine::ReceiveChar::handleChar(char c,
				                                            ArgList & args,
				                                            bool more)
{
	IState *next = this;
	switch (c)
	{
	case ' ':
		break;
	case '\'':
		next = new CLI::TokenizerStateMachine::TokenAposthrope();
		break;
	case '"':
		next = new CLI::TokenizerStateMachine::TokenQuotation();
		break;
	default:
		next = new CLI::TokenizerStateMachine::TokenSpace();
		next->handleChar(c, args, more);
		break;
	}

	return next;
}

void CLI::TokenizerStateMachine::handleChar(char c, ArgList & args, bool more)
{
	IState * nextState = myState_->handleChar(c, args, more);
	if (nextState != myState_)
	{
		delete myState_;
		myState_ = nextState;
	}
}

void CLI::tokenize()
{
	TokenizerStateMachine machine;
	lastCommandArgs_.clear();
	for (unsigned int i = 0; i < cmdLine_.length(); i++)
	{
		machine.handleChar(cmdLine_[i],
				           lastCommandArgs_, i != cmdLine_.size()-1);
	}
}

void CLI::execute()
{
	if (lastCommandArgs_.size() < 1)
		return;

	Commands::iterator cmdIt = availableCommands_.find(lastCommandArgs_[0]);
	if (cmdIt != availableCommands_.end())
	{
		cmdIt->second->execute(myEnv_, lastCommandArgs_);
	}
}

void CLI::registerCommand(const Command* command)
{
	availableCommands_.insert(CommandPair(command->getName(), command));
}

void CLI::registerTerminal(ITerminal* terminal)
{
	myEnv_.setVar(Environment::currTerminalKey_, terminal->getName());
}

void CLI::printHelp(ITerminal& terminal)
{
	for (Commands::iterator it = availableCommands_.begin();
		 it != availableCommands_.end(); ++it)
		terminal << it->second->getName() << '\n';
}

unsigned int CLI::handleKeystroke(ITerminal& terminal,
	                              const ITerminal::IKeystroke &ks)
{
	switch (ks.getChar())
	{
	case '\t':
		// Name completion
		printHelp(terminal);
		break;
	case '\n':
		//execute command
		tokenize();
		execute();
		cmdLine_.clear();
		break;
	default:
		cmdLine_+= ks.getChar();
		break;
	}

	if (ks.getChar() != '\t')
		terminal << ks.getChar();

	return 0;
}

void CLI::disposeCommands()
{
	for (Commands::iterator it = availableCommands_.begin();
		 it != availableCommands_.end(); ++it)
		delete it->second;
}

CLI::~CLI()
{

}

