/*
 * Log.h
 *
 *  Created on: May 27, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef LOG_H_
#define LOG_H_

#include <sstream>

class Log
{
public:
	enum LogLevel {
		Console = 10,
		Debug = 100,
	};
private:
	LogLevel level_;
	mutable std::stringstream* ss_;
	bool nLF_;
	static LogLevel gLevel_;
public:
	Log(LogLevel level);
	Log(const Log& l) : level_(l.level_), ss_(l.ss_), nLF_(l.nLF_) { l.ss_ = 0; };
	void setLevel(LogLevel l) { gLevel_ = l; };
	LogLevel getLevel() const { return level_; };
	Log& nLF();
	Log& LF();
	~Log();
	enum LogModifiers {
		Dec,
		Hex,
	};

	Log& operator<< (bool val);
	Log& operator<< (short val);
	Log& operator<< (unsigned short val);
	Log& operator<< (int val);
	Log& operator<< (unsigned int val);
	Log& operator<< (long val);
	Log& operator<< (unsigned long val);
	Log& operator<< (float val);
	Log& operator<< (double val);
	Log& operator<< (long double val);
	Log& operator<< (void* val);
	Log& operator<< (const char* val);
	Log& operator<< (const std::string &val);
	Log& operator<< (const LogModifiers val);
};


#endif /* LOG_H_ */
