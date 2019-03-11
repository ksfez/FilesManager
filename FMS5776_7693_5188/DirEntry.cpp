#include "DirEntry.h"

DirEntry::DirEntry(char *filename, int fSize)
{
	for(int i=0; i<12; i++)
		Filename[i]=filename[i];
	fileSize=fSize;

}
DirEntry::DirEntry()
{
}