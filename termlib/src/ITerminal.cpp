/*
 * ITerminal.cpp
 *
 *  Created on: 6 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "ITerminal.h"
#include "Terminal.h"

ITerminal* ITerminal::create(const std::string &termName)
{
	return new Terminal(termName);
}

ITerminal* ITerminal::create(const std::string &termName,
		                     IKeystrokeHandler *ksH)
{
	return new Terminal(termName, ksH);
}

void ITerminal::dispose(ITerminal* term)
{
	delete term;
}



