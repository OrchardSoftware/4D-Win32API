/*
*       ASIEXTRN.H
*       Copyright (c) 1991-7 Altura Software, Inc. All Rights Reserved.
*
*	mod 02/19/98 DDK - added WCF_LAYERASPALETTE
*	mod 02/06/98 DDK - added ASI_SF_SetFilterString and kASI_SF_UseCurrentFilter
*	mod 01/23/98 DDK - added ASI_SetMaximumPrinterResolution
*	mod 01/05/98 DLB - added ASISV_gl_SendRightButtonMessagesToParent flag to
*			let controls send their right clicks to their parent windows.
*	mod	11/28/97 RHK - added ASISV_gl_CountDirFiles
*	mod	11/19/97 DLB - added ASISV_gl_ListsAlwaysDrawItems
*	mod	11/19/97 DLB - added ASISV_gl_ListsUseDialogBkg
*	mod 10/29/97 DLB - added ASISV_gl_SupportMacStyleShowCursor
*	mod 10/29/97 DLB - added ASISV_gl_ProcessVBLQueueOnMouseDownOnly
*	mod 10/24/97 DDK - added ASISV_gl_SupportUpdateGWorld
*	mod 08/16/97 jg  - expose ASI_SizeOf
*	mod 08/11/97 jg  - expose ASI_SetPEFlags
*	mod 08/10/97 jg  - added ASISV_gl_MergeStaticColors
*	mod 08/10/97 jg  - added ASISV_gl_InitWithResourceCTable
*	mod 07/24/97 RHK - expose ASI_SetDlgInitialTextSize
*	mod 07/23/97 RHK - ASISV_gl_GetNextEvent_sleep_ticks for changing sleep_ticks
*	mod 07/17/97 RHK - ASISV_gl_WinKanjiKeyCode for special Kanji keyboard keys
*	mod 06/13/97 jg  - ASISV_gl_UseDitheredBrush: allow dithering of solid brush 
*	mod 06/12/97 RHK - added isEndOfLine for EOLHook Support
*	mod 06/05/97 jg  - add support for ASISV_gl_UseWindowsHiliteStyle
*	mod 06/04/97 RHK - added ASISV_gl_NextDialogUseFakeBtnControl
*	mod 06/02/97 jg  - added ASISV_gl_UseMac8CTab
*   mod 05/22/97 DSD - Added ASI_MacVirtualKeyTranslate for GraphSoft, bug #1285.
*	mod 04/09/97 jg  - added ASISV_gl_UseFullPalette
*	mod 04/07/97 jg  - added ASISV_gl_UseFullCopyBits
*	mod 8 1 96 tsh - restore ASISV_gl_CheckPrintDlgErrors to #61 as LJL
*			agreed in 1995
*	mod 03/28/96 RSE - added yet more ASISV flags
*	mod 02/23/96 RHK - expose ASI_IME calls
*	mod 11 22 95 tsh - fix warning from RSE 10/17/95 mod for WIN16
*	mod 10/17/95 RSE - fix more HANDLE definition problems
*	mod 09/20/95 RSE - move ASI_Copy... to asiport.h to ease include probs.
*	mod 07 26 95 lve - add ControlToHwnd and two new ASISV_gl values 
*	mod 07 05 95 shm - add ASI_SeparateFile 
*	mod 06 07 95 shm - add ASI_WinTabMessage
*	mod 06 07 95 shm - add WM_ASI_WTBASE for base of 16 WinTab messages
*			(Windows)
*	mod 03 20 95 shm - add ASI_MoveWindow
*	mod 03 17 95 shm - add ASI_GetItemEnabled
*       mod 02/16/95 RSE - add ACTIVECLIKLOOP flags for DeltaPoint
*	mod 02/02/95 csk - ASI_GetCurrentDC() ASI_PolyPolygon()
*	mod 12/29/94 BP  - Added M2W.m2w_lpAppSetupDlg 
*	mod 10/31/94 csk - expose menu mapping code
*	mod 10/06/94 dlb - prevent multi def warning of BOOLEAN
*	mod 10/14/94 DRR - added new system values ASISV_gl_WindowProcID
*			and ASISV_gl_WindowMenuHandle
*	mod 10/10/94 DRR - added new system value ASISV_gl_WindowCreateFlags
*			to replace other ways of modifying the behavior of
*			myNewWindow(); added WCF_* #defines to go with this
*	mod 09/13/94 DRR - added a new system value ASISV_gl_NoCloseCommand
*	mod 09/27/94 mes - add WING support 
*	mod 07/20/94 llb - Obsolete MapUpperTEOutput,
*			rename MapUpperInput to MapUpperChars.
*	mod 07/08/94 RSE - added LSETSEL flags for no kbd input, no selections
*	mod 07/06/94 RSE - changed name and effect of DlgKBHookProcPtr's last
*			parameter
*	mod 06/27/94 rb  - add UNFOLD_ReleaseHBM flag
*	mod 06/27/94 LLB - add ASISV_gl_ExternalMouseEventsInModalDialog.
*	mod 06/25/94 dac - add ASISV_gl_ModalDialogActive (read-only)
*	mod 05/19/94 llb - Added FOLD_4BitAs4Bit
*	mod 05/10/94 rb  - Added palette to ASI_NewDDBPixmap
*	mod 05/02/94 rb  - Added ASISV_gl_DoesPartialGetSetDIBits
*			and ASISV_gl_CurrentHPALETTE
*	mod 04/08/94 DRR - Added ASISV_gl_OnePixelPatternLines
*	mod 04/08/94 RSE - move HMENU stuff so it will compile
*	mod 03/15/94 RHK  - Add SysFontFam & Time
*	mod 03/15/94 llb - Added ASISV_gl_NoSetSysColors
*	mod 03/10/94 rb  - Add ASI_MemCheck()
*	mod 02/10/94 ms  - make ASI_AllowPictStatePush return previous state
*	mod 02/08/94 rb  - Added M2W.m2w_glDibDrvName
*			Added ASI_SystemValue ASISV_gl_MemSubMax
*	mod 02/07/94 llb - added ASI_ForcePalette
*	mod 02/01/94 rb  - ASI_NewDDBPixMap
*	mod 01/26/94 RSE - Add ASI_SetBigIText()
*	mod 01/23/94 jg  - change binding of ASI_DSSimple
*	mod 01/17/94 jg  - add UNFOLD_UsePalColors and UNFOLD_InvertPalColors
*	mod 01/06/94 jg  - add UNFOLD_NoBaseAddrReAlloc
*	mod 12/23/93 jg  - ASI_DSSimple, ASI_DSNotSimple, and
*			ASI_CopyBitsMacToDIB
*	mod 11/18/93 jg  - ASI_NewDIBPixMap
*	mod 10/28/93 jg  - ASI_GetDIBFromPixMap
*	mod 10/21/93 jg  - ASI_BeginPaletteAnimation / ASI_EndPaletteAnimation
*	mod 8 7 94 tsh - add ASI_DialogFontMode
*	mod 9 1 93 tsh - add ASI_QDInitTestScreen
*	mod 08 18 83 shm - add ASI_BandInfo
*	mod 08/20/93 LJL - add class name and resource name strings to
*			M2W struct, recompile ASIWINIT.C
*	mod 08/13/93 LJL - add freeHBM parameter to ASI_UnfoldPixMap
*	mod 07 09 93 shm - fix if _INC_WINDOWS already defined but _WINDOWS_
*		  is not
*   	mod 07 08 93 shm - make it compile in Borland C
*       mod 03/10/93  jg - #2349: use printer text metrics
*       mod  1/17/93  jg - #487: allow use of rectangle for line drawing
*       mod 12 19 92 tsh - add ASI_SetComboBoxSelect
*       mod 12 17 92 tsh - fix for Borland C
*       mod 12/10/92 DRR - add #defines for ASI_WinIniChange() return codes
*/

#ifndef __ASIEXTRN__
#define	__ASIEXTRN__
#ifndef	_ASIPORT_
#include "asiport.h"
#endif

#ifdef WINVER
#define WM_ASI_WTBASE 0x7ff0
#endif

#if !defined(BOOLEAN)
typedef unsigned int	BOOLEANUI;
#define BOOLEAN BOOLEANUI
#endif

//typedef void VOID;

#ifndef VOID
#define VOID void
#endif

#ifdef	WinPixMapPtr
#define	wPixMapPtr	WinPixMapPtr
#else
#define	wPixMapPtr	void *
#endif

#ifdef	WinRgnHandle
#define	wRgnHandle	WinRgnHandle
#else
#define	wRgnHandle	void **
#endif

#if defined(_WINDOWS_) || defined(__WINDOWS_H)
#define _INC_WINDOWS
#endif

#if !defined(_INC_WINDOWS)
#ifdef STRICT
typedef const void NEAR*        HANDLE;
#define DECLARE_HANDLE(name)    struct name##__ { int unused; }; \
                                typedef const struct name##__ NEAR* name
#define DECLARE_HANDLE32(name)  struct name##__ { int unused; }; \
                                typedef const struct name##__ FAR* name
#else   /* STRICT */
#ifdef WIN32
typedef void *			HANDLE;
#else
typedef UINT			HANDLE;
#endif
#define DECLARE_HANDLE(name)    typedef HANDLE name
#define DECLARE_HANDLE32(name)  typedef DWORD name
#endif  /* !STRICT */

DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HPALETTE);
DECLARE_HANDLE(HGLOBAL);
DECLARE_HANDLE(HCURSOR);

#define LPBITMAPINFO	void *
#endif

#ifndef __TYPES__
#include <MacTypes.H>
#endif

#ifndef __MEMORY__
#include <MemoryM.H>
#endif

#ifndef __QUICKDRAW__
#if LONG_FILENAMES
#include <Quickdraw.H>
#else
#include <Quickdra.H>
#endif
#endif

#ifndef __PALETTES__
#include <Palettes.H>
#endif

#ifndef __TEXTEDIT__
#include <TextEdit.H>
#endif

#ifndef __WINDOWS__
#include <WindowM.H>
#endif

#ifndef __DIALOGS__
#include <Dialogs.H>
#endif

#ifndef __STANDARDFILE__
#if LONG_FILENAMES
#include <StandardFile.h>
#else
#include <Standard.h>
#endif
#endif

#ifndef __LISTS__
#include <Lists.H>
#endif

#ifndef __MENUS__
#include <Menus.H>
#endif

#ifndef __SCRAP__
#include <Scrap.H>
#endif

#if PPC_HEADERS
  #ifndef __PRINTING__
    #include <Printing.H>
  #endif
#else
  #ifndef __PRINTTRAPS__
    #if LONG_FILENAMES
      #include <PrintTraps.H>
    #else
      #include <PrintTra.H>
    #endif
  #endif
#endif

#ifndef __FONTS__
#include <Fonts.H>
#endif

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
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

					/* structure for passing Mac2Win*/
					/*   session information in     */
					/*   ASI_InitSession()          */
