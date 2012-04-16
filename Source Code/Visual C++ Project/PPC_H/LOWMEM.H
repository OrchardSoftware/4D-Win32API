/*  LowMem.H
 *	This is a Windows-only version for use with the Altura Mac2Win DLL
 *
 *	Copyright (c) 1994 Altura Software, Inc. All Rights Reserved.
 *	Created by:	DRR	05/04/94
 *	Modified by:
 */
#ifndef __LOWMEM__
#define __LOWMEM__

#ifndef __TYPES__
#include <MacTypes.H>
#endif

#ifndef __CONTROLS__
#include <Controls.H>
#endif

#ifndef __EVENTS__
#include <Events.H>
#endif

#ifndef __FILES__
#include <Files.H>
#endif

#ifndef __FONTS__
#include <Fonts.H>
#endif

#ifndef __MEMORY__
#include <MemoryM.H>
#endif

#ifndef __WINDOWS__
#include <WindowM.H>
#endif

	// Added by Altura to get all the types defined
#ifndef __DEVICES__
#include <Devices.H>
#endif

#ifndef __DISKINIT__
#include <DiskInit.H>
#endif

#ifndef __SCRAP__
#include <Scrap.H>
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

	// NOTE: The externals below may not all be defined is ASIGlobl.C.
	//	Any variables that are not defined there are not currently
	//	supported by the DLL. If you use an supported global, examine
	//	the use to determine if there is a work-around (e.g. function
	//	calls instead of global references). If you feel the global
	//	needs to be support by the Altura DLL, contact us.

	// lve now that we have accessors in the DLL use them !!
#if 0
extern short		ApFontID;
extern Ptr		ApplLimit;
extern THz		ApplZone;

extern Handle		AppParmHandle;
extern short		AtMenuBottom;
extern AuxCtlHandle	AuxCtlHead;
extern AuxWinHandle	AuxWinHead;

extern short		BootDrive;
extern Ptr		BufPtr;

extern long		CaretTime;
extern char		CPUFlag;
extern Rect		CrsrPin;
extern char		CrsrNew;
extern WindowPtr	CurActivate;
#ifdef LONGFILENAMES
extern Str255		CurApName;
#else
extern Str31		CurApName;
#endif

extern short		CurApRefNum;
extern WindowPtr	CurDeactive;
extern long		CurDirStore;
extern short		CurJTOffset;
extern short		CurMap;
extern short		CurPageOption;
extern Ptr		CurrentA5;
extern Ptr		CurStackBase;

extern long		DefltStack;
extern VCB *		DefVCBPtr;
extern ProcPtr		DeskHook;
extern GDHandle		DeviceList;
extern long		DoubleTime;
extern ProcPtr		DragHook;
extern QHdr		DrvQHdr;
extern short		DSErrCode;

extern ProcPtr		EjectNotify;
extern QHdrPtr EventQueue;

extern Ptr		FCBSPtr;
extern HFSDefaults *	FmtDefaults;
extern char		FractEnable;
extern char		FScaleDisable;
extern short		FSFCBLen;
extern QHdr		FSQHdr;

extern WindowPtr	GhostWindow;
extern RgnHandle	GrayRgn;

extern Ptr		HeapEnd;
extern char		HiliteMode;
extern RGBColor		HiliteRGB;
extern short		HWCfgFlags;

extern ProcPtr		IAZNotify;
extern Ptr		IWM;

extern ProcPtr		JGNEFilter;

extern short		KeyRepThresh;
extern short		KeyThresh;
extern ProcPtr		Key1Trans;
extern ProcPtr		Key2Trans;

extern FamRec **	LastFOND;
extern long		Lo3Bytes;

extern GDHandle		MainDevice;
extern short		MBarEnable;
extern short		MBarHeight;
extern char		MBState;
extern ProcPtr		MBarHook;
extern short		MemErr;
extern Ptr		MemTop;
extern MCTableHandle	MenuCInfo;
extern long		MenuDisable;
extern short		MenuFlash;
extern Handle		MenuList;
extern ProcPtr		MenuHook;
extern long		MinStack;

