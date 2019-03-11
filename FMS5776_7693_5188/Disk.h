#include <iostream>
#include <bitset>
#include <fstream>
#include <string>
#include <ctime>
#include "VolumeHeader.h"
#include "DAT.h"
#include "DirEntry.h"
#include "Sector.h"
#include "Dir.h"
#include "FileHeader.h"
#pragma once

using namespace std;

//class FCB
//{
//	DirEntry fileDesc;
//	bitset<1600> FAT;
//	Sector Buffer;
//	bool inUse; //checks if the object is in use or not
//	unsigned int currRecNr; //num of the record on which we are
//	unsigned int currSecNr; //num of sector on which we are
//	unsigned int currRecNrlnBuff; //num of the record of the sector in the buffer
//public:
//	FCB();
//};
//
//FCB::FCB()
//{
//	//Buffer=
//	//fileDesc=
//	FAT=0;
//	inUse=false;
//	currRecNr=0;
//	currSecNr=0;
//	currRecNrlnBuff=0;
//}

class Disk
{
public:
	VolumeHeader vhd;
	DAT dat;
	RootDir rootDir;
	bool mounted;
	fstream dskfl;
	unsigned int currDiskSectorNr; //index of sector the file buffer
	//Sector *buffer; //current sector
	Disk();//s
	~Disk();//s
	Disk(string &, string&, char);//k
	void createDisk(string &, string &);//s
	void mountDisk(string &);//s
	void unmountdisk();//k
	void recreatedisk(string &);//k
	fstream *getdskfl();//s
	void seekToSector(unsigned int);//k
	void writeSector(unsigned int, Sector*);//s
	void writeSector(Sector *);//s
	void readSector(int, Sector*);//k
	void readSector(Sector*);//k
	void format(string &);//k
	int howmuchempty();//s
	void dealloc(DATtype &);//k
	void alloc(DATtype &, unsigned int, unsigned int);//s
	void allocextend(DATtype &, unsigned int, unsigned int);//s
	void firstFit(DATtype & fat, unsigned int );//s
	void bestFit(DATtype & fat, unsigned int);//k
	void worstFit(DATtype & fat, unsigned int);//k
};





