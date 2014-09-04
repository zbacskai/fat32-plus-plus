/*
 * ClusterChain.cpp
 *
 *  Created on: May 22, 2014
 *      Author: z.bacskai.jr@gmail.com
 */


#include "ClusterChain.h"
#include "FATDisk.h"

ClusterChain::ClusterChain(FATDisk& disk,
		                   unsigned int FirstCluster) : disk_(disk)
{
	elems_.push_back(FirstCluster);
	IFAT::NextCluster nc(disk_.getFAT()->getNextCluster(FirstCluster));
	while (nc.isValid())
	{
		elems_.push_back(nc);
		nc = disk_.getFAT()->getNextCluster(nc);
	}
}

unsigned int ClusterChain::operator [](unsigned int i)
{
	return elems_[i];
}

unsigned int ClusterChain::size()
{
	return elems_.size();
}

