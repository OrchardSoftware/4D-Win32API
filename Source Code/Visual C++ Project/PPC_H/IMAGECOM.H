/************************************************************

Created: Tuesday, August 13, 1991 at 4:25 PM
 ImageCompression.h
 C Interface to the Macintosh Libraries


 Copyright Apple Computer, Inc. 1991
 All rights reserved

************************************************************/


#ifndef __IMAGECOMPRESSION__
#define __IMAGECOMPRESSION__

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

#ifndef __QUICKDRAW__
#if LONG_FILENAMES
#include <Quickdraw.h>
#else
#include <Quickdra.h>
#endif
#endif

#ifndef __QDOFFSCREEN__
#if LONG_FILENAMES
#include <QDOffscreen.h>
#else
#include <QDOffscr.h>
#endif
#endif

#ifndef __TYPES__
#include <MacTypes.h>
#endif

#ifndef __COMPONENTS__
#if LONG_FILENAMES
#include <Components.h>
#else
#include <Componen.h>
#endif
#endif

#ifndef __STANDARDFILE__
#if LONG_FILENAMES
#include <StandardFile.h>
#else
#include <Standard.h>
#endif
#endif


#define gestaltCompressionMgr OST_icmp

/* These are the bits that are set in the Component flags, and also in the codecInfo struct. */

#define codecInfoDoes1 (1L<<0)						/* codec can work with 1-bit pixels */
#define codecInfoDoes2 (1L<<1)						/* codec can work with 2-bit pixels */
#define codecInfoDoes4 (1L<<2)						/* codec can work with 4-bit pixels */
#define codecInfoDoes8 (1L<<3)						/* codec can work with 8-bit pixels */
#define codecInfoDoes16 (1L<<4)						/* codec can work with 16-bit pixels */
#define codecInfoDoes32 (1L<<5)						/* codec can work with 32-bit pixels */
#define codecInfoDoesDither (1L<<6)					/* codec can do ditherMode */
#define codecInfoDoesStretch (1L<<7)				/* codec can stretch to arbitrary sizes */
#define codecInfoDoesShrink (1L<<8)					/* codec can shrink to arbitrary sizes */
#define codecInfoDoesMask (1L<<9)					/* codec can mask to clipping regions */

#define codecInfoDoesTemporal (1L<<10)				/* codec can handle temporal redundancy */

#define codecInfoDoesDouble (1L<<11)				/* codec can stretch to double size exactly */
#define codecInfoDoesQuad (1L<<12)					/* codec can stretch to quadruple size exactly */
#define codecInfoDoesHalf (1L<<13)					/* codec can shrink to half size */
#define codecInfoDoesQuarter (1L<<14)				/* codec can shrink to quarter size */

#define codecInfoDoesRotate (1L<<15)				/* codec can rotate on decompress */
#define codecInfoDoesHorizFlip (1L<<16)				/* codec can flip horizontally on decompress */
#define codecInfoDoesVertFlip (1L<<17)				/* codec can flip vertically on decompress */
#define codecInfoDoesSkew (1L<<18)					/* codec can skew on decompress */
#define codecInfoDoesBlend (1L<<19)					/* codec can blend on decompress */
#define codecInfoDoesWarp (1L<<20)					/* codec can warp arbitrarily on decompress */
#define codecInfoDoesRecompress (1L<<21)			/* codec can recompress image without accumulating errors */
#define	codecInfoDoesSpool			(1L<<22)	/* codec can spool image data */

#define codecInfoDepth1 (1L<<0)						/* compressed data at 1 bpp depth available */
#define codecInfoDepth2 (1L<<1)						/* compressed data at 2 bpp depth available */
#define codecInfoDepth4 (1L<<2)						/* compressed data at 4 bpp depth available */
#define codecInfoDepth8 (1L<<3)						/* compressed data at 8 bpp depth available */
#define codecInfoDepth16 (1L<<4)					/* compressed data at 16 bpp depth available */
#define codecInfoDepth32 (1L<<5)					/* compressed data at 32 bpp depth available */
#define codecInfoDepth24 (1L<<6)					/* compressed data at 24 bpp depth available */
#define codecInfoDepth33 (1L<<7)					/* compressed data at 1 bpp monochrome depth  available */
#define codecInfoDepth34 (1L<<8)					/* compressed data at 2 bpp grayscale depth available */
#define codecInfoDepth36 (1L<<9)					/* compressed data at 4 bpp grayscale depth available */
#define codecInfoDepth40 (1L<<10)					/* compressed data at 8 bpp grayscale depth available */
#define codecInfoStoresClut (1L<<11)				/* compressed data can have custom cluts */
#define	codecInfoDoesLossless		(1L<<12)		/* compressed data can be stored in lossless format */

