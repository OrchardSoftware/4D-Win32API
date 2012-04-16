/*
*	asiport.h	
*	Copyright (c) 1991,1992 Altura Software, Inc. All Rights Reserved.
*	Created by:	LJL	1/9/91
*	Modified by:	jg	1/11/94
*
*	mod 7 25 96 tsh - if WIN32 set WINVER to 0x0400
*	mod 7 17 96 tsh - fix for MSVC 4.2, fix warnings
*	mod 11 28 95 tsh - fix Borland C byte alignment pragma
*	mod 09/14/95 RSE - don't define VOID in 32 bit land, causes problems
*		with other typedefs not being performed.
*	mod 09/08/95 tci - added OST_Styl
*	mod 06/26/95 lve - add conditional on MOTOROLA for PPC NT
*	mod 06/26/95 lve - revert ASM386 to 0 when using Borland (does 
*		not work for me on 4.1) (add NO_ASM_FOR_BORLAND define)
*	mod 06/26/95 LJL - add define for struct alignment pragma
*	mod ??? - added TYPECAST_GET_UPP
*	mod 6 19 95 tsh - allow asm code under Borland C again
*	mod 5 28 95 tsh - speed up by putting multiple include tests here
*		fix Macintosh includes, fix Japan conditional, fix warning 
*		pragma's
*	mod 05 25 95 shm - change DBCS macro to Japan to follow Tom's
*		convention in Dabbler source
*	mod 05 04 95 shm - use DBCS macro for compilation of double-byte
*		version
*	mod 03/20/95 RHK - define MPW for MPW path only.  added OST_TMPL.
*       mod 07/12/94 RSE - don't do setjmp/longjmp on WIN32
* 	mod 06/22/94 gy - Changed WINVER from 0x0300 to 0x030a
*	mod 06/09/94 LLB - Added ASIApp flags:NOMAPUPPERINPUT, NOUPPERTEOUTPUT
*	mod 04/25/94 dwt  - Changed #include for lomem.h & lowmem.h
*	mod 03/07/94 RHK  - added #define HWND, HMENU, HDC & HRGN for MACVER,
*						added Drag.h
*	mod 01/11/94 jg  - add QDFLAG_SCREENINBASEADDR
*	mod 12/30/93 RSE - change defines for MENUID_ stuff for MenuMgr revamp
*	mod 12/17/93 RSE - don't include Lomem.h on Mac Version
*	mod 5 5 94 tsh - MetroWorks C
*	mod 1 21 94 tsh - assume Windows 3.1
*	mod 11 27 93 tsh - fix some Windows NT warnings
*	mod 11 23 93 tsh - fix warnings under Windows
*	mod 11 9 93 tsh - add ASM386NT, add MPW Mac path for Power PC
*	mod 11 5 93 tsh - C32 is 1 for NT, pascal => __pascal, far => __far
*	mod 11 1 93 tsh - fix for Think C 6.0 and MPW 3.3
*	mod 08/16/93 LJL - remove NO_OFFSET_SUPPORT define, was always true
*	mod 08/09/93 LJL - OST_CODE mis-spelling, GetPixe short form
*	mod 08/05/93 LJL - convert "_chsize" to "chsize" and "_getcwd" to 
*		"getcwd" to please BC++ 3.1 compiler
*	mod 07/23/93 LJL - add DEFINE_MACVER and DEFINE_WINVER as the defines
*		for #ifdef testing and always have WINVER and MACVER defined
*		and set correctly, as per Fractal's request
*	mod 07/20/93 LJL - don't define MACVER for windows version and don't
*		define WINVER for macintosh version
*	mod 07 16 93 shm - fix ASM_MOV_FS_SI
*	mod 07 12 93 shm - create MAKECOMMENT to make the ONLY* macros work 
*		and add some Intel assembly (ASM*) macros
*	mod 03/25/93 LJL - move ASI_SPEC.H lower in file
*	mod 02/12/93 RSE - add ASI_APP to used default MDI size and pos
*	mod 12 19 92 tsh - add double881 again
*	mod 12/17/92 LJL - don't have a flipped double881 type
*	mod 12 10 92 tsh - Borland C mods
*	mod 11 5 92 tsh - add 96-bit extended floating type -> double881
*	mod 10 30 92 tsh - add OpenDrive, CloseDrive
*	mod 07/29/92 LJL - add support for gl_ASIApp style bits
*	mod 01/16/92 LJL - add routines to notify on clipRgn change and
*			to check if a region is a simple rect
*	mod 01/07/92 RSE - Added OST_INTL
*	mod 12 20 91 tsh - fix H_IntlPkg for Think 5.0 on Mac
*	mod 12/17/91 LJL - add more header files and add Dispose/Dispos 
*			versions of renames
*	mod 12/16/91 LJL - move H_NO_MACHEADERS defintion to app. specific
*	mod 12/16/91 LJL - add _ASIPORT_ define so repeat includes of 
*			ASIPORT.H will be allowed
*	mod 12/16/91 LJL - create ASI_SPEC.H for painter that has the
*			OST_types that are unique to painter and
*			put it in the painter directory
*	mod 12/10/91 LJL - add menu_override structure so no strings are
*			hard coded in the ASIPORT.DLL
*	mod 12/06/91 LJL - fix H_Color to use Think C 5.0 QDOffscreen and
*			change OST_NULL from '' to 0x0L for Think C 5.0
*			compatibility
*	mod 09-04-91 TSH - Think C 5.0 uses MPW header names
*/


#ifndef	_ASIPORT_
#define _ASIPORT_ 1

// FM 21/01/2003 to avoid to define Compile4DLL everywhere
#ifndef Compile4DLL
#define Compile4DLL 1
#endif

#define PPC_HEADERS     1

// Window Flags	- DLB 101795
#define ASI_WF_MFCVIEW		0x00000001
#define ASI_WF_MFCCONTROLBAR	0x00000002
#define ASI_WF_DGPALETTE	0x00000004

#define	PROTO_BEGIN	/* */
#define	PROTO_END	/* */
#define	DOCUMENTATION	0

#ifdef WIN32
#define MACVER 0
#ifndef WINVER				/* Removing Warnings - DLB 941111 */
#define WINVER 0x0400
#endif					/* Removing Warnings - DLB 941111 */
#endif

#ifndef WINVER
// Mac, Think C case
#if defined(THINK_C) || defined(SYMANTEC_C)
#define MACVER 1
#define WINVER 0
#if THINK_C>=5
#define THINK4 0
#else
#define THINK4 1
#endif
#endif

// Mac, MPW or CodeWarrior cases
#if defined(MPW) || defined(__MWERKS__) || defined(CodeWarrior)
#define MACVER 1
#define WINVER 0
#if defined(MPW)
#undef MPW
#endif
#define MPW 1
#define THINK4 0
#endif

#ifdef DEFINE_MACVER
#define MACVER 1
#define WINVER 0
#endif

#ifdef DEFINE_WINVER
#define MACVER 0
#define WINVER 0x0400
#endif

// Windows case
#ifndef MACVER
#define MACVER 0
#define WINVER 0x0400
#define THINK4 1
#endif

#ifndef __BORLANDC__
#if WINVER
#ifndef WIN32
#pragma warning( disable:4341 )
#pragma warning( disable:4101 )
#pragma warning( disable:4209 )
#pragma warning( disable:4214 )
#else
			/* Supresses long double warning. DLB 10/04/94 */
#pragma warning( disable:4069 )
#endif
#endif
#endif

// make sure DEFINE_MACVER is defined for mac and undefined for windows

#if MACVER
#define DEFINE_MACVER 1
#undef DEFINE_WINVER
#else
#undef DEFINE_MACVER
#define DEFINE_WINVER 1
#endif
#endif

#if MACVER
#define PROCPTR_FUNC 0
#if MPW
#define OOPS 0
#define ASM68K 0
#define ASMNON 1
#else
#define OOPS 1
#define ASM68K 1
#define ASMNON 0
#endif // MPW
#define ASM386 0
#define ASM386NT 0
#define MSC_BUG 0
#define BIGEND 1
#define C32 1
#define HUGEPTR /**/
#define PTRTOMACGLOBAL(x) ((Ptr)(x))
#define ASI_DISPOSE_HANDLE_OBJECT(object)	DisposHandle((Handle)object)
#else // MACVER

#define PROCPTR_FUNC 1
#define OOPS 0
#define ASM68K 0
#define MSC_BUG 1
#define BIGEND 0
#define WM_EXITTOSHELL 0x0401
#define WM_MEMORYERROR 0x0402
#define HUGEPTR _huge
#define PTRTOMACGLOBAL(x) ((Ptr)(&x))
#define ASI_DISPOSE_HANDLE_OBJECT(object)	DisposPtr((Ptr)object)

#ifdef WIN32
#define huge
#define ASM386 0
// lve when compiling for PPC NT turn off 86 assembly
#if MOTOROLA || defined(NO_ASM_FOR_BORLAND)
#define ASM386NT 0
#else
#define ASM386NT 1
#endif
#define ASMNON 1
#define NO_OFFSET_SUPPORT 1
#define C32 1
#else // WIN32

#if defined(NO_ASM_FOR_BORLAND)
#define ASM386 0
#define ASM386NT 0
#define ASMNON 1
#define NO_OFFSET_SUPPORT 1
#define C32 0
#else
#define ASM386 1
#define ASM386NT 0
#define ASMNON 0
#define NO_OFFSET_SUPPORT 1
#define C32 0
#endif  // defined(NO_ASM_FOR_BORLAND)

#endif // WIN32

#include "CondMac.h"

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

