/*
 *  LoMem.h - definitions of lo-mem globals
 *
 *  Copyright (c) 1991 Symantec Corporation.  All rights reserved.
 *
 *  These interfaces are based on material copyrighted
 *  by Apple Computer, Inc., 1985-1993.
 *
 */

#ifndef __LOMEM__
#define __LOMEM__

#ifndef __TYPES__
#include <MacTypes.h>
#endif

#ifndef __CONTROLS__
#include <Controls.h>
#endif

#ifndef __DEVICES__
#include <Devices.h>
#endif

#ifndef __DISKINIT__
#include <DiskInit.h>
#endif

#ifndef __FONTS__
#include <Fonts.h>
#endif

#ifndef __MEMORY__
#include <MemoryM.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdra.h>
#endif

#ifndef __SCRAP__
#include <Scrap.h>
#endif

#ifndef __WINDOWS__
#include <WindowM.h>
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

#ifdef THINK_C
	#define	Declare_LoMem(type, name, address)	type (name) : (address)
#elif defined(THINK_CPLUS)
	#define Declare_LoMem(type, name, address)	type &(name) = *(type *) (address)
#else
	#define	Declare_LoMem(type, name, address)	extern type name
//	#error LoMem requires either C++ or THINK C
#endif

#if WINVER
extern unsigned char ApplScratch[];
#else
#define	ApplScratch	0x0a78
#endif

Declare_LoMem(short, ApFontID, 0x984);
Declare_LoMem(Ptr, ApplLimit, 0x130);
Declare_LoMem(THz, ApplZone, 0x2AA);

#ifdef THINK_C
	Handle AppPacks[] : 0xAB8;
#endif
Declare_LoMem(Handle, AppParmHandle, 0xAEC);
Declare_LoMem(short, AtMenuBottom, 0xA0C);
Declare_LoMem(AuxCtlHandle, AuxCtlHead, 0xCD4);
Declare_LoMem(AuxWinHandle, AuxWinHead, 0xCD0);

Declare_LoMem(short, BootDrive, 0x210);
Declare_LoMem(Ptr, BufPtr, 0x10C);

Declare_LoMem(long, CaretTime, 0x2F4);
Declare_LoMem(char, CPUFlag, 0x12F);
Declare_LoMem(Rect, CrsrPin, 0x834);
Declare_LoMem(char, CrsrNew, 0x8CE);
Declare_LoMem(char, CrsrBusy, 0x8CD);
Declare_LoMem(WindowPtr, CurActivate, 0xA64);
Declare_LoMem(Str31, CurApName, 0x910);
Declare_LoMem(Handle, GZMoveHnd, 0x330);
Declare_LoMem(Handle, GZRootHnd, 0x328);


Declare_LoMem(short, CurApRefNum, 0x900);
Declare_LoMem(WindowPtr, CurDeactive, 0xA68);
Declare_LoMem(long, CurDirStore, 0x398);
Declare_LoMem(short, CurJTOffset, 0x934);
Declare_LoMem(short, CurMap, 0xA5A);
Declare_LoMem(short, CurPageOption, 0x936);
Declare_LoMem(Ptr, CurrentA5, 0x904);
Declare_LoMem(Ptr, CurStackBase, 0x908);

#ifdef THINK_C
	StringHandle DAStrings[4] : 0xAA0;
#endif
Declare_LoMem(long, DefltStack, 0x322);
Declare_LoMem(VCB *, DefVCBPtr, 0x352);
Declare_LoMem(ProcPtr, DeskHook, 0xA6C);
#ifdef THINK_C
	Declare_LoMem(Pattern, DeskPattern, 0xA3C);
#endif
Declare_LoMem(GDHandle, DeviceList, 0x8A8);
Declare_LoMem(long, DoubleTime, 0x2F0);
Declare_LoMem(ProcPtr, DragHook, 0x9F6);
#ifdef THINK_C
	Declare_LoMem(Pattern, DragPattern, 0xA34);
#endif
Declare_LoMem(QHdr, DrvQHdr, 0x308);
Declare_LoMem(short, DSErrCode, 0xAF0);

Declare_LoMem(ProcPtr, EjectNotify, 0x338);
Declare_LoMem(QHdr, EventQueue, 0x14A);
#ifdef THINK_C
	ProcPtr ExtStsDT[] : 0x2BE;
#endif

Declare_LoMem(Ptr, FCBSPtr, 0x34E);
#ifdef THINK_C
	Declare_LoMem(Str15, FinderName, 0x2E0);
#endif
Declare_LoMem(HFSDefaults *, FmtDefaults, 0x39E);
Declare_LoMem(char, FractEnable, 0xBF4);
Declare_LoMem(char, FScaleDisable, 0xA63);
Declare_LoMem(short, FSFCBLen, 0x3F6);
Declare_LoMem(QHdr, FSQHdr, 0x360);

Declare_LoMem(WindowPtr, GhostWindow, 0xA84);
Declare_LoMem(RgnHandle, GrayRgn, 0x9EE);

Declare_LoMem(Ptr, HeapEnd, 0x114);
Declare_LoMem(char, HiliteMode, 0x938);
Declare_LoMem(RGBColor, HiliteRGB, 0xDA0);
Declare_LoMem(short, HWCfgFlags, 0xB22);

