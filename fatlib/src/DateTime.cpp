/*
 * Time.cpp
 *
 *  Created on: May 28, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#include "DateTime.h"
#include <sstream>

bool Time::isValid()
{
	if (msec_ > 99)
		return false;
	if (sec_ > 59)
		return false;
	if (min_ >59)
		return false;
	if (hour_> 24)
		return false;

	return true;
}

Time::operator std::string()
{
	if (isValid())
	{
		std::stringstream ss;
		ss << hour_ << ":" << min_ << ":" << sec_ << "." << msec_;
		return ss.str();
	}
	return "Invalid";
}

bool Date::isValid()
{
	if (year_ < 1980)
		return false;
	if (month_ > 12)
		return false;
	if (day_ >31)
		return false;

	return true;
}

Date::operator std::string()
{
	if (isValid())
	{
		std::stringstream ss;
		ss << day_ << "/" << month_ << "/" << year_;
		return ss.str();
	}
	return "Invalid";
}
