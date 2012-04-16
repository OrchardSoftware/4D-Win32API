/************************************************************

	string.h
	String handling
	
	Copyright Apple Computer,Inc.  1987-1990, 1993
	All rights reserved

************************************************************/




#include "asiport.h"



#ifndef __STRING__
#define __STRING__

#ifndef __size_t__
#define __size_t__
#ifdef powerc
typedef unsigned long size_t;
#else
typedef unsigned int size_t;
#endif /* powerc */
#endif

#ifndef NULL
#define NULL 0
#endif

/*
 *	Copying functions
 */

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

void *memcpy (void *s1, const void *s2, size_t n);
void *memmove (void *s1, const void *s2, size_t n);
char *strcpy (char *s1, const char *s2);
char *strncpy (char *s1, const char *s2, size_t n);

#if __STDC__ == 0
void *memccpy(void *s1, const void *s2, int c, size_t n);
#endif

/*
 *	Concatenation functions
 */

char *strcat (char *s1, const char *s2);
char *strncat (char *s1, const char *s2, size_t n);

/*
 *	Comparison functions
 */

int memcmp (const void *s1, const void *s2, size_t n);
int strcmp (const char *s1, const char *s2);
int strcoll (const char *s1, const char *s2);
int strncmp (const char *s1, const char *s2, size_t n);
size_t strxfrm (char *s1, const char *s2, size_t n);


/*
 *	Search functions
 */

void *memchr (const void *s, int c, size_t n);
char *strchr (const char *s, int c);
size_t strcspn (const char *s1, const char *s2);
char * strpbrk (const char *s1, const char *s2);
char *strrchr (const char *s, int c);
size_t strspn (const char *s1, const char *s2);
char *strstr (const char *s1, const char *s2);
char *strtok (char *s1, const char *s2);


/*
 *	Miscellaneous functions
 */

void *memset (void *s, int c, size_t n);
char *strerror (int errnum);
size_t strlen (const char *s);

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

