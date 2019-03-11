#include <iostream>
#pragma once
using namespace std;

struct Sector
{
	unsigned int sectorNr;
	char rawData[1020];
public:
	Sector();
	~Sector();
};
