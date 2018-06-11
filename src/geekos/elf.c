/*
 * ELF executable loading
 * Copyright (c) 2003, Jeffrey K. Hollingsworth <hollings@cs.umd.edu>
 * Copyright (c) 2003, David H. Hovemeyer <daveho@cs.umd.edu>
 *
 * All rights reserved.
 *
 * This code may not be resdistributed without the permission of the copyright holders.
 * Any student solutions using any of this code base constitute derviced work and may
 * not be redistributed in any form.  This includes (but is not limited to) posting on
 * public forums or web sites, providing copies to (past, present, or future) students
 * enrolled in similar operating systems courses the University of Maryland's CMSC412 course.
 *
 * $Revision: 1.31 $
 *
 */

#include <geekos/errno.h>
#include <geekos/kassert.h>
#include <geekos/ktypes.h>
#include <geekos/screen.h> /* for debug Print() statements */
#include <geekos/pfat.h>
#include <geekos/malloc.h>
#include <geekos/string.h>
#include <geekos/user.h>
#include <geekos/fileio.h>
#include <geekos/elf.h>

#include <geekos/paging.h>

int elfDebug = 0;

/**
 * INU 201503120 Park Jun Young - Do not cheat !
 * From the data of an ELF executable, determine how its segments
 * need to be loaded into memory.
 * @param exeFileData buffer containing the executable file
 * @param exeFileLength length of the executable file in bytes
 * @param exeFormat structure describing the executable's segments
 *   and entry address; to be filled in
 * @return 0 if successful, < 0 on error
 * INU 201503120 Park Jun Young - Do not cheat !
 */
int Parse_ELF_Executable(char *exeFileData, ulong_t exeFileLength,
						 struct Exe_Format *exeFormat)
{
	int pcnt;	 // Program Header Counter
	int zcnt = 0; // Zero Entry Counter

	elfHeader *ehdr; // ELF Header address is the start address of exeFileData.
	programHeader *phdr;
	// Program Header starts away from exeFileData to Program Header Offset.

	// Exe_Segment Address
	// For test - Print Addr, Magic Code, Program Header Segment
	// Print(" Addr of ELF Header : 0x%08x\n",(int)ehdr);
	// Print(" ehdr->ident : %s\n",ehdr->ident);
	// Print(" # of Prgram Header : %d\n",ehdr->phnum);

	if (exeFileData == 0)
	{					  // Check NULL File
		return ENOTFOUND; // To make Get_Error_String(-2) - No file ...
	}

	ehdr = (elfHeader *)exeFileData;
	
	// Check ehdr->ident[0~3] = {0x74,'E','L','F'}
	if (ehdr->ident[0] != 0x7F && ehdr->ident[1] != 'E' && ehdr->ident[2] != 'L' && ehdr->ident[3] != 'F')
	{
		return -1; // To make Get_Error_String(-1) - Unspecified
	}

	exeFormat->numSegments = ehdr->phnum - zcnt; // Determine number of non-empty program header segments
	exeFormat->entryAddr = ehdr->entry;			 // Copy entry address of ELF Header for exeFormat

	phdr = (programHeader *)(exeFileData + ehdr->phoff);
	for (pcnt = 0; pcnt < ehdr->phnum; pcnt++)
	{ // Iterate for each Program Header ...
		struct Exe_Segment *xseg = &exeFormat->segmentList[pcnt];

		if (ehdr->phentsize == 0)
		{			//Empty Program Header
			xseg++; // Next Exe Segment
			phdr++; // Next Program Header Segment
			zcnt++; // Count empty Program Headers
			continue;
		}
		// Copy Program Header Segment to  Exe Segment
		xseg->offsetInFile = phdr->offset;
		xseg->lengthInFile = phdr->fileSize;
		xseg->startAddress = phdr->vaddr;
		xseg->sizeInMemory = phdr->memSize;
		xseg->protFlags = phdr->flags;

		xseg++; // Next Exe Segment
		phdr++; // Next Program Header Segment
	}
	// For test - Print the number of empty program headers
	// Print(" # of Empty Program Header : %d\n",zcnt);

	return 0;
}