extern short		PaintWhite;
extern short		PrintErr;

extern char		QDExist;

extern Ptr		RAMBase;
extern short		ResErr;
extern ProcPtr		ResErrProc;
extern Boolean		ResLoad;
extern ProcPtr		ResumeProc;
extern long		RndSeed;
extern Ptr		ROMBase;
extern short		RomMapInsert;
extern short		ROM85;

extern short		SaveUpdate;
extern RgnHandle	SaveVisRgn;
extern Ptr		SCCRd;
extern Ptr		SCCWr;
extern ScrapStuff	ScrapInfo;
#if WINVER
#define ScrapCount	(ScrapInfo.scrapCount)
#endif
extern char		ScrDmpEnb;
extern short		ScreenRow;
extern short		ScrHRes;
extern Ptr		ScrnBase;
extern short		ScrVRes;
extern char		SdVolume;
extern char		SEvtEnb;
extern short		SFSaveDisk;
extern short		SysEvtMask;
extern short		SysMap;
extern Handle		SysMapHndl;
extern SysParmType	SysParam;
extern short		SysVersion;
extern THz		SysZone;

extern Handle		TEScrpHandle;
extern short		TEScrpLength;
extern GDHandle		TheGDevice;
extern short		TheMenu;
extern THz		TheZone;
extern long		Ticks;
extern volatile long	TimeLM;
extern Handle	    	TopMapHndl;
extern short		TopMenuItem;

extern short		UnitNtryCnt;
extern DCtlHandle *	UTableBase;

extern QHdr		VBLQueue;	 
extern QHdr		VCBQHdr;
extern Ptr		VIA;

extern WidthTable **	WidthTabHandle;
extern WindowPeek	WindowList;
extern CGrafPtr		WMgrCPort;
extern GrafPtr		WMgrPort;
extern char		WWExist;

#endif

/* TNG - Needed to place this extrn in here for a call to  ASI_GetSysColor() from the LMGetHiliteRGB macro
         call below */
#if WINVER               
extern F_ASIBND( void ) ASI_GetSysColor( int DisplayElement, RGBColor *MacColor );
#endif

#if 1

#define DefineLMFuncs(lowmemtype, lowmemname) \
	extern F_PASCAL(lowmemtype) LMGet##lowmemname();\
	extern F_PASCAL(void) LMSet##lowmemname(lowmemtype x);

DefineLMFuncs(short, CurMap)

DefineLMFuncs(short, CurApRefNum)

DefineLMFuncs(long, MinusOne)

DefineLMFuncs(short, SFSaveDisk)

DefineLMFuncs(long, CurDirStore)

DefineLMFuncs(GDHandle, MainDevice)

DefineLMFuncs(short, SFVRefNum)

DefineLMFuncs(GDHandle, DeviceList)

DefineLMFuncs(GrafPtr, WMgrPort)

DefineLMFuncs(char, HiliteMode)

DefineLMFuncs(GDHandle, TheGDevice)

// DefineLMFuncs(RGBColor, HiliteRGB)
extern F_PASCAL(void) LMGetHiliteRGB(RGBColor *hiliteRGBValue);

DefineLMFuncs(short, KeyRepThresh)

DefineLMFuncs(short, KeyThresh)

DefineLMFuncs(long, Ticks)

DefineLMFuncs(long, CaretTime)

DefineLMFuncs(char, MBState)

DefineLMFuncs(Point, MTemp)

DefineLMFuncs(Point, RawMouse)

DefineLMFuncs(Point, Mouse)

DefineLMFuncs(short, SysEvtMask)

DefineLMFuncs(long, DoubleTime)

DefineLMFuncs(short, MBarHeight)

DefineLMFuncs(short, TheMenu)

DefineLMFuncs(Handle, MenuList)

DefineLMFuncs(short, MenuFlash)

DefineLMFuncs(long, MenuDisable)

DefineLMFuncs(short, MemErr)

DefineLMFuncs(Ptr, ApplLimit)

DefineLMFuncs(THz, ApplZone)

