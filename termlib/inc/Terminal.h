/*
 * Terminal.h
 *
 *  Created on: 30 May 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <termios.h>
#include <string>

#include "ITerminal.h"

class Terminal : public ITerminal {
private:
	std::string name_;
	bool isInputPrompt_;
	void setTcs();
public:
	class Keystroke : public IKeystroke {
	private:
		char chr_;
	public:
		Keystroke(char c) : chr_(c) {};
		char getChar() const { return chr_;};
	};

	Terminal(const std::string &termName);
	Terminal(const std::string &termName, IKeystrokeHandler *ksH);
	void registerKeystrokeHandler(IKeystrokeHandler *ksH) { ksH_ = ksH; };
	void inputPrompt();
	void exitPrompt() { isInputPrompt_ = false; };
	const std::string& getName() { return name_; };
	~Terminal();
	Terminal& operator<< (bool val);
	Terminal& operator<< (short val);
	Terminal& operator<< (unsigned short val);
	Terminal& operator<< (int val);
	Terminal& operator<< (unsigned int val);
	Terminal& operator<< (long val);
	Terminal& operator<< (unsigned long val);
	Terminal& operator<< (float val);
	Terminal& operator<< (double val);
	Terminal& operator<< (long double val);
	Terminal& operator<< (void* val);
	Terminal& operator<< (const char* val);
	Terminal& operator<< (char val);
	Terminal& operator<< (unsigned char val);
	Terminal& operator<< (const std::string& val);
private:
	struct termios otios_;
	IKeystrokeHandler *ksH_;
};


#endif /* TERMINAL_H_ */