#ifdef __BORLANDC__
// Borland C Compiler
#include "mstob.h"  // Microsoft->Borland symbol name mappings
#define PRAGMA_PACK_1 option -a1
#define PRAGMA_PACK_RESET option -a.
#define _emit db
#define ASM_PUSH_FS db 0x0f,0xa0
#define ASM_POP_FS db 0x0f,0xa1
#define ASM_PUSH_GS db 0x0f,0xa8
#define ASM_POP_GS db 0x0f,0xa9
#define ASM_SEGMENT_CS db 0x2E
#define ASM_SEGMENT_DS db 0x3E
#define ASM_SEGMENT_ES db 0x26
#define ASM_SEGMENT_SS db 0x36
#define ASM_SEGMENT_FS db 0x64
#define ASM_SEGMENT_GS db 0x65
#define ASM_MOV_FS_AX db 0x8E,0xE0
#define ASM_MOV_GS_AX db 0x8E,0xE8
#define ASM_MOV_AX_FS db 0x8C,0xE0
#define ASM_MOV_AX_GS db 0x8C,0xE8
#define ASM_MOV_FS_SI db 0x8E,0xE6
#define ASM_32OPERAND db 0x66
#define ASM_32ADDRESS db 0x67
#define	ASM_SHLD_SI_BX(cnt)	db 0x0F,0xA4,0xDE,cnt
#define	ASM_SHRD_SI_BX(cnt)	db 0x0F,0xAC,0xDE,cnt
#define	ASM_SHLD_AX_DX(cnt)	db 0x0F,0xA4,0xD0,cnt
#define	ASM_SHRD_AX_DX(cnt)	db 0x0F,0xAC,0xD0,cnt
#define ALAB(x) } x: _asm {
#define SOFFSET(typ,field) .(typ)field
#define ONLYBC                    
#define	MAKECOMMENT(a,b) a##b
#define ONLYC7 MAKECOMMENT(/,/)
#define ONLYSTDMD MAKECOMMENT(/,/)
#if 0
#define _getcwd getcwd
#define _chsize chsize
#define _chmod chmod
#define _S_IREAD S_IREAD
#define _S_IWRITE S_IWRITE
#endif //0
#else // __BORLANDC__
// Microsoft C ompiler
#define PRAGMA_PACK_1 pack(1)
#define PRAGMA_PACK_RESET pack()
#define ASM_PUSH_FS __asm _emit 0x0f __asm _emit 0xa0
#define ASM_POP_FS __asm _emit 0x0f __asm _emit 0xa1
#define ASM_PUSH_GS __asm _emit 0x0f __asm _emit 0xa8
#define ASM_POP_GS __asm _emit 0x0f __asm _emit 0xa9
#define ASM_SEGMENT_CS __asm _emit 0x2E
#define ASM_SEGMENT_DS __asm _emit 0x3E
#define ASM_SEGMENT_ES __asm _emit 0x26
#define ASM_SEGMENT_SS __asm _emit 0x36
#define ASM_SEGMENT_FS __asm _emit 0x64
#define ASM_SEGMENT_GS __asm _emit 0x65
#define ASM_MOV_FS_AX __asm _emit 0x8E __asm _emit 0xE0
#define ASM_MOV_GS_AX __asm _emit 0x8E __asm _emit 0xE8
#define ASM_MOV_FS_SI __asm _emit 0x8E __asm _emit 0xE6
#define ASM_MOV_AX_FS __asm _emit 0x8C __asm _emit 0xE0
#define ASM_MOV_AX_GS __asm _emit 0x8C __asm _emit 0xE8
#define ASM_32OPERAND __asm _emit 0x66
#define ASM_32ADDRESS __asm _emit 0x67
#define	ASM_SHLD_SI_BX(cnt)	__asm _emit 0x0F __asm _emit 0xA4 __asm _emit 0xDE __asm _emit cnt 
#define	ASM_SHRD_SI_BX(cnt)	__asm _emit 0x0F __asm _emit 0xAC __asm _emit 0xDE __asm _emit cnt
#define	ASM_SHLD_AX_DX(cnt)	__asm _emit 0x0F __asm _emit 0xA4 __asm _emit 0xD0 __asm _emit cnt
#define	ASM_SHRD_AX_DX(cnt)	__asm _emit 0x0F __asm _emit 0xAC __asm _emit 0xD0 __asm _emit cnt
#define ALAB(x) x:
#define SOFFSET(typ,field) typ.field                 
#define	MAKECOMMENT(a,b) a##b
#define ONLYBC MAKECOMMENT(/,/)
#define ONLYC7
#define ONLYSTDMD
#endif

#endif //MACVER

#define ENUMTYPE int

#if WINVER
#define	WINPASCAL	pascal
#else
#define	WINPASCAL	/* */
#endif
#if MACVER
#define SysTime Time
#endif

#if WINVER
#define WindowRef		WindowPtr
#define DialogRef		DialogPtr
#define ControlRef	ControlHandle
#endif

#if PPC_HEADERS
#define dangerousPattern 0
#else
#define dangerousPattern 1
#endif
					/* bits for gl_ASIApp style	*/
					/*   UWORD			*/

#define ASIAPP_MDI 0x0001L		/* create as MDI application	*/
#define ASIAPP_NOMENUOVER 0x0002L	/* don't do menu fixups		*/
#define ASIAPP_MEMDEBUG 0x0004L		/* turn memory debug helps on	*/
#define ASIAPP_MEMSUBALLOC 0x0008L	/* turn memory sub-allocation on*/
#define ASIAPP_ISGRAYSCALE 0x0010L	/* do gray scale copy bits	*/
#define ASIAPP_SCALEPRINT 0x0020L	/* scale printing from 72 dpi	*/
#define ASIAPP_SCALEDIALOG 0x0040L	/* scale dialogs for hires screen*/
#define ASIAPP_NUM2FUNCKEY 0x0080L	/* force Ctrl+# to Ctrl+F#	*/
#define ASIAPP_LOOSECLIP 0x0100L	/* loosely clip dialog items	*/
					/* FixDS <app> so MakeProcInst	*/
					/*   not needed, use with 	*/
					/*   Mac2Win.Lib		*/
#define ASIAPP_NOMAKEPROCINSTANCE 0x0200L 
					/* send right mouse button as 	*/
					/*   mouseDown			*/
#define ASIAPP_RBUTTONASMOUSEDOWN 0x0400L
					/* right mouse button control+	*/
					/*   mouseDown			*/
#define ASIAPP_RBUTTONASCONTROLMD 0x0800L
					/* center COMMDLG dialogs on	*/
					/*   the whole screen		*/
#define ASIAPP_CENTERCOMMONDIALOGS 0x1000L
					/* Create MDI Windows with	*/
					/*   default size and position	*/
#define ASIAPP_MDIDEFAULTSIZEANDPOS 0x2000L
					/* check removable drives in	*/
					/*   GetVInfo(), will cause	*/
					/*   error alert on empty drive	*/
#define ASIAPP_CHECKFLOPPY 0x4000L
					/* byteswap routine wants to be	*/
					/*   called with the resource	*/
					/*   ID				*/
#define ASIAPP_BYTESWAPWITHID 0x8000L
					/* turn on/off saving of DrDirID*/
					/*   for directories in PBGetCatInfo*/
					/*   and PBGetFInfo		*/
#define ASIAPP_MAKEDRDIRID 0x00010000L
					/* diable DebugStr() alerts	*/
#define	ASIAPP_NODEBUGSTR 0x00020000L
					/* disable menubar, caption	*/
					/*   adjustment on Main GDevice	*/
#define	ASIAPP_NOSHRINKGDEVICE 0x00040000L
					// enable Windows click behavior
					//  i.e. click brings window to front
					//  and is processed by the control
#define	ASIAPP_WINDOWSCLICKBEHAVIOR 0x00080000L

// when this is set three D look will be enabled
// if possible : Win95 or Win31/NT with CTL3D/CTL3D32
#define	ASIAPP_WANTS3DLOOK 0x00100000L

// L.E. 12/11/03 leaks checking is always off for final versions and on by default for debug versions.
// this flag let you turn it off for debug versions. 
#define	ASIAPP_CHECK_LEAKS 0x00200000L
					/* bits for qdflags in routine	*/
					/*   ASI_SetASIFlags		*/
#define QDFLAG_NOFASTLINES 0x0001
#define QDFLAG_WIREFRAME 0x0002
#define QDFLAG_NOOFFSCREENPORTPIX 0x0004
#define QDFLAG_TINYOFFSCREENPORTPIX 0x0008
#define QDFLAG_SUPPORTPOSTSCRIPTPICT 0x0010
#define QDFLAG_ENABLEPRINTBANDING 0x0020
					/* perform the banded printing	*/
					/*   inside of CopyBits for	*/
					/*   performance reasons	*/
#define QDFLAG_BANDEDCOPYBITS 0x0040
	  				/* check mask in PlotCIcon 	*/
					/*   (i.e., test for CopyMask()	*/
					/*   if your icons have no masks*/
					/*   (i.e., all 0xFF) then use	*/
					/*   to disable turn off check	*/
#define QDFLAG_CHECKMASKINPLOTCICON 0x0080

					/* during SetPortPix/SetPortBits*/
					/*   initialize the windows hBM	*/
					/*   based on the Mac PixMap/	*/
					/*   BitMap's baseAddr memory	*/
#define QDFLAG_INITBITMAPINSETPORTPIX 0x0100

					/* when set, CopyBits will	*/
					/*   update baseAddr memory when*/
					/*   copying from screen to	*/
					/*   PixMap, as well as storing	*/
					/*   bitmap handle		*/
#define	QDFLAG_SCREENINBASEADDR 0x0200
#define QDFLAG_NO8BITMAPS 0x0400

#define QDFLAG_NO16BITMAPS 0x1000
#define QDFLAG_MONALISA 0x2000
					/* don't use the default 236	*/
					/*   dither palette like Painter*/
					/*   and other major app's but	*/
					/*   instead use full mac emul.	*/
					/*   of SetEntries() on all	*/
					/*   256 colors			*/
#define QDFLAG_USESETENTRIES 0x4000

					/* set to allow use of DIB.DRV	*/
					/*   for offscreen pixmap drawing*/
#define QDFLAG_USEDIBDRIVER 0x8000

					// set to allow TEs to have password
					//   mode so that typing becomes "***"
					//   this is stored in clikStuff
#define TECS_FORPASSWORD 0x8000
					// used internally to indicate fonts
					//   have been recently set
#define TECS_FONTSSET 0x0001 
					// used internally to indicate we
					//   should call ASI_ExtDrawText
#define TECS_USEEXTTEXTOUT 0x0002 

					// used internally to indicate we
					//   should call ASI_ExtDrawText
					//   with bUseCmd == 2
#define TECS_NOPRINTERWIDTHS 0x0004 

					// used to indicate we should	
					//   call application-defined
					//   DrawText, TextWidth, InvertRec
					//   calls
#define TECS_USECUSTOMHOOKS 0x0008

			// between line displays at end of previous line
			// see Tech Note 127
#define TECS_FOLLOWSBETWEEN 0x1000


#ifndef H_NO_RENAME

#if WINVER
#include "DoName.H"
#endif // WINVER
#endif

