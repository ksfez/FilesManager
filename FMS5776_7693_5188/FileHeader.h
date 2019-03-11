#include <iostream>
#include "DirEntry.h"
# include "DAT.h"
#pragma once
using namespace std;


class FileHeader
{
public:
	unsigned int SectorNr;
	DirEntry fileDesc;
	DATtype FAT;
	char emptyArea[744];
	FileHeader(int sectorNr, DirEntry fD);
};

