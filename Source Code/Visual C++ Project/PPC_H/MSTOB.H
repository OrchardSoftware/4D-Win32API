// Resolves symbol name differences between Microsoft and Borland compilers.
// Use when compiling MS source code w/ Borland compiler.
// created: 5/25/94	vdBijl

#ifndef _MSTOB_H_
#define _MSTOB_H_

#define _MK_FP					MK_FP
#define _FP_SEG				FP_SEG
#define _mkdir					mkdir
#define _filelength			filelength
// #define _chmod					chmod
#define _S_IREAD				S_IREAD
#define _S_IWRITE				S_IWRITE
#define _FPE_OVERFLOW		FPE_OVERFLOW
#define _FPE_UNDERFLOW		FPE_UNDERFLOW
#define _FPE_INVALID			FPE_INVALID
#define _FPE_ZERODIVIDE		FPE_ZERODIVIDE

#endif // _MSTOB_H_