#define codecFlagUseImageBuffer (1L<<0)				/* (input) allocate buffer for whole image on decompress sequence */
#define codecFlagUseScreenBuffer (1L<<1)			/* (input) allocate buffer for screen on decompress sequence for faster updates */
#define codecFlagUpdatePrevious (1L<<2)				/* (input) udate previous buffer on compress sequence */
#define codecFlagNoScreenUpdate (1L<<3)				/* (input) dont update screen, but do update image buffer if allocated */
#define codecFlagWasCompressed (1L<<4)				/* (input) hint to compressor that image was previously compressed */
#define codecFlagDontOffscreen (1L<<5)				/* return error instead of automatically going offscreen */
#define codecFlagUpdatePreviousComp (1L<<6)			/* (input) udate previous buffer on compress sequence */
#define codecFlagForceKeyFrame (1L<<7)				/* when sent to CompressSequenceFrame, forces that frame to be a key frame */
#define	codecFlagOnlyScreenUpdate	(1L<<8)		/* (input) only update screen from */
#define	codecFlagLiveGrab			(1L<<9)		/* (input) data being compressed is from live source (speed is critical) */



#define	codecFlagUsedNewImageBuffer	(1L<<14)	/* (output) indicates that image buffer was first used on this decompress sequence */
#define codecFlagUsedImageBuffer (1L<<15)			/* (output) indicates that image buffer was used on this decompress sequence */

#define codecErr -8960								/* the codec returned an error */
#define noCodecErr -8961							/* the specified codec could not be found */
#define codecUnimpErr -8962							/* this feature is not implemented by the specified codec */
#define codecSizeErr -8963
#define codecScreenBufErr -8964						/* the screen buffer could not be allocated */
#define codecImageBufErr -8965						/* the image buffer could not be allocated */
#define codecSpoolErr -8966							/* the compressed data must be in memory (spooling did not work) */
#define codecAbortErr -8967							/* the operation was aborted by the progress proc */
#define codecWouldOffscreenErr -8968				/* an offscreen access would have been used, but wasn't allowed because codecFlagDontOffscreen was set */
#define codecBadDataErr -8969						/* compressed data was found to have inconsistencies */
#define codecDataVersErr -8970						/* compressed data was of a format version that codec couldn't handle */
#define	codecConditionErr	-8972					/* codec can not do requested operation */
#define	codecOpenErr		-8973					/* the codec  could not be opened */


#define	codecMinimumDataSize	32768						/* The minimum data size for spooling in or out data */


#define compressorComponentType OST_imco				/* the type for "Components" which compress images */
#define decompressorComponentType OST_imdc			/* the type for "Components" which decompress images */


typedef Component CompressorComponent;				/* a Component which compresses images */
typedef Component DecompressorComponent;			/* a Component which decompresses images */
typedef Component CodecComponent;					/* a Component which decompresses or compresses images */




#define anyCodec ((CodecComponent)0)				/* take first working codec of given type */
#define bestSpeedCodec ((CodecComponent)-1)			/* take fastest codec of given type */
#define bestFidelityCodec ((CodecComponent)-2)		/* take codec which is most accurate*/
#define bestCompressionCodec ((CodecComponent)-3)	/* take codec of given type that is most accurate */


typedef short sBoolean;								/* to make sure stack frames are right */

typedef long CodecType;								/* type descriptor for codecs i.e: 'appl','jpeg','rle ' */

typedef unsigned short CodecFlags;					/* flags for codec manager calls */

typedef unsigned long CodecQ;




#define codecLosslessQuality 0x400L
#define codecMaxQuality 0x3FFL
#define codecMinQuality 0x000L
#define codecLowQuality 0x100L
#define codecNormalQuality 0x200L
#define codecHighQuality 0x300L

