// ASI_PPC.H
// defines so source changes for Compilation with Universal Headers will
// still compile with old headers
//
#ifndef _ASI_PPC_H
#define _ASI_PPC_H

#if PPC_HEADERS

#define LMGetGrayRgn()		GetGrayRgn()

#define GrayRgn				LMGetGrayRgn()

#define	screenBits			qd.screenBits
#define	thePort				qd.thePort

#endif


#endif // _ASI_PPC_H
