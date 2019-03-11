#include "Disk.h"
#include <iostream>
#include <bitset>
#include <fstream>
#include <cstring>
#include <ctime>
#include "VolumeHeader.h"
#include "DAT.h"
#include "DirEntry.h"
#include "Sector.h"
#include "Dir.h"
#include "FileHeader.h"
#pragma once


Disk::Disk()
{
	vhd.initializeVH();
	dat.initializeDAT();
	mounted=false;
	currDiskSectorNr=0;
}

Disk::Disk(string & dName, string& oName , char letter) //verifier les crochers il faut recevoir 3 ou 4 parametres
{
	mounted=false;
	currDiskSectorNr=0;
	dat.initializeDAT();
	if(letter='c'){
		createDisk(dName, oName);
		mountDisk(dName);
	}
	if(letter='m'){
		mountDisk(dName);
	}
}

Disk::~Disk()
{
	if(mounted==true)
		unmountdisk();
}

//checks the writeSector the vhd is not written in the file!!!!
void Disk::createDisk(string & dname, string & oname)
{

	//check if the file exists
	if(dskfl.is_open())
	{
		dskfl.close();
		throw exception();
		//return;
	}

	dskfl.open(dname);
	vhd.sectorNr=0;
	//342 343 362 376
	//memcpy(vhd.diskName, dname.c_str(), 12)
	int i;
	for(i=0; i<dname.length(); i++)
		vhd.diskName[i]=dname[i];
	vhd.diskName[i]='\0';
	for(i=0; i<oname.length(); i++)
		vhd.diskOwner[i]=oname[i];
	vhd.diskOwner[i]='\0';
	//enter the date of production
	for(i=0; i<((string)__DATE__).length(); i++)
		vhd.prodDate[i]=((string)__DATE__)[i];
	vhd.prodDate[i]='\0';
	//number of cluster
	vhd.ClusQty=3200/2;
	//number of cluster in data
	vhd.dataClusQty=vhd.ClusQty-4;
	//adress of DAT
	vhd.addrDAT=1; //to check
	//adress of RootDirectory
	vhd.addrRootDir=2; //to check
	//writte copy
	//adress of cluster Data
	vhd.addrDataStart=2;
	vhd.isFormated=false;
	dskfl.open(dname, ios::out | ios::binary);
	for(unsigned int i=0; i<3200; i++)//creates the sectors in the file
	{
		currDiskSectorNr=i;
		//buffer->sectorNr=i;
		Sector *ptr=new Sector();
		ptr->sectorNr=i;
		writeSector(i, (Sector*) ptr);
		//writeSector(i) ne recoit pas de int ????
	}
	writeSector(vhd.sectorNr, (Sector*) &vhd);


	dskfl.close();
}

void Disk::mountDisk(string & dName)
{
	dskfl.open(dName, ios::in | ios::out | ios::binary);
	if(dskfl.is_open()){
		mounted=true;
		//to fill all the parameters of the disk
		readSector(0, (Sector*) &vhd); //on prend du file pour copier dans les champs de la classe
		readSector(1, (Sector*) &dat);
		readSector(2, (Sector*) &rootDir.msbSector);
		readSector(3, (Sector*) &rootDir.lsbSector);
	}
	else{
		mounted=false;
		throw exception ("Error the file doesn't exist!");
	}

}

void Disk::unmountdisk()
{
	if(!dskfl.is_open())
		throw exception ("Error the file doesn't exist!");

	//to update all the parameters of the disk in the file
	writeSector(0, (Sector*) &vhd); //copie des champs dans le file
	writeSector(1, (Sector*) &dat);
	writeSector(2, (Sector*) &rootDir.msbSector);
	writeSector(3, (Sector*) &rootDir.lsbSector);
	dskfl.close();
	mounted=false;
}

//378
void Disk::recreatedisk(string & downer)
{
	if(downer!=vhd.diskOwner)
		throw exception("Error you can't recreate this disk!");
	if(mounted==true)
		throw exception("Error the disk is mounted!");
	if(downer==vhd.diskOwner && mounted==false /*&& checks disk exists*/)
	{
		dskfl.open(vhd.diskName);
		vhd.sectorNr=0;
		//enter the date of production
		for(int i=0; i<((string)__DATE__).length(); i++)
			vhd.prodDate[i]=((string)__DATE__)[i];
		//number of cluster
		vhd.ClusQty=3200/2;
		//number of cluster in data
		vhd.dataClusQty=vhd.ClusQty-4;
		//adress of DAT
		vhd.addrDAT=1; //to check
		//adress of RootDirectory
		vhd.addrRootDir=2; //to check
		vhd.addrDATcpy=0;
		vhd.addrRootDirCpy=0;
		//adress of cluster Data
		vhd.addrDataStart=4;
		vhd.isFormated=false;
		dskfl.open(vhd.diskName, ios::out | ios::binary);
		for(unsigned int i=0; i<3200; i++)//creates the sectors in the file
		{
			currDiskSectorNr=i;
			writeSector(i, (Sector*) "");
			//writeSector(i) ne recoit pas de int ????
		}

		writeSector(vhd.sectorNr, (Sector*) &vhd);

		dskfl.close();
		mountDisk((string) (vhd.diskName));
	}

}

fstream * Disk::getdskfl(){
	try
	{
		fstream *ptr;
		ptr=&dskfl;
		return ptr;
	}
	catch(exception)
	{
		return NULL;
	}

}

void Disk::seekToSector(unsigned int i)
{
	dskfl.seekg(i*1024);
	dskfl.seekp(i*1024);
	currDiskSectorNr=i;
}