#if 0
typedef F_PASCAL( OSErr ) (*DataProcPtr)(Ptr *dataP,long bytesNeeded,long refCon);  
typedef F_PASCAL( OSErr ) (*FlushProcPtr)(Ptr data,long bytesAdded,long refCon);
#else
typedef iProcPtr	DataProcPtr;
typedef iProcPtr	FlushProcPtr;
#endif


#define codecCompletionSource (1<<0)		/* asynchronous codec is done with source data */
#define codecCompletionDest (1<<1)			/* asynchronous codec is done with destination data */

#if 0
typedef F_PASCAL( void ) (*CompletionProcPtr)(OSErr result,short flags,long refCon);
#else
typedef iProcPtr	CompletionProcPtr;
#endif

enum {

 codecProgressOpen = 0,
 codecProgressUpdatePercent = 1,
 codecProgressClose = 2
};

#if 0
typedef F_PASCAL( OSErr ) (*ProgressProcPtr)(short message,Fixed completeness,long refCon);
#else
typedef iProcPtr	ProgressProcPtr;
#endif

typedef long ImageSequence;

struct ProgressProcRecord {
 ProgressProcPtr progressProc;
 long progressRefCon;
};

typedef struct ProgressProcRecord ProgressProcRecord;
typedef ProgressProcRecord *ProgressProcRecordPtr;

struct CompletionProcRecord {
 CompletionProcPtr completionProc;
 long completionRefCon;
};

typedef struct CompletionProcRecord CompletionProcRecord;
typedef CompletionProcRecord *CompletionProcRecordPtr;

struct DataProcRecord {
 DataProcPtr dataProc;
 long dataRefCon;
};

typedef struct DataProcRecord DataProcRecord;
typedef DataProcRecord *DataProcRecordPtr;

struct FlushProcRecord {
 FlushProcPtr flushProc;
 long flushRefCon;
};

typedef struct FlushProcRecord FlushProcRecord;
typedef FlushProcRecord *FlushProcRecordPtr;

/* 
    The ImageDescription is private data which is produced when an image or sequence 
    is compressed. It fully describes the format of the compressed data.*/

struct ImageDescription {
 long idSize;										/* total size of ImageDescription including extra data ( CLUTs and other per sequence data */
 CodecType cType;									/* what kind of codec compressed this data */
 long resvd1;										/* reserved for apple use */
 short resvd2;										/* reserved for apple use */
 short dataRefIndex;								/* set to zero */
 short version;										/* which version is this data */
 short revisionLevel;								/* what version of that codec did this */
 long vendor;										/* whose  codec compressed this data */
 CodecQ temporalQuality;							/* what was the temporal quality factor          */
 CodecQ spatialQuality;								/* what was the spatial quality factor          */
 short width;										/* how many pixels wide is this data */
 short height;										/* how many pixels high is this data */
 Fixed hRes;										/* horizontal resolution */
 Fixed vRes;										/* vertical resolution */
 long dataSize;										/* if known, the size of data for this image descriptor */
 short frameCount;									/* number of frames this description applies to */
 char name[32];										/* name of codec ( in case not installed ) */
 short depth;										/* what depth is this data (1-32) or ( 33-40 grayscale ) */
 short clutID;										/* clut id or if 0 clut follows  or -1 if no clut */
};

typedef struct ImageDescription ImageDescription;
typedef ImageDescription *ImageDescriptionPtr, **ImageDescriptionHandle;

/* 
    The CodecInfo is the information returned as the codecInfo struct by a codec Component
    to the codec manager or to the caller. It is specific to the particular codec
    implementation and not to the codec type.*/

