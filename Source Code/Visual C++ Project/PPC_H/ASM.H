
/*
 *  asm.h - definitions useful with inline assembly
 *
 *  Copyright (c) 1991 Symantec Corporation.  All rights reserved.
 *
 *  These interfaces are based on material copyrighted
 *  by Apple Computer, Inc., 1985-1991.
 *
 */

#ifndef __asm__
#define __asm__


/*  trap modifier flags, e.g. "NewHandle  SYS+CLEAR"  */

enum {
		/*  Memory Manager traps  */
	SYS = 2,			/*  applies to system heap  */
	CLEAR = 1,			/*  zero allocated block  */
		/*  File Manager and Device Manager traps  */
	ASYNC = 2,			/*  asynchronous I/O  */
	HFS = 1,			/*  HFS version of trap (File Manager)  */
	IMMED = 1,			/*  bypass driver queue (Device Manager)  */
		/*  string operations  */
	MARKS = 1,			/*  ignore diacritical marks  */
	CASE = 2,			/*  don't ignore case  */
		/*  GetTrapAddress, SetTrapAddress  */
	NEWOS = 1,			/*  new trap numbering, OS trap  */
	NEWTOOL = 3,		/*  new trap numbering, Toolbox trap  */
		/*  Toolbox traps  */
	AUTOPOP = 2			/*  return directly to caller's caller  */
};


#if WINVER
/*  field offsets, e.g. "mov  ax,[si]OFFSET(Rect,bottom)"  */
#ifdef __BORLANDC__
#define OFFSET(typ,field) .(typ)field
#else
#define OFFSET(typ,field) typ.field
#endif
#else
/*  field offsets, e.g. "move.w  d0,OFFSET(Rect,bottom)(a2)"  */

#define OFFSET(type, field)		((int) &((type *) 0)->field)
#endif



/*
 *  additional trap definitions
 *
 *  Two kinds of traps are defined here.
 *
 *	¥	Traps that may be useful in inline assembly, or that have
 *		traditionally been available in inline assembly in THINK C,
 *		but for which the Apple headers do not supply inline definitions.
 *
 *	¥	Traps for which the Apple headers supply inline definitions
 *		that, though fine in C, are inappropriate for use in inline
 *		assembly.  (The inline assembler knows to prefer a trap defined
 *		with a leading underscore, regardless of whether an underscore
 *		is used when the trap is invoked.)
 *
 */

#if WINVER
#else
void _ADBOp(void);
void _AddDrive(void);

void _Chain(void);
void _CmpString(void);

void _Date2Secs(void);
void _DecStr68K(void);
void _Delay(void);
void _DeleteUserIdentity(void);

void _Elems68K(void);
void _EqualString(void);

void _FindFolder(void);
void _FlushEvents(void);
void _FP68K(void);

void _Gestalt(void);
void _GetDefaultUser(void);
void _GetHandleSize(void);
void _GetItemStyle(void);
void _GetOSEvent(void);
void _GetPhysical(void);
void _GetPtrSize(void);

void _HandToHand(void);
void _HCreateResFile(void);
void _HFSDispatch(void);
void _HOpenResFile(void);

void _InitZone(void);
void _InternalWait(void);

void _Launch(void);
void _LwrString(void);
void _LwrText(void);

void _MaxMem(void);

void _NewGestalt(void);
void _NMInstall(void);
void _NMRemove(void);

void _OSDispatch(void);
void _OSEventAvail(void);

void _Pack0(void);
void _Pack2(void);
void _Pack3(void);
void _Pack4(void);
void _Pack5(void);
void _Pack6(void);
void _Pack7(void);
void _Pack12(void);
void _PPostEvent(void);
void _PrGlue(void);
void _PtrToHand(void);
void _PurgeSpace(void);

void _RelString(void);
void _ReplaceGestalt(void);

void _ScriptUtil(void);
void _SCSIDispatch(void);
void _SetFractEnable(void);
void _ShutDown(void);
void _SlotManager(void);
void _StartSecureSession(void);
void _StripAddress(void);
void _SwapMMUMode(void);
void _SysEnvirons(void);
void _SysError(void);

void _TEDispatch(void);

void _UprString(void);
void _UprText(void);

void _WriteParam(void);
#endif

#endif // __asm__

