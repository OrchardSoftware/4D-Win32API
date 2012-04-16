/************************************************************

 StdCompression.h

 Copyright Apple Computer, Inc. 1991
 All rights reserved

************************************************************/


#ifndef	__STDCOMPRESSION__
#define	__STDCOMPRESSION__

#ifndef __IMAGECOMPRESSION__
#if LONG_FILENAMES
#include <ImageCompression.h>
#else
#include <ImageCom.h>
#endif
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

typedef struct {
	long			flags;
	CodecType		theCodecType;
	CodecComponent	theCodec;
	CodecQ			spatialQuality;
	CodecQ			temporalQuality;
	short			depth;
	Fixed			frameRate;
	long			keyFrameRate;
	long			reserved1;
	long			reserved2;
} SCParams;

typedef Boolean (pascal * SCModalFilterProcPtr)(DialogPtr theDialog,
	EventRecord *theEvent, short *itemHit, long refcon);

typedef short (pascal * SCModalHookProcPtr)(DialogPtr theDialog,
	short itemHit, SCParams *params, long refcon);

/*
 *	Flags for SCParams.
 */
#define	scShowMotionSettings	(1L<<0)
#define	scListEveryCodec	(1L<<1)
#define	scAllowZeroFrameRate	(1L<<2)
#define	scAllowZeroKeyFrameRate	(1L<<3)


/*
 *	Possible test flags for setting test image.
 */
#define	scPreferCropping			(1<<0)
#define	scPreferScaling				(1<<1)
#define scPreferScalingAndCropping	(scPreferScaling + scPreferCropping)


/*
 *	Dimensions of the image preview box.
 */
#define	scTestImageWidth	80
#define	scTestImageHeight	80


/*
 *	Possible items returned by hookProc.
 */
#define	scSettingsChangedItem	-1
#define	scOKItem				1
#define	scCancelItem			2
#define	scCustomItem			3

#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

F_PASCAL( ComponentInstance )
OpenStdCompression(void);

F_PASCAL( ComponentResult )
SCGetCompressionExtended(ComponentInstance ci,SCParams *params,Point where,
	SCModalFilterProcPtr filterProc,SCModalHookProcPtr hookProc,long refcon,StringPtr customName);

F_PASCAL( ComponentResult )
SCGetCompression(ComponentInstance ci,SCParams *params,Point where);

F_PASCAL( ComponentResult )
SCPositionRect(ComponentInstance ci,Rect *rp,Point *where);

F_PASCAL( ComponentResult )
SCPositionDialog(ComponentInstance ci,short id,Point *where);

F_PASCAL( ComponentResult )
SCSetTestImagePictHandle(ComponentInstance ci, PicHandle testPict, Rect *testRect, short testFlags);

F_PASCAL( ComponentResult )
SCSetTestImagePictFile(ComponentInstance ci, short testFileRef, Rect *testRect, short testFlags);

F_PASCAL( ComponentResult )
SCSetTestImagePixMap(ComponentInstance ci, PixMapHandle testPixMap, Rect *testRect, short testFlags);

F_PASCAL( ComponentResult )
SCGetBestDeviceRect(ComponentInstance ci, Rect *r);
	
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
#endif	// __cplusplus

#endif __STDCOMPRESSION__


