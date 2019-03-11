#include <iostream>
#include <string.h>
#pragma once
using namespace std;

class VolumeHeader
{
public:
	unsigned int sectorNr;
	char diskName[12];
	char diskOwner[12];
	char prodDate[10]; //date of production
	unsigned int ClusQty; //num of cluster in the disk
	unsigned int dataClusQty; //num cluster in data
	unsigned int addrDAT; //address of the sector that contains the DAT
	unsigned int addrRootDir; //address of the sector that contains RootDirectory
	unsigned int addrDATcpy; //address of the sector that contains the copy of the DAT
	unsigned int addrRootDirCpy; //address of the sector that contains the copy of RootDirectory
	unsigned int addrDataStart; //address of cluster of Data start
	char formatDate[10]; //date of formating (date de mise en forme)
	bool isFormated;
	char emptyArea[947];
	VolumeHeader();
	void initializeVH();
};

