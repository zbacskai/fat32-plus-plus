/*
 * Terminal.cpp
 *
 *  Created on: 30 May 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "Terminal.h"

#include <stdio.h>
#include <unistd.h>

#include <iostream>

void Terminal::setTcs()
{
	struct termios ntios;
	tcgetattr(STDIN_FILENO, &otios_);
	ntios = otios_;
	ntios.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &ntios);
}

Terminal::Terminal(const std::string &termName) : name_(termName), ksH_(0)
{
	setTcs();
}

Terminal::Terminal(const std::string &termName, IKeystrokeHandler *ksH) :
																name_(termName),
																ksH_(ksH)
{
	setTcs();
}

void Terminal::inputPrompt()
{
	isInputPrompt_ = true;
	while(isInputPrompt_)
	{
		int ch = getchar();
		Keystroke ks(ch);
		if (ksH_)
			ksH_->handleKeystroke(*this, ks);
		else
			std::cout << "No keystroke handler registered";
	}
}

Terminal::~Terminal()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &otios_);
}


//TODO: introduce proper implementation later
Terminal& Terminal::operator<< (bool val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (short val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (unsigned short val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (int val)
{
	std::cout << val;
	return *this;
}
Terminal& Terminal::operator<< (unsigned int val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (long val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (unsigned long val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (float val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (double val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (long double val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (void* val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (const char* val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (char val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (unsigned char val)
{
	std::cout << val;
	return *this;
}

Terminal& Terminal::operator<< (const std::string& val)
{
	std::cout << val;
	return *this;
}

