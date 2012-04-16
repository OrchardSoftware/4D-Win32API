/*
	File:		SegLoad.h

	Copyright:	© 1983-1993 by Apple Computer, Inc.
				All rights reserved.

	Version:	System 7.1 for ETO #11
	Created:	Tuesday, March 30, 1993 18:00

*/

#ifndef __SEGLOAD__
#define __SEGLOAD__

#ifndef __TYPES__
#include "MacTypes.h"
#endif

#if PRAGMA_IMPORT
#pragma import on
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
	#pragma pack(2)
#endif

enum {

    appOpen = 0,    /*Open the Document (s)*/
    appPrint = 1    /*Print the Document (s)*/
};

struct AppFile {
    short vRefNum;
    OSType fType;
    short versNum;  /*versNum in high byte*/
    Str255 fName;
};

typedef struct AppFile AppFile;


#ifdef __cplusplus
extern "C" {
#endif

#if WINVER
#define UnloadSeg(p) /* */
#else
F_PASCAL( void ) UnloadSeg(void * routineAddr); 
#endif
F_PASCAL( void ) ExitToShell(void); 
F_PASCAL( void ) GetAppParms(Str255 apName,short *apRefNum,Handle *apParam); 
F_PASCAL( void ) CountAppFiles(short *message,short *count); 
F_PASCAL( void ) GetAppFiles(short index,AppFile *theFile); 
F_PASCAL( void ) ClrAppFiles(short index); 
void getappparms(char *apName,short *apRefNum,Handle *apParam); 
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
	#pragma pack()
#endif

#ifdef PRAGMA_IMPORT_OFF
#pragma import off
#elif PRAGMA_IMPORT
#pragma import reset
#endif

#ifdef __cplusplus
}
#endif

#endif