#define OST(c1,c2,c3,c4) ((DWORD)c1<<24|(DWORD)c2<<16|(DWORD)c3<<8|(DWORD)c4)

#define QUADCONST       OST
#define ELLIPSIS_CHAR 0xC9
#define OST_NULL 0x0L

	// 12/06/93 Some of the OST_ constants have been changed to make
	//	them consistent with the MacToWinTool. Turn on the #define's
	//	below for older applications that use the previous #define's.
#if 1
#define	OST_QUES		OST_4Ques
#define OST_DASHDASHDASHDASH    OST_4Dash
#define	OST_ICNNumber		OST_ICNList
#define	OST_PATNumber		OST_PATList
#define	OST_STRNumber		OST_STRList
#define	OST_icmNumber		OST_icmList
#define	OST_icsNumber		OST_icsList
#define	OST_scrNumber		OST_scrList

#define	OST_ICNLIST			OST_ICNList
#define	OST_PATLIST			OST_PATList
#define	OST_STRLIST			OST_STRList
#define	OST_icmLIST			OST_icmList
#define	OST_icsLIST			OST_icsList
#define	OST_scrLIST			OST_scrList
#define	OST_popBANG		OST_popBang

#define	OST_agcSPC		OST_agcSpc
#define	OST_crmSPC		OST_crmSpc
#define	OST_errSPC		OST_errSpc
#define	OST_fpuSPC		OST_fpuSpc
#define	OST_fsSPCSPC		OST_fsSpcSpc
#define	OST_fssSPC		OST_fssSpc
#define	OST_kbdSPC		OST_kbdSpc
#define	OST_mmuSPC		OST_mmuSpc
#define	OST_osSPCSPC		OST_osSpcSpc
#define	OST_ppcSPC		OST_ppcSpc
#define	OST_psnSPC		OST_psnSpc
#define	OST_qdSPCSPC		OST_qdSpcSpc
#define	OST_ramSPC		OST_ramSpc
#define	OST_romSPC		OST_romSpc
#define	OST_serSPC		OST_serSpc
#define	OST_sndSPC		OST_sndSpc
#define	OST_teSPCSPC		OST_teSpcSpc
#define	OST_vmSPCSPC		OST_vmSpcSpc
#define	OST_agcSPC		OST_agcSpc
#define	OST_crmSPC		OST_crmSpc
#define	OST_errSPC		OST_errSpc
#define	OST_fpuSPC		OST_fpuSpc
#define	OST_fsSPCSPC		OST_fsSpcSpc
#define	OST_fssSPC		OST_fssSpc
#define	OST_kbdSPC		OST_kbdSpc
#define	OST_mmuSPC		OST_mmuSpc
#define	OST_osSPCSPC		OST_osSpcSpc
#define	OST_ppcSPC		OST_ppcSpc
#define	OST_psnSPC		OST_psnSpc
#define	OST_qdSPCSPC		OST_qdSpcSpc
#define	OST_ramSPC		OST_ramSpc
#define	OST_romSPC		OST_romSpc
#define	OST_serSPC		OST_serSpc
#define	OST_sndSPC		OST_sndSpc
#define	OST_teSPCSPC		OST_teSpcSpc
#define	OST_vmSPCSPC		OST_vmSpcSpc
#endif

