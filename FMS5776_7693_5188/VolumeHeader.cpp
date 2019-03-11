#include "VolumeHeader.h"
#include <iostream>
#include <cstring>
#pragma once

VolumeHeader::VolumeHeader(){
	diskName[0]='\0';
	diskOwner[0]='\0';
	prodDate[0]='\0';
	formatDate[0]='\0';
	ClusQty=3200/2;
	dataClusQty=ClusQty-4;
	addrDAT=1; 
	addrRootDir=2; 
	addrDataStart=4;
	isFormated=false;
	addrDATcpy=0;
	addrRootDirCpy=0;
}

void VolumeHeader::initializeVH(){
	diskName[0]='\0';
	diskOwner[0]='\0';
}