struct CodecInfo {
 char typeName[32];									/* name of the codec type i.e.: 'Apple Image Compression' */
 short version;										/* version of the codec data that this codec knows about */
 short revisionLevel;								/* revision level of this codec i.e: 0x00010001 (1.0.1) */
 long vendor;										/* Maker of this codec i.e: 'appl' */
 long decompressFlags;								/* codecInfo flags for decompression capabilities */
 long compressFlags;								/* codecInfo flags for compression capabilities */
 long formatFlags;									/* codecInfo flags for compression format details */
 unsigned char compressionAccuracy;					/* measure (1-255) of accuracy of this codec for compress (0 if unknown) */
 unsigned char decompressionAccuracy;				/* measure (1-255) of accuracy of this codec for decompress (0 if unknown) */
 unsigned short compressionSpeed;					/* ( millisecs for compressing 320x240 on base mac II) (0 if unknown) */
 unsigned short decompressionSpeed;					/* ( millisecs for decompressing 320x240 on mac II)(0 if unknown) */
 unsigned char compressionLevel;					/* measure (1-255) of compression level of this codec (0 if unknown) */
 char resvd;										/* pad */
 short minimumHeight;								/* minimum height of image (block size) */
 short minimumWidth;								/* minimum width of image (block size) */
 short decompressPipelineLatency;					/* in milliseconds ( for asynchronous codecs ) */
 short compressPipelineLatency;						/* in milliseconds ( for asynchronous codecs ) */
 long privateData;
};

typedef struct CodecInfo CodecInfo;

/* Table used for fast dithering. */
struct DitherTable {
 long tag;
 long seed;
 long reserved;
 char data[1];
};

typedef struct DitherTable DitherTable;
typedef DitherTable *DTabPtr, **DTabHandle;

/* Name list returned by GetCodecNameList. */
struct CodecNameSpec {
 CodecComponent codec;
 CodecType cType;
 char typeName[32];
 Handle name;
};

typedef struct CodecNameSpec CodecNameSpec;

struct CodecNameSpecList {
 short count;
 CodecNameSpec list[1];
};

typedef struct CodecNameSpecList CodecNameSpecList;
typedef CodecNameSpecList *CodecNameSpecListPtr;

/*  Matrix stuff  */
struct MatrixRecord {
 Fixed matrix[3][3];
};

typedef struct MatrixRecord  MatrixRecord;
typedef MatrixRecord  *MatrixRecordPtr;

#ifndef _FixedPoint_
#define _FixedPoint_
struct FixedPoint {
 Fixed x;
 Fixed y;
};

typedef struct FixedPoint FixedPoint;
#endif _FixedPoint_

#ifndef _FixedRect_
#define _FixedRect_
struct FixedRect {
 Fixed left;
 Fixed top;
 Fixed right;
 Fixed bottom;
};

typedef struct FixedRect FixedRect;
#endif _FixedRect_


F_PASCAL( OSErr ) CodecManagerVersion(long *version); 
F_PASCAL( OSErr ) GetCodecNameList(CodecNameSpecListPtr *CNSlist,sBoolean showAll);
F_PASCAL( OSErr ) DisposeCodecNameList(CodecNameSpecListPtr CNSlist);
F_PASCAL( OSErr ) GetCodecInfo(CodecInfo *info,CodecType cType,CodecComponent codec); 
F_PASCAL( OSErr ) GetMaxCompressionSize(PixMapHandle src,const Rect *srcRect,short colorDepth,
 CodecQ quality,CodecType cType,CompressorComponent codec,long *size); 
F_PASCAL( OSErr ) GetCompressionTime(PixMapHandle src,const Rect *srcRect,short colorDepth,
 CodecType cType,CompressorComponent codec,CodecQ *spatialQuality,CodecQ *temporalQuality,
 unsigned long *compressTime); 
F_PASCAL( OSErr ) CompressImage(PixMapHandle src,const  Rect *srcRect,CodecQ quality,
 CodecType cType,ImageDescriptionHandle desc,Ptr data); 
F_PASCAL( OSErr ) FCompressImage(PixMapHandle src,const  Rect *srcRect,short colorDepth,
 CodecQ quality,CodecType cType,CompressorComponent codec,CTabHandle clut,
 CodecFlags flags,long bufferSize,FlushProcRecordPtr flushProc,ProgressProcRecordPtr progressProc,
 ImageDescriptionHandle desc,Ptr data); 
F_PASCAL( OSErr ) DecompressImage(Ptr data,ImageDescriptionHandle desc,PixMapHandle dst,
 const Rect *srcRect,const  Rect *dstRect,short mode,RgnHandle mask); 