DefineLMFuncs(Ptr, CurrentA5)

DefineLMFuncs(Ptr, CurStackBase)

DefineLMFuncs(THz, TheZone)

DefineLMFuncs(RgnHandle, GrayRgn)

DefineLMFuncs(WindowPtr, WindowList)

DefineLMFuncs(WindowPtr, CurActivate)

DefineLMFuncs(WindowPtr, CurDeactive)

DefineLMFuncs(short, PaintWhite)

DefineLMFuncs(short, ResErr)

DefineLMFuncs(Boolean, ResLoad)

DefineLMFuncs(ScrapStuff, ScrapInfo)

DefineLMFuncs(short, TEScrpLength)

DefineLMFuncs(Handle, TEScrpHandle)

DefineLMFuncs(short, TEScrpHasStyle)

DefineLMFuncs(Handle, TEScrpStyleHandle)

DefineLMFuncs(short, PrintErr)

DefineLMFuncs(char, FractEnable)

DefineLMFuncs(WidthTable**, WidthTabHandle)

#define LMGetCrsrBusy() 		0
#define LMSetCrsrBusy( x )		(x)

/*
#define		LMGetMenuHook()			ASI_GetMenuHook()
void		LMSetMenuHook( x )		ASI_SetMenuHook( (MenuHookProcPtr)(x) )
*/
DefineLMFuncs(short, TopMenuItem)

DefineLMFuncs(short, AtMenuBottom)
//GLOBAL short	*lpAtMenuBottom;
//short		LMGetAtMenuBottom()					{return *lpAtMenuBottom;}
//void		LMSetAtMenuBottom(short x)			{*lpAtMenuBottom = x;}

DefineLMFuncs(short, ApFontID)

//DefineLMFuncs(QHdrPtr, EventQueue)
// we need to do this one explicitly because the set and get are different
// levels of indirection
extern F_PASCAL( QHdrPtr )LMGetEventQueue(void);
extern F_PASCAL( void ) LMSetEventQueue(struct QHdr x);

DefineLMFuncs(ProcPtr, ResumeProc)

DefineLMFuncs(short, SysFontFam)

DefineLMFuncs(short, SysFontSize)

DefineLMFuncs(short, SysMap)

DefineLMFuncs(long, Time)

DefineLMFuncs(Fixed, LastSPExtra)

DefineLMFuncs(WindowPtr, GhostWindow)

DefineLMFuncs(Handle, GZMoveHnd) 

DefineLMFuncs(Handle, GZRootHnd)
//DefineLMFuncs(Handle, TopMapHndl)

DefineLMFuncs(short, UnitNtryCnt)

extern F_PASCAL(StringPtr) LMGetCurApName();
extern F_PASCAL(void) LMSetCurApName(StringPtr x );

DefineLMFuncs(short, ROMMapInsert)

DefineLMFuncs(short, FSFCBLen)

DefineLMFuncs(short, HWCfgFlags)

DefineLMFuncs(DCtlHandle *, UTableBase)

#else
	// Under Windows using the Altura DLL, low memory globals are simply
	//	global variables declared in ASIGlobl.C

#define LMGetMinusOne()			MinusOne
#define LMSetMinusOne( x )		(MinusOne = x)

#define LMGetSFSaveDisk()		SFSaveDisk
#define LMSetSFSaveDisk( x )		(SFSaveDisk = x)

#define LMGetCurDirStore()		CurDirStore
#define LMSetCurDirStore( x )		(CurDirStore = x)

#define LMGetSFVRefNum()		SFVRefNum
#define LMSetSFVRefNum( x )		(SFVRefNum = x)

#define LMGetMainDevice()		MainDevice
#define LMSetMainDevice( x )		(MainDevice = x)

#define LMGetDeviceList()		DeviceList
#define LMSetDeviceList( x )		(DeviceList = x)

#define LMGetWMgrPort()			WMgrPort
#define LMSetWMgrPort( x )		(WMgrPort = x)

#define LMGetHiliteMode()		HiliteMode
#define LMSetHiliteMode( x )		(HiliteMode = x)

