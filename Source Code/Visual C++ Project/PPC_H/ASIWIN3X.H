/*
*	ASIWIN3x.H
*	Copyright (c) 1994 Altura Software, Inc. All Rights Reserved.
*	Created by:	RHK	06/14/94
*
*/

#ifdef WIN32

#define _fmemcpy	memcpy
#define	SetWindowOrg3x( hDC, X, Y, pPT ) \
			SetWindowOrgEx( hDC, X, Y, pPT );
#define	SetWindowExt3x( hDC, X, Y, pSZ ) \
			SetWindowExtEx( hDC, X, Y, pSZ );
#else

#define	SetWindowOrg3x( hDC, X, Y, pPT ) \
			SetWindowOrg( hDC, X, Y );
				
#define	SetWindowExt3x( hDC, X, Y, pSZ ) \
			SetWindowExt( hDC, X, Y );
#endif

