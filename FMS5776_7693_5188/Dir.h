#include <iostream>
#include "DirEntry.h"
#pragma once
using namespace std;

struct SectorDir
{
	int sectorNr;
	DirEntry dirEntry[14];
	char unUse[12];
};

struct RootDir
{
	SectorDir msbSector;
	SectorDir lsbSector;
public:
	RootDir(void);
	~RootDir(void);
};
