#include <iostream>
#include <cstring>
#include <string>
#include <bitset>
#include <ctime>
#include <fstream>
#include "Disk.h"
#include "Dir.h"
#include "DirEntry.h"
#include "FileHeader.h"
#include "Sector.h"
#include "VolumeHeader.h"
# pragma once

using namespace std;

static char* toChar(string msg)
{
	char *key;
	key=new char [msg.length()+1];
	for(int i=0; i<msg.length(); i++)
		key[i]=msg[i];
	return key;
}

static void printStructSize()
{
	//cout<< "Size of disk   "<<sizeof(Disk)<<endl;
	cout << "Size Of Sector -->" << sizeof(Sector) << endl;
	cout << "Size Of VolumeHeader -->" << sizeof(VolumeHeader) << endl;
	cout << "Size Of DAT -->" << sizeof(DAT) << endl;
	cout << "Size Of DirEntry -->" << sizeof(DirEntry) << endl;
	cout << "Size Of SectorDir -->" << sizeof(SectorDir) << endl;
	cout << "Size Of FileHeader -->" << sizeof(FileHeader) << endl;
	cout << "Size Of RootDir -->" << sizeof(RootDir) << endl;
}

static void printDiskInfo(Disk& d)
{
	VolumeHeader* vh = &d.vhd;
	
	cout << "Sector Number = "<<vh->sectorNr<<endl;
	cout << "Disk name = " << vh->diskName << endl;
	cout << "Owner Name = " << vh->diskOwner << endl;
	cout << "Product Date = " << vh->prodDate << endl;
	cout << "Format Date = " << vh->formatDate << endl;
	cout << "IsFormated = " << vh->isFormated << endl;
	cout << "Address of Data Start = " << vh->addrDataStart << endl;
	cout << "Cluster Qty = " << vh->ClusQty << endl;
	cout << "Data Cluster Qty = " << vh->dataClusQty << endl;
	cout << "Address of DAT = " << vh->addrDAT << endl;
	cout << "Address of RootDirectory = " << vh->addrRootDir << endl;
	cout << "Address of DAT copy = " << vh->addrDATcpy << endl;
	cout << "Address of RootDirectory Copy = " << vh->addrRootDirCpy << endl << endl;

}

static void test_create(string diskName, string ownerName)
{
	Disk d;
	cout << "\npre createdisk: " << endl;
	printDiskInfo(d);
	cout << "post createdisk: " << endl;
	d.createDisk(diskName, ownerName);
	printDiskInfo(d);
}

long fileSize(char* fname)
{
	ifstream f;
	long size = 0;

	f.open(fname);

	if(f)
	{
		f.seekg (SEEK_END, 0);
		size=(int)f.tellg();
		f.close();           
	}
	return size;
}


static void test_mount(string diskName)
{
	Disk d;
	cout << "\npre mountdisk: " << endl;
	printDiskInfo(d);
	d.mountDisk(diskName);
	cout << "post mountdisk: " << endl;
	printDiskInfo(d);
	d.unmountdisk();
}


static void test_rwSector(string diskName)
{
	Disk d;
	Sector sector;
	d.mountDisk(diskName);

	cout << "\nread sector: " << endl;
	d.readSector(8, &sector);
	strcpy_s(sector.rawData, "this is write temp sector");
	d.writeSector(8, &sector);
	d.unmountdisk();

}

//void writeFile(char *fname, int size, Disk disk)
//{
//	Sector *buffer;
//	int numOfsector; 
//	float numSec;
//	numSec=size/1020;
//	if(numSec-(int)numSec>0)
//		numOfsector=(int)numSec+1;
//	else numOfsector=(int)numSec;
//	cout<<"Sector needed = "<<numOfsector<< endl;
//	fstream f;
//	f.open(fname, ios::in| ios::binary); //to open the file in read binary
//	disk.seekToSector(4);
//
//	for(int i=0;i<numOfsector;i++)
//	{
//		f.seekg(i*1020);
//		buffer->sectorNr=i;
//		if(i<numOfsector-1)
//			f.read(buffer->rawData, 1020);
//		else
//			f.read(buffer->rawData, size-1020*i);
//		disk.writeSector(buffer);
//	}
//}

//void readFile(char *fname, Disk disk)
//{
//	Sector *buffer;
//	int i=4;
//	fstream file;
//	file.open(fname, ios::in| ios::out);
//	do{
//		disk.readSector(i, buffer);
//		i++;
//		file.write(buffer->rawData, 1020);
//	}while(buffer->rawData!='\0');
//
//}

