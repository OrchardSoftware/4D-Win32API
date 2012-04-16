/*
	File:		Unmangler.h

	Copyright:	© 1984-1993 by Apple Computer, Inc., all rights reserved.
*/

#ifndef __UNMANGLER__
#define __UNMANGLER__

#ifdef __cplusplus
extern "C" {
#endif

extern int unmangle(char *dst, char *src, int limit);

extern int pascal Unmangle(char *dst, char *src, int limit);

#ifdef __cplusplus
}
#endif

#endif

