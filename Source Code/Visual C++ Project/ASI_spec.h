/*
*	ASI_SPEC.H for ASIPORT.DLL
*	Copyright (c) 1991 Altura Software, Inc. All Rights Reserved.
*	Created by:	LJL	12/16/91
*	Modified by:	LJL	12/16/91
*
*	mod 07/09/96 RSE - added conditional compile for SHANA
*	mod 8 10 95 tsh - add Fractal Design Conditionals
*	mod 6 22 95 tsh - disable some MSC warnings that are annoying
*		rather than helpful
*/

#ifndef H_NO_MACHEADERS
#if WINVER
#define H_MacTypes 1
#define H_QuickDr 1
#define H_MenuMgr 1
#define H_MemoryMgr 1
#define H_WindowMgr 1
#define H_ControlMgr 1
#define H_TextEdit 1
#define H_EventMgr 1
#define H_DialogMgr 1
#define H_ListMgr 1
#define H_ResourceMgr 1
#define H_FileMgr 1
#define H_HFS 1
#define H_ToolboxUtil 1

#define H_SegmentLdr 1
#define H_DeviceMgr 1
#define H_IntlPkg 1
#define H_OSUtil 1
#define H_ScrapMgr 1

#if 0
#define H_PackageMgr 1
#define H_StdFilePkg 1
#endif

#endif
#endif

#if _MSC_VER
#pragma warning( disable : 4049 4051 4101 4102 4129 4135 4136 4309 4341 4746 4761 )
#endif

	// Do not include LowMem.H, since we should never reference these
	//	globals directly
#define __LOWMEM__

			/* Macro for absolute value			*/
			/*   x may be any numeric type		*/
#ifndef Abs
#define	Abs(x)		((x) < 0 ? -(x) : (x))
#endif

			/* Macros for maximum and minimum	*/
		/*   of two numbers					*/
#ifndef Max
#define	Max(x, y)	(((x) > (y)) ? (x) : (y))
#endif
#ifndef Min
#define	Min(x, y)	(((x) < (y)) ? (x) : (y))
#endif
#ifndef max
#define	max(x, y)	(((x) > (y)) ? (x) : (y))
#endif
#ifndef min
#define	min(x, y)	(((x) < (y)) ? (x) : (y))
#endif

#if 0

/* for new MS version 1.1 NT C++ compiler		*/
#ifdef WIN32
/* export routines if NT case */
#undef F_PASCAL
#undef F_ASIBND
#define F_PASCAL( thetype ) __declspec( dllexport ) thetype __stdcall
#define F_ASIBND( thetype ) __declspec( dllexport ) thetype __stdcall
#pragma warning( disable: 4005 4069 4135 4761 )
#endif

#endif

#ifdef __BORLANDC__
/* export routines if Borland C case */
#undef F_PASCAL
#undef F_ASIBND
#ifdef WIN32
#define F_PASCAL( thetype )  thetype __stdcall __export
#define F_ASIBND( thetype )  thetype __stdcall __export
#else
#define F_PASCAL( thetype )  thetype _far _export _pascal
#define F_ASIBND( thetype )  thetype _far _export _pascal
#endif // WIN32
#endif

// -----------------------------------------------------------
// More Menu Items dialog data
//

#define IDD_MORE_MENU_ITEMS	4100

// dialog items

#define	IDDS_MMI_LISTBOX	4101


// HPS should be 0 normally, make sure it is set that way except for their
// builds
#define HPS 0

// DRR 090794 Added this to preserve some changes made specifically for
//   GraphSoft. Some of these changes may no longer be needed, or should
//   possibly be fixed another way. Talk to Jim Gittleson about their
//   behavior using the unmodified DLL. (The changes were made by them when
//   they had our source code.)
// LJL 021996 changed to not always build with this set to 1
#ifndef GRAPHSOFT
	#define	GRAPHSOFT	0
#endif
// RSE 4/25/97 Made the GRAPHSOFT defines GRAPHSOFT_COMBO in dialogm and 
//   controlm so that code would be part of the normal build because it better
//   supports comboboxes
#define GRAPHSOFT_COMBO 1