#if MACVER || WIN32
#define	OST_CopyRight	'(c) '
#define OST_4Ques	'\?\?\?\?'
#define	OST_4Star	'****'
#define	OST_4Dash	'----'
#define	OST_ACE2	'ACE2'
#define	OST_ACE8	'ACE8'
#define	OST_AESD	'AESD'
#define	OST_AIFC	'AIFC'
#define	OST_AIFF	'AIFF'
#define	OST_ALRT	'ALRT'
#define	OST_ANNO	'ANNO'
#define	OST_APPL	'APPL'
#define	OST_ASID	'ASID'
#define	OST_AUTH	'AUTH'
#define	OST_BNDL	'BNDL'
#define	OST_CDEF	'CDEF'
#define	OST_CMNU	'CMNU'
#define	OST_Cmps	'Cmps'
#define	OST_CNTL	'CNTL'
#define	OST_CODE	'CODE'
#define	OST_COMM	'COMM'
#define	OST_COMT	'COMT'
#define	OST_CURS	'CURS'
#define	OST_DITL	'DITL'
#define	OST_DLOG	'DLOG'
#define	OST_DRVR	'DRVR'
#define	OST_FOND	'FOND'
#define	OST_FONT	'FONT'
#define	OST_FORM	'FORM'
#define	OST_FREF	'FREF'
#define	OST_FVER	'FVER'
#define	OST_ICON	'ICON'
#define	OST_ICNList	'ICN#'
#define	OST_INIT	'INIT'
#define	OST_INST	'INST'
#define	OST_INTL	'INTL'
#define	OST_LANG	'LANG'
#define	OST_LC		'LC'
#define	OST_LDEF	'LDEF'
#define	OST_MAC3	'MAC3'
#define	OST_MAC6	'MAC6'
#define	OST_MARK	'MARK'
#define	OST_MBAR	'MBAR'
#define	OST_MBDF	'MBDF'
#define	OST_MDEF	'MDEF'
#define	OST_MENU	'MENU'
#define	OST_MIDI	'MIDI'
#define	OST_MapC	'MapC'
#define	OST_NAME	'NAME'
#define	OST_NONE	'NONE'
#define	OST_PACK	'PACK'
#define	OST_PAT		'PAT '
#define	OST_PATList	'PAT#'
#define	OST_PICT	'PICT'
#define	OST_QDpt	'QDpt'
#define	OST_SICN	'SICN'
#define	OST_SIZE	'SIZE'
#define	OST_SSND	'SSND'
#define	OST_STR		'STR '
#define OST_STRList	'STR#'
#define	OST_TEST	'TEST'
#define	OST_TEXT     	'TEXT'
#define	OST_TMPL	'TMPL'
#define	OST_WCTB	'wctb'
#define	OST_WDEF	'WDEF'
#define	OST_WIND	'WIND'
#define	OST_acur	'acur'
#define	OST_aux		'a/ux'
#define	OST_addr	'addr'
#define	OST_adrp	'adrp'
#define	OST_aevt	'aevt'
#define	OST_afpm	'afpm'
#define	OST_agcSpc	'agc '
#define	OST_alis	'alis'
#define	OST_amnu	'amnu'
#define	OST_ansr	'ansr'
#define	OST_appa	'appa'
#define	OST_asyn	'asyn'
#define	OST_atlk	'atlk'
#define	OST_best	'best'
#define	OST_betr	'betr'
#define	OST_bool	'bool'
#define	OST_buff	'buff'
#define	OST_cbnd	'cbnd'
#define	OST_cdef	'cdef'
#define	OST_chac	'chac'
#define	OST_chan	'chan'
#define	OST_chav	'chav'
#define	OST_cicn	'cicn'
#define	OST_clau	'clau'
#define	OST_cloc	'cloc'
#define	OST_clos	'clos'
#define	OST_clut	'clut'
#define	OST_CLUT	'CLUT'
#define	OST_cmav	'cmav'
#define	OST_cmfa	'cmfa'
#define	OST_cmhd	'cmhd'
#define	OST_cmpi	'cmpi'
#define	OST_cncl	'cncl'
#define	OST_colb	'colb'
#define	OST_comp	'comp'
#define	OST_conn	'conn'
#define	OST_cont	'cont'
#define	OST_cpnt	'cpnt'
#define	OST_cpos	'cpos'
#define	OST_crea	'crea'
#define	OST_crmSpc	'crm '
#define	OST_crsr	'crsr'
#define	OST_cscr	'cscr'
#define	OST_cset	'cset'
#define	OST_ctbv	'ctbv'
#define	OST_ctrl	'ctrl'
#define	OST_cval	'cval'
#define	OST_date	'date'
#define	OST_dbac	'dbac'
#define	OST_dbin	'dbin'
#define	OST_dcon	'dcon'
#define	OST_dctb	'dctb'
#define	OST_deci	'deci'
#define	OST_desk	'desk'
#define	OST_dict	'dict'
#define	OST_disc	'disc'
#define	OST_ditl	'ditl'
#define	OST_doub	'doub'
#define	OST_dllv	'dllv'//EV 20/09/96 pour Gestalt
#define	OST_drop	'drop'
#define	OST_eadr	'eadr'
#define	OST_easy	'easy'
#define	OST_edtn	'edtn'
#define	OST_edtp	'edtp'
#define	OST_edts	'edts'
#define	OST_edtt	'edtt'
#define	OST_edtu	'edtu'
#define	OST_empt	'empt'
#define	OST_enum	'enum'
#define	OST_errSpc	'err '
#define	OST_errn	'errn'
#define	OST_errs	'errs'
#define	OST_esrc	'esrc'
#define	OST_evcl	'evcl'
#define	OST_evid	'evid'
#define	OST_evnt	'evnt'
#define	OST_exte	'exte'
#define	OST_extn	'extn'
#define	OST_faam	'faam'
#define	OST_fact	'fact'
#define	OST_fadr	'fadr'
#define	OST_faet	'faet'
#define	OST_faex	'faex'
#define	OST_fals	'fals'
#define	OST_famn	'famn'
#define	OST_fapf	'fapf'
#define	OST_fapn	'fapn'
#define	OST_fash	'fash'
#define	OST_fast	'fast'
#define	OST_fasy	'fasy'
#define	OST_fbnd	'fbnd'
#define	OST_fdef	'fdef'
#define	OST_fdrp	'fdrp'
#define	OST_fixl	'fixl'
#define	OST_floc	'floc'
#define	OST_flpy	'flpy'
#define	OST_fmts	'fmts'
#define OST_fndr        'fndr'
#define	OST_fold	'fold'
#define	OST_font	'font'
#define	OST_fpuSpc	'fpu '
#define	OST_fsSpcSpc	'fs  '
#define	OST_fscr	'fscr'
#define	OST_fset	'fset'
#define	OST_fssSpc	'fss '
#define	OST_fval	'fval'
#define	OST_fxfr	'fxfr'
#define	OST_gerr	'gerr'
#define	OST_good	'good'
#define	OST_gpos	'gpos'
#define	OST_hdlg	'hdlg'
#define	OST_hdsk	'hdsk'
#define	OST_hdwr	'hdwr'
#define	OST_help	'help'
#define	OST_hfdr	'hfdr'
#define	OST_hmnu	'hmnu'
#define	OST_hovr	'hovr'
#define	OST_hrct	'hrct'
#define	OST_hrng	'hrng'
#define	OST_hwin	'hwin'
#define	OST_icl4	'icl4'
#define	OST_icl8	'icl8'
#define	OST_icmList	'icm#'
#define	OST_icm4	'icm4'
#define	OST_icm8	'icm8'
#define	OST_icon	'icon'
#define	OST_icsList	'ics#'
#define	OST_ics4	'ics4'
#define	OST_ics8	'ics8'
#define	OST_init	'init'
#define	OST_inpm	'inpm'
#define	OST_inte	'inte'
#define	OST_intl	'intl'
#define	OST_jaym	'jaym'
#define	OST_kang	'kang'
#define	OST_kbdSpc	'kbd '
#define	OST_kdat	'kdat'
#define	OST_keyw	'keyw'
#define	OST_klef	'klef'
#define	OST_ktas	'ktas'
#define	OST_ktlh	'ktlh'
#define	OST_ktps	'ktps'
#define	OST_ktxf	'ktxf'
#define	OST_lbin	'lbin'
#define	OST_lcha	'lcha'
#define	OST_leng	'leng'
#define	OST_list	'list'
#define	OST_lmac	'lmac'
#define	OST_lmem	'lmem'
#define	OST_lmet	'lmet'
#define	OST_lock	'lock'
#define	OST_long	'long'
#define	OST_lram	'lram'
#define	OST_mach	'mach'
#define	OST_macs	'macs'
#define	OST_magn	'magn'
#define	OST_micn	'micn'
#define	OST_misc	'misc'
#define	OST_miss	'miss'
#define	OST_mmuSpc	'mmu '
#define	OST_mone	'mone'
#define	OST_mvvw	'mvvw'
#define	OST_name	'name'
#define	OST_nfdr	'nfdr'
#define	OST_nlup	'nlup'
#define	OST_nmgr	'nmgr'
#define	OST_none	'none'
#define	OST_null	'null'
#define	OST_nxbd	'nxbd'
#define	OST_oapp	'oapp'
#define	OST_obit	'obit'
#define	OST_odoc	'odoc'
#define	OST_ofay	'ofay'
#define	OST_ofst	'ofst'
#define	OST_optd	'optd'
#define	OST_optk	'optk'
#define	OST_optn	'optn'
#define	OST_osSpcSpc	'os  '
#define	OST_ostt	'ostt'
#define	OST_p2st	'p2st'
#define	OST_paus	'paus'
#define	OST_pdoc	'pdoc'
#define	OST_pgsz	'pgsz'
#define	OST_phac	'phac'
#define	OST_plth	'plth'
#define	OST_pltt	'pltt'
#define	OST_pnrg	'pnrg'
#define	OST_popBang	'pop!'
#define	OST_powr	'powr'
#define	OST_ppat	'ppat'
#define	OST_ppcSpc	'ppc '
#define	OST_pref	'pref'
#define	OST_prnt	'prnt'
#define	OST_proc	'proc'
#define	OST_prop	'prop'
#define	OST_prty	'prty'
#define	OST_prvw	'prvw'
#define	OST_psnSpc	'psn '
#define	OST_qdSpcSpc	'qd  '
#define	OST_qdrw	'qdrw'
#define	OST_qual	'qual'
#define	OST_quia	'quia'
#define	OST_quit	'quit'
#define	OST_ramSpc	'ram '
#define	OST_read	'read'
#define	OST_reco	'reco'
#define	OST_refc	'refc'
#define	OST_rest	'rest'
#define	OST_rgnc	'rgnc'
#define	OST_romSpc	'rom '
#define	OST_romv	'romv'
#define	OST_rowb	'rowb'
#define	OST_rplc	'rplc'
#define	OST_rply	'rply'
#define	OST_rsrc	'rsrc'
#define	OST_rtid	'rtid'
#define	OST_rtrn	'rtrn'
#define	OST_rtyp	'rtyp'
#define	OST_sclg	'sclg'
#define	OST_scrList	'scr#'
#define	OST_scri	'scri'
#define	OST_scrl	'scrl'
#define	OST_sect	'sect'
#define	OST_selh	'selh'
#define	OST_serSpc	'ser '
#define	OST_sfnt	'sfnt'
#define	OST_shiw	'shiw'
#define	OST_shor	'shor'
#define	OST_shut	'shut'
#define	OST_sign	'sign'
#define	OST_sing	'sing'
#define	OST_sltc	'sltc'
#define	OST_sndSpc	'snd '
#define	OST_snth	'snth'
#define	OST_sour	'sour'
#define	OST_srat	'srat'
#define	OST_srav	'srav'
#define	OST_srvi	'srvi'
#define	OST_srvr	'srvr'
#define	OST_ssav	'ssav'
#define	OST_ssid	'ssid'
#define	OST_ssiz	'ssiz'
#define	OST_st2p	'st2p'
#define	OST_stat	'stat'
#define	OST_stdf	'stdf'
#define	OST_strt	'strt'
#define	OST_styl	'styl'
#define	OST_Styl	'Styl'
#define	OST_sysv	'sysv'
#define	OST_tTXT	'tTXT'
#define	OST_targ	'targ'
#define	OST_tbnd	'tbnd'
#define	OST_tbtt	'tbtt'
#define	OST_tdef	'tdef'
#define	OST_teSpcSpc	'te  '
#define	OST_temp	'temp'
#define	OST_term	'term'
#define	OST_time	'time'
#define	OST_timo	'timo'
#define	OST_tims	'tims'
#define	OST_tloc	'tloc'
#define	OST_tmgr	'tmgr'
#define	OST_tran	'tran'
#define	OST_tray	'tray'
#define	OST_trsh	'trsh'
#define	OST_true	'true'
#define	OST_tscr	'tscr'
#define	OST_tset	'tset'
#define	OST_tsmv	'tsmv'
#define	OST_tsvc	'tsvc'
#define	OST_tval	'tval'
#define	OST_twos	'twos'
#define	OST_txrn	'txrn'
#define	OST_type	'type'
#define	OST_udng	'udng'
#define	OST_unkn	'unkn'
#define	OST_updt	'updt'
#define	OST_user	'user'
#define	OST_vbin	'vbin'
#define	OST_vcha	'vcha'
#define	OST_vers	'vers'
#define	OST_vmSpcSpc	'vm  '
#define	OST_voxr	'voxr'
#define	OST_voxs	'voxs'
#define	OST_writ	'writ'
#define	OST_xttt	'xttt'
// shm for DeltaPoint
#define	OST_rawSPC	'raw '
#define	OST_rleSPC	'rle '
#define	OST_TVOD	'TVOD'
#define	OST_PVPL	'PVPL'
#define	OST_PNTG	'PNTG'
#if WEBANIMATOR || DELTAGRAPH
#define	OST_KNVS	'DPWA'
#else
#define	OST_KNVS	'KNVS'
#endif
#define	OST_STBF	'STBF'
#define	OST_APPL	'APPL'
#define	OST_KVPR	'KVPR'
#define	OST_SFXBang	'SFX!'
#define	OST_SFXB	'SFXB'
#define	OST_FSSD	'FSSD'
#define	OST_INFO	'INFO'
#define	OST_REPT	'REPT'
#define	OST_jB1SPC	'jB1 '
#define	OST_sfil	'sfil'
#define	OST_OBJF	'OBJF'
#define	OST_PICS	'PICS'
#define	OST_PALT	'PALT'
#define	OST_play	'play'
#define	OST_MooV	'MooV'
#define	OST_PVCN	'PVCN'
#define	OST_PVTP	'PVTP'
#define	OST_PVPC	'PVPC' 
#define	OST_PVPF	'PVPF'
#define	OST_PVLB	'PVLB'
#define	OST_jpeg	'jpeg'
#define	OST_qtim	'qtim'
#define	OST_soun	'soun'
#define	OST_vide	'vide'
#define	OST_TIFF 'TIFF'
#define	OST_WBMP 'WBMP'
#define	OST_GIF  'GIF '
#define	OST_GIFf 'GIFf'
#define	OST_TGA  'TGA '
#define	OST_PCX  'PCX '
#define	OST_JPEG 'JPEG'
#define	OST_EPS  'EPSF'
#define	OST_PCD	'PCDI'
#define	OST_WDIB	'DIB '
#define	OST_FAX	'FAX3'
#define	OST_PDF	'PDF '
#define	OST_PNG	'PNG '
#define	OST_HEXA 'HEXA'
#define	OST_ALL 	'*.* '
#define OST_drag	'drag'
#define OST_hfsSpc 'hfs '
#define OST_phfs 'phfs'
#define OST_diry 'diry'
#define OST_ictb 'ictb'
#define OST_cnfg 'cnfg'
#define OST_dspl 'dspl'
#define OST_4DSG	'4DSG'
#define OST_4DID	'4DID'
#else
#define	OST_CopyRight	0x28632920L
#define OST_4Ques	0x3F3F3F3FL
#define	OST_4Star	0x2A2A2A2AL
#define	OST_4Dash	0x2D2D2D2DL
#define	OST_ACE2	0x41434532L
#define	OST_ACE8	0x41434538L
#define	OST_AESD	0x41455344L
#define	OST_AIFC	0x41494643L
#define	OST_AIFF	0x41494646L
#define	OST_ALRT	0x414C5254L
#define	OST_ANNO	0x414E4E4FL
#define	OST_APPL	0x4150504CL
#define	OST_ASID	0x41534944L
#define	OST_AUTH	0x41555448L
#define	OST_BNDL	0x424E444CL
#define OST_CDEF	0x43444546L
#define	OST_CMNU	0x434D4E55L
#define	OST_Cmps	0x436D7073L
#define	OST_CNTL	0x434E544CL
#define	OST_CODE	0x434F4445L
#define	OST_COMM	0x434F4D4DL
#define	OST_COMT	0x434F4D54L
#define	OST_CURS	0x43555253L
#define	OST_DITL	0x4449544CL
#define	OST_DLOG	0x444C4F47L
#define OST_drag	0x64726167L
#define	OST_DRVR	0x44525652L
#define	OST_FOND	0x464F4E44L
#define	OST_FONT	0x464F4E54L
#define	OST_FORM	0x464F524DL
#define	OST_FREF	0x46524546L
#define	OST_FVER	0x46564552L
#define OST_ICON	0x49434F4EL
#define	OST_ICNList	0x49434E23L
#define	OST_INIT	0x494E4954L
#define	OST_INST	0x494E5354L
#define	OST_INTL	0x494E544CL
#define	OST_LANG	0x4C414E47L
#define	OST_LC		0x4C430000L
#define OST_LDEF	0x4C444546L
#define	OST_MAC3	0x4D414333L
#define	OST_MAC6	0x4D414336L
#define	OST_MARK	0x4D41524BL
#define OST_MBAR	0x4D424152L
#define	OST_MBDF	0x4D424446L
#define OST_MDEF	0x4D444546L
#define OST_MENU	0x4D454E55L
#define	OST_MIDI	0x4D494449L
#define	OST_MapC	0x4D617043L
#define	OST_NAME	0x4E414D45L
#define	OST_NONE	0x4E4F4E45L
#define OST_PACK	0x5041434BL
#define	OST_PAT		0x50415420L
#define	OST_PATList	0x50415423L
#define	OST_PICT	0x50494354L
#define	OST_QDpt	0x51447074L
#define	OST_SICN	0x5349434EL
#define	OST_SIZE	0x53495A45L
#define	OST_SSND	0x53534E44L
#define OST_STR		0x53545220L
#define OST_STRList	0x53545223L
#define	OST_TEST	0x54455354L
#define	OST_TEXT	0x54455854L
#define	OST_TMPL	0x544D504CL
#define	OST_WCTB	0x77637462L
#define	OST_WDEF	0x57444546L
#define	OST_WIND	0x57494E44L
#define	OST_acur	0x61637572L
#define	OST_aux		0x612F7578L
#define	OST_addr	0x61646472L
#define	OST_adrp	0x61647270L
#define	OST_aevt	0x61657674L
#define	OST_afpm	0x6166706DL
#define	OST_agcSpc	0x61676320L
#define	OST_alis	0x616C6973L
#define	OST_amnu	0x616D6E75L
#define	OST_ansr	0x616E7372L
#define	OST_appa	0x61707061L
#define	OST_asyn	0x6173796EL
#define	OST_atlk	0x61746C6BL
#define	OST_best	0x62657374L
#define	OST_betr	0x62657472L
#define	OST_bool	0x626F6F6CL
#define	OST_buff	0x62756666L
#define	OST_cbnd	0x63626E64L
#define	OST_cdef	0x63646566L
#define	OST_chac	0x63686163L
#define	OST_chan	0x6368616EL
#define	OST_chav	0x63686176L
#define	OST_cicn	0x6369636EL
#define	OST_clau	0x636C6175L
#define	OST_cloc	0x636C6F63L
#define	OST_clos	0x636C6F73L
#define	OST_clut	0x636C7574L
#define	OST_CLUT	0x434C5554L
#define	OST_cmav	0x636D6176L
#define	OST_cmfa	0x636D6661L
#define	OST_cmhd	0x636D6864L
#define	OST_cmpi	0x636D7069L
#define	OST_cncl	0x636E636CL
#define	OST_colb	0x636F6C62L
#define	OST_comp	0x636F6D70L
#define	OST_conn	0x636F6E6EL
#define	OST_cont	0x636F6E74L
#define	OST_cpnt	0x63706E74L
#define	OST_cpos	0x63706F73L
#define	OST_crea	0x63726561L
#define	OST_crmSpc	0x63726D20L
#define	OST_crsr	0x63727372L
#define	OST_cscr	0x63736372L
#define	OST_cset	0x63736574L
#define	OST_ctbv	0x63746276L
#define	OST_ctrl	0x6374726CL
#define	OST_cval	0x6376616CL
#define	OST_date	0x64617465L
#define	OST_dbac	0x64626163L
#define	OST_dbin	0x6462696EL
#define	OST_dcon	0x64636F6EL
#define	OST_dctb	0x64637462L
#define	OST_deci	0x64656369L
#define	OST_desk	0x6465736BL
#define	OST_dict	0x64696374L
#define	OST_disc	0x64697363L
#define	OST_ditl	0x6469746CL
#define	OST_doub	0x646F7562L
#define	OST_dllv	0x64464676L//EV 20/09/96 pour Gestalt
#define	OST_drop	0x64726F70L
#define	OST_eadr	0x65616472L
#define	OST_easy	0x65617379L
#define	OST_edtn	0x6564746EL
#define	OST_edtp	0x65647470L
#define	OST_edts	0x65647473L
#define	OST_edtt	0x65647474L
#define	OST_edtu	0x65647475L
#define	OST_empt	0x656D7074L
#define	OST_enum	0x656E756DL
#define	OST_errSpc	0x65727220L
#define	OST_errn	0x6572726EL
#define	OST_errs	0x65727273L
#define	OST_esrc	0x65737263L
#define	OST_evcl	0x6576636CL
#define	OST_evid	0x65766964L
#define	OST_evnt	0x65766E74L
#define	OST_exte	0x65787465L
#define	OST_extn	0x6578746EL
#define	OST_faam	0x6661616DL
#define	OST_fact	0x66616374L
#define	OST_fadr	0x66616472L
#define	OST_faet	0x66616574L
#define	OST_faex	0x66616578L
#define	OST_fals	0x66616C73L
#define	OST_famn	0x66616D6EL
#define	OST_fapf	0x66617066L
#define	OST_fapn	0x6661706EL
#define	OST_fash	0x66617368L
#define	OST_fast	0x66617374L
#define	OST_fasy	0x66617379L
#define	OST_fbnd	0x66626E64L
#define	OST_fdef	0x66646566L
#define	OST_fdrp	0x66647270L
#define	OST_fixl	0x6669786CL
#define	OST_floc	0x666C6F63L
#define	OST_flpy	0x666C7079L
#define	OST_fmts	0x666D7473L
#define OST_fndr        0x666E6472L
#define	OST_fold	0x666F6C64L
#define	OST_font	0x666F6E74L
#define	OST_fpuSpc	0x66707520L
#define	OST_fsSpcSpc	0x66732020L
#define	OST_fscr	0x66736372L
#define	OST_fset	0x66736574L
#define	OST_fssSpc	0x66737320L
#define	OST_fval	0x6676616CL
#define	OST_fxfr	0x66786672L
#define	OST_gerr	0x67657272L
#define	OST_good	0x676F6F64L
#define	OST_gpos	0x67706F73L
#define	OST_hdlg	0x68646C67L
#define	OST_hdsk	0x6864736BL
#define	OST_hdwr	0x68647772L
#define	OST_help	0x68656C70L
#define	OST_hfdr	0x68666472L
#define	OST_hmnu	0x686D6E75L
#define	OST_hovr	0x686F7672L
#define	OST_hrct	0x68726374L
#define	OST_hrng	0x68726E67L
#define	OST_hwin	0x6877696EL
#define	OST_icl4	0x69636C34L
#define	OST_icl8	0x69636C38L
#define	OST_icmList	0x69636D23L
#define	OST_icm4	0x69636D34L
#define	OST_icm8	0x69636D38L
#define	OST_icon	0x69636F6EL
#define	OST_icsList	0x69637323L
#define	OST_ics4	0x69637334L
#define	OST_ics8	0x69637338L
#define	OST_init	0x696E6974L
#define	OST_inpm	0x696E706DL
#define	OST_inte	0x696E7465L
#define	OST_intl	0x696E746CL
#define	OST_jaym	0x6A61796DL
#define	OST_kang	0x6B616E67L
#define	OST_kbdSpc	0x6B626420L
#define	OST_kdat	0x6B646174L
#define	OST_keyw	0x6B657977L
#define	OST_klef	0x6B6C6566L
#define	OST_ktas	0x6B746173L
#define	OST_ktlh	0x6B746C68L
#define	OST_ktps	0x6B747073L
#define	OST_ktxf	0x6B747866L
#define	OST_lbin	0x6C62696EL
#define	OST_lcha	0x6C636861L
#define	OST_leng	0x6C656E67L
#define	OST_list	0x6C697374L
#define	OST_lmac	0x6C6D6163L
#define	OST_lmem	0x6C6D656DL
#define	OST_lmet	0x6C6D6574L
#define	OST_lock	0x6C6F636BL
#define	OST_long	0x6C6F6E67L
#define	OST_lram	0x6C72616DL
#define	OST_mach	0x6D616368L
#define	OST_macs	0x6D616373L
#define	OST_magn	0x6D61676EL
#define	OST_micn	0x6D69636EL
#define	OST_misc	0x6D697363L
#define	OST_miss	0x6D697373L
#define	OST_mmuSpc	0x6D6D7520L
#define	OST_mone	0x6D6F6E65L
#define	OST_mvvw	0x6D767677L
#define	OST_name	0x6E616D65L
#define	OST_nfdr	0x6E666472L
#define	OST_nlup	0x6E6C7570L
#define	OST_nmgr	0x6E6D6772L
#define	OST_none	0x6E6F6E65L
#define	OST_null	0x6E756C6CL
#define	OST_nxbd	0x6E786264L
#define	OST_oapp	0x6F617070L
#define	OST_obit	0x6F626974L
#define	OST_odoc	0x6F646F63L
#define	OST_ofay	0x6F666179L
#define	OST_ofst	0x6F667374L
#define	OST_optd	0x6F707464L
#define	OST_optk	0x6F70746BL
#define	OST_optn	0x6F70746EL
#define	OST_osSpcSpc	0x6F732020L
#define	OST_ostt	0x6F737474L
#define	OST_p2st	0x70327374L
#define	OST_paus	0x70617573L
#define	OST_pdoc	0x70646F63L
#define	OST_pgsz	0x7067737AL
#define	OST_phac	0x70686163L
#define	OST_plth	0x706C7468L
#define	OST_pltt	0x706C7474L
#define	OST_pnrg	0x706E7267L
#define	OST_popBang	0x706F7021L
#define	OST_powr	0x706F7772L
#define	OST_ppat	0x70706174L
#define	OST_ppcSpc	0x70706320L
#define	OST_pref	0x70726566L
#define	OST_prnt	0x70726E74L
#define	OST_proc	0x70726F63L
#define	OST_prop	0x70726F70L
#define	OST_prty	0x70727479L
#define	OST_prvw	0x70727677L
#define	OST_psnSpc	0x70736E20L
#define	OST_qdSpcSpc	0x71642020L
#define	OST_qdrw	0x71647277L
#define	OST_qual	0x7175616CL
#define	OST_quia	0x71756961L
#define	OST_quit	0x71756974L
#define	OST_ramSpc	0x72616D20L
#define	OST_read	0x72656164L
#define	OST_reco	0x7265636FL
#define	OST_refc	0x72656663L
#define	OST_rest	0x72657374L
#define	OST_rgnc	0x72676E63L
#define	OST_romSpc	0x726F6D20L
#define	OST_romv	0x726F6D76L
#define	OST_rowb	0x726F7762L
#define	OST_rplc	0x72706C63L
#define	OST_rply	0x72706C79L
#define	OST_rsrc	0x72737263L
#define	OST_rtid	0x72746964L
#define	OST_rtrn	0x7274726EL
#define	OST_rtyp	0x72747970L
#define	OST_sclg	0x73636C67L
#define	OST_scrList	0x73637223L
#define	OST_scri	0x73637269L
#define	OST_scrl	0x7363726CL
#define	OST_sect	0x73656374L
#define	OST_selh	0x73656C68L
#define	OST_serSpc	0x73657220L
#define OST_sfnt	0x73666e74L
#define	OST_shiw	0x73686977L
#define	OST_shor	0x73686F72L
#define	OST_shut	0x73687574L
#define	OST_sign	0x7369676EL
#define	OST_sing	0x73696E67L
#define	OST_sltc	0x736C7463L
#define	OST_sndSpc	0x736E6420L
#define	OST_snth	0x736E7468L
#define	OST_sour	0x736F7572L
#define	OST_srat	0x73726174L
#define	OST_srav	0x73726176L
#define	OST_srvi	0x73727669L
#define	OST_srvr	0x73727672L
#define	OST_ssav	0x73736176L
#define	OST_ssid	0x73736964L
#define	OST_ssiz	0x7373697AL
#define	OST_st2p	0x73743270L
#define	OST_stat	0x73746174L
#define	OST_stdf	0x73746466L
#define	OST_strt	0x73747274L
#define	OST_styl	0x7374796CL
#define	OST_Styl	0x5374796CL
#define	OST_sysv	0x73797376L
#define	OST_tTXT	0x74545854L
#define	OST_targ	0x74617267L
#define	OST_tbnd	0x74626E64L
#define	OST_tbtt	0x74627474L
#define	OST_tdef	0x74646566L
#define	OST_teSpcSpc	0x74652020L
#define	OST_temp	0x74656D70L
#define	OST_term	0x7465726DL
#define	OST_time	0x74696D65L
#define	OST_timo	0x74696D6FL
#define	OST_tims	0x74696D73L
#define	OST_tloc	0x746C6F63L
#define	OST_tmgr	0x746D6772L
#define	OST_tran	0x7472616EL
#define	OST_tray	0x74726179L
#define	OST_trsh	0x74727368L
#define	OST_true	0x74727565L
#define	OST_tscr	0x74736372L
#define	OST_tset	0x74736574L
#define	OST_tsmv	0x74736D76L
#define	OST_tsvc	0x74737663L
#define	OST_tval	0x7476616CL
#define	OST_twos	0x74776F73L
#define	OST_txrn	0x7478726EL
#define	OST_type	0x74797065L
#define	OST_udng	0x75646E67L
#define	OST_unkn	0x756E6B6EL
#define	OST_updt	0x75706474L
#define	OST_user	0x75736572L
#define	OST_vbin	0x7662696EL
#define	OST_vcha	0x76636861L
#define	OST_vers	0x76657273L
#define	OST_vmSpcSpc	0x766D2020L
#define	OST_voxr	0x766F7872L
#define	OST_voxs	0x766F7873L
#define	OST_writ	0x77726974L
#define	OST_xttt	0x78747474L
// shm for DeltaPoint
#define	OST_rawSPC	0x72617720L
#define	OST_rleSPC	0x726C6520L
#define	OST_TVOD	0x54564F44L
#define	OST_PVPL	0x5056504CL
#define	OST_PNTG	0x504E5447L
#if WEBANIMATOR
#define	OST_KNVS	0x44505741L
#else
#define	OST_KNVS	0x4B4E5653L
#endif
#define	OST_STBF	0x53544246L
#define	OST_APPL	0x4150504CL
#define	OST_KVPR	0x4B565052L
#define	OST_SFXBang	0x53465821L
#define	OST_SFXB	0x53465842L
#define	OST_FSSD	0x46535344L
#define	OST_INFO	0x494E464FL
#define	OST_REPT	0x52455054L
#define	OST_jB1SPC	0x6A423120L
#define	OST_sfil	0x7366696CL
#define	OST_OBJF	0x4F424A46L
#define	OST_PICS	0x50494353L
#define	OST_PALT	0x50414C54L
#define	OST_play	0x706C6179L
#define	OST_MooV	0x4D6F6F56L
#define	OST_PVCN	0x5056434EL
#define	OST_PVTP	0x50565450L
#define	OST_PVPC	0x50565043L
#define	OST_PVPF	0x50565046L
#define	OST_PVLB	0x50564C42L
#define	OST_jpeg	0x6A706567L
#define	OST_qtim	0x7174696DL
#define	OST_soun	0x776F756EL
#define	OST_vide	0x76696465L
#define	OST_TIFF 0x54494646L
#define	OST_WBMP 0x57424D50L
#define	OST_GIF  0x47494620L
#define	OST_GIFf 0x47494666L
#define	OST_TGA  0x54474120L
#define	OST_PCX  0x50435820L
#define	OST_JPEG 0x4A504547L
#define	OST_EPS  0x45505346L
#define	OST_PCD	0x50434449L
#define	OST_WDIB	0x44494220L
#define	OST_FAX	0x46415833L
#define	OST_PDF	0x50444620L
#define	OST_PNG	0x504E4720L
#define OST_HEXA	0x48455841L
#define OST_ALL 	0x2A2E2A20L
#define OST_hfsSpc 0x68667320
#define OST_phfs 0x70686673
#define OST_diry 0x64697279
#define OST_ictb 0x69637462L
#define OST_cnfg 0x636E6667L
#define OST_dspl 0x6473706CL
#ifndef __cplusplus
// shm For now
#define topLeft(r)	(((Point *) &(r))[0])
#define botRight(r)	(((Point *) &(r))[1])
#endif