Declare_LoMem(ProcPtr, IAZNotify, 0x33C);
Declare_LoMem(Ptr, IWM, 0x1E0);

Declare_LoMem(ProcPtr, JGNEFilter, 0x29A);

Declare_LoMem(short, KeyRepThresh, 0x190);
Declare_LoMem(short, KeyThresh, 0x18E);
Declare_LoMem(ProcPtr, Key1Trans, 0x29E);
Declare_LoMem(ProcPtr, Key2Trans, 0x2A2);

Declare_LoMem(FamRec **, LastFOND, 0xBC2);
Declare_LoMem(long, Lo3Bytes, 0x31A);
#ifdef THINK_C
	ProcPtr Lvl1DT[] : 0x192;
	ProcPtr Lvl2DT[] : 0x1B2;
#endif

Declare_LoMem(GDHandle, MainDevice, 0x8A4);
Declare_LoMem(short, MBarEnable, 0xA20);
Declare_LoMem(short, MBarHeight, 0xBAA);
#if WINVER
extern char MBState;
#endif
Declare_LoMem(ProcPtr, MBarHook, 0xA2C);
Declare_LoMem(short, MemErr, 0x220);
Declare_LoMem(Ptr, MemTop, 0x108);
Declare_LoMem(MCTableHandle, MenuCInfo, 0xD50);
Declare_LoMem(long, MenuDisable, 0xB54);
Declare_LoMem(short, MenuFlash, 0xA24);
Declare_LoMem(Handle, MenuList, 0xA1C);
Declare_LoMem(ProcPtr, MenuHook, 0xA30);
Declare_LoMem(long, MinStack, 0x31E);

Declare_LoMem(short, PaintWhite, 0x9DC);
Declare_LoMem(short, PrintErr, 0x944);

Declare_LoMem(char, QDExist, 0x8F3);

Declare_LoMem(Ptr, RAMBase, 0x2B2);
Declare_LoMem(short, ResErr, 0xA60);
Declare_LoMem(ProcPtr, ResErrProc, 0xAF2);
Declare_LoMem(Boolean, ResLoad, 0xA5E);
Declare_LoMem(ProcPtr, ResumeProc, 0xA8C);
Declare_LoMem(long, RndSeed, 0x156);
Declare_LoMem(Ptr, ROMBase, 0x2AE);
Declare_LoMem(short, RomMapInsert, 0xB9E);
Declare_LoMem(short, ROM85, 0x28E);

Declare_LoMem(short, SaveUpdate, 0x9DA);
Declare_LoMem(RgnHandle, SaveVisRgn, 0x9F2);
Declare_LoMem(Ptr, SCCRd, 0x1D8);
Declare_LoMem(Ptr, SCCWr, 0x1DC);
Declare_LoMem(ScrapStuff, ScrapInfo, 0x960);
#if WINVER
#define ScrapCount (ScrapInfo.scrapCount)
#endif
Declare_LoMem(char, ScrDmpEnb, 0x2F8);
Declare_LoMem(short, ScreenRow, 0x106);
Declare_LoMem(short, ScrHRes, 0x104);
Declare_LoMem(Ptr, ScrnBase, 0x824);
Declare_LoMem(short, ScrVRes, 0x102);
Declare_LoMem(char, SdVolume, 0x260);
Declare_LoMem(char, SEvtEnb, 0x15C);
Declare_LoMem(short, SFSaveDisk, 0x214);
Declare_LoMem(short, SysEvtMask, 0x144);
Declare_LoMem(short, SysMap, 0xA58);
Declare_LoMem(Handle, SysMapHndl, 0xA54);
Declare_LoMem(SysParmType, SysParam, 0x1F8);
#ifdef THINK_C
	unsigned char SysResName[20] : 0xAD8;
#endif
Declare_LoMem(short, SysVersion, 0x15A);
Declare_LoMem(THz, SysZone, 0x2A6);

Declare_LoMem(Handle, TEScrpHandle, 0xAB4);
Declare_LoMem(short, TEScrpLength, 0xAB0);
Declare_LoMem(GDHandle, TheGDevice, 0xCC8);
Declare_LoMem(short, TheMenu, 0xA26);
Declare_LoMem(THz, TheZone, 0x118);
Declare_LoMem(long, Ticks, 0x16A);
#if WINVER
#else
Declare_LoMem(volatile long, Time, 0x20C);
#endif
Declare_LoMem(volatile long, TimeLM, 0x20C);
Declare_LoMem(Handle, TopMapHndl, 0xA50);
Declare_LoMem(short, TopMenuItem, 0xA0A);

Declare_LoMem(short, UnitNtryCnt, 0x1D2);
Declare_LoMem(DCtlHandle *,UTableBase, 0x11C);

Declare_LoMem(QHdr, VBLQueue, 0x160);
Declare_LoMem(QHdr, VCBQHdr, 0x356);
Declare_LoMem(Ptr, VIA, 0x1D4);

Declare_LoMem(WidthTable **, WidthTabHandle, 0xB2A);
Declare_LoMem(WindowPeek, WindowList, 0x9D6);
Declare_LoMem(CGrafPtr, WMgrCPort, 0xD2C);
Declare_LoMem(GrafPtr, WMgrPort, 0x9DE);
Declare_LoMem(char, WWExist, 0x8F2);

#undef Declare_LoMem

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

#endif /* __LOMEM__ */