F_PASCAL( OSErr ) FDecompressImage(Ptr data,ImageDescriptionHandle desc,PixMapHandle dst,
 const  Rect *srcRect,MatrixRecordPtr matrix,short mode,RgnHandle mask,PixMapHandle matte,
 const  Rect *matteRect,CodecQ accuracy,DecompressorComponent codec,long bufferSize,
 DataProcRecordPtr dataProc,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) CompressSequenceBegin(ImageSequence *seqID,PixMapHandle src,
 PixMapHandle prev,const Rect *srcRect,const  Rect *prevRect,short colorDepth,CodecType cType,
 CompressorComponent codec,CodecQ spatialQuality,CodecQ temporalQuality,
 long keyFrameRate,CTabHandle clut,CodecFlags flags,ImageDescriptionHandle desc); 
F_PASCAL( OSErr ) CompressSequenceFrame(ImageSequence seqID,PixMapHandle src,
 const Rect *srcRect,CodecFlags flags,Ptr data,long *dataSize,unsigned char *similarity,
 CompletionProcRecordPtr asyncCompletionProc); 
F_PASCAL( OSErr ) DecompressSequenceBegin(ImageSequence *seqID,ImageDescriptionHandle desc,
 CGrafPtr port,GDHandle gdh,const Rect *srcRect,MatrixRecordPtr matrix,short mode,
 RgnHandle mask,CodecFlags flags,CodecQ accuracy,DecompressorComponent codec); 
F_PASCAL( OSErr ) DecompressSequenceFrame(ImageSequence seqID,Ptr data,CodecFlags inFlags,
 CodecFlags *outFlags,CompletionProcRecordPtr asyncCompletionProc); 
F_PASCAL( OSErr ) SetDSequenceMatrix(ImageSequence seqID,MatrixRecordPtr matrix); 
F_PASCAL( OSErr ) SetDSequenceMatte(ImageSequence seqID,PixMapHandle matte,const Rect *matteRect); 
F_PASCAL( OSErr ) SetDSequenceMask(ImageSequence seqID,RgnHandle mask); 
F_PASCAL( OSErr ) SetDSequenceTransferMode(ImageSequence seqID,short mode,const RGBColor *opColor); 
F_PASCAL( OSErr ) SetDSequenceDataProc(ImageSequence seqID,DataProcRecordPtr dataProc,
 long bufferSize); 
F_PASCAL( OSErr ) SetDSequenceAccuracy(ImageSequence seqID,CodecQ accuracy); 
F_PASCAL( OSErr ) SetDSequenceSrcRect(ImageSequence seqID,const Rect *srcRect); 
F_PASCAL( OSErr ) GetDSequenceImageBuffer(ImageSequence seqID,GWorldPtr *gworld); 
F_PASCAL( OSErr ) GetDSequenceScreenBuffer(ImageSequence seqID,GWorldPtr *gworld); 
F_PASCAL( OSErr ) SetCSequenceQuality(ImageSequence seqID,CodecQ spatialQuality,
 CodecQ temporalQuality); 
F_PASCAL( OSErr ) SetCSequencePrev(ImageSequence seqID,PixMapHandle prev,const Rect *prevRect); 
F_PASCAL( OSErr ) SetCSequenceFlushProc(ImageSequence seqID,FlushProcRecordPtr flushProc,
 long bufferSize); 
F_PASCAL( OSErr ) SetCSequenceKeyFrameRate(ImageSequence seqID,long keyframerate); 
F_PASCAL( OSErr ) GetCSequencePrevBuffer(ImageSequence seqID,GWorldPtr *gworld); 
F_PASCAL( OSErr ) CDSequenceBusy(ImageSequence seqID); 
F_PASCAL( OSErr ) CDSequenceEnd(ImageSequence seqID); 
F_PASCAL( OSErr ) GetCompressedImageSize(ImageDescriptionHandle desc,Ptr data,
 long bufferSize,DataProcRecordPtr dataProc,long *dataSize); 
F_PASCAL( OSErr ) GetSimilarity(PixMapHandle src,const Rect *srcRect,ImageDescriptionHandle desc,
 Ptr data,Fixed *similarity); 
F_PASCAL( OSErr ) GetImageDescriptionCTable(ImageDescriptionHandle desc,CTabHandle *ctable); 
F_PASCAL( OSErr ) SetImageDescriptionCTable(ImageDescriptionHandle desc,CTabHandle ctable); 
F_PASCAL( OSErr ) GetImageDescriptionExtension(ImageDescriptionHandle desc,Handle *extension,
 long idType,long index); 
