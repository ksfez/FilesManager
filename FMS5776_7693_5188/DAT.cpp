#include "DAT.h"
#pragma once

void DAT::initializeDAT()
{
	sectorNr=1;
	//the first and the second clusters are busy for the disk information
	dat[0]=0;
	dat[1]=0;
	for(int i=2; i<1600; i++)
		dat[i]=1;
}


ostream& operator<<(ostream os, DAT& dat)
{
	os<<"The current DAT is"<<endl;
	os<<"Free Space=" <<(double)dat.dat.count()/(dat.dat.size()-2)*100.0 << "%"<<endl;
	return os;
}
