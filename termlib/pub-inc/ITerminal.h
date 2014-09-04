/*
 * ITerminal.h
 *
 *  Created on: 6 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef ITERMINAL_H_
#define ITERMINAL_H_

#include <string>

class ITerminal {
public:
	class IKeystroke {
	public:
		virtual char getChar() const = 0;
	};

	class IKeystrokeHandler
	{
	public:
		virtual unsigned int handleKeystroke(ITerminal& term,
                							 const IKeystroke& keystroke) = 0;
		virtual ~IKeystrokeHandler() {};
	};

	virtual void registerKeystrokeHandler(IKeystrokeHandler *ksH) = 0;
	virtual void inputPrompt() = 0;
	virtual void exitPrompt() = 0;
	virtual const std::string& getName() = 0;
	virtual ~ITerminal() {};
	virtual ITerminal& operator<< (bool val) = 0;
	virtual ITerminal& operator<< (short val) = 0;
	virtual ITerminal& operator<< (unsigned short val) = 0;
	virtual ITerminal& operator<< (int val) = 0;
	virtual ITerminal& operator<< (unsigned int val) = 0;
	virtual ITerminal& operator<< (long val) = 0;
	virtual ITerminal& operator<< (unsigned long val) = 0;
	virtual ITerminal& operator<< (float val) = 0;
	virtual ITerminal& operator<< (double val) = 0;
	virtual ITerminal& operator<< (long double val) = 0;
	virtual ITerminal& operator<< (void* val) = 0;
	virtual ITerminal& operator<< (const char* val) = 0;
	virtual ITerminal& operator<< (char val) = 0;
	virtual ITerminal& operator<< (unsigned char val) = 0;
	virtual ITerminal& operator<< (const std::string& val) = 0;

	static ITerminal* create(const std::string &termName);
	static ITerminal* create(const std::string &termName, IKeystrokeHandler *ksH);
	static void dispose(ITerminal* term);
};

#endif /* ITERMINAL_H_ */