// RSE 070996 Added this to preserve some changes made specifically for
//   Shana. Some of these changes may no longer be needed, or should
//   possibly be fixed another way. Talk to Kent McPhee about their
//   behavior using the unmodified DLL.
#ifndef SHANA
	#define	SHANA	0
#endif

// RHK 02/19/96 for ConcertWare from Jump! Software
#define CONCERTWARE 0

// LJL GROUP TECHNOLOGY
#define GROUP_TECH 0

// BP 01/30/95 Need the same thing for DeltaGraph for Dialog handling
#define DELTAGRAPH 0
#define WEBANIMATOR 0

// EJK 05/23/95 Need the same thing for Mangia
#define MANGIA 0

// LJL 08/31/95 use for ACI
// test if already defined so that we can control this from the makefile
#ifndef ACI_4D
#define ACI_4D 0
#endif
#ifndef ACI_OM
#define ACI_OM 0
#endif

// Fractal Design Options (enable by -DFRACTAL)
// Use timer for TickCount and mouse spooler to VblTask
#define FRACTAL_TIMER_CODE	0 || defined(FRACTAL)
// we use fmemory.lib to get _fmemcpy and _fmemmove that work for hugeptr's
#define Fast_fmemcpy		0 || defined(FRACTAL)
// FDC memory sizing
#define Fractal_Design_Memory	0 || defined(FRACTAL)

#define FRACTAL_MENUCHECK_CODE	0 || defined(FRACTAL)

// String defines for font names.
#define IDS_SYS_FONT			100		/* Basic system font					*/
#define IDS_MONO_FONT		101		/* Mono font							*/
#define IDS_W30_FONT			102		/* Dialog font for Windows 3.0 	*/
#define IDS_W31_FONT			103		/* Dialog font for Windows 3.1+ 	*/
#define IDS_SMFONT_SIZE		104		/* Height for small font 			*/
#define IDS_MONOFONT_SIZE	105		/* Height for mono font 			*/ 
#define IDS_SMMONOFONT_SIZE 106		/* Height for small mono font 	*/ 
#define IDS_SMDLGFONT_SIZE 107		/* Height for small dialog font 	*/ 
#define IDS_SMFONT_WT		108		/* Weight for small font 			*/
#define IDS_MONOFONT_WT		109		/* Weight for mono font 			*/ 
#define IDS_SMMONOFONT_WT 	110		/* Weight for small mono font 	*/ 
#define IDS_SMDLGFONT_WT 	111		/* Weight for small dialog font 	*/ 

// Fonts used in dialogs in .rc files.
#if	ASI_KANJI
#define DLG_FONT		"標準ゴシック"
#define DLG_FONT_2	"標準ゴシック"
#define DLG_FONT_SM	12
#else						// Non-Japanese version
#define DLG_FONT		"Helv"
#define DLG_FONT_2	"MS Sans Serif"
#define DLG_FONT_SM	8
#endif

// Alternate days of week
#define IDS_ALT_DOW		120

// GET_WM_*() macros that used to be part of WindowsX.H
#if !defined( RC_INVOKED ) && !defined( GET_WM_ACTIVATE_STATE ) && !defined(WIN32)
//#include "get_wm.h"
#endif

#ifdef DELTAGRAPH
// Directives for record dialog
#define IDD_RECORD                      130
#define IDC_RECORD                      1003
#define IDC_STOP                        1004
#define IDC_PLAY                        1005
#define IDC_PAUSE                       1006
#define IDR_RECMENU                     137
#define IDC_SCROLLBAR                   1027
#define IDC_POS                         1033
#define IDC_LEN                         1034
#define IDC_STATIC1						1040
#define IDC_STATIC2						1041
#define IDC_STATIC3						1042
#define IDC_STATIC4						1043
#define IDC_LEVELRECT                   1035
#define IDC_FILE_SAVE					9000
#endif // DELTAGRAPH