#endif

#if MACVER
#define S_PASCAL( thetype ) pascal static thetype
#define F_PASCAL( thetype ) pascal thetype
#define F_ASIBND( thetype ) pascal thetype
#define F_FAR( thetype ) thetype
#define F_PASCAL_PTR( thetype, function_name ) pascal thetype (function_name)
#define S_PASCAL_PTR( thetype, function_name ) static pascal thetype (function_name)
#define CONST_EXPR(expression)			const ##expression
#define CONST_TYPE(type, expression)		const ##type ##expression

#define UNDEF_ARRAY /**/
#else

#ifdef WIN32
#define pascal __stdcall
#define far
#define _huge
//	Removed because of redefinition warning in Windef.h. DLB 941014
//#define FAR
#define S_PASCAL( thetype ) static thetype __stdcall
#define F_PASCAL( thetype ) thetype __stdcall /* __export */
#define F_ASIBND( thetype ) thetype __stdcall /* __export */
#define F_FAR( thetype ) thetype
#define F_PASCAL_PTR( thetype, function_name ) thetype (__stdcall function_name)
#define S_PASCAL_PTR( thetype, function_name ) static thetype (__stdcall function_name)
#define CALLBACK            __stdcall
#define CONST_EXPR(expression)			const ##expression
#define CONST_TYPE(type, expression)	const ##type ##expression
#else
#define S_PASCAL( thetype ) static thetype far pascal
#define F_PASCAL( thetype ) thetype far pascal /* __export */
#define F_ASIBND( thetype ) thetype far pascal /* __export */
#define F_FAR( thetype ) thetype far
#define F_PASCAL_PTR( thetype, function_name ) thetype (far pascal function_name)
#define S_PASCAL_PTR( thetype, function_name ) static thetype (far pascal function_name)
#define CALLBACK            _far _pascal
#define CONST_EXPR(expression)			enum { ##expression }
#define CONST_TYPE(type, expression)		enum { ##expression }
#endif

