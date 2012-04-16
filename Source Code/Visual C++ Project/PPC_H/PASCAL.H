
/*
 *  pascal.h
 *
 *  Copyright (c) 1991 Symantec Corporation.  All rights reserved.
 *
 */

#ifndef __pascal__
#define __pascal__

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

//  these functions modify their argument in place - don't use with constants
#if WINVER
#ifndef	_ASIPORT_
EXTERN F_ASIBND( char * ) ASI_CopyPtoC( BYTE const *pstr, BYTE *pcstr );
EXTERN F_ASIBND( unsigned char * ) ASI_CopyCtoP( BYTE const *pcstr, BYTE *pstr );
#endif
#define PtoCstr(p) ASI_CopyPtoC( (BYTE const *)p, (BYTE *)p )
#define CtoPstr(p) ASI_CopyCtoP( (BYTE const *)p, (BYTE *)p )
#define C2PStr(p) CtoPstr(p)
#define P2CStr(p) PtoCstr(p)
#else
unsigned char *CtoPstr(char *);
char *PtoCstr(unsigned char *);
unsigned char * pascal C2PStr(char *);
char * pascal P2CStr(unsigned char *);
#endif

#define c2pstr(s) CtoPstr(s)
#define p2cstr(s) PtoCstr(s)

#if MACVER
#ifndef __cplusplus
//  no longer needed - provided for backward compatibility
void pascal CallPascal (...) = { 0x205F, 0x4E90 };
char pascal CallPascalB(...) = { 0x205F, 0x4E90 };
short pascal  CallPascalW(...) = { 0x205F, 0x4E90 };
long pascal CallPascalL(...) = { 0x205F, 0x4E90 };
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

#ifdef __cplusplus
}
#endif

#endif // __pascal__

