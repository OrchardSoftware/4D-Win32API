/*
*	ASIDBCS.H
*	Copyright (c) 1994, 1995 Altura Software, Inc. All Rights Reserved.
*	Created by:     RHK     12/12/94
*
*	mod 07/27/95 lve  - control compile flags through the command line
*			so that there is no need to change that file
*			this only sets default values (to false) if nothing
*			has been defined (changed by not including this
*			file from asiport.h unless japan is defined)
*	mod 07/12/95 RHK  - added unsigned char cast
*	mod 06/17/95 RHK  - changed isKanji macro and added _ASI_IME_ compile flag 
*	mod 06/07/95 RHK  - changed HANKATA and isKanji macro
*	mod 05/30/95 RHK  - added Input Method Editor Support with ASI_IMESUPPORT
*/

#ifndef _ASIDBCS_
#define _ASIDBCS_

// MultiByte Character Set Support
#define ASI_DBCS	1
		//#define _MBCS			0
// Japanese/ShiftJis Character Set Support
#define ASI_KANJI	1
// Keyboard Input Method Editor Support
#if _ASI_IME_
#define ASI_IMESUPPORT	1
#endif
// SJIS1 check for ShiftJis leadbyte code ==> IsDBCSLeadByte()
// SJIS2 check for ShiftJis trailbytes
// HANKATA check for Half-Width katakana
// IsKanji return TRUE for ShiftJis Leadbyte or HANKATA (Half-Width katakana)
#define SJIS1(A)  ( (((unsigned char)A) >= 129 && ((unsigned char)A) <= 159) \
				 || (((unsigned char)A) >= 224 && ((unsigned char)A) <= 239) )

#define SJIS2(A)  ( (((unsigned char)A) != 127) &&  \
					(((unsigned char)A) >= 64 && ((unsigned char)A) <= 252) )

#define HANKATA(A)	(((unsigned char)A) >= 161 && ((unsigned char)A) <= 223)

#define	IsKanji(A)	( SJIS1(A) )

#endif // _ASIDBCS_