#define UNDEF_ARRAY 1
#endif

#define STATICFN		static
#define MLOCAL			static
#define EXTERN			extern
#define GLOBAL			/**/

#ifndef WIN32
#define VOID			void
#endif

typedef int			BOOL;
typedef char			SBYTE;
typedef unsigned char		BYTE;
typedef unsigned char  		UBYTE;
#ifndef WINAPI
typedef unsigned short	    	WORD;
#endif
typedef short			SWORD;
typedef unsigned short		UWORD;
#ifndef WINAPI
typedef long			LONG;
#endif
typedef unsigned long 		DWORD;
typedef	long	 		DSWORD;
typedef unsigned long 		DUWORD;

#if MACVER
/* 96-bit 68881 extended type defined via a struct */
typedef struct
{
	unsigned short	characteristic;
	unsigned short	zeroes;
	unsigned long	mantissa[2];
} double881;
#else
/* 96-bit 68881 extended type defined via a struct */
typedef struct
{
	unsigned long	mantissa[2];
	unsigned short	zeroes;
	unsigned short	characteristic;
} double881;
#endif

#if MACVER
typedef double			DOUBLE;

#define	CEIL	ceil
#define	FLOOR	floor
#define	FMOD	fmod
#define	FABS	fabs
#define	COS	cos
#define	SIN	sin
#define	TAN	tan
#define	ACOS	acos
#define	ASIN	asin
#define	ATAN	atan
#define	ATAN2	atan2
#define	EXP	exp
#define	LOG	log
#define	LOG10	log10
#define	SQRT	sqrt
#endif

typedef F_FAR(BYTE) 		*LPSZ;
typedef F_FAR(char)		*LPSTR;
typedef F_FAR(WORD)		*LPWORD;
typedef F_FAR(SWORD)		*LPSWORD;
typedef F_FAR(UWORD)		*LPUWORD;
typedef F_FAR(LONG)		*LPLONG;
typedef F_FAR(DWORD)		*LPDWORD;
typedef F_FAR(DSWORD)		*LPDSWORD;
typedef F_FAR(DUWORD)		*LPDUWORD;

typedef unsigned int		UINT;

