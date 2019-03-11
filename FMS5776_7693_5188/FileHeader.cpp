#include "FileHeader.h"

FileHeader::FileHeader(int sectorNr, DirEntry fD)
{
	SectorNr=sectorNr;
	fileDesc=fD;
}