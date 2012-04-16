/*
*       ASIBSwap.H
*       Copyright (c) 1991 Altura Software, Inc. All Rights Reserved.
*       Created by:     LJL     9/30/92
*       Modified by:    LJL     7/19/93
*
*	mod 07/19/93 LJL - merged from David Jennings at MacroMedia
*/

#ifndef __ASIBSWAP__
#define __ASIBSWAP__

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

#ifndef	__TYPES__
#include <Types.h>
#endif

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