#ifdef WIN32
#if defined(__cplusplus)
extern "C" {
#endif
#if 1
// this doesn't work with NT and Symantec's compiler/library, so forget it
// for now
// This really does work for Borland and Visual C++, so turn back on
  #include <setjmp.h>
  typedef jmp_buf CATCHBUF;
  typedef CATCHBUF *LPCATCHBUF;
  #define Throw(jb,rc) longjmp(jb,rc)
  #define Catch(jb) setjmp(jb)
#else
  typedef long CATCHBUF[34];
  #define Throw(jb,rc) 
  #define Catch(jb) 0
#endif
#if defined(__cplusplus)
}
#endif
#endif

#ifndef WIN32
typedef unsigned int	BOOLEAN;
#endif

#if WINVER
#ifndef WIN32
#ifdef STRICT
typedef const void NEAR*        HANDLE;
#define DECLARE_HANDLE(name)    struct name##__ { int unused; }; \
                                typedef const struct name##__ NEAR* name
#define DECLARE_HANDLE32(name)  struct name##__ { int unused; }; \
                                typedef const struct name##__ FAR* name
#else   /* STRICT */
typedef UINT                    HANDLE;
#define DECLARE_HANDLE(name)    typedef UINT name
#define DECLARE_HANDLE32(name)  typedef DWORD name
#endif  /* !STRICT */

DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HGLOBAL);
#endif /* WIN32 */
#else	/* !WINVER */
typedef UINT			HANDLE;
typedef HANDLE			HWND;
typedef HANDLE			HMENU;
typedef HANDLE			HDC;
typedef HANDLE			HRGN;
#endif  /* WINVER */

#define	PASCAL_TRUE	0xFFFF
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define NIL 0L

#ifdef THINK_C
#ifndef NULL
#define NULL		((void *) 0)
#endif
#else
#ifndef NULL			/* Removing Warnings - DLB 941111 */
#define NULL 0
#endif				/* Removing Warnings - DLB 941111 */
#endif

#define NULLPTR	 0L
#define NULLCHAR '\0'

/* MSVC20 was reporting a warning of zero size for arrays of length
 of NULL_ARRAY. The code below makes it of minimum size 1 if build a 
WINVER app.  TNG 10/10/95 */
#if defined( __SC__ ) || defined(__cplusplus) || defined(MACAPP31)
#define	NULL_ARRAY	1
#else
#define	NULL_ARRAY /**/
#endif

typedef long (*LProcPtr)();
typedef int (*iProcPtr)();

#if WINVER

typedef struct
{
    UWORD		mnov_flags;		/* menu override flags	*/
#define MNOV_INDEX_FROM_TOP	0x0100
#define MNOV_INDEX_FROM_BOTTOM	0x0200
#define MNOV_THIS_IS_QUIT	0x0400
#define MNOV_END_OF_LIST	0x8000
    UWORD		mnov_menuindex;		/* menu bar index,File=1*/
    UWORD		mnov_popindex;		/* popup index,		*/
    						/*   if FROM_TOP, then	*/
						/*      1 == 1st	*/
						/*   if FROM_BOT, then 	*/
						/*      1 == last	*/
    BYTE		mnov_char;
    BYTE		mnov_resv;
    char		mnov_text[64];		/* menu override string	*/
} menu_override;


typedef struct
{
    unsigned long	mac_type;		/* Mac filetype */
#if Compile4DLL
    unsigned char	ext[3];			/* DOS extension */
#else
    unsigned char	ext[10];			/* DOS extension */
#endif
    unsigned char	description[33];	/* Windows SF Description */
} rsrc_map;


typedef struct
{
	BYTE		asid_LabelID;
	BYTE		asid_FocusID;
	char		asid_ShortCut;
	BYTE		asid_IsPopup		: 1;
	BYTE		asid_MultiLineTE	: 1;
	BYTE		asid_IsGroupBox		: 1;
	BYTE		asid_unused3		: 1;
	BYTE		asid_GroupID		: 4;
} ASI_DITEM, *ASI_DITEMPtr;


typedef struct
{
	BYTE		asid_ShiftUp;
	BYTE		asid_unused0		: 1;
	BYTE		asid_unused1		: 1;
	BYTE		asid_unused2		: 1;
	BYTE		asid_unused3		: 1;
	BYTE		asid_AssumeWhiteBkgd	: 1;
	BYTE		asid_SwapOKCancel	: 1;
	BYTE		asid_TabOrder		: 1;
	BYTE		asid_SmallFont		: 1;
	SWORD		asid_NumItems;
	ASI_DITEM	asid_Item[1];
} ASI_DITL, *ASI_DITLPtr, **ASI_DITLHandle;
#endif  /* WINVER */

#define MakeLong( hi, lo )	(long)( (((long)hi) << 16L) | ((long)lo) )

#define	WdHi( uwordNum )	(long)( ((long)((UWORD)uwordNum)) << 16 )
#define	WdLo( uwordNum )	(long)( (UWORD) uwordNum )
			/* used to make longs out of shorts	*/
#define LW(x) ( (LONG)((UWORD)(x)) )
#define HW(x) ((LONG)((UWORD)x) << 16)
			/* Efficient C versions of HiWord	*/
			/*   and LoWord Traps				*/
#define LOUWORD(l)	    ((UWORD)(l))
#define HIUWORD(l)	    ((UWORD)(((DUWORD)(l) >> 16) & 0xFFFF))
#define		HiShort(longNum)	(short)(((longNum) >> 16) & 0xFFFF)
#define		LoShort(longNum)	(short)((longNum) & 0xFFFF)

			/* Macros for extracting the first	*/
			/*   and last 8 bits of a short		*/
#define		HiByte(shortNum)	(((shortNum) >> 8) & 0x00FF)
#define		LoByte(shortNum)	((shortNum) & 0x00FF)



#if 0
// these are the old menu thangs
#define MENUID_ISHIER	0x8000
#define MENUID_ISPOPUP	0x4000
#define MENUID_SUBSHIFT	8	/* next 6 bits, 64 menus max		*/
#define MENUID_SUBMASKH	0x3f00	/* MENUID_SUBMASKL << MENUID_SUBSHIFT	*/
#define MENUID_SUBMASKL	0x003F	/* MENUID_SUBMASKH >> MENUID_SUBSHIFT	*/
				/*   so MAX_MENU_TITLE + 		*/
				/*   MAX_HEIR_MENUS <= 64		*/
#define MENUID_ITEMMASK	0x00FF

#define MENUID_ISPALETTEPOPUP (MENUID_ISPOPUP | MENUID_SUBMASKH)

#else
// These are the new menu thangs
#define MENUID_ISHIER	0x80
#define MENUID_ISPOPUP	0x40
#define MENUID_SUBSHIFT	8

#define MENUID_SUBMASKH	0xFF00	/* MENUID_SUBMASKL << MENUID_SUBSHIFT	*/
#define MENUID_SUBMASKL	0x00FF	/* MENUID_SUBMASKH >> MENUID_SUBSHIFT	*/

#define MENUID_ITEMMASK	0x00FF

#endif

#ifndef WIN32
typedef F_PASCAL_PTR(BOOL, *PEEKMESSAGEPROC)(VOID *, HWND, UINT, UINT, UINT);
#endif

#include "asi_spec.h"


#ifdef H_MacTypes
#ifdef DEFINE_MACVER
#ifndef __TYPES__
#include <Types.h>
#endif
#else
#ifndef __MACTYPES__
#include <MacTypes.h>
#endif
#endif
#endif

#ifdef H_Quickdraw
#ifndef __QUICKDRAW__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <Quickdraw.h>
#else
#include <Quickdra.h>
#endif
#endif
#endif

#ifdef H_WindowMgr
#ifndef __WINDOWS__
#ifdef DEFINE_MACVER
#include <Windows.h>
#else
#include <WindowM.h>
#endif
#endif
#endif

#ifdef H_ControlMgr
#ifndef __CONTROLS__
#include <Controls.h>
#endif
#endif

#ifdef H_TextEdit
#ifndef __TEXTEDIT__
#include <TextEdit.h>
#endif
#endif

#ifdef H_EventMgr
#ifndef __EVENTS__
#include <Events.h>
#endif
#endif

#ifdef H_MenuMgr
#ifndef __MENUS__
#include <Menus.h>
#endif
#endif

#ifdef H_DialogMgr
#ifndef __DIALOGS__
#include <Dialogs.h>
#endif
#endif

#ifdef H_DeskMgr
#ifndef __DESK__
#include <Desk.h>
#endif
#endif

#ifdef H_Think
#if !MPW
#ifndef __THINK__
#include <Think.h>
#endif
#endif
#endif

#ifdef H_DeviceMgr
#ifndef __DEVICES__
#include <Devices.h>
#endif
#endif

#ifdef H_FileMgr
#ifndef __FILES__
#include <Files.h>
#endif
#endif

#ifdef H_HFS
#ifndef __FILES__
#include <Files.h>
#endif
#endif

#ifdef H_IntlPkg
#if WINVER
#ifndef __TEXTUTILS__
#if LONG_FILENAMES
#include <TextUtils.h>
#else
#include <TextUtil.h>
#endif
#endif
#endif
#define H_PackageMgr 1
#endif

#ifdef H_ListMgr
#ifndef __LISTS__
#include <Lists.h>
#endif
#endif

#ifdef H_MemoryMgr
#ifndef __MEMORY__
#ifdef DEFINE_MACVER
#include <Memory.h>
#else
#include <MemoryM.h>
#endif
#endif
#endif

#ifdef H_Notification
#ifndef __NOTIFICATION__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <Notification.h>
#else
#include <Notifica.h>
#endif
#endif
#endif

#ifdef H_OSUtil
#ifndef __OSUTILS__
#include <OSUtils.h>
#endif
#endif

#ifdef H_PackageMgr
#ifndef __PACKAGES__
#include <Packages.h>
#endif
#endif

#ifdef H_ResourceMgr
#ifndef __RESOURCES__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <Resources.h>
#else
#include <Resource.h>
#endif
#endif
#endif

#ifdef H_ScrapMgr
#ifndef __SCRAP__
#include <Scrap.h>
#endif
#endif

#ifdef H_SegmentLdr
#ifndef __SEGLOAD__
#include <SegLoad.h>
#endif
#endif

#ifdef H_StdFilePkg
#ifndef __STANDARDFILE__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <StandardFile.h>
#else
#include <Standard.h>
#endif
#endif
#endif

#ifdef H_ToolboxUtil
#ifndef __TOOLUTILS__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <ToolUtils.h>
#else
#include <ToolUtil.h>
#endif
#endif
#endif

