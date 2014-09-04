/*
 * Singleton.h
 *
 *  Created on: 4 Jun 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

template <class T>
class Singleton {
public:
	static T& instance()
	{
		static T myObj;
		return myObj;
	}
};


#endif /* SINGLETON_H_ */
