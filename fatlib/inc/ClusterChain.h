/*
 * ClusterChain.h
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */

#ifndef CLUSTERCHAIN_H_
#define CLUSTERCHAIN_H_

#include <vector>
#include <IFAT.h>

class FATDisk;

class ClusterChain
{
protected:
	std::vector<unsigned int> elems_;
	FATDisk& disk_;
public:
	ClusterChain(FATDisk &disk, unsigned int FirstCluster);
	unsigned int operator[](unsigned int i);
	unsigned int size();
};


#endif /* CLUSTERCHAIN_H_ */