#define LMGetTheGDevice()		TheGDevice
#define LMSetTheGDevice( x )		(TheGDevice = x)


#if WINVER /* TNG - Put this code in so that the windows side of this call works - 10/04/95 */
  #define LMGetHiliteRGB( x )		ASI_GetSysColor( 13, (RGBColorPtr) x )  /* 13 represents the COLOR_HIGHLIGHT for GetSysColor() Win API call  */
#else		  
  #define LMGetHiliteRGB()		HiliteRGB
#endif

#define LMSetHiliteRGB( x )		(HiliteRGB = x)

#define LMGetResumeProc()		ResumeProc
#define LMSetResumeProc( x )		(ResumeProc = x)

#define LMGetEventQueue()		&EventQueue
#define LMSetEventQueue( x )		(EventQueue = x)

#define LMGetKeyThresh()		KeyThresh
#define LMSetKeyThresh( x )		(KeyThresh = x)

#define LMGetTicks()			Ticks
#define LMSetTicks( x )			(Ticks = x)

#define LMGetCaretTime()		CaretTime
#define LMSetCaretTime( x )		(CaretTime = x)

#define LMGetMBState()			MBState
#define LMSetMBState( x )		(MBState = x)

#define LMGetMTemp()			MTemp
#define LMSetMTemp( x )			(MTemp = x)

#define LMGetRawMouse()			RawMouse
#define LMSetRawMouse( x )		(RawMouse = x)

#define LMGetMouse()			Mouse
#define LMSetMouse( x )			(Mouse = x)

#define LMGetSysEvtMask()		SysEvtMask
#define LMSetSysEvtMask( x )		(SysEvtMask = x)

#define LMGetDoubleTime()		DoubleTime
#define LMSetDoubleTime( x )		(DoubleTime = x)

#define LMGetMBarHeight()		MBarHeight
#define LMSetMBarHeight( x )		(MBarHeight = x)

#define LMGetTheMenu()			TheMenu
#define LMSetTheMenu( x )		(TheMenu = x)

#define LMGetMenuList()			MenuList
#define LMSetMenuList( x )		(MenuList = x)

#define LMGetMenuFlash()		MenuFlash
#define LMSetMenuFlash( x )		(MenuFlash = x)

#define LMGetMenuDisable()		MenuDisable
#define LMSetMenuDisable( x )		(MenuDisable = x)

#define LMGetMemErr()			MemErr
#define LMSetMemErr( x )		(MemErr = x)

#define LMGetApplLimit()		ApplLimit
#define LMSetApplLimit( x )		(ApplLimit = x)

#define LMGetApplZone()			ApplZone
#define LMSetApplZone( x )		(ApplZone = x)

#define LMGetCurrentA5()		CurrentA5
#define LMSetCurrentA5( x )		(CurrentA5 = x)

#define LMGetCurStackBase()		CurStackBase
#define LMSetCurStackBase( x )		(CurStackBase = x)

#define LMGetTheZone()			TheZone
#define LMSetTheZone( x )		(TheZone = x)

#define LMGetGrayRgn()			GetGrayRgn()
#define LMSetGrayRgn( x )		(GrayRgn = x)

#define LMGetWindowList()		((WindowPtr)WindowList)
#define LMSetWindowList( x )		(WindowList = ((WindowPeek)(x)))

#define LMGetCurActivate()		CurActivate
#define LMSetCurActivate( x )		(CurActivate = x)

#define LMGetCurDeactive()		CurDeactive
#define LMSetCurDeactive( x )		(CurDeactive = x)

#define LMGetPaintWhite()		PaintWhite
#define LMSetPaintWhite( x )		(PaintWhite = x)

#define LMGetCrsrBusy() 		0
#define LMSetCrsrBusy( x )		(x)

#define LMGetCrsrNew()			CrsrNew
#define LMSetCrsrNew( x )		(CrsrNew = x)

#define LMGetResErr()			ResErr
#define LMSetResErr( x )		(ResErr = x)

