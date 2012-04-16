/*
	PLStringFuncs.h -- C string conversion functions for pascal strings.

	Copyright Apple Computer,Inc.  1989-1991, 1993.
	All rights reserved

	This file is used in these builds: ROM System

	Change History (most recent first):

		<3>	8/8/91	JL		Update copyright
		<2>	6/17/91	JL		Checked in MPW 3.2Ä version.  Changed PLstrchr and PLstrstr 2nd
			param type from char to short.

	To Do:
*/

#ifndef __PLSTRINGFUNCS__
#define __PLSTRINGFUNCS__

#ifndef	__TYPES__
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

short pascal		PLstrcmp(ConstStr255Param str1, ConstStr255Param str2);
short pascal		PLstrncmp(ConstStr255Param str1, ConstStr255Param str2, short num);
StringPtr pascal	PLstrcpy(StringPtr str1, ConstStr255Param str2);
StringPtr pascal	PLstrncpy(StringPtr str1, ConstStr255Param str2, short num);
StringPtr pascal	PLstrcat(StringPtr str1, ConstStr255Param str2);
StringPtr pascal	PLstrncat(StringPtr str1, ConstStr255Param str2, short num);
Ptr pascal			PLstrchr(ConstStr255Param str1, short ch1);
Ptr pascal			PLstrrchr(ConstStr255Param str1, short ch1);
Ptr pascal			PLstrpbrk(ConstStr255Param str1, ConstStr255Param str2);
short pascal		PLstrspn(ConstStr255Param str1, ConstStr255Param str2);
Ptr pascal			PLstrstr(ConstStr255Param str1, ConstStr255Param str2);
short pascal		PLstrlen(ConstStr255Param str);
short pascal		PLpos(ConstStr255Param str1, ConstStr255Param str2);


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

