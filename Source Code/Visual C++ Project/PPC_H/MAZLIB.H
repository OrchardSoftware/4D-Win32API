/*
mod 5 23 95 tsh - use define to prevent double include
mod 11 29 93 tsh - add short versions of copy and fill
mod 6 1 92 tsh - make 'null' a void *
mod 8 12 91 tsh - Think C 5.0
MOD 09-27-90 MAZ - add prototypes to cover mazlib.c, mazstuff.c, and mazlongstuff.c
mod 8 4 89 tsh - Think 4.0 version, add constants from old math.h
mod 10 18 86 tsh - LSC 1.41 version

MAZlib.h -- defines for MAZlib c-code and for external use

*/

#ifndef __MAZLIB__
#define __MAZLIB__

#ifndef	_Types_
#include <Types.h>
#endif

/* useful math constants */
#define	PI		(3.14159265358979323846)
#define	PI2		(1.57079632679489661923)
#define	PI4		(0.78539816339744830966)
#define	E		(2.71828182845904523536)

#define null (void *)0L
#define maxidlen 32
#define maxlinelen 512
#define maxfname 32
#define forever for(;;)
#define CR '\r'
#define bool Boolean

typedef struct _queue
    {
    struct _queue *que_next;
    struct _queue *que_prev;
    } queue;

typedef struct _h_queue
    {
    struct _h_queue **que_next;
    struct _h_queue **que_prev;
    } h_queue;

/* prototypes */
extern void initqueue(queue *);
extern short countqueue(queue *);
extern bool emptyqueue(queue *);
extern void h_initqueue(h_queue **);
extern bool h_emptyqueue(h_queue **);
extern short h_countqueue(h_queue **);
extern bool input_file_name(char *, char *);
extern bool output_file_name(char *, char *, char *);
extern short texttoint(char *);
extern char *inttotext(char *, short);
extern char *cpystr(char *, char *);
extern bool cmpbuf(char *, char *, short);
extern void upper(char *, short);
extern void lower(char *, short);
extern short scnstr(char *, char);
extern short ffset(char *, short, short);
extern short rotate(unsigned short, short);
extern long disk_bytes_free(short);

#ifdef _MAZlib_source_
/* for compiling MAZlib.h */
extern void insqueue(queue *, queue *);
extern void remqueue(queue *);
extern void h_insqueue(h_queue **, h_queue **);
extern void h_remqueue(h_queue **);
#if !__powerc
extern void cpybuf(char *, char *, unsigned short);
extern void cpylong(long *, long *, unsigned short);
extern void cpyshort(short *, short *, unsigned short);
extern void fillbuffer(char *, unsigned short, char);
extern void longcpybuf(char *, char *, long);
extern void longcpylong(long *, long *, long);
extern void longcpyshort(short *, short *, long);
extern void longfillbuffer(char *, long, char);
#endif
extern void longfilllong(long *, long, long);
extern void longfillshort(short *, long, short);
extern void filllong(long *, unsigned short, long);
extern void fillshort(short *, unsigned short, short);
extern bool legal_address(char *);
#else
/* for calling MAZlib.h */
extern void insqueue(void *, void *);
extern void remqueue(void *);
extern void h_insqueue(void *, void *);
extern void h_remqueue(void *);
#if !__powerc
extern void cpybuf(void *, void *, unsigned short);
extern void cpylong(void *, void *, unsigned short);
extern void cpyshort(void *, void *, unsigned short);
extern void fillbuffer(void *, unsigned short, char);
extern void longcpybuf(void *, void *, long);
extern void longcpylong(void *, void *, long);
extern void longcpyshort(void *, void *, long);
extern void longfillbuffer(void *, long, char);
#endif
extern void longfilllong(void *, long, long);
extern void longfillshort(void *, long, short);
extern void filllong(void *, unsigned short, long);
extern void fillshort(void *, unsigned short, short);
extern bool legal_address(void *);
#endif

#if __powerc
/* use ANSI routines for Power PC */
#include <string.h>
#define cpybuf(dst,src,cnt) memmove(dst,src,cnt)
#define longcpybuf(dst,src,cnt) memmove(dst,src,cnt)
#define cpylong(dst,src,cnt) memmove(dst,src,(cnt)*sizeof(long))
#define cpyshort(dst,src,cnt) memmove(dst,src,(cnt)*sizeof(short))
#define longcpylong(dst,src,cnt) memmove(dst,src,(cnt)*sizeof(long))
#define longcpyshort(dst,src,cnt) memmove(dst,src,(cnt)*sizeof(short))
#define fillbuffer(dst,cnt,ch) memset(dst,ch,cnt)
#define longfillbuffer(dst,cnt,ch) memset(dst,ch,cnt)
#endif

#endif