#define M2W_VERSION 0x0102

#if MACVER
#define _huge
#endif

#ifdef _INC_WINDOWS
typedef struct
{
	SWORD           m2w_size;
	SWORD           m2w_version;
	LONG            m2w_SessionID;
	HINSTANCE       m2w_hInst;
	HWND            m2w_hMainWnd;
	HWND            m2w_hDeskWnd;
	LONG            m2w_ASIApp;
	HACCEL          m2w_hAccTable;
	FARPROC         m2w_lpMakeProcInst;
	FARPROC         m2w_lpFreeProcInst;
	FARPROC         m2w_lpAppScrap;
	FARPROC         m2w_lpAppByteSwap;
	ResType         *m2w_lpAppResType;
	LPSTR           m2w_lpCmdLine;
	FARPROC         m2w_lpAppPrintDlg;
	LPSTR           m2w_lpCurDir;
	LPSTR		m2w_lpMainWndClassName;
	LPSTR		m2w_lpSubWndClassName;
	LPSTR		m2w_lpSubMDIWndClassName;
	LPSTR		m2w_lpASIPortRSRName;
	LPSTR		m2w_lpDibDrvName;
	FARPROC		m2w_lpAppSetupDlg;
	LPSTR		m2w_lpDialogWndClassName;
} M2W, *pM2W, **hM2W;
#endif

					/* ptr to Mac Globals structure */
					/*   used by ASI_InitGlobals()  */
#define MG_VERSION 0x0100

#if MACVER
#define WidthTableHandle Handle
#endif

typedef struct
{
	SWORD           mg_size;
	SWORD           mg_version;
	short           *mg_SFSaveDisk;
	long            *mg_CurDirStore;
	short           *mg_SFVRefNum;
	GDHandle        *mg_MainDevice;
	GDHandle        *mg_DeviceList;
	GrafPtr         *mg_WMgrPort;
	struct QHdr     *mg_EventQueue;
	SWORD           *mg_MBarHeight;
	SWORD           *mg_MemErr;
	RgnHandle       *mg_GrayRgn;
	WindowPeek      *mg_WindowList;
	GrafPtr         *mg_thePort;
	Pattern         *mg_white;
	Pattern         *mg_black;
	Pattern         *mg_gray;
	Pattern         *mg_ltGray;
	Pattern         *mg_dkGray;
	Cursor          *mg_arrow;
	BitMap          *mg_screenBits;
	long            *mg_randSeed;
	ScrapStuff      *mg_ScrapInfo;
	short           *mg_PrintErr;
	long            *mg_Ticks;
	Ptr             *mg_ApplLimit;
	THz             *mg_ApplZone;
	Ptr             *mg_CurrentA5;
	Ptr             *mg_CurStackBase;
	long            *mg_CaretTime;
	WindowPtr       *mg_CurActivate;
	char            *mg_HiliteMode;
	GDHandle        *mg_TheGDevice;
	SWORD           *mg_ResErr;
	char            *mg_CrsrNew;
	RGBColor        *mg_HiliteRGB;
	char            *mg_MBState;
	Point           *mg_MTemp;
	Point           *mg_Mouse;
	Point           *mg_RawMouse;
	char            *mg_FractEnable;
	WidthTableHandle *mg_WidthTabHandle;
	SWORD           *mg_TheMenu;
	ProcPtr         *mg_ResumeProc;
	short           *mg_SysEvtMask;
	long            *mg_DoubleTime;
	THz             *mg_TheZone;
	Handle          *mg_MenuList;
	short           *mg_MenuFlash;
	short           *mg_TEScrpLength;
	Handle          *mg_TEScrpHandle;
	Boolean         *mg_ResLoad;
	short           *mg_PaintWhite; 
	long            *mg_MenuDisable;
	WindowPtr       *mg_CurDeactive;
	short           *mg_KeyRepThresh;
	short           *mg_KeyThresh;
	short	        *mg_TEScrpHasStyle;
	Handle          *mg_TEScrpStyleHandle;
	Handle		*mg_AppParmHandle;
	short		*mg_AtMenuBottom;
	short		*mg_TopMenuItem;
#if 0
	short		*mg_SysFontFam;
	unsigned long		*mg_Time;
#endif
} MG, *pMG, **hMG;

#define WS_EX_LAYOUTRTL         0x00400000L // Right to left mirroring
#define LAYOUT_RTL              0x00000001 // Right to left
unsigned long __stdcall SetLayout(HDC, unsigned long);
unsigned long __stdcall GetLayout(HDC);

// ALIASMGR.C
EXTERN F_PASCAL(void) ASI_FullPathToFSSpec(LPSTR cFileName, FSSpec *theSpec);
EXTERN F_PASCAL(OSErr) ASI_MakeAliasFile(const FSSpec *target, 
						const FSSpec *theAliasFile);

//ASIMAIN.c
EXTERN F_ASIBND( LONG ) ASI_SetFlags( LONG ASI_carebits, LONG newASIApp, 
		 	UWORD qd_carebits, UWORD qdflags );
#if WINVER
EXTERN F_ASIBND( VOID ) ASI_Set_MacWin(rsrc_map *mm, menu_override *mo);
EXTERN F_ASIBND( VOID ) ASI_InitColors(void);
#ifdef WIN32
#define ASI_IPC_ALLOCATE	1
#define ASI_IPC_FREE		2
#define ASI_IPC_GETDATA		3
#define ASI_IPC_SETDATA		4

EXTERN F_ASIBND( long ) ASI_IPC_DataBlock(short ip_cmd, long db_index, long db_size, void *p_datablock);
#endif  // WIN32
#endif  // WINVER

#ifdef _INC_WINDOWS
EXTERN F_ASIBND( UWORD )        ASI_InitGlobals( pMG pAppMG );
EXTERN F_ASIBND( VOID ) ASI_InitSession( pM2W pAppM2W );
EXTERN F_ASIBND( VOID ) ASI_ExitSession(void);
#endif

EXTERN F_ASIBND( BOOLEAN )      ASI_EnumWnd(HWND hWnd, LONG lvalue);
EXTERN F_ASIBND( VOID ) ASI_MakeAppPathName(char * szFileName, char * szAddName);

//CONTROLM.c
EXTERN F_ASIBND( void ) ASI_SetControlVis(ControlHandle control_h, unsigned char vis);

EXTERN F_ASIBND( VOID ) ASI_SetScrollBarWH( SWORD w, SWORD h );
EXTERN F_ASIBND( VOID ) ASI_SetWindowDefaultButton( ControlHandle ch, 
						Boolean SetDefault );
EXTERN F_PASCAL( void )	ASI_SetCtlFocus(ControlHandle aCMgrControl, 
						Boolean on);
EXTERN F_PASCAL( void ) ASI_ContrlOwnerChange( ControlHandle aCMgrControl, WindowPtr newowner);
EXTERN	F_ASIBND( HWND ) ASI_ControlToHWND( ControlHandle ch );
EXTERN F_ASIBND( void )	ASI_SetWin95ControlIDs( short * pIDs, short count );
EXTERN F_ASIBND( short ) ASI_AddControlFont( char * pName, short height,
							short weight );
EXTERN F_ASIBND( void )	ASI_FlushControlFonts( char * pName, short height,
							short weight );
EXTERN F_ASIBND( void )	ASI_SetControlFont( ControlHandle ch,
							short fontIndex );
EXTERN F_ASIBND( void ) ASI_Draw1Control( ControlHandle ch );

#if DELTAGRAPH
// Added for support of combo box control
EXTERN F_PASCAL( ControlHandle ) DG_NewComboControl(WindowPtr theWindow, const Rect *boundsRect,
								 ConstStr255Param title, Boolean visible, short value,
								 short min, short max, short procID, long refCon);
#endif