int saveFileToDisk(Disk& disk, Sector *buffer)
{
	//Sector buffer[1020];

	cout << "Enter file name to save: ";
	string fileName;
	cin >> fileName;
	fstream file;
	file.open(fileName, ios::in | ios::binary);
	if(!file.is_open())
		throw exception("File not exists!");
	// Find file size
	file.seekg(0, SEEK_END);
	int size = file.tellg();
	int sectors = size / sizeof(buffer->rawData) + ((size % sizeof(buffer->rawData) > 0) ? 1 : 0);
	cout << "File size = " << size << " bytes\n";
	cout << "We need " << sectors << " sectors\n";
	for(int i = 0; i < sectors; i++)
	{
		file.seekg(i * sizeof(buffer),SEEK_SET);
		if(i < sectors - 1)
			file.read(buffer->rawData, sizeof(buffer->rawData));
		else // last sector
			file.read(buffer->rawData, size - sizeof(buffer->rawData)*i);

		disk.writeSector(i+4, buffer);
	}
	file.close();
	cout << "The file " << fileName << " has been written to disk\n";
	return size;
}

void createFileFromDisk(Disk& disk, int size, int firstSector)
{
	char buffer[sizeof(Sector)];

	cout << "Enter file name to create: ";
	string fileName;
	cin >> fileName;
	fstream file;
	file.open(fileName,ios::out | ios::binary);
	int sectors = size / (sizeof(Sector)-4) + ((size % (sizeof(Sector)-4) > 0) ? 1 : 0);
	cout << "Total sectors to read: " << sectors << endl;
	for(int i = 0; i < sectors; i++)
	{
		disk.readSector(i+firstSector, (Sector*)buffer);

		if(i < sectors - 1)
			file.write(buffer+4, sizeof(Sector)-4);
		else // last sector
			file.write(buffer+4, size - (sizeof(Sector)-4)*i);
	}
	file.close();
	cout << "The file " << fileName << " has been created from disk\n";
}

void testAllocation(Disk& disk, DATtype myFAT[])
{
	int fileNum, sectors, fitType, location, size, i = 1;
	char action;
	string types[] = {"First fit", "Best fit", "Worst fit"};

	ifstream file("alloc.txt");
	if(!file)
		throw "alloc.txt not exists";
	while(!file.eof())
	{
		file >> action >> fileNum;
		if(action == 'A' || action == 'E')
		{
			file >> sectors >> fitType;
			if(action == 'A')
			{
				cout << i++ << ". Allocate file " << fileNum << ". Sectors = " << sectors << " Type = " << types[fitType] << endl;
				disk.alloc(myFAT[fileNum-1],sectors,fitType);
			}
			else
			{
				cout << i++ << ". Extend Allocate file " << fileNum << ". Sectors = " << sectors << " Type = " << types[fitType] << endl;
				disk.allocextend(myFAT[fileNum-1],sectors,fitType);
			}
			cout << "The FAT of file " << fileNum << " is: \n" << myFAT[fileNum-1] << endl;
		}
		else
		{
			cout << i++ << ". Deallocate file " << fileNum <<  endl;
			disk.dealloc(myFAT[fileNum-1]);
		}
		system("pause");
	}
	file.close();
}