F_PASCAL( OSErr ) SetImageDescriptionExtension(ImageDescriptionHandle desc,Handle extension,
 long idType); 
F_PASCAL( OSErr ) FindCodec(CodecType cType,CodecComponent specCodec,CompressorComponent *compressor,
 DecompressorComponent *decompressor); 
F_PASCAL( OSErr ) CompressPicture(PicHandle srcPicture,PicHandle dstPicture,
 CodecQ quality,CodecType cType); 
F_PASCAL( OSErr ) FCompressPicture(PicHandle srcPicture,PicHandle dstPicture,
 short colorDepth,CTabHandle clut,CodecQ quality,sBoolean doDither,sBoolean compressAgain,
 ProgressProcRecordPtr progressProc,CodecType cType,CompressorComponent codec); 
F_PASCAL( OSErr ) CompressPictureFile(short srcRefNum,short dstRefNum,CodecQ quality,
 CodecType cType); 
F_PASCAL( OSErr ) FCompressPictureFile(short srcRefNum,short dstRefNum,short colorDepth,
 CTabHandle clut,CodecQ quality,sBoolean doDither,sBoolean compressAgain,
 ProgressProcRecordPtr progressProc,CodecType cType,CompressorComponent codec); 
#if 0
F_PASCAL( OSErr ) GetPictureFileHeader(short refNum,Rect *frame,OpenCPicParams *header); 
#else
F_PASCAL( OSErr ) GetPictureFileHeader(short refNum,Rect *frame,void *header); 
#endif
F_PASCAL( OSErr ) DrawPictureFile(short refNum,const Rect *frame,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) DrawTrimmedPicture(PicHandle srcPicture,const Rect *frame,RgnHandle trimMask,
 sBoolean doDither,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) DrawTrimmedPictureFile(short srcRefnum,const Rect *frame,RgnHandle trimMask,
 sBoolean doDither,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) MakeThumbnailFromPicture(PicHandle picture,short colorDepth,
 PicHandle thumbnail,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) MakeThumbnailFromPictureFile(short refNum,short colorDepth,
 PicHandle thumbnail,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) MakeThumbnailFromPixMap(PixMapHandle src,const Rect *srcRect,short colorDepth,
 PicHandle thumbnail,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) TrimImage(ImageDescriptionHandle desc,Ptr inData,long inBufferSize,
 DataProcRecordPtr dataProc,Ptr outData,long outBufferSize,FlushProcRecordPtr flushProc,
 Rect *trimRect,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) ConvertImage(ImageDescriptionHandle srcDD,Ptr srcData,short colorDepth,
 CTabHandle clut,CodecQ accuracy,CodecQ quality,CodecType cType,CodecComponent codec,
 ImageDescriptionHandle dstDD,Ptr dstData); 
F_PASCAL( OSErr ) MakeDitherTable(GDHandle gdev,DTabHandle *ditherTable); 
F_PASCAL( OSErr ) DisposeDitherTable(DTabHandle ditherTable); 
 

F_PASCAL( void ) 
StdPix(PixMapPtr src,const  Rect *srcRect,MatrixRecordPtr matrix,short mode,
	RgnHandle mask,PixMapPtr matte,const Rect *matteRect,short flags);

F_PASCAL( OSErr ) GetCompressedPixMapInfo(PixMapPtr pix,ImageDescriptionHandle *desc,
 Ptr *data,long *bufferSize,DataProcRecord *dataProc,ProgressProcRecord *progressProc); 
F_PASCAL( OSErr ) SetCompressedPixMapInfo(PixMapPtr pix,ImageDescriptionHandle desc,
 Ptr data,long bufferSize,DataProcRecordPtr dataProc,ProgressProcRecordPtr progressProc); 
F_PASCAL( OSErr ) TransformRgn(MatrixRecordPtr mp,RgnHandle r);

enum {
 identityMatrixType			= 0x00, 						/* result if matrix is identity */
 translateMatrixType		= 0x01, 						/* result if matrix translates */
 scaleMatrixType			= 0x02, 						/* result if matrix scales */
 scaleTranslateMatrixType	= 0x03, 						/* result if matrix scales and translates */
 linearMatrixType			= 0x04, 						/* result if matrix is general 2 x 2 */
 linearTranslateMatrixType	= 0x05, 						/* result if matrix is general 2 x 2 and translates */
 perspectiveMatrixType		= 0x06 							/* result if matrix is general 3 x 3 */
};
typedef unsigned char MatrixFlags;