#if 0
#define LMGetResLoad()			ResLoad
#define LMSetResLoad( x )		(ResLoad = x)
#else
Boolean LMGetResLoad(void);
void	LMSetResLoad(Boolean x );
#endif

#define LMGetScrapInfo()		ScrapInfo
#define LMSetScrapInfo( x )		(ScrapInfo = x)

#define LMGetTEScrpLength()		TEScrpLength
#define LMSetTEScrpLength( x )		(TEScrpLength = x)

#define LMGetTEScrpHandle()		TEScrpHandle
#define LMSetTEScrpHandle( x )		(TEScrpHandle = x)

#define LMGetTEScrpHasStyle()		TEScrpHasStyle
#define LMSetTEScrpHasStyle( x )	(TEScrpHasStyle = x)

#define LMGetTEScrpStyleHandle()	TEScrpStyleHandle
#define LMSetTEScrpStyleHandle( x )	(TEScrpStyleHandle = x)

#define LMGetPrintErr()			PrintErr
#define LMSetPrintErr( x )		(PrintErr = x)

#define LMGetFractEnable()		FractEnable
#define LMSetFractEnable( x )		(FractEnable = x)

#define LMGetWidthTabHandle()		WidthTabHandle
#define LMSetWidthTabHandle( x )	(WidthTabHandle = x)

#define LMGetMenuHook()			ASI_GetMenuHook()
#define LMSetMenuHook( x )		ASI_SetMenuHook( (MenuHookProcPtr)(x) )

//RHK 04/15/95 for HomePublish

#define LMGetTime()			TimeLM
#define LMSetTime( x )			(TimeLM = x)

// #define LMGetTopMapHndl()		TopMapHndl
// #define LMSetTopMapHndl( x )		(TopMapHndl = x)

//RHK 05/06/95 for MacApp3.0 clean link

#define LMGetCurApName()		CurApName
#define LMSetCurApName( x )		ASI_CopyPtoP( x, CurApName )

#define LMGetCurApRefNum()		CurApRefNum
#define LMSetCurApRefNum( x )		(CurApRefNum = x)

#define LMGetCurJTOffset()		CurJTOffset
#define LMSetCurJTOffset( x )		(CurJTOffset = x)

#define LMGetROMMapInsert()		RomMapInsert
#define LMSetROMMapInsert( x )		(RomMapInsert = x)

#define LMGetFSFCBLen()			FSFCBLen
#define LMSetFSFCBLen( x )		(FSFCBLen = x)

#define LMGetHWCfgFlags()			HWCfgFlags
#define LMSetHWCfgFlags( x )		(HWCfgFlags = x)

#define LMGetGZMoveHnd()			GZMoveHnd
#define LMSetGZMoveHnd( x )		(GZMoveHnd = x)

#define LMGetGhostWindow()		GhostWindow
#define LMSetGhostWindow( x )	(GhostWindow = x)

#define LMGetGZRootHnd()		GZRootHnd
#define LMSetGZRootHnd( x )		(GZRootHnd = x)

#define LMGetUnitNtryCnt()		UnitNtryCnt
#define LMSetUnitNtryCnt( x )		(UnitNtryCnt = x)

#define LMGetKeyRepThresh()		KeyRepThresh
#define LMSetKeyRepThresh( x )		(KeyRepThresh = x)

//dw: the following four defines are now supported
#define LMGetTopMenuItem()		TopMenuItem
#define LMSetTopMenuItem( x )		(TopMenuItem = x)

#define LMGetAtMenuBottom()		AtMenuBottom
#define LMSetAtMenuBottom( x )		(AtMenuBottom = x)

#define LMGetCurMap()			CurMap
#define LMSetCurMap( x )		(CurMap = x)

#endif	//!!CAM!!

#if 0		// Globals not yet supported by the Altura Mac2Win DLL

#define LMGetAuxWinHead()		AuxWinHead
#define LMSetAuxWinHead( x )		(AuxWinHead = x)

#define LMGetAuxCtlHead()		AuxCtlHead
#define LMSetAuxCtlHead( x )		(AuxCtlHead = x)

