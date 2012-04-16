/*
	File:		ErrMgr.h

	Copyright:	© 1984-1993 by Apple Computer, Inc., all rights reserved.
*/

#ifndef __ERRMGR__
#define __ERRMGR__

#ifndef __TYPES__
#include <MacTypes.H>
#endif

#ifdef __cplusplus
extern "C" {
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

extern void InitErrMgr(char *toolErrFilename, char *sysErrFilename, Boolean showToolErrNbrs);

extern Boolean pascalStrings;

extern void CloseErrMgr(void);

extern char *GetSysErrText(short msgNbr, char *errMsg);

extern char *GetToolErrText(short msgNbr, char *errInsert, char *errMsg);

extern void AddErrInsert(unsigned char *insert, unsigned char *msgString);

extern unsigned char *addInserts(unsigned char *msgString, unsigned char *insert, ...);

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