#ifdef H_asm
#ifndef __asm__
#include <asm.h>
#endif
#endif

#ifdef H_Pascal
#if !MPW
#ifndef __pascal__
#include <pascal.h>
#endif
#endif
#endif

#ifdef H_Appletalk
#ifndef __APPLETALK__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <Appletalk.h>
#else
#include <Appletal.h>
#endif
#endif
#endif

#ifdef H_Color
#ifndef __PALETTES__
#include <Palettes.h>
#endif
#endif

#ifdef H_Palettes
#ifndef __PALETTES__
#include <Palettes.h>
#endif
#endif

#ifdef H_ColorToolbox
#ifndef __PALETTES__
#include <Palettes.h>
#endif
#endif

#ifdef H_DeskBus
#ifndef __DESKBUS__
#include <DeskBus.h>
#endif
#endif

#ifdef H_DiskDvr
#ifndef __DISKS__
#include <Disks.h>
#endif
#endif

#ifdef H_Fonts
#define H_FontMgr 1
#endif

#ifdef H_FontMgr
#ifndef __FONTS__
#include <Fonts.h>
#endif
#endif

#ifdef H_PrintTraps
#if PPC_HEADERS
#ifndef __PRINTING__
#include <Printing.h>
#endif
#else
#include <PrintTra.h>
#endif
#endif

#ifdef H_PrintMgr
#ifndef __PRINTING__
#include <Printing.h>
#endif
#endif

#ifdef H_SANE
#ifndef __SANE__
#include <SANE.h>
#endif
#endif

#ifdef H_ScriptMgr
#ifndef __SCRIPT__
#include <Script.h>
#endif
#endif

#ifdef H_SCSIMgr
#ifndef __SCSI__
#include <SCSI.h>
#endif
#endif

#ifdef H_SerialDvr
#ifndef __SERIAL__
#include <Serial.h>
#endif
#endif

#ifdef H_SlotMgr
#ifndef __SLOTS__
#include <Slots.h>
#endif
#endif

#ifdef H_Sound
#ifndef __SOUND__
#include <Sound.h>
#endif
#endif

#ifdef H_Midi
#ifndef __MIDI__
#include <Midi.h>
#endif
#endif

#ifdef H_StartMgr
#ifndef __START__
#include <Start.h>
#endif
#endif

#ifdef H_SysErr
#ifndef __ERRORS__
#include <Errors.h>
#endif
#endif

#ifdef H_TimeMgr
#ifndef __TIMER__
#include <Timer.h>
#endif
#endif

#ifdef H_Video
#ifndef __VIDEO__
#include <Video.h>
#endif
#endif

#ifdef H_VRetraceMgr
#ifndef __RETRACE__
#include <Retrace.h>
#endif
#endif

#ifdef H_GraphDevMgr
#ifndef __QDOFFSCREEN__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <QDOffscreen.h>
#else
#include <QDOffscr.h>
#endif
#endif
#endif

#ifdef H_Aliases
#define H_AliasMgr 1
#endif

#ifdef H_AliasMgr
#ifndef __ALIASES__
#include <Aliases.h>
#endif
#endif

#ifdef H_AppleEvents
#ifndef __APPLEEVENTS__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <AppleEvents.h>
#else
#include <AppleEve.h>
#endif
#endif
#endif

#ifdef H_Drag
#ifndef __DRAG__
#include <Drag.h>
#endif
#endif

#ifdef H_Traps
#ifndef __TRAPS__
#include <Traps.h>
#endif
#endif

#ifdef H_Gestalt
#ifndef __GESTALT__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <GestaltEqu.h>
#else
#include <GestaltE.h>
#endif
#endif
#endif

#ifdef H_EditionMgr
#ifndef __EDITIONS__
#include <Editions.h>
#endif
#endif

#ifdef H_Processes
#ifndef __PROCESSES__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <Processes.h>
#else
#include <Processe.h>
#endif
#endif
#endif

#ifdef H_FixMath
#ifndef __FIXMATH__
#include <FixMath.h>
#endif
#endif

#ifdef H_Picker
#ifdef DEFINE_MACVER
#ifndef __COLORPICKER__
#include <ColorPicker.h>
#endif
#else
#ifndef __PICKER__	
#include <Picker.h>
#endif
#endif
#endif

#ifdef H_Connections
#ifndef __CONNECTIONS__
#ifdef DEFINE_MACVER
#include <Connections.h>
#else
#include <Connecti.h>
#endif
#endif
#endif

#ifdef H_Balloons
#ifndef __BALLOONS__
#include <Balloons.h>
#endif
#endif

#ifdef H_Terminals
#if WINVER
#ifndef __TERMINALS__
#if LONG_FILENAMES
#include <Terminals.h>
#else
#include <Terminal.h>
#endif
#endif
#endif
#endif

#ifdef H_FileTransfers
#ifndef __FILETRANFERS__
#ifdef DEFINE_MACVER
#include <FileTransfers.h>
#else
#include <FileTran.h>
#endif
#endif
#endif

#ifdef H_ShutDown
#ifndef __SHUTDOWN__
#include <ShutDown.h>
#endif
#endif

#if PPC_HEADERS
#ifndef __LOWMEM__
#include <LowMem.h>    // For all the extern lowmem globals
#endif
#else
#ifndef __LOMEM__
#include <LoMem.h>      // 04/26/93 DRR
#endif
#endif  // PPC_HEADERS

#ifdef H_OutlineCalls
#ifndef __OUTLINECALLS__
#ifdef DEFINE_MACVER
#include <OutlineCalls.h>
#else
#include <OutlineC.h>
#endif
#endif
#endif

#ifdef H_PPCToolBox
#ifndef __PPCTOOLBOX__
#if LONG_FILENAMES || defined( DEFINE_MACVER )
#include <PPCToolBox.h>
#else
#include <PPCToolB.h>
#endif
#endif
#endif

#ifdef	H_Icons
#ifndef __ICONS__
#include <Icons.h>
#endif
#endif
#ifdef	H_Strings
#ifndef __STRINGS__
#include <Strings.h>
#endif
#endif

#if WINVER
#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus
void __cdecl AppMain(void);
#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// WINVER

#ifdef WIN32
#undef VOID
//#include "windows.h"
//#include "windowsx.h"
#endif

#if WINVER	// Added here to minimize changes to Apple's files
EXTERN F_PASCAL( void ) InitEvents( void );

#if !defined(__BORLANDC__) && !defined(WIN32)
	// DWT 5/2/94 Suppress MSVC warning about zero sized arrays 
#pragma warning ( disable : 4200 )
#endif //WIN32

#if PPC_HEADERS
#ifndef __TYPES__
#include <MacTypes.H>
#endif
typedef struct WinTemp {
	Rect			bounds;
	SWORD			procID;
	Boolean			visible;
	char			pad1;
	Boolean			goAwayFlag;
	char			pad2;
	LONG			refCon;
	char			title[NULL_ARRAY];
} WinTemplate, *WinTemplatePtr;
#endif //PPC_HEADERS

#if defined(_ASI_DBCS_) || defined(Japan)
#include "asidbcs.h"
#endif //_ASI_DBCS_


#define ASI_MAXPATHNAME	256

typedef struct ASI_CFSTRUCT
{
	char	szPathName[ASI_MAXPATHNAME];
	DWORD	dwAccess;
	DWORD	dwShareMode;
	DWORD	dwCreate;
	DWORD	dwAttrsAndFlags;
} ASI_CFSTRUCT, *ASI_PCFSTRUCT;


#ifdef WIN32
#define	ASI_HFILE_ERROR	INVALID_HANDLE_VALUE
#define	ASI_HFILE	HANDLE
#else
#define	ASI_HFILE_ERROR	HFILE_ERROR
#define	ASI_HFILE	SWORD
#endif

#define ASI_INVALID_VREFNUM ((short) 0xEEEE)
#endif //WINVER

// RHK 04/01/95 added the following Macros to setup ProcInfo and
// Routine Descriptor required for PowerPC version of OS CallBacks
// Macros will not change the calls for 68K or PC versions.
#ifdef powerc
	#define MAKE_UPP(info, func_ptr)	\
		RoutineDescriptor	rd_ ##func_ptr = BUILD_ROUTINE_DESCRIPTOR(info, func_ptr)
	#define MAKE_LOCAL_UPP(info, func_ptr)	\
		static RoutineDescriptor	rd_ ##func_ptr = BUILD_ROUTINE_DESCRIPTOR(info, func_ptr)
	#define EXTRN_UPP(func_ptr)	\
		extern RoutineDescriptor	rd_ ##func_ptr
	#define GET_UPP(type, func_ptr)	&rd_ ##func_ptr
	#define TYPECAST_GET_UPP(type, func_ptr)		((type)(&rd_ ##func_ptr))
#else
	#define MAKE_UPP(info, func_ptr)		enum { hoop ##func_ptr }
	#define MAKE_LOCAL_UPP(info, func_ptr)	enum { hoop_dee ##func_ptr }
	#define EXTRN_UPP(func_ptr)				enum { hoop_dee_doo ##func_ptr }
	#define GET_UPP(type, func_ptr)		(type)(func_ptr)
	#define TYPECAST_GET_UPP(type, func_ptr)		((type)(func_ptr))
#endif	// powerc


// These are some common prototypes that can be genericized to solve problems
//   with include file collisions.
#ifdef __cplusplus
extern "C" {
#endif
EXTERN F_ASIBND( void ) ASI_CopyMacPtoWinC( unsigned char *pstr,
	unsigned char *pcstr, unsigned char *palpha );
EXTERN F_ASIBND( void ) ASI_CopyPtoP( unsigned char const *pstrsrc,
	unsigned char *pstrdst );
EXTERN F_ASIBND( char * ) ASI_CopyPtoC( unsigned char const *pstr,
	unsigned char *pcstr );
EXTERN F_ASIBND( unsigned char * ) ASI_CopyCtoP( unsigned char const *pcstr,
	unsigned char *pstr );
EXTERN F_ASIBND(Boolean) IsCachedBrush(void* hBR);
#ifdef __cplusplus
}
#endif

#if SmartHeapSupport
#ifdef	MEM_DEBUG
#include <stdlib.h>
#ifdef	__cplusplus
#include <smrtheap.hpp>
#else
#include <smrtheap.h>
#endif
#endif
#endif

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

#endif // _ASIPORT_