#define LMGetMenuCInfo()		MenuCInfo
#define LMSetMenuCInfo( x )		(MenuCInfo = x)

#define LMGetUTableBase()		UTableBase
#define LMSetUTableBase( x )		(UTableBase = x)

#define LMGetLvl2DT( i )		Lvl2DT[i]
#define LMSetLvl2DT( x, i )		(Lvl2DT[i] = x)

#define LMGetVIA()			VIA
#define LMSetVIA( x )			(VIA = x)

#define LMGetSCCRd()			SCCRd
#define LMSetSCCRd( x )			(SCCRd = x)

#define LMGetSCCWr()			SCCWr
#define LMSetSCCWr( x )			(SCCWr = x)

#define LMGetIWM()			IWM
#define LMSetIWM( x )			(IWM = x)

#define LMGetExtStsDT( i )		ExtStsDT[i]
#define LMSetExtStsDT( x, i )		(ExtStsDT[i] = x)

#define LMGetSEvtEnb()			SEvtEnb
#define LMSetSEvtEnb( x )		(SEvtEnb = x)

#define LMGetKeyRepThresh()		KeyRepThresh
#define LMSetKeyRepThresh( x )		(KeyRepThresh = x)

#define LMGetJGNEFilter()		JGNEFilter
#define LMSetJGNEFilter( x )		(JGNEFilter = x)

#define LMGetKey1Trans()		Key1Trans
#define LMSetKey1Trans( x )		(Key1Trans = x)

#define LMGetKey2Trans()		Key2Trans
#define LMSetKey2Trans( x )		(Key2Trans = x)

#define LMGetScrDmpEnb()		ScrDmpEnb
#define LMSetScrDmpEnb( x )		(ScrDmpEnb = x)

#define LMGetBootDrive()		BootDrive
#define LMSetBootDrive( x )		(BootDrive = x)

#define LMGetDrvQHdr()			DrvQHdr
#define LMSetDrvQHdr( x )		(DrvQHdr = x)

#define LMGetEjectNotify()		EjectNotify
#define LMSetEjectNotify( x )		(EjectNotify = x)

#define LMGetFCBSPtr()			FCBSPtr
#define LMSetFCBSPtr( x )		(FCBSPtr = x)

#define LMGetDefVCBPtr()		DefVCBPtr
#define LMSetDefVCBPtr( x )		(DefVCBPtr = x)

#define LMGetVCBQHdr()			VCBQHdr
#define LMSetVCBQHdr( x )		(VCBQHdr = x)

#define LMGetFSQHdr()			FSQHdr
#define LMSetFSQHdr( x )		(FSQHdr = x)

#define LMGetApFontID()			ApFontID
#define LMSetApFontID( x )		(ApFontID = x)

#define LMGetFScaleDisable()		FScaleDisable
#define LMSetFScaleDisable( x )		(FScaleDisable = x)

#define LMGetLastFOND()			LastFOND
#define LMSetLastFOND( x )		(LastFOND = x)

#define LMGetFSFCBLen()			FSFCBLen
#define LMSetFSFCBLen( x )		(FSFCBLen = x)

#define LMGetFmtDefaults()		FmtDefaults
#define LMSetFmtDefaults( x )		(FmtDefaults = x)

#define LMGetROM85()			ROM85
#define LMSetROM85( x )			(ROM85 = x)

#define LMGetDSErrCode()		DSErrCode
#define LMSetDSErrCode( x )		(DSErrCode = x)

#define LMGetMemTop()			MemTop
#define LMSetMemTop( x )		(MemTop = x)

#define LMGetBufPtr()			BufPtr
#define LMSetBufPtr( x )		(BufPtr = x)

#define LMGetHeapEnd()			HeapEnd
#define LMSetHeapEnd( x )		(HeapEnd = x)

#define LMGetSysZone()			SysZone
#define LMSetSysZone( x )		(SysZone = x)

#define LMGetROMBase()			ROMBase
#define LMSetROMBase( x )		(ROMBase = x)