int main()
{
	int choice, size, fileSize, numOfsector, code, sectorNr, fileNum;
	cout<<endl;
	Disk disk;
	string dname, oname;
	char fname[12];
	Sector *buffer;
	buffer=new Sector();
	DATtype myFAT[3];
	do{
		cout<<endl<<"MAIN MENU\n";
		cout<<"=========\n";
		cout<<"1) Create disk.\n";
		cout<<"2) Mount disk.\n";
		cout<<"3) Print sizes.\n";
		cout<<"4) Volume header.\n";
		cout<<"5) Write to sector.\n";
		cout<<"6) Read from sector.\n";
		cout<<"7) Write file to disk.\n";
		cout<<"8) Read file from disk.\n";
		cout<<"9) unmount disk"<<endl;
		cout<<"10) recreate disk"<<endl;
		cout << "11) Format Disk.\n";
		cout << "12) Allocate sectors.\n";
		cout << "13) Extend Allocate sectors.\n";
		cout << "14) Show all FATs.\n";
		cout << "15) Deallocate File.\n";
		cout << "16) Test allocation from file.\n";
		cout<< "17) Show DAT \n";
		cout<<"0) Exit \n";
		cout<<"Please enter your selection: ";
		cin>>choice;

		switch(choice){
		case 1: //create the disk
			cout<<"Enter disk name: ";
			cin>>dname;
			cout<<"Enter disk owner name: ";
			cin>>oname;
			disk.createDisk(dname, oname);
			cout<<"The disk have been created successfully\n";
			cout<<endl;
			break;
		case 2: //mount the disk
			try
			{
				cout<<"Enter disk name: ";
				cin>>dname;
				disk.mountDisk(dname);
				cout<<"Disk "<<dname<<" has been mounted.\n";
			}
			catch(exception)
			{
				cout<<"This disk doesn't exist\n";
			}
			cout<<endl;
			break;
		case 3: //print sizes
			printStructSize();
			cout<<endl;
			
			break;
		case 4: //print infos on volume header
			try{
				if(disk.mounted==true)
				{
					cout<<"Volume header:\n";
					printDiskInfo(disk);
				}
				else
					throw exception();
			}
			catch(exception)
			{
				cout<<"Error the disk isn't mounted!\n";
			}
			cout<<endl;
			

			break;
		case 5: //write to sector
			try{
				cout<<"Enter sector number (from 4 to 3199): ";
				cin>>sectorNr;
				cin.ignore();
				cout<<"Enter data for this sector:\n";
				buffer->sectorNr=sectorNr;
				cin.getline(buffer->rawData, 1020);
				cout<<"hello"<<buffer->rawData;
				disk.writeSector(sectorNr, buffer);
				buffer->sectorNr=0;
				buffer->rawData[0]='\0';
			}
			catch(exception)
			{
				buffer->sectorNr=0;
				buffer->rawData[0]='\0';
				cout<<"Error there is no open disk!\n";
			}
			cout<<endl;
			
			break;
		case 6: //Read from sector
			try{
				cout<<"Enter sector number(4-3199):\n";//4-3199
				cin>>sectorNr;
				disk.readSector(sectorNr, buffer);
				if(buffer->rawData[0]=='\0')
					cout<<"The sector is empty!"<<endl;
				else{
					cout<<"The data is: ";
					cout<<buffer->rawData<<endl<<endl;
				}
			}
			catch(exception)
			{
				cout<<endl<<"Error there is no open disk!\n";
			}
			
			break;
		case 7: //Write file to disk
			fileSize=saveFileToDisk(disk, buffer);
			
			break;
		case 8: //Read file from disk
			if(fileSize>0)
				createFileFromDisk(disk, fileSize, 4);
		case 9: //unmount
			disk.unmountdisk();
			cout<<"Disk has been unmounted sucessfully\n";
			break;
		case 10: //recreate disk
			try{
				cout<<"Enter the disk owner:"<<endl;
				cin>>oname;
				disk.recreatedisk(oname);
				cout<<"Disk has been recreated\n";
			}
			catch(exception)
			{
				cout<<"No right owner name!"<<endl;
			}
			break;

		case 11://format the disk
			cout << "Enter owner name: ";
			cin >> oname;
			disk.format(oname);
			break;
		case 12://alloc
			cout << "Enter number of file: ";
			cin >> fileNum;
			cout << "Enter number of sectors to allocate: ";
			cin >> numOfsector;
			cout << "Enter 0 for first fit, 1 for best fit, or 2 for wrost fit: ";
			cin >> code;
			disk.alloc(myFAT[fileNum-1],numOfsector,code);
			cout << "The FAT of file " << fileNum << " is: \n" << myFAT[fileNum-1].to_string() << endl;
			break;
		case 13://allocextend
			cout << "Enter number of file: ";
			cin >> fileNum;
			cout << "Enter number of sectors to allocate: ";
			cin >> numOfsector;
			disk.allocextend(myFAT[fileNum-1],numOfsector,0);
			cout << "The FAT of file " << fileNum << " is: \n" << myFAT[fileNum-1].to_string() << endl;
			break;
		case 14://to print the fat
			for(int i = 0; i < 3; i++)
			{
				cout << "FAT " << i+1 << ":\n";
				cout << myFAT[i].to_string();
				cout << endl;
			}
			break;
		case 15://dealloc
			cout << "Enter number of file: ";
			cin >> fileNum;
			disk.dealloc(myFAT[fileNum-1]);
			cout << "The FAT of file " << fileNum << " is: \n" << myFAT[fileNum-1].to_string() << endl;
			break;
		case 16:
			testAllocation(disk, myFAT);
			break;
		case 17:
			cout<<disk.dat.dat;
			break;

		default:
			cout << "Bad input, Try again!\n";
		}

		cout << "Enter any key to continue...\n";
		char c[2];
		flushall();
		gets(c);

	} while(choice!=0);
}