//void Disk::writeSector(unsigned int i, Sector* secInBuffer)
//{
//	if(dskfl.is_open()){
//		if(i==NULL){
//			i=currDiskSectorNr;
//		}
//		seekToSector(i);
//		dskfl.write((char*) secInBuffer, sizeof(*secInBuffer));
//		//buffer=secInBuffer;
//		currDiskSectorNr=i+1;
//	}
//	else
//		throw exception();
//}
//
//void Disk::writeSector(Sector* secInBuffer)
//{
//	if(dskfl.is_open()){
//		dskfl.write((char*) secInBuffer, sizeof(*secInBuffer));
//		//buffer=secInBuffer;
//		currDiskSectorNr++;
//	}
//	else
//		throw exception();
//}

void Disk :: writeSector(Sector* buffer) //writes from the buffer to the current sector
{
	if(!dskfl.is_open())
		throw exception();
	dskfl.write((char*)&(*buffer), sizeof(Sector)); 
	//currDiskSectorNr= 
}

void Disk :: writeSector(unsigned int index, Sector* buffer)
{
	if(!dskfl.is_open())
		throw exception();
	seekToSector(index); // moves the put and get pointers to the sector
	writeSector(buffer); //calls the function to write from the buffer to the current sector
}

void Disk::readSector(int i, Sector* secInBuffer)
{
	if(dskfl.is_open()){
		if(i==NULL){
			i=currDiskSectorNr;
		}
		seekToSector(i);
		dskfl.read((char*) secInBuffer, sizeof(*secInBuffer));
		currDiskSectorNr=i+1;
	}
	else
		throw exception();
}

void Disk::readSector(Sector* secInBuffer)
{
	if(dskfl.is_open()){
		//seekToSector(currDiskSectorNr);
		dskfl.read((char*) secInBuffer, sizeof(*secInBuffer));
	}
	else
		throw exception();
}

void Disk::format(string & ownerName) 
{
	try{
		if(ownerName!=vhd.diskOwner)
			throw exception();
		dat.initializeDAT();
		for (int i=0;i<14;i++)
		{
			rootDir.lsbSector.dirEntry[i].entryStatus='0';
			rootDir.msbSector.dirEntry[i].entryStatus='0';
		}
	}
	catch(exception)
	{
		cout<<"You are not the disk owner"<<endl;
	}
}

int Disk::howmuchempty()
{
	int counter=0;
	for(int i=0;i<1600;i++)
		if(dat.dat[i]==1)
			counter++;
	return counter;
}

void Disk::dealloc(DATtype & fat) 
{
	dat.dat=dat.dat|fat;
	for(int i=0; i<1600; i++)
		if(fat[i]==1)
		{
			fat[i]=0;
		}


}

void Disk::bestFit(DATtype & fat, unsigned int clusters)
{
	int counter=0;
	int min=15;
	int indexMin;
	for(int i=2;i<1599;i++){
		if(dat.dat[i]==1){
			while(dat.dat[i]==1&&i<1599){
				counter++;
				i++;
			}
		}
		if(min>counter&&counter>=clusters){
			min=counter;
			indexMin=i-counter;
			counter=0;
		}
		else
			counter=0;
	}
	if(min==1599)
		worstFit(fat, clusters);
	else{
		for(int k=indexMin; k<indexMin+clusters; k++)
		{
			fat[k]=1;
			dat.dat[k]=0;
		}
	}
}

void Disk::worstFit(DATtype & fat, unsigned int clusters)
{
	int counter;
	do{
		counter=0;
		int max=0;
		int indexMax;
		for(int i=2;i<1599;i++){
			if(dat.dat[i]==1){
				while(dat.dat[i]==1&&i<1599){
					counter++;
					i++;
				}
			}
			if(max<counter){
				max=counter;
				indexMax=i-counter;
				counter=0;
			}
		}
		
		int counter2=max;
		//to check if the data needs to be separated
		if(max>clusters)
			counter2=clusters;
		//if there are enough place it fills until clusters if not util max
		for(int k=indexMax; k<indexMax+counter2; k++)
		{
			fat[k]=1;
			dat.dat[k]=0;
		}

		if(counter2==clusters)
			return;
		clusters=clusters-counter2;
	}while(clusters!=0);

}

//verif DAT ne se remplit pas erreur sur ou|
void Disk::firstFit(DATtype & fat, unsigned int clusters)
{
	int j,i;
	for( i=2;i<1599;i++){
		if(dat.dat[i]==1){
			for(j=i;j<clusters+i && j<1599;j++)
			{
				if(dat.dat[j]==0)
					break;
			}
			if(j==(clusters+i))
			{
				for(int k=i; k<clusters+i; k++)
				{
					fat[k]=1;
					dat.dat[k]=0;
				}
				return;
			}
			else
			{
				i=j;
			}
		}

	}
	if(i==1600)
		worstFit(fat, clusters);
}


void Disk::alloc(DATtype & fat, unsigned int sector, unsigned int algorithm)
{
	//to initialize fat to 0
	fat.reset();
	allocextend(fat, sector, algorithm);
}

void Disk::allocextend(DATtype & fat, unsigned int sectors, unsigned int algorithm) 
{
	try{
		int clusters;
		if(sectors%2==0)
			clusters=sectors/2;
		else
			clusters=sectors/2+1;

		//checks if there is enough place in the disk
		if(clusters>howmuchempty())
			throw exception("No place in the disk!");

		switch(algorithm)
		{
		case 0://first fit
			firstFit(fat,clusters);
			break;

		case 1: //best fit
			bestFit(fat,clusters);
			break;

		case 2: //worst fit
			worstFit(fat,clusters);
			break;
		}

	}
	catch(exception)
	{
		cout<<"No place in the disk!"<<endl;
	}
}