//EVENTMGR.c
EXTERN F_ASIBND( UINT )	ASI_MacVirtualKeyTranslate( short macVirtualKey );
EXTERN F_ASIBND( BOOLEAN )      ASI_PlayBack( BOOLEAN playbackon );
EXTERN F_ASIBND( VOID ) ASI_ActivateApp( BOOLEAN activateIt );
EXTERN F_ASIBND( VOID ) ASI_VBL_Interrupt(VOID);
EXTERN F_ASIBND( BOOLEAN )	ASI_TestUpdate( WindowPeek wPtrTemp );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_EventKey( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( LONG ) ASI_EventMouse( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( VOID ) ASI_EventCommand( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( HWND )	ASI_SetMainWindow( HWND newMain );
#endif
EXTERN F_PASCAL( OSErr ) ASI_PostActivateEvent(long eventMsg, unsigned short modifiers);

EXTERN F_PASCAL( VOID ) ASI_PostInGrowEvent(VOID);
EXTERN F_ASIBND( SWORD )        ASI_ForceCapture( SWORD newValue );
EXTERN F_ASIBND( VOID ) ASI_SetCapture( BOOLEAN set_flag );

EXTERN F_ASIBND( void ) ASI_SetWinCapture(WindowPtr wptr, BOOLEAN set_flag);
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_SetMessageHook( FARPROC lpfnMessageHook );
EXTERN F_ASIBND( VOID ) ASI_SetPeekMessageProc( FARPROC lpfnPeekMessageProc );
#endif
EXTERN F_ASIBND( ProcPtr ) ASI_SetMac2WinHook(short selector, ProcPtr HookProc);

//MENUMGR.c
EXTERN F_ASIBND( VOID ) ASI_AddResFontMenu( MenuHandle theMenu, 
						BOOLEAN trueTypeOnly );
EXTERN F_ASIBND( MenuHandle )	ASI_SearchHierMenuList( BOOLEAN searchFirst );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_InitMenuMsg( HMENU hSubMenu , HWND * hLastWndFocused);//[MI] le 23/04/2003 correction du bug ACI0026940
EXTERN F_ASIBND( VOID ) ASI_EventMenuSel( UWORD menuID );
EXTERN F_ASIBND( BOOLEAN )      ASI_MenuSelect( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( VOID ) ASI_EnterIdle( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( BOOLEAN )      ASI_MenuUserItem( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( VOID ) ASI_ParentNotify( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( SWORD ) ASI_MenuIDFromCMD(HMENU theMenu, UWORD cmd  );
EXTERN F_ASIBND( SWORD ) ASI_MenuIDFromHMENU(HMENU theMenu  );
EXTERN F_PASCAL( void ) ASI_InitInPlaceMenus( HMENU hInPlace );
EXTERN F_PASCAL( BOOLEAN ) ASI_BuildInPlaceMenuBar( HMENU hInPlace, 
					short firstMenu, short lastMenu );
EXTERN F_ASIBND( VOID ) ASI_InPlaceInitMenuMsg( HMENU hSubMenu );
#endif
#if !PPC_HEADERS
typedef void (pascal *MenuHookProcPtr)();
#endif
EXTERN F_ASIBND( void )	ASI_SetMenuHook( MenuHookProcPtr menuHookProc );
EXTERN F_ASIBND( MenuHookProcPtr )	ASI_GetMenuHook( void );
EXTERN F_ASIBND( BOOLEAN ) ASI_SetMenuResFile( short index,
				MenuHandle theMenu, short resFH );
EXTERN F_ASIBND( long ) ASI_PopUpMenuSelectUsingDiffFont( MenuHandle theMenu, 
					short top, short left, 
					short popUpItem, LPSTR lpFontName,
					short fontHeight );
EXTERN F_ASIBND( long ) ASI_MenuKey(short ch, BOOLEAN hasShift, BOOLEAN hasOption );


//PRINTTRA.c
EXTERN F_ASIBND( SWORD )        ASI_GetCollateCopies(void);
EXTERN F_ASIBND( void )		ASI_GetWinPrintInfo(THPrint hPrint, 
					HDC * pPrDC, HANDLE * pPrDlg);
EXTERN F_ASIBND( VOID ) ASI_SetPrintFileName(LPSTR lp);
EXTERN F_ASIBND( VOID ) ASI_SetMaximumPrinterResolution(short maxRes);

	/*  Defines for return code from ASI_WinIniChange()     */
#define INI_CHANGE_WINDOWS      (1 << 0)
#define INI_CHANGE_DEVICES      (1 << 1)
#define INI_CHANGE_INTL         (1 << 2)
EXTERN F_ASIBND( UWORD )        ASI_WinIniChange(LPSTR lp);
EXTERN F_ASIBND( VOID )		ASI_AbortToTop(void);
EXTERN F_ASIBND( BOOLEAN )	ASI_BandInfo( BOOLEAN *pfGraphics, 
							BOOLEAN *pfText );
EXTERN F_ASIBND( BOOLEAN )      ASI_PrOpenPageBand(TPPrPort pPrPort, 
					TPRect pPageFrame, Rect *pBandRect );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) 	ASI_SetupPrintDlgHook(FARPROC apPrintDlg);
EXTERN F_ASIBND( VOID ) 	ASI_SetupPrSetupDlgHook(FARPROC apSetupDlg);
#endif
EXTERN F_ASIBND( VOID )		ASI_ReleasePrinter(THPrint hPrint);
EXTERN F_ASIBND( VOID )		ASI_Print72DPI(THPrint hPrint, short set);
EXTERN F_ASIBND( BOOL )		ASI_SetPrintTemplates( THPrint hPrint,
	LPSTR lpPrintTemplateName, LPSTR lpSetupTemplateName );

EXTERN F_ASIBND( VOID )		ASI_FreeTHPrintRef (THPrint hPrint);
EXTERN F_ASIBND( VOID )		ASI_RetainTHPrintRef (THPrint hPrint);

//DEVICEM.C
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( LRESULT ) ASI_WinTabMessage( HWND hWnd, UINT message,
					WPARAM wParam, LPARAM lParam );
#endif

//QUICKDR.c
EXTERN F_ASIBND( VOID ) ASI_StdProcFix( BOOLEAN makeThem );
#define SWAPHDC_RESTORE_DC 0
#define SWAPHDC_ISMETAFILE_DC 1
#define SWAPHDC_ISENHMETAFILE_DC 2
EXTERN F_ASIBND( HDC ) ASI_SwaphDC( HDC hNewDC, SWORD cmdMetaFileType );
EXTERN F_ASIBND( VOID ) ASI_SetNextOpenhDC( HDC hNextDC );
EXTERN F_ASIBND( VOID ) ASI_PortRectChange( VOID );
EXTERN F_ASIBND( VOID ) ASI_UseRectForLine( BOOLEAN bUseRect );
EXTERN F_ASIBND( HWND ) ASI_SetWMgrPortDC( BOOLEAN setIt, HWND hNewWnd, 
					HDC hNewScrnDC, Rect *pNewPortRect );
EXTERN F_ASIBND( VOID )	OpenPortForHDC( CGrafPtr pCPort, HDC hDC );
EXTERN F_ASIBND( VOID ) ASI_MoveFixedAmount( BOOLEAN bRelative, Fixed horz,
	Fixed vert );
EXTERN F_ASIBND( VOID ) ASI_ChangeThePortAttr( VOID );
EXTERN F_ASIBND( VOID ) ASI_DialogFontMode( WindowPtr wind, BOOLEAN dlgmode );
EXTERN F_ASIBND( BOOLEAN ) ASI_SetCopyMaskGlobal( BOOLEAN newValue );
EXTERN F_ASIBND( BOOLEAN ) ASI_GetItemEnabled( MenuHandle, short );
EXTERN F_ASIBND( VOID ) ASI_MoveWindow( WindowPtr, short, short, Boolean);

// lve these two are broken they take into account the frame window origin twice
// EXTERN F_ASIBND( void )	ASI_ScreenToLocal( Point *pt );
// EXTERN F_ASIBND( void )	ASI_LocalToScreen( Point *pt );
// When saving windows coordinates from session to session you need to save
// Application Window relative coordinates. The global coordinates system of mac2Win
// is the screen which is not good convert from global to app window and vice versa when
// saving/reopening windows using these two helper functions:

EXTERN F_PASCAL( void ) ASI_MainWndToLocal( Point *pt );
EXTERN F_PASCAL( void ) ASI_MainWndToGlobal( Point *pt );
EXTERN F_PASCAL(void) ASI_LocalToMainWnd( Point *pt );
EXTERN F_PASCAL( void ) ASI_GlobalToMainWnd( Point *pt );
//[MI] le 18/12/2000 : correction du BUG ACI0014660
EXTERN F_PASCAL(void) ASI_LocalToDeskTop( Point *pt );
//fin [MI]

EXTERN F_ASIBND( void ) ASI_XFormToQDRect( Rect *r );
EXTERN F_ASIBND( VOID )	ASI_ResetDC();
EXTERN F_ASIBND( VOID )	ASI_ResetForeColor();
EXTERN F_ASIBND( HDC ) 	ASI_GetCurrentDC(void);
EXTERN F_ASIBND( HDC ) 	ASI_GetCurrentPrintDC(void);
EXTERN F_ASIBND( Boolean ) ASI_IsPortValid (GrafPtr pPort);

// Defines for ASI_GetSysColor
#define ASI_COLOR_SCROLLBAR		0
#define ASI_COLOR_BACKGROUND		1
#define ASI_COLOR_ACTIVECAPTION		2
#define ASI_COLOR_INACTIVECAPTION	3
#define ASI_COLOR_MENU			4
#define ASI_COLOR_WINDOW		5
#define ASI_COLOR_WINDOWFRAME		6
#define ASI_COLOR_MENUTEXT		7
#define ASI_COLOR_WINDOWTEXT		8
#define ASI_COLOR_CAPTIONTEXT		9
#define ASI_COLOR_ACTIVEBORDER		10
#define ASI_COLOR_INACTIVEBORDER	11
#define ASI_COLOR_APPWORKSPACE		12
#define ASI_COLOR_HIGHLIGHT		13
#define ASI_COLOR_HIGHLIGHTTEXT		14
#define ASI_COLOR_BTNFACE		15
#define ASI_COLOR_BTNSHADOW		16
#define ASI_COLOR_GRAYTEXT		17
#define ASI_COLOR_BTNTEXT		18
#define ASI_COLOR_INACTIVECAPTIONTEXT	19
#define ASI_COLOR_BTNHIGHLIGHT		20
#define ASI_COLOR_3DDKSHADOW        21
#define ASI_COLOR_3DLIGHT           22
#define ASI_COLOR_INFOTEXT          23
#define ASI_COLOR_INFOBK            24

#define ASI_COLOR_DESKTOP           ASI_COLOR_BACKGROUND
#define ASI_COLOR_3DFACE            ASI_COLOR_BTNFACE
#define ASI_COLOR_3DSHADOW          ASI_COLOR_BTNSHADOW
#define ASI_COLOR_3DHIGHLIGHT       ASI_COLOR_BTNHIGHLIGHT
#define ASI_COLOR_3DHILIGHT         ASI_COLOR_BTNHIGHLIGHT
#define ASI_COLOR_BTNHILIGHT        ASI_COLOR_BTNHIGHLIGHT

EXTERN 	F_ASIBND( VOID ) ASI_GetSysColor( int DisplayElement,
	RGBColor *MacColor );
#ifdef _INC_WINDOWS
extern 	F_ASIBND( COLORREF ) ASI_RGBColor2COLORREF( RGBColor *MacColor );
#endif

// Defines for ASI_SizeOf:
#define	SIZE_GDEVICE			1
EXTERN F_ASIBND( UWORD ) ASI_SizeOf( UWORD DataType );

//QDFONTIN.C
EXTERN F_ASIBND( SWORD ) ASI_GetIntlSystemFont( VOID );

//QDINIT.C
EXTERN F_ASIBND( VOID ) ASI_QDInitTestScreen( VOID );

//SANE.c
// included in SANE.H
//EXTERN F_ASIBND( extended )     ASI_Cos( extended );
//EXTERN F_ASIBND( extended )     ASI_Sin( extended );
//EXTERN F_ASIBND( extended )     ASI_Tan( extended );
//EXTERN F_ASIBND( extended )     ASI_Exp( extended );

//SCRAPMGR.c
EXTERN F_ASIBND( VOID ) ASI_DrawClipboard(void);
EXTERN F_ASIBND( short )        ASI_AnsiToMacBuf( unsigned char const *AnsiStr,
				unsigned char *MacStr, short ansiLen);
#define MTA_ISTRUETYPE 0x0001
#define MTA_FOROUTPUT 0x0002
#define MTA_FOREXTTEXTOUT 0x0004
#define MTA_USESHIFTJIS (0x0008)		/* RHKj 12/29/94 ShiftJis support  */

EXTERN F_ASIBND( short )        ASI_MacToAnsiBuf(unsigned char const *MacStr,
				unsigned char *AnsiStr, short macLen, 
				short convFlag );
EXTERN F_ASIBND(unsigned char)  ASI_MacToAnsiChar( unsigned char const MacChar );

//WINDOWM.c
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( BOOLEAN )      ASI_GetMinMaxInfo( HWND hWnd, 
		MINMAXINFO *lpWinPos );
EXTERN F_ASIBND( VOID )         ASI_GetMinMaxInfo2( HWND hWnd, 
		MINMAXINFO *lpWinPos, SWORD minClientX, SWORD minClientY);
EXTERN F_ASIBND( VOID ) ASI_GetMinMaxInfo4( HWND hWnd,
		MINMAXINFO FAR *lpWinPos, SWORD minClientX, SWORD minClientY,
		SWORD maxClientX, SWORD maxClientY);
EXTERN F_ASIBND( BOOLEAN )      ASI_SysCommand( BOOLEAN isMain, HWND hWnd, UWORD SysCmd );
F_ASIBND( UINT ) ASI_EventUpdate( HWND hSubWnd);
EXTERN F_ASIBND( LONG ) ASI_EventActivate( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( BOOLEAN ) ASI_KitchenSink( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LONG *result );
EXTERN	F_ASIBND( VOID ) ASI_FixupSystemMenu( HWND hWnd, HMENU hMenu );
#endif

EXTERN F_ASIBND( VOID ) ASI_SetMinWinSize( short Width, short Height );
EXTERN F_ASIBND( VOID )	ASI_SetwPtrMinSize( WindowPeek wPtr, short Width, short Height );
EXTERN F_ASIBND( VOID ) ASI_ClipRectChange( GrafPtr pPort );
EXTERN F_ASIBND( VOID ) ASI_ClipRgnChange( GrafPtr pPort );
EXTERN F_ASIBND( VOID ) ASI_VisRgnChange( GrafPtr pPort );
EXTERN F_ASIBND( BOOLEAN )      ASI_CheckUpdateRgn( WindowPeek wPtr );
EXTERN F_ASIBND( LONG ) ASI_WindDiffSize( WindowPeek wPtr, BOOLEAN *pIsIconic );
EXTERN F_ASIBND( VOID ) ASI_WindowLocation( BOOLEAN saveIt, WindowPeek wPtr, 
							Rect *pWinLocation );
EXTERN F_ASIBND( VOID ) ASI_CheckMaxWindowRect( Rect *pwr, BOOLEAN adjustWH, 
						BOOLEAN screenRelative );
EXTERN  F_ASIBND( VOID ) ASI_TopMainWnd(VOID);
EXTERN F_ASIBND( VOID ) ASI_OverrideDefaultWindowCaptionHeight( short Height );

//BYTESWAP.c
EXTERN F_ASIBND( VOID ) ASI_ByteSwapWORD( UWORD *ptheWd );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapLONG( LONG *ptheLg );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapRect( Rect *ptheRect );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapLONGS( LONG *pData, LONG lSize );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapWORDS( UWORD *pData, LONG lSize );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapFLOAT( float * pFloat );
EXTERN F_ASIBND( VOID ) ASI_ByteSwapFLOATS( float * pFloat, short size );
EXTERN F_ASIBND( VOID ) ASI_ByteSwap( BOOLEAN Mac2Win, ResType theRT, 
				SWORD theID, LONG lres_size, LPSTR lpdata );
EXTERN F_ASIBND( void )	ASI_ByteSwapDOUBLE( double * pDouble );
EXTERN F_ASIBND( void )	ASI_ByteSwapEXTENDED( long double * pDouble );
EXTERN F_ASIBND( void ) ASI_ByteSwap881( double881 *pDouble );
EXTERN F_ASIBND( void ) ASI_ByteSwapCLUT( BOOLEAN Mac2Win, CTabPtr pCTab );
EXTERN F_ASIBND( VOID )	ASI_Mac2WinCursor( BOOLEAN createCursor, 
				BOOLEAN isColorCursor, LPSTR lpData,
				BOOLEAN Mac2Win, BOOLEAN byteswapit );
F_ASIBND(HCURSOR) ASI_WindowsCursorFromMac(const Cursor *crsr);	/* This is in current Mondrian project. This is here for the A&A cut. */

#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_ByteSwap_Code( BOOLEAN Mac2Win, ResType theRT,
				SWORD theID, LONG lres_size, LPSTR lpdata,
				FARPROC optByteSwapProc);
#endif

//QDRGN.c
EXTERN F_ASIBND( BOOLEAN )      ASI_IsRectRgn( RgnHandle rgn );

#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_RgnGetBBoxRECTFromRgn( RgnHandle rgn, RECT *pr );
#endif

EXTERN F_ASIBND( HRGN ) ASI_RgnSetToRealRectRgn( RgnHandle rgn );
EXTERN F_ASIBND( HRGN ) ASI_RgnSetToFakeRectRgn( RgnHandle rgn );
EXTERN F_ASIBND( UWORD )        ASI_SelectClipRgn( HDC hDC, RgnHandle rgn );

//HFS.c
EXTERN  F_ASIBND( SWORD )       ASI_FHCreate( LPSTR lpName, UWORD attributes );
EXTERN  F_ASIBND( BOOLEAN )     ASI_FHReOpen( BOOLEAN doReOpen, SWORD MacRefNum );
EXTERN  F_ASIBND( VOID )        ASI_FHReOpenAll( BOOLEAN doReOpen );
EXTERN  F_ASIBND( SWORD )       ASI_FHOpen( LPSTR lpName, UWORD mode, SWORD *pError );
#ifdef _INC_WINDOWS
EXTERN  F_ASIBND( ASI_HFILE )   ASI_FHMap( SWORD MacRefNum );
EXTERN  F_ASIBND( SWORD )       ASI_FHUnMap( ASI_HFILE DosFH );
#endif
EXTERN  F_ASIBND( SWORD )       ASI_FHClose( SWORD MacRefNum ); 
EXTERN  F_ASIBND( LONG )        ASI_FHRead( SWORD MacRefNum, void _huge* buffptr, LONG lcnt);
EXTERN  F_ASIBND( LONG )        ASI_FHWrite( SWORD MacRefNum, void _huge* buffptr, LONG lcnt);
EXTERN  F_ASIBND( LONG )        ASI_FHLSeek( SWORD MacRefNum, LONG seekPtr, UWORD position );
EXTERN  F_ASIBND( OSErr )       ASI_FHSetEOF0( SWORD MacRefNum );
EXTERN  F_ASIBND( UWORD )       ASI_FHGetAttr( SWORD MacRefNum );
EXTERN	F_ASIBND( void )	ASI_VerifyDriveLetter( LPSTR pPath );

//FILEMGR.C
EXTERN  F_ASIBND( short )       ASI_DOSVolToVRefNum( LPSTR pPath );
EXTERN  F_ASIBND( BOOLEAN )     ASI_VRefNumToDOSVol( short VRefNum, LPSTR pPath );
EXTERN F_ASIBND(OSErr)		ASI_HFSpec( long ioDirID, short VRefnum,
					Str255 fileName, LPSTR cfilename );
EXTERN F_ASIBND(OSErr)		ASI_FSpec( SWORD VRefNum, Str255 fileName, 
					LPSTR cfilename );
EXTERN F_ASIBND( long )         ASI_DirIDFromPath( LPSTR pPath,
                                        short * pVRefNum, short * pWDRefNum );
EXTERN F_ASIBND( void )		ASI_UpdateDriveLetters( void );
EXTERN 	F_ASIBND( UWORD )	ASI_CurDirvRefNum( LPSTR DirPath );

#ifdef WIN32
F_PASCAL(long) ASI_MakeMacAFPRSR(short srcVRefNum, long srcDirID, ConstStr255Param srcName);
F_PASCAL(long) ASI_MakeMacAFPInfo(short srcVRefNum, long srcDirID, ConstStr255Param srcName,
																	OSType fileType, OSType fileCreator);
#endif

//STDFILEP.c
#define kASI_SF_UseCurrentFilter -99

EXTERN F_ASIBND( VOID ) ASI_SF_SetFilterString(LPSTR filterStr, SWORD defIndex);
EXTERN F_ASIBND( VOID ) ASI_SF_String(SWORD numTypes, SWORD defTypeIndex, SFTypeList typeList);
EXTERN F_ASIBND( VOID ) ASI_DosFSpec2MacSFReply( BOOLEAN appendExt, 
			BOOLEAN overrideExt, SWORD SFExtIndex, 
			BOOLEAN updateSFCurDir,
			BOOLEAN updateLastGetName,
			LPSTR pFileSpec, SFReply *reply );
EXTERN F_ASIBND( VOID ) ASI_SeparateFile(char *lpDestPath, char *lpDestFileName, char *lpSrcFileName);
EXTERN F_ASIBND( VOID ) ASI_CustomGetFile_Prompt(unsigned char *lpPrompt);

//DRAWPIC.c
EXTERN F_ASIBND( PicHandle )    ASI_OpenScrapPict( Rect *picFrame );
EXTERN F_ASIBND( BOOLEAN )	ASI_AllowPictStatePush( BOOLEAN bAllow );
EXTERN F_ASIBND( void )		ASI_SetFixXORMode( short state );
EXTERN F_ASIBND( void *) ASI_PictToEmf(PicHandle srcpict,char* filename); // ev 18/12/97

				/* defines for sv_cmd		*/
#define ASISV_CMD_GET 0
#define ASISV_CMD_SET 1
				/* defines for sv_field		*/
#define ASISV_gl_DrawPicHadCopyBits 1
#define ASISV_gl_LayerNextDialogAsDocument 2
#define ASISV_SM_CXVSCROLL 3
#define ASISV_SM_CYHSCROLL 4
#define ASISV_gl_EventUpdateCheckCnt 5
#define ASISV_gl_PeekMessageYieldCnt 6
#define ASISV_gl_NoScrollRectEraseRgn 7
#define ASISV_gl_MacFHForFontMapAndRsrcInEXE 8
#define ASISV_gl_LSeekForFontMapInEXE 9
#define ASISV_gl_LSeekForSystemRsrcInEXE 10
#define ASISV_gl_LSeekForAppRsrcInEXE 11
#define ASISV_gl_AlertProcID 12
#define ASISV_gl_HasAppleMenu 13
#define ASISV_gl_256ColorPaletteStatus 14
#define ASISV_gl_MemSubMax 15
#define ASISV_gl_EraseDuringTextRedraw 16
#define ASISV_gl_NoSetSysColors 17
#define ASISV_gl_UseASIDForInitialFocus 18
#define	ASISV_gl_OnePixelPatternLines 19
#define	ASISV_gl_OpenResFilesReadOnly 20
#define ASISV_gl_DoesPartialGetSetDIBits 21		/* read-only */
#define ASISV_gl_CurrentHPALETTE 22			/* read-only */
#define ASISV_gl_DisableClipping 23
#define ASISV_gl_MapUpperChars 24
//#define ASISV_gl_MapUpperTEOutput 25
#define ASISV_gl_AccurateColor2Index 26
#define ASISV_gl_ModalDialogActive 27          /* read-only */
#define ASISV_gl_ExternalMouseEventsInModalDialog 28
#define ASISV_gl_EnableColorUse 29
#define ASISV_gl_OrigCntlValue 30
//#define ASISV_gl_NoCloseCommand 31	// replaced by below
#define	ASISV_gl_WindowCreateFlags 31
		// Does the window act like a palette (layers above doc windows)
	#define	WCF_ACTSLIKEPALETTE_NO		0x0001
	#define	WCF_ACTSLIKEPALETTE_YES		0x0002
		// Does the window look like a palette (small title bar)
	#define	WCF_LOOKSLIKEPALETTE_NO		0x0004
	#define	WCF_LOOKSLIKEPALETTE_YES	0x0008
		// Layer the next dialog as a regular document window
	#define	WCF_LAYERASDOCUMENT		0x0010
		// Layer the next window as if it were a dialog
	#define	WCF_LAYERASDIALOG 		0x0020
		// Use the bounds rect as App-relative (the default)
	#define	WCF_APPMAINRELATIVE		0x0040
		// Use the bounds rect as truly screen relative regardless
		//   of the position of the App's main window
	#define	WCF_SCREENRELATIVE		0x0080
		// Remove the Close command from the system menu
	#define	WCF_NOCLOSECOMMAND		0x0100
		// Create the window in maximized state
	#define	WCF_MAXIMIZE			0x0200
		// Keep the size command in the system menu of a dialog
	#define	WCF_KEEPSIZECOMMAND		0x0400
		// Layer the next dialog as a palette window
	#define	WCF_LAYERASPALETTE		0x0800
#define ASISV_gl_LayerNextDocumentAsDialog 32
#define	ASISV_gl_WindowProcID 33
#define	ASISV_gl_WindowMenuHandle 34
#define ASISV_gl_AddStarDotStarToFilters 35
#define	ASISV_gl_DefaultType 36
#define ASISV_gl_DoSimpleCopyMask 37
#define ASISV_gl_MacLike_StdState 38
#define	ASISV_gl_CreateWindowHandler 39

#define ASISV_gl_UseComboboxControl 40
#define ASISV_gl_CreateWingDib 41
#define ASISV_GetIMEStringSize 42
#define ASISV_FlushIME 43
#define ASISV_gl_KeepDCInPrintDefault 44
#define ASISV_gl_MapExtendedToStrikeOut 45
#define ASISV_gl_ScaleScreenFonts 46
#define	ASISV_gl_CalcAscentLeadingFlags 47
	// how do we build ascent and descent
	#define CALF_ASCENT_EQUALS_PLUS_TMINTERNALLEADING 0x0001
	#define CALF_ASCENT_EQUALS_PLUS_TMEXTERNALLEADING 0x0002
	#define CALF_ASCENT_EQUALS_MINUS_TMINTERNALLEADING 0x0004
	#define CALF_ASCENT_EQUALS_MINUS_TMEXTERNALLEADING 0x0008

	#define CALF_LEADING_EQUALS_PLUS_TMINTERNALLEADING 0x0010
	#define CALF_LEADING_EQUALS_PLUS_TMEXTERNALLEADING 0x0020
	#define CALF_LEADING_EQUALS_MINUS_TMINTERNALLEADING 0x0040
	#define CALF_LEADING_EQUALS_MINUS_TMEXTERNALLEADING 0x0080

	#define CALF_EXTTEXTOUT_ASCENT_EQUALS_PLUS_TMINTERNALLEADING 0x0100
	#define CALF_EXTTEXTOUT_ASCENT_EQUALS_PLUS_TMEXTERNALLEADING 0x0200
	#define CALF_EXTTEXTOUT_ASCENT_EQUALS_MINUS_TMINTERNALLEADING 0x0400
	#define CALF_EXTTEXTOUT_ASCENT_EQUALS_MINUS_TMEXTERNALLEADING 0x0800

	#define CALF_EXTTEXTOUT_LEADING_EQUALS_PLUS_TMINTERNALLEADING 0x1000
	#define CALF_EXTTEXTOUT_LEADING_EQUALS_PLUS_TMEXTERNALLEADING 0x2000
	#define CALF_EXTTEXTOUT_LEADING_EQUALS_MINUS_TMINTERNALLEADING 0x4000
	#define CALF_EXTTEXTOUT_LEADING_EQUALS_MINUS_TMEXTERNALLEADING 0x8000
#define	ASISV_gl_MDIClassNameOverride 48
#define ASISV_gl_PopUpMenuCallBack 49
#define ASISV_gl_MemFreeBias 50
#define ASISV_gl_UseEnhancedMenuShortCuts 51
#define ASISV_gl_DrawPicUseStretchText 52
#define ASISV_gl_Force3D 53
#define ASISV_gl_HiliteScrollbars 54
#define	ASISV_gl_Palette_Support 55
#define ASISV_gl_CtrlKeyAsCmdCtrlKeys 56
#define ASISV_gl_MDIS_ALLCHILDSTYLES 57
#define ASISV_gl_MDIS_MFC 58
#define ASISV_gl_StaticTextRightJust 59
#define ASISV_gl_NextDialogHasWhiteBkGnd 60
#define ASISV_gl_CheckPrintDlgErrors 61
#define ASISV_gl_PictRecordClipFilter 62
	#define PRCF_None 0
	#define PRCF_Stupid 1
#define ASISV_gl_UseDashedLineInXorMarquee 63
#define ASISV_gl_AddAnyFilesToFilters 64
#define ASISV_gl_AllowListNonIntegralHeight 65
#define ASISV_gl_ReUsePrintDC 66
#define ASISV_gl_MakeInitialMDIStateMaximized 67
#define	ASISV_resv 68
#define	ASISV_gl_NoPaintWhite 69
#define ASISV_gl_VerifyDirPaths 70
#define ASISV_gl_BrushAlignToClient 71
#define ASISV_gl_SizeBox_Support 72
#define	ASISV_gl_DialogOriginOffset 73
#define ASISV_gl_MacApp 74
#define ASISV_gl_PCExchangeRSR 75
#define ASISV_gl_DBCS 76
#define ASISV_gl_MaxMenuItems 77
#define ASISV_gl_UseFullCopyBits 78
#define ASISV_gl_UseFullPalette 79
#define ASISV_gl_UseMDEFCodeRegistry 80
#define ASISV_gl_UseMac8CTab 81
#define ASISV_gl_NextDialogUseFakeBtnControl 82
#define	ASISV_gl_UseWindowsHiliteStyle 83
#define ASISV_gl_UseDitheredBrush	84
#define ASISV_gl_WinKanjiKeyCode	85
#define ASISV_gl_SetNextNewWindowStyles 86
#define ASISV_gl_GetNextEvent_sleep_ticks 87
#define ASISV_gl_Mercutio 88
#define ASISV_gl_InitWithResourceCTable 89
#define ASISV_gl_MergeStaticColors 90
#define ASISV_gl_MapTabToSpace 91
#define ASISV_gl_DelayFirstDialogUpdate 92
#define ASISV_gl_SetDialogFont 93
#define ASISV_gl_SetApplCreator 94
#define ASISV_gl_SupportUpdateGWorld 95
#define ASISV_gl_SupportMacStyleShowCursor 96
#define ASISV_gl_ProcessVBLQueueOnMouseDownOnly 97
#define ASISV_gl_ListsUseDialogBkg 98
#define ASISV_gl_ListsAlwaysDrawItems 99
#define ASISV_gl_CountDirFiles 100
#define ASISV_gl_DLLDrawsGrowIcon 101
#define ASISV_gl_SendRightButtonMessagesToParent 102
#define ASISV_gl_Just_WM_CHAR 103
#define ASISV_gl_GenerateMouseMoveEvt 104
#define ASISV_gl_IgnoreMacDlgHook 105
#define ASISV_gl_SupportHelios 106
#define ASISV_gl_SupportNTFS 107
#define ASISV_gl_UseHiliteModeTextSelection 108
#define ASISV_gl_InvalidateForLDraw 109
#define ASISV_gl_UseWindowsSize  110
#define ASISV_gl_UseWindowsColorsForTextSelection 111
#define ASISV_gl_DontClipTextEdit 155
#define ASISV_gl_HasSetDIBitsToDevice 156
#define ASISV_gl_PostScript_Dont_Erase 157
#define ASISV_gl_M2W 158
#define ASISV_gl_IsTips 159 //[MI] le 23/04/2003 correction du bug ACI0026940

#define ASISV_gl_DrawPicHadQuickTime 1000 // ev 27/01/99 (je le mets en 1000 pour faire un range ACI)
#define ASISV_gl_InSizeWindow 1001 // indique si on est dans un SizeWindow (used in WM_GETMINMAXINFO)
#define ASISV_gl_bShowInitError 1002	// CV 17/09/99 (pour message erreur printer)
#define ASISV_gl_AFPNextType 1003	// L.E. 13/01/2000 prochain finder type a utiliser
#define ASISV_gl_AFPNextCreator 1004	// L.E. 13/01/2000 prochain creator a utiliser
#define ASISV_gl_ArabicDrawFlags 1005 //[MI] le 12/03/2001 : pour la version ARABE
#define ASISV_gl_NewGWorldAlwaysUseScreenDC 1006 // pp 25/02/04	
#define ASISV_gl_szShiftCtrlTempStr 1007	// m.c, June 2004
#define ASISV_gl_DoNotMaximize 1008	// m.c, Feb. 2005
#define ASISV_gl_ResetFileObject 1009	// m.c, March 2005
#define ASISV_gl_bForceBlittingLtoR 1010 //[MI] le 18/10/2005 : pour la version ARABE
#define ASISV_gl_bIgnoreForceBlittingLtoR 1011

	
	#define	ARABIC_NO_REORDER				0x0001
	#define	ARABIC_GLYPH_LINKAFTER			0x0002
	#define	ARABIC_GLYPH_LINKBEFORE 		0x0004

// structure passed to ASI_SystemValue when using ASISV_gl_SetNextNewWindowStyles
typedef struct {
	HWND hParentWnd;
	LONG WindowStyle, ExWindowStyle;
	short Caption; // caption height , 0 if no caption  (should be consistent with WindowStyle)
	short BorderHeight, BorderWidth; // Borders height and width (should be consistent with WindowStyle)
	BOOLEAN LooksLikePalette;
	BOOLEAN ActsLikePalette;
	BOOLEAN HasGrowBox;
	LONG	windowFlags;
} SetNextWindowStyle;

// structure passed to ASI_SystemValue when using ASISV_gl_Mercutio
typedef struct {
	Boolean enable; // true to enable, false to turn off (default is off)
	short MDEFID; // ID of Mercutio MDEF, menus with that mdef will be considered Mercutio-style menus 
	long MercutioVersion; // which version of the MDEF to emulate (they use different mapping schemes from version to version)
} MercutioStyle;


EXTERN F_ASIBND( long )		ASI_SystemValue( short sv_cmd,
					short sv_field, long sv_value );

// Hooks to customize Mac2Win behaviour
#define CustomGetFOSTypeHook 1
typedef BOOLEAN (*CustomGetFOSTypeProc)( BYTE *fileName, short vRefNum,
OSType *pType, OSType *pCreator );

//QDBITS.c
EXTERN F_ASIBND( BOOLEAN )      ASI_QueryNewPalette( HWND hWnd );
EXTERN F_ASIBND( VOID ) ASI_PaletteChanged( HWND hWnd, HWND chgWnd );

//QDMACDIB.c
EXTERN F_ASIBND( VOID )	    ASI_CopyBitsMacToDIB( wPixMapPtr lpWPMSrc,
				wPixMapPtr lpWPMDst,
				Rect sr, Rect dr, SWORD Mode,
				CTabHandle hCTSrc, CTabHandle hCTDst,
				RgnHandle hRgnMask );

//QDDIBOUT.c
EXTERN F_ASIBND( VOID )	    ASI_DSSimple( wPixMapPtr lpWPMSrc,
				Rect sr, Rect dr,
				CTabHandle hCTSrc );
EXTERN F_ASIBND( VOID )	    ASI_DSNotSimple( wPixMapPtr lpWPMSrc,
				wPixMapPtr lpWPMDst,
				Rect sr, Rect dr, SWORD Mode,
				CTabHandle hCTSrc, CTabHandle hCTDst,
				wRgnHandle hRgnMask );
EXTERN F_ASIBND( VOID ) ASI_NextWriteIsToScreen ();
EXTERN F_ASIBND ( VOID ) ASI_WinGBitBlt( HDC hdcDest, int nXOriginDest,
        int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc,
        int nXOriginSrc, int nYOriginSrc );
EXTERN F_ASIBND ( BOOLEAN ) ASI_LoadWinG (void);

//QDMASK.c
EXTERN F_ASIBND( Handle )   ASI_CreateMask( BitMap *bmSrc,
				BOOLEAN fFlood, BOOLEAN fRetRgn );

//QDPoly.c
#ifdef _INC_WINDOWS
F_ASIBND( BOOL ) ASI_PolyPolygon( HDC, LPPOINT, LPINT, int);
#endif

//QDBRUSH.c
	F_ASIBND( VOID )
ASI_BrushAlign( SWORD wantAlign );

// QDPIC.c
EXTERN F_ASIBND( void )	ASI_AddEmfToCurrentPict( void* emf,Rect* rr ); // from ev 24/09/97


//WINDUTIL.c
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( LONG ) ASI_NCMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( LONG ) ASI_WindChange( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
EXTERN F_ASIBND( LRESULT )      ASI_MDIMessage( UINT msg, WPARAM wParam, LPARAM lParam );

EXTERN F_ASIBND( LONG ) ASI_MDIDefaultState( BOOLEAN setIt, LONG lNewMDIStyle,
			MDICREATESTRUCT *mcs );
#endif
// moved outside for DeltaGraph
EXTERN F_ASIBND( VOID ) ASI_GetCurHDCs( HDC *curHdc, HDC *curPrHdc, 
							SWORD *isPSPrinter  );

//RESOURCE.c

#define CODE_RESOURCE_UNREGISTER 0
#define CODE_RESOURCE_REGISTER 1
#define CODE_RESOURCE_INIT 2
#define CODE_RESOURCE_FIND 3
#define CODE_RESOURCE_EXIT 4
#define CODE_RESOURCE_EXISTS 5
EXTERN F_ASIBND( ProcPtr ) ASI_CodeResource( UWORD cmd, OSType DefType, 
				UWORD resID, ProcPtr theFP );

EXTERN F_ASIBND( ProcPtr ) ASI_SetSwapResourceProc(SWORD fh,ProcPtr swapProc);

EXTERN F_PASCAL(void) ASI_MakeResSpec(FSSpec *resSpec);

//LISTMGR.c
#define LSETSEL_MASK		0x00FF
#define LSETSEL_USEDIALOGBKG	0x4000
#define LSETSEL_ALWAYSDRAWITEMS	0x2000
#define LSETSEL_ACTIVECLIKLOOP	0x1000
#define LSETSEL_MULTICOL	0x0800
#define LSETSEL_REAL2DLIST	0x0400
#define LSETSEL_NOKBDINPUT	0x0200
#define LSETSEL_NOSELECTION	0x0100
EXTERN F_ASIBND( VOID ) ASI_LSetSelFlags( UWORD selFlags );

#define LTEAROFF_INIT		0x0001
#define LTEAROFF_TEAROFFITEM	0x0002
#define LTEAROFF_CMD_MASK	0x000F
#define LTEAROFF_FLAG_AUTOSCROLL 0x8000
#define LTEAROFF_FLAG_DONTDRAGRECT 0x4000
EXTERN F_ASIBND( BOOLEAN )      ASI_LTearOff( ListHandle lHandle, short cmd, Point *pPt );

#if WINVER
typedef BOOLEAN (CALLBACK *TearOffHookProcPtr)( Point *pLocMouse,
		Cell *lCell, Rect *prExclude, SWORD dragW, SWORD dragH );

EXTERN	F_ASIBND( VOID ) ASI_LSetTearOffDragHook( ListHandle lh, 
					TearOffHookProcPtr dragHook );

typedef BOOLEAN (CALLBACK *ClickHookProcPtr)(ListHandle lh);

	F_ASIBND( ClickHookProcPtr )
ASI_LSetClickHook( ListHandle lh, ClickHookProcPtr clickHook );

#endif
F_ASIBND( HWND )		ASI_GetHWNDFromListHandle( ListHandle lh );
F_ASIBND( VOID )		ASI_ListUpdateVisible( ListPtr pL );

//QDFONT.c
EXTERN  F_ASIBND( VOID )        ASI_UpdateFontList( BOOLEAN forceReEnumerate );

//QDTEXT.c
EXTERN  F_ASIBND( short )       ASI_ExtDrawText( UWORD bUse, WidthTable *lpWT, 
							UWORD *lpMacDX);
EXTERN  F_ASIBND( VOID )        ASI_SetXFormAngle( SWORD angle );
EXTERN  F_ASIBND( VOID )        ASI_SetXFormFlip( SWORD flipFlag ); 
EXTERN  F_ASIBND( SWORD )        ASI_GetXFormAngle( VOID );
EXTERN  F_ASIBND( SWORD )        ASI_GetXFormFlip( VOID );
EXTERN	F_ASIBND( BOOLEAN )		 ASI_BindTextAttributes( VOID ); 
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( HFONT )	ASI_GetBindTextAttributesHFONT(
					ENUMLOGFONT *anELogFont );
EXTERN F_ASIBND( HFONT )	ASI_SetBindTextAttributesHFONT( HFONT hFontUse );
#endif
// moved outside for DeltaGraph
EXTERN  F_ASIBND( VOID )        ASI_USER_WinTextOutWidths( 
				   UINT *lpUserPositioning, short numWidths );

//WINDUPDT.c
EXTERN F_ASIBND( VOID ) ASI_UpdateWindows( SWORD doit );

//QDDEVBIT.c
EXTERN F_ASIBND( PixMapHandle ) ASI_NewDDBPixMap( long lbm, long lpal );

//QDDIB.c
EXTERN F_ASIBND( PixMapHandle ) ASI_NewDIBPixMap( void * lpdib );

//OFFSCRN.c
EXTERN F_ASIBND( BOOLEAN )      ASI_BeginOffScreen( BOOLEAN bErase, Rect *pr );
EXTERN F_ASIBND( VOID ) ASI_EndOffScreen( BOOLEAN bDraw, BOOLEAN bExcludeUpdateRgn, SWORD blitMode );
EXTERN F_ASIBND( BOOLEAN )      ASI_OffScreenRotate( PixMapHandle hPM, SWORD Angle );
EXTERN F_ASIBND( BOOLEAN )      ASI_OffScreenFlip( PixMapHandle hPM, BOOLEAN bHorzFlip );
EXTERN F_ASIBND( Handle )       ASI_PackPixMap( PixMapHandle hPM );
EXTERN F_ASIBND( PixMapHandle ) ASI_UnpackPixMap( Handle hPack );
EXTERN F_ASIBND( BOOLEAN )      ASI_ExcludeUpdateRgn( WindowPeek wPtr, 
				  BOOLEAN excludeIt, RgnHandle *pOldVis );

#define OSDC_ALLOCATE_DC 0x0000		// allocate our own compatible DC
#define OSDC_SHARE_MEMDC 0x0001		// share the global Memory DC
#define OSDC_USE_DIBDC 0x0002		// allocate DIB driver based DC
EXTERN F_ASIBND( HDC )		ASI_OffScreenDC( PixMapHandle hPixMap, 
					UWORD shareFlag );

//QDDIBBIT.c
// Bit flags for DIB to PixMap Conversion process, used to set UnfoldFlag.

#define UNFOLD_FreeHBM 0x0001
#define UNFOLD_BasedOnDestPixMap 0x0002
#define UNFOLD_BasedOnSourceBitMap 0x0004

#define UNFOLD_NoBaseAddrReAlloc 0x0008	// fail if baseAddr not big enough
#define UNFOLD_UsePalColors	   0x0010	// RB: use identity palette
#define UNFOLD_InvertPalColors     0x0020 // RB: invert palette index
#define UNFOLD_ReleaseHBM 	 0x0040	  // RB: zero out HBM, but don't free

#define UNFOLD_Force1Bit 0x0100	// GetDIBits() for monochrome
#define UNFOLD_Force4Bit 0x0200	// GetDIBits() for 16 colors
#define UNFOLD_Force8Bit 0x0400	// GetDIBits() for 256 colors
#define UNFOLD_Force32Bit 0x0800	// GetDIBits() for RGB colors

#define UNFOLD_Pack32in24 0x1000	// PackType == 2
#define UNFOLD_Pack32byRLE 0x2000	// PackType == 4, not supported yet
#define UNFOLD_Pack16byRLE 0x4000	// PackType == 3, not supported yet

EXTERN F_ASIBND( BOOLEAN )	ASI_UnfoldPixMap( BitMapPtr bmSrc, 
							BitMapPtr bmDst,
							Rect *prSrc,
							Rect *prDst,
							UWORD UnfoldFlag );


#define FOLD_PutInDIB 0x0002
#define FOLD_IgnoreMacBaseAddr 0x0004
#define FOLD_PutInDDB 0x0008
#define FOLD_UpdateDIBRGBFromCTab 0x0010
#define FOLD_4BitAs4Bit 0x0020
EXTERN F_ASIBND( BOOLEAN )	ASI_FoldPixMap( BitMapPtr bmDst, 
							Rect *prDst,
							UWORD FoldFlag );

EXTERN F_ASIBND( BOOLEAN )	ASI_GetPixMapInfo( BitMapPtr bmSrc );
   
EXTERN F_ASIBND( HBITMAP )	ASI_CopyBM( HBITMAP hBMSrc );

EXTERN F_ASIBND( LPBITMAPINFO )	ASI_GetDIBFromPixMap( PixMapHandle hPM,
						      Ptr far *lplpBits );
EXTERN F_ASIBND( VOID         )	ASI_GetHdcFromPixMap( PixMapHandle hPM,
			      HANDLE far* phDC, BOOLEAN far* pIsWing );

//QDHILITE.c
EXTERN F_ASIBND( VOID ) ASI_HiliteNotSectRect( Rect r1, Rect r2, BOOLEAN bUseHilite );

//DLGKEYHK.c
EXTERN F_ASIBND( WindowPtr )    ASI_FocusWindow(void);

EXTERN F_ASIBND( HWND )	ASI_KeyHook( HWND hHookWnd, BOOLEAN hook_it );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( SWORD )        ASI_DlgKeyHook(SWORD iCode, WPARAM wParam, LPARAM lParam);
#endif

#define WNDTYPE_UNKNOWN -1
#define WNDTYPE_EDIT 1
#define WNDTYPE_SCROLLBAR 1
#define WNDTYPE_BUTTON 2
#define WNDTYPE_USERITEM 3
#define WNDTYPE_CDEF 4
#define WNDTYPE_STATIC 5
#define WNDTYPE_LISTBOX 6
#define WNDTYPE_COMBOBOX 7
typedef F_PASCAL_PTR( VOID, *DlgKBHookProcPtr )( WindowPtr wPtr, 
				SWORD itemID, SWORD itemWndType,
				UWORD resv, EventRecord *pEvent,
      				BOOLEAN *pPassToDialog, 
				BOOLEAN *pPassToControl );
EXTERN	F_ASIBND( VOID )	ASI_SetDlgKBHook( DlgKBHookProcPtr lpfnDlgKBHook );


EXTERN F_ASIBND( VOID ) ASI_AddASIDItems( DialogPeek dPtr, SWORD ResId );
#if WINVER
EXTERN F_ASIBND( ASI_DITLHandle ) ASI_UseASIDItems( DialogPeek dPtr, 
								SWORD ResId );
EXTERN F_ASIBND( BOOLEAN ) ASI_SetItemFocus( DialogPeek dPtr , SWORD itemNo); // WTJ 10/30/95
EXTERN F_ASIBND( SWORD ) ASI_GetFocusCtrlID( DialogPeek dPtr ); // WTJ 10/25/95
#endif
EXTERN F_ASIBND( HWND ) ASI_GethWnd( WindowPeek wPtr );
EXTERN F_ASIBND( BOOLEAN )      ASI_GetwPtr( HWND hSubWnd, WindowPeek *pwPtr );
EXTERN F_ASIBND( VOID ) ASI_EndSession( BOOLEAN InEndSession );
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( VOID ) ASI_EnableHelpHook(FARPROC helpProc);
EXTERN F_ASIBND( VOID ) ASI_DisableHelpHook(void);
#endif
 
//TEXTEDIT.c
EXTERN F_ASIBND( BOOLEAN )     ASI_SethTEfromhWnd( TEHandle hTE, BOOLEAN ForceChange );
EXTERN F_ASIBND( VOID ) 		ASI_SetTE3D(TEHandle hTE, Boolean on);
EXTERN F_ASIBND( Boolean ) 	ASI_GetTE3D(TEHandle hTE);
#if PPC_HEADERS
#define	ClikLoopProcPtr		TEClickLoopUPP
#endif // PPC_HEADERS

EXTERN F_PASCAL( void )		ASI_MakePIClickLoop( ClikLoopProcPtr *clikProc );

typedef void (pascal *draw_text_proc_ptr)( TEHandle hTE, 
		const void *textBuf, short firstByte, short byteCount );
typedef short (pascal *text_width_proc_ptr)(TEHandle hTE, 
		const void *textBuf, short firstByte, short byteCount );
typedef void (pascal *qdrect_proc_ptr)(GrafVerb gv,	const Rect *r_ptr);

EXTERN	F_ASIBND( void ) ASI_TECustomHooks(TEHandle hTE, 
				draw_text_proc_ptr AppDrawText, 
				text_width_proc_ptr AppTextWidth,
				qdrect_proc_ptr AppQDRectProc );

typedef void (pascal *DrawHookProcPtr)(unsigned short textOffset,
		unsigned short drawLen, char *textBufferPtr, TEPtr pTE, TEHandle hTE);
typedef unsigned short (pascal *WidthHookProcPtr)(unsigned short textLen,
		unsigned short textOffset, char *textBufferPtr, TEPtr pTE, TEHandle hTE);
typedef unsigned short (pascal *TextWidthHookProcPtr)(unsigned short textLen,
		unsigned short textOffset, char *textBufferPtr, TEPtr pTE, TEHandle hTE);
typedef unsigned short (pascal *NWidthHookProcPtr)(unsigned short styleRunLen,
		unsigned short styleRunOffset, short slop, short direction,
		char *textBufferPtr, short *lineStart, TEPtr pTE, TEHandle hTE);
typedef Boolean (pascal *HitTestHookProcPtr)(unsigned short styleRunLen,
		unsigned short styleRunOffset, unsigned short slop, char *textBufferPtr,
		TEPtr pTE, TEHandle hTE, unsigned short *pixelWidth,
		unsigned short *charOffset, Boolean *pixelInChar);
typedef Boolean (pascal *EOLHookProcPtr)(char theChar, TEPtr pTE, TEHandle hTE);

EXTERN	F_ASIBND( void ) ASI_TECustomHooksUPP( TEHandle hTE,
				     DrawHookProcPtr AppDrawHook,
					 WidthHookProcPtr AppWidthHook,
					 TextWidthHookProcPtr AppTextWidthHook,
					 NWidthHookProcPtr AppNWidthHook,
					 HitTestHookProcPtr AppHitTestHook,
					 EOLHookProcPtr AppEOLHook );
EXTERN	F_PASCAL( Boolean ) isEndOfLine( char theChar, TEPtr pTE, TEHandle hTE );
EXTERN	F_PASCAL( Boolean ) isCReturn( char theChar, TEPtr pTE, TEHandle hTE );
EXTERN	F_PASCAL( Boolean ) ASI_WordBreak (Ptr ptr,short offSet);
					 
//DIALOGM.c
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( BOOLEAN )      ASI_UserItem( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );
#endif

#ifdef WIN32
typedef Boolean (far pascal *ASI_AltKeyProc)(unsigned char ch);
#else
typedef F_PASCAL_PTR( Boolean, *ASI_AltKeyProc ) (unsigned char ch);
#endif

EXTERN F_ASIBND( void ) ASI_PrepareModalWindow(WindowPeek wp, HWND *hPrevKeyHookWnd, WindowPtr *prevModalWindow,
												SWORD *oldCapture, ASI_AltKeyProc theProc);
EXTERN F_ASIBND( void ) ASI_CloseModalWindow(WindowPeek wp, HWND hPrevKeyHookWnd, WindowPtr prevModalWindow,
												SWORD oldCapture);

EXTERN F_ASIBND( VOID ) ASI_MaintainTEFocus( TEHandle hTE );
EXTERN F_ASIBND( VOID ) ASI_SetDlgTextFont( DialogPeek dPtr, short itemNo );
EXTERN F_ASIBND( VOID ) ASI_AddDitlItems( DialogPeek dPtr, short OrigCnt );
EXTERN F_ASIBND( VOID ) ASI_DisposeDlgItem( DialogPeek dPtr, short itemNo );
EXTERN F_ASIBND( VOID ) ASI_BuildComboBox( DialogPeek dPtr, SWORD dItemNo,
				 MenuHandle hMenu, SWORD mDefItemNo );
EXTERN F_ASIBND( VOID ) ASI_ResetDisplayedComboBoxItems(HWND hComboBox);  // Added by CDN @ Graphsoft.
EXTERN F_ASIBND( SWORD )        ASI_GetComboBoxSelect( DialogPeek dPtr, 
							SWORD dItemNo );
EXTERN F_ASIBND( BOOLEAN )      ASI_SetComboBoxSelect( DialogPeek dPtr, 
					SWORD dItemNo, SWORD cItemNo );
EXTERN F_ASIBND( HWND ) ASI_SetComboBoxString( DialogPeek dPtr, SWORD dItemNo,
				 SWORD mItemNo, ConstStr255Param newText);
EXTERN F_ASIBND( VOID ) ASI_ScaleDlgItemRect( BOOLEAN ScaleUp, Rect *pR );
EXTERN F_ASIBND( VOID ) ASI_OffsetDlgItemRect( BOOLEAN ScaleUp, Rect *pBounds );
EXTERN F_ASIBND( VOID ) ASI_ScaleDlgWindow( BOOLEAN ScaleUp, Rect *pBounds );
EXTERN F_ASIBND( VOID ) ASI_ScaleDlgPoint( BOOLEAN ScaleUp, Point *pPt );
EXTERN F_ASIBND( HWND ) ASI_EnableWindows( WindowPeek wPtr, BOOLEAN Enable, 
			BOOLEAN IncludeThisWin );
EXTERN F_ASIBND( HWND ) ASI_EnableWindowsEx( WindowPeek wPtr, BOOLEAN Enable, 
			BOOLEAN IncludeThisWin, BOOLEAN IncludeMainWin );
EXTERN F_ASIBND( VOID ) ASI_UpdateParamStr(DialogPtr dPtr, short paramNum);
EXTERN F_ASIBND( VOID ) ASI_SetBigIText( Handle item, Handle stringH, short stringLen );
EXTERN F_ASIBND( BOOLEAN ) ASI_IsComboBox( DialogPeek dPtr, SWORD dItemNo );
EXTERN F_ASIBND( VOID ) ASI_SetDefaultButtonAndFocus( DialogPeek dPtr, 
					SWORD item, BOOLEAN permanent );
EXTERN F_ASIBND( int ) ASI_ActivateIME( HWND hWnd, Boolean visible_flag );
EXTERN F_ASIBND( SWORD ) ASI_GetComboBoxSelectAsControl(
					ControlHandle theControl );
EXTERN 	F_ASIBND( BOOLEAN ) ASI_SetComboBoxSelectAsControl(
				ControlHandle theControl, SWORD cItemNo );
EXTERN	F_ASIBND ( VOID ) ASI_EnableControl ( WindowPeek wPtr, SWORD dItemNo,
					BOOLEAN bEnable );
EXTERN	F_ASIBND ( BOOLEAN ) ASI_IsControlEnabled ( WindowPeek wPtr,
					SWORD dItemNo );
EXTERN	F_ASIBND( BOOLEAN ) ASI_DialogWhiteBackground( HWND hWnd );
EXTERN	F_ASIBND( VOID ) ASI_SetWindowHasWhiteBkGnd ( WindowPeek wPtr );
EXTERN	F_ASIBND( BOOLEAN ) ASI_WindowIsDialogKind( HWND hWnd );
EXTERN	F_PASCAL( void ) ASI_SetPasswordItem( Handle item );
EXTERN	F_ASIBND( HWND ) ASI_GetDlgItemHWND( DialogPtr dPtr, short itemNo );
EXTERN	F_ASIBND( VOID ) ASI_UpdateParamStrItem( DialogPtr dPtr, short item, Str255 newString );
EXTERN	F_ASIBND( BOOLEAN ) ASI_GetComboBoxText( DialogPeek dPtr, SWORD dItemNo, Str255 theString );
EXTERN	F_ASIBND( VOID ) ASI_SetDlgInitialTextSize( short theInitialTextSize );

//MEMORYM.c
EXTERN F_ASIBND( UWORD ) ASI_GetFreeSystemResources( UWORD fuSysResource );
EXTERN F_ASIBND( VOID )	 ASI_SetMemoryLimits( Boolean bEnabled, 
							LONG lMaxMemoryUse, LONG lMinTrackSize, SWORD sMaxSelectors );
	F_ASIBND( VOID )
ASI_ComputeMemorySize( DWORD * virtualTotal, DWORD * virtualFree,
						DWORD * physicalTotal, DWORD * physicalFree,
						DWORD * allocated );
EXTERN F_ASIBND( BOOLEAN )	ASI_MemCheck( void );

#if Compile4DLL

EXTERN VOID ASI_SetMemLimits (long heapSize, SWORD nbHeaps);
EXTERN VOID ASI_ATHCA (BOOLEAN newState);
EXTERN BOOLEAN ASI_ACHTA (); // OA 23/07/01 développement bdd debug
EXTERN void ASI_CheckMacHeap (void);
EXTERN VOID ASI_DumpMacLeaks( char *filename, BOOLEAN inIncremental);
EXTERN VOID ASI_DumpStack( char *filename, const char *format, ... );
EXTERN void ACI_XFormXY (SWORD* pX,SWORD* pY); // ev 12/01/99 
EXTERN void ACI_XFormQDRect (Rect* pRect); // ev 12/01/99 
EXTERN void ACI_XFormRECT (void* pRect); // ev 14/02/99  pRect is RECT
EXTERN void ACI_GetCurrentXFORM(void* pXForm); // ev 12/01/99 pXForm is XFORM
EXTERN TEHandle ACI_TENew(const Rect *destRect, const Rect *viewRect, Boolean inWithScrollbar, ControlRef hdlScrollBar);
//[MI] le 27/11/2000
EXTERN F_PASCAL(BOOL) ACI_GetLinesBreak(const Rect* editRect,const unsigned char * text,int len, int justification,int * nbLines/*Sortie*/, int * lineStarts/*Sortie*/);
//fin [MI]

//[MI] le 02/03/2001
EXTERN F_PASCAL(BOOL) ACI_IsArabic(void);
//EXTERN F_PASCAL( void ) ACI_ArabicDrawTextEx(const void *text, long length, const Rect *box, short just,short type);
//fin [MI]

//[MI] le 13/03/2001
EXTERN unsigned long ArabicGetCharacterPlacement(HDC hDC, const char * cur_lpWinTextChars, int byteCount, const Rect *r, const int *lpDx, void * lpResult);
//fin[MI]

//[MI] le 05/02/2001
EXTERN F_PASCAL(BOOL) ACI_WindowTextEditMgrIsEnabled();
//fin [MI]

//[MI] le 26/02/2001
EXTERN void ACI_AnalyseArabicString( const char  * ArabicStr/*E*/, int byteCount/*E*/, char * TabClass /*S*/ );
//fin [MI]

//[MI] le 01/12/2000
EXTERN F_PASCAL(BOOL) ACI_GetMaxVisibleLine(const Rect* editRect,const unsigned char * text,int len, int justification,int * visibleLenght/*Sortie*/,int * withVisibleLength/*Sortie*/, HANDLE * hVisibleText/*Sortie*/,BOOL *bLinkBefore/*Sortie*/,BOOL *bLinkAfter/*Sortie*/);
//[MI]

EXTERN Boolean ACI_MacRefNumToAnsiFullPath(short pMacRefNum,char* pDestPath,long pPathMaxLength); // ev 10/02/99
EXTERN void ACI_GetModuleVersion(const char *filename,unsigned short *bcdvers,unsigned short *build); // fgr 180399
EXTERN Boolean ACI_CreateResFile_AFPStream( const StringPtr inMacFileName, OSType inType, OSType inCreator, const char *inResSuffix); // L.E. 20/01/00 
#if USE_NEW_QUICKTIME
EXTERN void ACI_SetQuickTimePlayer(void* pProc);
#endif
#ifdef _INC_WINDOWS
EXTERN F_ASIBND( BOOLEAN ) ACI_SysCommand( HWND hWnd, WPARAM wParam, LPARAM lParam, BOOL isMDIChild ); // FMi, le 3.3.99
#endif
void ACI_HFSUseMacCharSet(); // FMi, le 12 mar 99
void ACI_ConvertToHFSCharSet( BYTE const* src, BYTE* tgt ); // FMi, le 12 mar 99
void ACI_ConvertFromHFSCharSet( BYTE const* src, BYTE* tgt ); // FM 17/11/99

EXTERN F_ASIBND( MenuHandle ) ACI_GetNthMenu( short inIndex);
EXTERN F_ASIBND( MenuHandle ) ACI_GetNthHierMenu( short inIndex);

EXTERN F_ASIBND( BOOLEAN ) IsXP(void); // CV 13/12/2001
EXTERN F_ASIBND( BOOLEAN ) IsWindowsVista(void); // m.c
#endif

//COLOR.c
EXTERN F_ASIBND( void )	ASI_SetPixBM( PixMapHandle, HBITMAP );
EXTERN F_ASIBND( void )	ASI_ForcePalette( VOID );
EXTERN F_ASIBND( DWORD ) ASI_GetPixMapSize( PixMapHandle hPix );
EXTERN F_ASIBND( void ) ASI_GetWindowsPalette( WindowPtr theWindow, 
					long* hPal );	/* DLB 941111 */
EXTERN F_ASIBND( VOID ) ASI_GetAppWinRect( Rect *AppRect );

//ZCOLOR.c
F_ASIBND( VOID ) ASI_SetPEFlags( PaletteHandle hPal, BYTE peFlags,
								 short FirstEntry, short Count );

//ASIUTILC.c
EXTERN F_ASIBND( VOID ) ASI_BeginPaletteAnimation( VOID );
EXTERN F_ASIBND( VOID ) ASI_EndPaletteAnimation( VOID );
EXTERN F_ASIBND( LONG ) ASI_GetByteSwapLONG( BYTE *lpb );
EXTERN F_ASIBND( UWORD )        ASI_GetByteSwapWORD( BYTE *lpb );
EXTERN F_ASIBND( VOID ) ASI_GetByteSwapRect( BYTE *lpb, Rect *pr );

//ASIWPROC.c
EXTERN F_ASIBND( VOID )	ASI_ModalDialog( ModalFilterProcPtr FilterProc,
				short *pItemHit );

//DLGPROCS.C
typedef BOOLEAN (CALLBACK *ButtonFocusHookProcPtr)( HWND hWnd, SWORD CtrlID);
EXTERN F_ASIBND( VOID ) ASI_SetButtonFocusHookProc( ButtonFocusHookProcPtr HookProc );

//SEGMENTL.C
EXTERN F_ASIBND( void )	ASI_ParseCommandLine( LPSTR lpCmdLine );

// ASI_IME.C
EXTERN 	F_ASIBND( WORD ) ASI_IMESetWindowPos( HWND hWnd, short left, short top, BOOLEAN bMacXY );
EXTERN 	F_ASIBND( LONG ) ASI_IMEGetConversionMode( HWND hWnd );
EXTERN 	F_ASIBND( UWORD ) ASI_IMEGetStringSize( HWND hWnd );
EXTERN 	F_ASIBND( BOOLEAN ) ASI_IMEFlushInput( HWND hWnd  );
EXTERN 	F_ASIBND( BOOLEAN ) ASI_IMEDetermineString( HWND hWnd  );
EXTERN 	F_ASIBND( VOID ) ASI_IMESetConversionFont( HWND hWnd, short font, short size, Style style );
EXTERN 	F_ASIBND( LONG ) ASI_IMESetConversionMode( HWND hWnd, LONG wIMEMode );
EXTERN 	F_ASIBND( WORD ) ASI_IMESetWindowBounds( HWND hWnd, Point Carret, Point TopLeft, Point BotRight );
EXTERN 	F_ASIBND( WORD ) ASI_IMESetToDefault( HWND hWnd );
EXTERN 	F_ASIBND( int ) ASI_IMEActivate( HWND hWnd, Boolean visible_flag );
EXTERN	F_ASIBND( BOOLEAN ) ASI_IMEGetOpen( HWND hWnd );

EXTERN F_ASIBND( VOID ) ASI_SetBtnSysPalMode(LONG mode);
EXTERN F_ASIBND( VOID ) ASI_DisablePaletteColors(short disable);
#if DELTAGRAPH
EXTERN F_ASIBND( VOID ) ASI_SetDeltagraphMakingWMF(short enable);
#endif
#if WEBANIMATOR
EXTERN F_PASCAL( void ) ASI_ClearGlobalScrollWin( void );
#endif

#ifdef _INC_WINDOWS
//EXTERN F_ASIBND( VOID ) ASI_ConvRtoR( const Rect *pr1, RECT *pr2 );
EXTERN F_ASIBND( VOID ) ASI_ConvMRtoWR( const Rect *pr1, RECT *pr2 );
EXTERN F_ASIBND( VOID ) ASI_ConvWRtoMR( const RECT *pR1, Rect *pr2 );
#endif

// OSUTIL.C
EXTERN F_ASIBND( BOOLEAN ) ASI_TaskBarIconAddRemove( BOOLEAN AddIcon, 
			UINT AppNotifierID, LPSTR szAppIconName, LPSTR szToolTip); 


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
#endif  // __cplusplus

#if MACVER
#define ASI_IsRectRgn( rgn ) ((**rgn).rgnSize == 10)
#define ASI_SetControlVis( ch, vis ) ((**ch).contrlVis = vis)
#define ASI_ClipRectChange( pPort ) /**/
#define ASI_ClipRgnChange( pPort ) /**/
#define ASI_VisRgnChange(pPort)
#else
#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus
	
#ifdef	_Quickdraw_
F_ASIBND( BOOLEAN )	ASI_IsRectRgn(RgnHandle rgn);
F_ASIBND( VOID )	ASI_ClipRectChange(GrafPtr pPort);
F_ASIBND( VOID )	ASI_ClipRgnChange(GrafPtr pPort);
F_ASIBND( VOID )	ASI_VisRgnChange(GrafPtr pPort);
#endif

EXTERN F_ASIBND( VOID) ASI_SetWindowRTOL(WindowPtr inWindow, BOOLEAN inRToL);

#if !defined(_INC_WINDOWS)
#undef LPBITMAPINFO
#endif

#undef	wPixMapPtr
#undef	wRgnHandle

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// MACVER

#endif	// #ifndef __ASIEXTRN__
