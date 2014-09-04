/*
 * Time.h
 *
 *  Created on: May 28, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef TIME_H_
#define TIME_H_

#include <string>

class Time {
	private:
		unsigned int hour_;
		unsigned int min_;
		unsigned int sec_;
		unsigned int msec_;
	public:
		Time(unsigned int hour, unsigned int min,
				unsigned int sec, unsigned int msec) : hour_(hour), min_(min),
				sec_(sec), msec_(msec) {};
		bool isValid();
		operator std::string();
};

class Date {
	private:
		unsigned int year_;
		unsigned int month_;
		unsigned int day_;
	public:
		Date(unsigned int year, unsigned int month, unsigned int day) :
			year_(year), month_(month), day_(day) {};
		bool isValid();
		operator std::string();
};

#endif /* TIME_H_ */