F_PASCAL( short ) GetMatrixType(MatrixRecordPtr m); 
F_PASCAL( void ) CopyMatrix(MatrixRecordPtr m1,MatrixRecord *m2); 
F_PASCAL( Boolean ) EqualMatrix(MatrixRecordPtr m1,MatrixRecordPtr m2); 
F_PASCAL( void ) SetIdentityMatrix(MatrixRecord *matrix); 
F_PASCAL( void ) TranslateMatrix(MatrixRecord *m,Fixed deltaH,Fixed deltaV); 
F_PASCAL( void ) RotateMatrix(MatrixRecord *m,Fixed degrees,Fixed aboutX,Fixed aboutY); 
F_PASCAL( void ) ScaleMatrix(MatrixRecord *m,Fixed scaleX,Fixed scaleY,Fixed aboutX,
 Fixed aboutY); 
F_PASCAL( void ) SkewMatrix(MatrixRecord *m,Fixed skewX,Fixed skewY,Fixed aboutX,
 Fixed aboutY); 
F_PASCAL( OSErr ) TransformFixedPoints(MatrixRecordPtr m,FixedPoint *fpt,long count); 
F_PASCAL( OSErr ) TransformPoints(MatrixRecordPtr mp,Point *pt1,long count); 
F_PASCAL( Boolean ) TransformFixedRect(MatrixRecordPtr m,FixedRect *fr,FixedPoint *fpp); 
F_PASCAL( Boolean ) TransformRect(MatrixRecordPtr m,Rect *r,FixedPoint *fpp); 
F_PASCAL( Boolean ) InverseMatrix(MatrixRecordPtr m,MatrixRecord *im); 
F_PASCAL( void ) ConcatMatrix(MatrixRecordPtr a,MatrixRecord *b); 
F_PASCAL( void ) RectMatrix(MatrixRecord *matrix,Rect *srcRect,Rect *dstRect); 
F_PASCAL( void ) MapMatrix(MatrixRecord *matrix,Rect *fromRect,Rect *toRect); 

/*  Standard Preview */

F_PASCAL( void ) SFGetFilePreview(Point where,
                      ConstStr255Param prompt,
                      FileFilterProcPtr fileFilter,
                      short numTypes,
                      SFTypeList typeList,
                      DlgHookProcPtr dlgHook,
                      SFReply *reply); 

F_PASCAL( void ) SFPGetFilePreview(Point where,
                       ConstStr255Param prompt,
                       FileFilterProcPtr fileFilter,
                       short numTypes,
                       SFTypeList typeList,
                       DlgHookProcPtr dlgHook,
                       SFReply *reply,
                       short dlgID,
                       ModalFilterProcPtr filterProc); 

F_PASCAL( void ) StandardGetFilePreview(FileFilterProcPtr fileFilter,
                            short numTypes,
                            SFTypeList typeList,
                            StandardFileReply *reply); 

F_PASCAL( void ) CustomGetFilePreview( FileFilterYDProcPtr fileFilter,
                          short numTypes,
                          SFTypeList typeList,
                          StandardFileReply *reply,
                          short dlgID,
                          Point where,
                          DlgHookYDProcPtr dlgHook,
                          ModalFilterYDProcPtr filterProc,
                          short *activeList,
                          ActivateYDProcPtr activateProc,
                          void *yourDataPtr); 

F_PASCAL( OSErr ) MakeFilePreview(short resRefNum, ProgressProcRecordPtr progress); 
F_PASCAL( OSErr ) AddFilePreview(short resRefNum, OSType previewType, Handle previewData); 

enum {
    sfpItemPreviewAreaUser = 11,
    sfpItemPreviewStaticText = 12,
    sfpItemPreviewDividerUser = 13,
    sfpItemCreatePreviewButton = 14,
    sfpItemShowPreviewButton = 15
};

struct PreviewResourceRecord {
	unsigned long	modDate;
	short			version;
	OSType			resType;
	short			resID;
};

typedef struct PreviewResourceRecord PreviewResourceRecord;
typedef PreviewResourceRecord *PreviewResourcePtr, **PreviewResource;

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

