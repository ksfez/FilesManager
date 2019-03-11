#include <iostream>
#include <bitset>
#pragma once
using namespace std;

typedef bitset<1600> DATtype;

class DAT
{
public:
	unsigned int sectorNr;
	DATtype dat;
	char emptyArea[820];
	void initializeDAT();
	friend ostream& operator<<(ostream os, DAT& dat);
};