#define LMGetRAMBase()			RAMBase
#define LMSetRAMBase( x )		(RAMBase = x)

#define LMGetLo3Bytes()			Lo3Bytes
#define LMSetLo3Bytes( x )		(Lo3Bytes = x)

#define LMGetIAZNotify()		IAZNotify
#define LMSetIAZNotify( x )		(IAZNotify = x)

#define LMGetMBarEnable()		MBarEnable
#define LMSetMBarEnable( x )		(MBarEnable = x)

#define LMGetMBarHook()			MBarHook
#define LMSetMBarHook( x )		(MBarHook = x)

#define LMGetMenuHook()			MenuHook
#define LMSetMenuHook( x )		(MenuHook = x)

#define LMGetSysVersion()		SysVersion
#define LMSetSysVersion( x )		(SysVersion = x)

#define LMGetSysParam()			SysParam
#define LMSetSysParam( x )		(SysParam = x)

#define LMGetSysTime()			SysTime
#define LMSetSysTime( x )		(SysTime = x)

#define LMGetScrVRes()			ScrVRes
#define LMSetScrVRes( x )		(ScrVRes = x)

#define LMGetScrHRes()			ScrHRes
#define LMSetScrHRes( x )		(ScrHRes = x)

#define LMGetScreenRow()		ScreenRow
#define LMSetScreenRow( x )		(ScreenRow = x)

#define LMGetScrnBase()			ScrnBase
#define LMSetScrnBase( x )		(ScrnBase = x)

#define LMGetCrsrPin()			CrsrPin
#define LMSetCrsrPin( x )		(CrsrPin = x)

#define LMGetRndSeed()			RndSeed
#define LMSetRndSeed( x )		(RndSeed = x)

#define LMGetRomMapInsert()		RomMapInsert
#define LMSetRomMapInsert( x )		(RomMapInsert = x)

//#define LMGetTopMapHndl()		TopMapHndl
//#define LMSetTopMapHndl( x )		(TopMapHndl = x)

#define LMGetSysMapHndl()		SysMapHndl
#define LMSetSysMapHndl( x )		(SysMapHndl = x)

#define LMGetSysMap()			SysMap
#define LMSetSysMap( x )		(SysMap = x)

#define LMGetResErrProc()		ResErrProc
#define LMSetResErrProc( x )		(ResErrProc = x)

#define LMGetSysResName()		SysResName
#define LMSetSysResName( x )		ASI_CopyPtoP( x, SysResName )

#define LMGetFinderName()		FinderName
#define LMSetFinderName( x )		ASI_CopyPtoP( x, FinderName )

#define LMGetCurJTOffset()		CurJTOffset
#define LMSetCurJTOffset( x )		(CurJTOffset = x)

#define LMGetCurPageOption()		CurPageOption
#define LMSetCurPageOption( x )		(CurPageOption = x)

#define LMGetAppParmHandle()		AppParmHandle
#define LMSetAppParmHandle( x )		(AppParmHandle = x)

#define LMGetSdVolume()			SdVolume
#define LMSetSdVolume( x )		(SdVolume = x)

#define LMGetVBLQueue()			VBLQueue
#define LMSetVBLQueue( x )		(VBLQueue = x)

#define LMGetWMgrCPort()		WMgrCPort
#define LMSetWMgrCPort( x )		(WMgrCPort = x)

#define LMGetSaveUpdate()		SaveUpdate
#define LMSetSaveUpdate( x )		(SaveUpdate = x)

#define LMGetDragHook()			DragHook
#define LMSetDragHook( x )		(DragHook = x)

#define LMGetDragPattern()		DragPattern
#define LMSetDragPattern( x )		(DragPattern = x)

#define LMGetDeskPattern()		DeskPattern
#define LMSetDeskPattern( x )		(DeskPattern = x)

#define LMGetDeskHook()			DeskHook
#define LMSetDeskHook( x )		(DeskHook = x)

#endif	// #if 0 -- Globals not yet supported by the Altura Mac2Win DLL

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

#endif	// #ifndef __LOWMEM__

