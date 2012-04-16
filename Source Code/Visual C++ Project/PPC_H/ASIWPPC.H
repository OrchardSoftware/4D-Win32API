// ASIWPPC.H

#ifndef _ASIWPPC_H
#define _ASIWPPC_H


#if PPC_HEADERS

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

#define LMGetGrayRgn()		GetGrayRgn()

#define LMGetUTableBase()	UTableBase
#define LMGetCurStackBase()	CurStackBase
#define LMGetCurApRefNum()	CurApRefNum
#define LMGetKeyRepThresh()	KeyRepThresh
#define LMGetKeyThresh()	KeyThresh
#define LMGetCurJTOffset()	CurJTOffset
#define LMGetWindowList()	WindowList
#define LMGetGhostWindow()  GhostWindow
#define LMGetResLoad()		ResLoad
#define LMGetHiliteMode()	HiliteMode
#define LMGetCurApName()	CurApName
#define LMGetFSFCBLen()		FSFCBLen
#define LMGetCrsrBusy()		CrsrBusy
#define LMGetHWCfgFlags()	HWCfgFlags
#define GetUnitEntryCount()	UnitNtryCnt

#define LMGetGZRootHnd()	GZRootHnd
#define LMGetGZMoveHnd()	GZMoveHnd

void LMSetROMMapInsert(unsigned char ROMMapInsertValue);
void LMSetResLoad(unsigned char ResLoadValue);
void LMSetHiliteRGB(RGBColor *HiliteRGBValue);


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

void LMSetROMMapInsert(unsigned char ROMMapInsertValue)
{
	RomMapInsert = ROMMapInsertValue;
}

void LMSetResLoad(unsigned char ResLoadValue)
{
	ResLoad = ResLoadValue;	
}

void LMSetHiliteRGB(RGBColor *HiliteRGBValue)
{
	HiliteRGB = HiliteRGBValue;	
}


#endif // PPC_HEADERS

#endif // _ASIWPPC_H

