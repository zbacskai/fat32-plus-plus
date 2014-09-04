/*
 * Log.cpp
 *
 *  Created on: May 27, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "Log.h"
#include <iostream>
#include <string>

Log::LogLevel Log::gLevel_;

Log::Log(LogLevel level) : level_(level)
{
	if (level <= gLevel_)
		ss_ = new std::stringstream();
	else
		ss_=0;

	nLF_ = false;
}

Log& Log::nLF()
{
	nLF_ = true;
	return *this;
}

Log& Log::LF()
{
	if (ss_)
		*ss_ << std::endl;

	return *this;
}

Log::~Log()
{
	if (ss_ != 0)
	{
		std::cout << ss_->str();
		if (!nLF_)
			std::cout << std::endl;

		delete ss_;
	}
}

#define oper(x) \
	{ if (ss_) \
		 *ss_ << val; };

Log& Log::operator<< (bool val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (short val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (unsigned short val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (int val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (unsigned int val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (long val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (unsigned long val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (float val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (const char* val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (double val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (long double val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (void* val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (const std::string &val)
{
	oper(val);
	return *this;
}

Log& Log::operator<< (const LogModifiers val)
{
	switch (val)
	{
		case Hex:
			std::cout << std::hex;
			break;
		case Dec:
			// Fall through
		default:
			std::cout << std::dec;
			break;
	}

	return *this;
}
