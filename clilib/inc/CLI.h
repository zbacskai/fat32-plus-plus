/*
 * CLI.h
 *
 *  Created on: 30 May 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef CLI_H_
#define CLI_H_

#include "ITerminal.h"
#include "Command.h"
#include "Environment.h"

#include <string>
#include <vector>
#include <map>

class CLI : public ITerminal::IKeystrokeHandler {
private:
	std::string cmdLine_;
	typedef std::vector<std::string> ArgList;
	ArgList lastCommandArgs_;
	typedef std::map<std::string, const Command *> Commands;
	typedef std::pair<std::string, const Command *> CommandPair;
	Commands availableCommands_;
	Environment myEnv_;
	void readDiskName(std::string &fName);
	void tokenize();
	void execute();
	void printHelp(ITerminal& terminal);
public:
	unsigned int handleKeystroke(ITerminal& terminal,
	                             const ITerminal::IKeystroke &ks);
	void registerCommand(const Command* command);
	void registerTerminal(ITerminal* terminal);
	CLI(ITerminal* terminal);
	void disposeCommands();
	~CLI();
private:
	class TokenizerStateMachine
	{
	private:

		class IState {
		protected:
			std::string arg_;
		public:
			virtual IState* handleChar(char c, ArgList & args, bool more) = 0;
			virtual ~IState() {};
		};

		class ReceiveChar : public IState {
		public:
			virtual IState* handleChar(char c, ArgList & args, bool more);
		};

		class TokenSpace : public IState {
		public:
			virtual IState* handleChar(char c, ArgList & args, bool more);
		};

		class TokenAposthrope : public IState {
		public:
			virtual IState* handleChar(char c, ArgList & args, bool more);
		};

		class TokenQuotation : public IState {
		public:
			virtual IState* handleChar(char c, ArgList & args, bool more);
		};

		IState *myState_;
		TokenizerStateMachine(const TokenizerStateMachine& t) : myState_(0) {};
	public:
		TokenizerStateMachine() { myState_ = new ReceiveChar(); };
		void handleChar(char c, ArgList & args, bool more);
		~TokenizerStateMachine() { delete myState_; };
	};
};


#endif /* CLI_H_ */
