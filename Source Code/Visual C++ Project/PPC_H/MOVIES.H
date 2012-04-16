/************************************************************

Created: Wednesday, August 14, 1991 at 12:23 PM
 Movies.h
 C Interface to the Macintosh Libraries


  Copyright Apple Computer, Inc. 1991
  All rights reserved


************************************************************/


#ifndef __MOVIES__
#define __MOVIES__

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
#include <Quickdraw.H>
#else
#include <Quickdra.H>
#endif
#endif

#ifndef __QDOFFSCREEN__
#if LONG_FILENAMES
#include <QDOffscreen.H>
#else
#include <QDOffscr.H>
#endif
#endif

#ifndef __EVENTS__
#include <Events.H>
#endif

#ifndef __ALIASES__
#include <Aliases.H>
#endif

#ifndef __COMPONENTS__
#if LONG_FILENAMES
#include <Components.H>
#else
#include <Componen.H>
#endif
#endif

#ifndef __IMAGECOMPRESSION__
#if LONG_FILENAMES
#include <ImageCompression.H>
#else
#include <ImageCom.H>
#endif
#endif


#define gestaltQuickTime OST_qtim

#define MovieFileType OST_MooV

#define VideoMediaType OST_vide
#define SoundMediaType OST_soun

#define DoTheRightThing 0L

#if 0
// These are enums in Sound.h  causing compile error and
// were not in the original MAC version of Movies.h,
// include Sound.h to reference these.  RHK 07/06/95
#define kFullVolume 0x100			/* 8.8 format */
#define kNoVolume 0
#endif

struct MovieRecord {
 long data[1];
};

typedef struct MovieRecord MovieRecord;
typedef MovieRecord *Movie;

struct TrackRecord {
 long data[1];
};

typedef struct TrackRecord TrackRecord;
typedef TrackRecord *Track;

struct MediaRecord {
 long data[1];
};

typedef struct MediaRecord MediaRecord;
typedef MediaRecord *Media;

struct UserDataRecord {
 long data[1];
};

typedef struct UserDataRecord UserDataRecord;
typedef UserDataRecord *UserData;

struct TrackEditStateRecord {
 long data[1];
};

typedef struct TrackEditStateRecord TrackEditStateRecord;
typedef TrackEditStateRecord *TrackEditState;

struct MovieEditStateRecord {
 long data[1];
};

typedef struct MovieEditStateRecord MovieEditStateRecord;
typedef MovieEditStateRecord *MovieEditState;


typedef long TimeValue;
typedef long TimeScale;

struct TimeBaseRecord {
 long data[1];
};

typedef struct TimeBaseRecord TimeBaseRecord;
typedef TimeBaseRecord *TimeBase;

struct CallBackRecord {
 long data[1];
};

typedef struct CallBackRecord CallBackRecord;
typedef CallBackRecord *QTCallBack;


struct Int64 {
#if WINVER	// RB 5/6/94: QTW reverses these because of Intel byte order
 long lo;
 long hi;
#else
 long hi;
 long lo;
#endif
};

typedef struct Int64 Int64;

struct TimeRecord {
 Int64 value;
 TimeScale scale;
 TimeBase base;
};

typedef struct TimeRecord TimeRecord;

struct SampleDescription {
 long descSize;
 long dataFormat;
 long resvd1;
 short resvd2;
 short dataRefIndex;
};

typedef struct SampleDescription SampleDescription;
typedef SampleDescription *SampleDescriptionPtr, **SampleDescriptionHandle;

struct SoundDescription {
 long descSize;
 long dataFormat;
 long resvd1;
 short resvd2;
 short dataRefIndex;
 short version;
 short revlevel;
 long vendor;
 short numChannels;
 short sampleSize;
 short compressionID;
 short packetSize;
 Fixed sampleRate;
};

typedef struct SoundDescription SoundDescription;
typedef SoundDescription *SoundDescriptionPtr, **SoundDescriptionHandle;


enum {movieProgressOpen = 0,movieProgressUpdatePercent = 1,movieProgressClose = 2};
typedef unsigned char movieProgressMessages;

enum {progressOpFlatten = 1,progressOpInsertTrackSegment = 2,progressOpInsertMovieSegment = 3,
 progressOpPaste = 4,progressOpAddMovieSelection = 5,progressOpCopy = 6,
 progressOpCut = 7,progressOpLoadMovieIntoRam = 8,progressOpLoadTrackIntoRam = 9,
	progressOpLoadMediaIntoRam = 10};
typedef unsigned char movieProgressOperations;

enum {mediaQualityDraft = 0x0000, mediaQualityNormal = 0x0040, mediaQualityBetter = 0x0080,
	mediaQualityBest = 0x00C0};

typedef OSErr (pascal * MovieProgressProcPtr)	(Movie m, short message, short whatOperation, Fixed percentDone, long refCon);        
typedef OSErr (pascal * MovieRgnCoverProc)		(Movie m, RgnHandle changedRgn, long refcon);  

typedef Handle MediaInformationHandle;
typedef ComponentInstance MediaHandler;
typedef Component MediaHandlerComponent;
typedef ComponentInstance DataHandler;
typedef Component DataHandlerComponent;
typedef ComponentResult HandlerError;
typedef ComponentInstance MovieController;

typedef void (pascal * ErrorProcPtr)(OSErr theErr, long refcon);        
typedef Boolean (pascal * MoviePreviewCallOutProc)(long refcon);        

enum {newMovieActive = 1<<0,newMovieDontResolveDataRefs = 1<<1,newMovieDontAskUnresolvedDataRefs = 1<<2};
typedef unsigned char newMovieFlagsEnum;

enum {trackUsageInMovie = 1<<1,trackUsageInPreview = 1<<2,trackUsageInPoster = 1<<3};
typedef unsigned char trackUsageEnum;

enum {mediaSampleNotSync = 1};
typedef unsigned char mediaSampleFlagsEnum;

enum {nextTimeMediaSample = 1<<0,nextTimeMediaEdit = 1<<1,nextTimeTrackEdit = 1<<2,
 nextTimeSyncSample = 1<<3,nextTimeEdgeOK = 1<<14,nextTimeIgnoreActiveSegment = 1<<15};
typedef unsigned short nextTimeFlagsEnum;

#define createMovieFileDeleteCurFile	(1L << 31)
#define createMovieFileDontCreateMovie	(1L << 30)
#define createMovieFileDontOpenFile	(1L << 29)

typedef unsigned long createMovieFileFlagsEnum;

enum {flattenAddMovieToDataFork = 1L<<0,flattenActiveTracksOnly = 1L<<2,
 flattenDontInterleaveFlatten = 1L<<3};
typedef unsigned char movieFlattenFlagsEnum;

enum {movieScrapDontZeroScrap = 1<<0,movieScrapOnlyPutMovie = 1<<1};
typedef unsigned char movieScrapFlagsEnum;

enum {hintsScrubMode = 1<<0};
typedef unsigned char playHintsEnum;


#ifdef WIN32
#if QTMLWindows
#include "QTMLName.h"
#define F_QTML(typ) typ __declspec(dllimport) __cdecl
#elif	BuildMac2Win
#include "MovieNam.h"
#define F_QTML(typ) __declspec(dllexport) F_PASCAL(typ)
#else
#include "MovieNam.h"
#if __MWERKS__
#define F_QTML(typ) F_PASCAL(typ)
#else
#define F_QTML(typ) __declspec(dllimport) F_PASCAL(typ)
#endif
#endif
#else
#define F_QTML(typ) F_PASCAL(typ)
#endif



/*  Initialization Routines  */
F_QTML( OSErr ) EnterMovies(void); 
F_QTML( void ) ExitMovies(void); 

/*  error Routines  */
F_QTML( OSErr ) GetMoviesError(void); 
F_QTML( void ) ClearMoviesStickyError(void); 
F_QTML( OSErr ) GetMoviesStickyError(void); 

F_QTML( void ) SetMoviesErrorProc(ErrorProcPtr errProc,long refcon); 

F_QTML( ComponentResult ) GetTrackStatus(Track t); 
F_QTML( ComponentResult ) GetMovieStatus(Movie m,Track *firstProblemTrack); 

/*  Playback Routines  */
F_QTML( void ) MoviesTask(Movie m,long maxMilliSecToUse); 

F_QTML( void ) StartMovie(Movie m); 
F_QTML( void ) StopMovie(Movie m); 
F_QTML( void ) GoToBeginningOfMovie(Movie m); 
F_QTML( void ) GoToEndOfMovie(Movie m); 
F_QTML( Boolean ) IsMovieDone(Movie m); 

F_QTML( Boolean ) GetMoviePreviewMode(Movie m); 
F_QTML( void ) SetMoviePreviewMode(Movie m,Boolean usePreview); 
F_QTML( void ) ShowMoviePoster(Movie m); 

F_QTML( void ) GetMovieGWorld(Movie m,CGrafPtr *port,GDHandle *gdh); 
F_QTML( void ) SetMovieGWorld(Movie m,CGrafPtr port,GDHandle gdh); 

F_QTML( OSErr ) PrerollMovie(Movie m,TimeValue time,Fixed Rate); 
F_QTML( OSErr ) LoadMovieIntoRam(Movie m,TimeValue time,TimeValue duration,
 long flags); 
F_QTML( OSErr ) LoadTrackIntoRam(Track t,TimeValue time,TimeValue duration,
 long flags); 
F_QTML( OSErr ) LoadMediaIntoRam(Media m,TimeValue time,TimeValue duration,
 long flags); 

F_QTML( void ) SetMovieActive(Movie m,Boolean active); 
F_QTML( Boolean ) GetMovieActive(Movie m); 

F_QTML( void ) PlayMoviePreview(Movie m,MoviePreviewCallOutProc callOutProc,
 long refcon); 

/*  Movie PICT Routines  */
F_QTML( PicHandle ) GetMoviePict(Movie m,TimeValue time); 
F_QTML( PicHandle ) GetTrackPict(Track t,TimeValue time); 

F_QTML( PicHandle ) GetMoviePosterPict(Movie m); 

/*  movie TimeBase Routines  */
F_QTML( TimeBase ) GetMovieTimeBase(Movie m); 

F_QTML( void ) SetMovieMasterTimeBase(Movie m,TimeBase tb,const TimeRecord *slaveZero); 
F_QTML( void ) SetMovieMasterClock(Movie m,Component clockMeister,const TimeRecord *slaveZero); 

/*  spatial movie Routines  */
F_QTML( void ) GetMovieBox(Movie m,Rect *boxRect); 
F_QTML( void ) SetMovieBox(Movie m,const Rect *boxRect); 

/*  movie display clip  */
F_QTML( RgnHandle ) GetMovieDisplayClipRgn(Movie m); 
F_QTML( void ) SetMovieDisplayClipRgn(Movie m,RgnHandle theClip); 

/*  movie clip  */
F_QTML( RgnHandle ) GetMovieClipRgn(Movie m); 
F_QTML( void ) SetMovieClipRgn(Movie m,RgnHandle theClip); 

/*  track clip  */
F_QTML( RgnHandle ) GetTrackClipRgn(Track t); 
F_QTML( void ) SetTrackClipRgn(Track t,RgnHandle theClip); 

/*  bounds in display space  */
F_QTML( RgnHandle ) GetMovieDisplayBoundsRgn(Movie m); 
F_QTML( RgnHandle ) GetTrackDisplayBoundsRgn(Track t); 

/*  bounds in movie space  */
F_QTML( RgnHandle ) GetMovieBoundsRgn(Movie m); 
F_QTML( RgnHandle ) GetTrackMovieBoundsRgn(Track t); 

/*  bounds in track space  */
F_QTML( RgnHandle ) GetTrackBoundsRgn(Track t); 

/*  movie poster box  */
F_QTML( void ) GetPosterBox(Movie m,Rect *boxRect); 
F_QTML( void ) SetPosterBox(Movie m,const Rect *boxRect); 

/*  movie/track bounds over a range of time  */
F_QTML( RgnHandle ) GetMovieSegmentDisplayBoundsRgn(Movie m,TimeValue time,
 TimeValue duration); 
F_QTML( RgnHandle ) GetTrackSegmentDisplayBoundsRgn(Track t,TimeValue time,
 TimeValue duration); 

F_QTML( void ) SetMovieCoverProcs(Movie m,MovieRgnCoverProc uncoverProc,MovieRgnCoverProc coverProc,
 long refCon); 

/*  mattes - always in track space  */
F_QTML( PixMapHandle ) GetTrackMatte(Track t); 
F_QTML( void ) SetTrackMatte(Track t,PixMapHandle theMatte); 
F_QTML( void ) DisposeMatte(PixMapHandle theMatte); 

/*  movie creation Routines  */
F_QTML( OSErr ) NewMovieFromHandle(Movie *m,Handle h,short newMovieFlags,Boolean *dataRefWasChanged); 
F_QTML( OSErr ) PutMovieIntoHandle(Movie m,Handle h); 

F_QTML( Movie ) NewMovie(long newMovieFlags); 
F_QTML( void ) DisposeMovie(Movie m); 

/*  Movie Routines  */
F_QTML( unsigned long ) GetMovieCreationTime(Movie m); 
F_QTML( unsigned long ) GetMovieModificationTime(Movie m); 

F_QTML( TimeScale ) GetMovieTimeScale(Movie m); 
F_QTML( void ) SetMovieTimeScale(Movie m,TimeScale timeScale); 

F_QTML( TimeValue ) GetMovieDuration(Movie m); 

F_QTML( Fixed ) GetMovieRate(Movie m); 
F_QTML( void ) SetMovieRate(Movie m,Fixed rate); 

F_QTML( Fixed ) GetMoviePreferredRate(Movie m); 
F_QTML( void ) SetMoviePreferredRate(Movie m,Fixed rate); 

F_QTML( short ) GetMoviePreferredVolume(Movie m); 
F_QTML( void ) SetMoviePreferredVolume(Movie m,short volume); 

F_QTML( short ) GetMovieVolume(Movie m); 
F_QTML( void ) SetMovieVolume(Movie m,short volume); 

F_QTML( void ) GetMovieMatrix(Movie m,MatrixRecord *matrix); 
F_QTML( void ) SetMovieMatrix(Movie m,const MatrixRecord *matrix); 

F_QTML( void ) GetMoviePreviewTime(Movie m,TimeValue *previewTime,TimeValue *previewDuration); 
F_QTML( void ) SetMoviePreviewTime(Movie m,TimeValue previewTime,TimeValue previewDuration); 

F_QTML( TimeValue ) GetMoviePosterTime(Movie m); 
F_QTML( void ) SetMoviePosterTime(Movie m,TimeValue posterTime); 

F_QTML( void ) GetMovieSelection(Movie m,TimeValue *selectionTime,TimeValue *selectionDuration); 
F_QTML( void ) SetMovieSelection(Movie m,TimeValue selectionTime,TimeValue selectionDuration); 

F_QTML( void ) SetMovieActiveSegment(Movie m,TimeValue startTime,TimeValue duration); 
F_QTML( void ) GetMovieActiveSegment(Movie m,TimeValue *startTime,TimeValue *duration); 

F_QTML( TimeValue ) GetMovieTime(Movie m,TimeRecord *currentTime); 
F_QTML( void ) SetMovieTime(Movie m,const TimeRecord *newtime); 
F_QTML( void ) SetMovieTimeValue(Movie m,TimeValue newtime); 

F_QTML( UserData ) GetMovieUserData(Movie m); 

F_QTML( void ) SetMoviePlayHints(Movie movie,long flags,long flagsMask);

/*  Movie/Track/Media finding Routines  */
F_QTML( long ) GetMovieTrackCount(Movie m); 
F_QTML( Track ) GetMovieTrack(Movie m,long trackID); 

F_QTML( Track ) GetMovieIndTrack(Movie m,long index); 

F_QTML( long ) GetTrackID(Track t); 

F_QTML( Movie ) GetTrackMovie(Track t); 

F_QTML( Media ) GetTrackMedia(Track t); 
F_QTML( Track ) GetMediaTrack(Media m); 

/*  Track creation Routines  */
F_QTML( Track ) NewMovieTrack(Movie m,Fixed width,Fixed height,short trackVolume); 
F_QTML( void ) DisposeMovieTrack(Track t); 

/*  Track Routines  */
F_QTML( unsigned long ) GetTrackCreationTime(Track t); 
F_QTML( unsigned long ) GetTrackModificationTime(Track t); 

F_QTML( Boolean ) GetTrackEnabled(Track t); 
F_QTML( void ) SetTrackEnabled(Track t,Boolean isEnabled); 

F_QTML( long ) GetTrackUsage(Track t); 
F_QTML( void ) SetTrackUsage(Track t,long usage); 

F_QTML( TimeValue ) GetTrackDuration(Track t); 

F_QTML( TimeValue ) GetTrackOffset(Track t); 
F_QTML( void ) SetTrackOffset(Track t,TimeValue offset); 

F_QTML( short ) GetTrackLayer(Track t); 
F_QTML( void ) SetTrackLayer(Track t,short layer); 

F_QTML( Track ) GetTrackAlternate(Track t); 
F_QTML( void ) SetTrackAlternate(Track t,Track alternateT); 

F_QTML( short ) GetTrackVolume(Track t); 
F_QTML( void ) SetTrackVolume(Track t,short volume); 

F_QTML( void ) GetTrackMatrix(Track t,MatrixRecord *matrix); 
F_QTML( void ) SetTrackMatrix(Track t,const MatrixRecord *matrix); 

F_QTML( void ) GetTrackDimensions(Track t,Fixed *width,Fixed *height); 
F_QTML( void ) SetTrackDimensions(Track t,Fixed width,Fixed height); 

F_QTML( UserData ) GetTrackUserData(Track t); 

/*  Media creation Routines  */
F_QTML( Media ) NewTrackMedia(Track t,OSType mediaType,TimeScale timeScale,
 Handle dataRef,OSType dataRefType); 
F_QTML( void ) DisposeTrackMedia(Media m); 

/*  Media Routines  */
F_QTML( unsigned long ) GetMediaCreationTime(Media m); 
F_QTML( unsigned long ) GetMediaModificationTime(Media m); 

F_QTML( TimeScale ) GetMediaTimeScale(Media m); 
F_QTML( void ) SetMediaTimeScale(Media m,TimeScale timeScale); 

F_QTML( TimeValue ) GetMediaDuration(Media m); 

F_QTML( short ) GetMediaLanguage(Media m); 
F_QTML( void ) SetMediaLanguage(Media m,short language); 

F_QTML( short ) GetMediaQuality(Media m); 
F_QTML( void ) SetMediaQuality(Media m,short quality); 

F_QTML( UserData ) GetMediaUserData(Media m); 

F_QTML( void ) SetMediaPlayHints(Movie movie,long flags,long flagsMask);

/*  Media Handler Routines  */
F_QTML( MediaHandler ) GetMediaHandler(Media m); 
F_QTML( OSErr ) SetMediaHandler(Media m,MediaHandlerComponent mH); 

F_QTML( void ) GetMediaHandlerDescription(Media m,OSType *mediaType, Str255 creatorName,
 OSType *creatorManufacturer); 

enum {dataRefSelfReference = 1<<0,dataRefWasNotResolved = 1<<1};
typedef unsigned char dataRefAttributesFlags;


/*  Media dataRef Routines  */
F_QTML( OSErr ) GetMediaDataRef(Media m,short index,Handle *dataRef,OSType *dataRefType,
 long *dataRefAttributes); 
F_QTML( OSErr ) AddMediaDataRef(Media m,short *index,Handle dataRef,OSType dataRefType); 

F_QTML( OSErr ) GetMediaDataRefCount(Media m,short *count); 

/*  Media data handler Routines  */
F_QTML( DataHandler ) GetMediaDataHandler(Media m,short index); 
F_QTML( OSErr ) SetMediaDataHandler(Media m,short index,DataHandlerComponent dataHandler); 

F_QTML( void ) GetMediaDataHandlerDescription(Media m,short index,OSType *dhType,
 Str255 creatorName,OSType *creatorManufacturer); 

/*  Media sample Routines  */
F_QTML( long ) GetMediaSampleDescriptionCount(Media m); 
F_QTML( void ) GetMediaSampleDescription(Media m,long index,SampleDescriptionHandle descH); 
F_QTML( long ) GetMediaSampleCount(Media m); 
F_QTML( void ) SampleNumToMediaTime(Media m,long logicalSampleNum,TimeValue *sampleTime,
 TimeValue *sampleDuration); 
F_QTML( void ) MediaTimeToSampleNum(Media m,TimeValue time,long *sampleNum,
 TimeValue *sampleTime,TimeValue *sampleDuration); 

F_QTML( OSErr ) AddMediaSample(Media m,Handle dataIn,long inOffset,unsigned long size,
 TimeValue durationPerSample,SampleDescriptionHandle sampleDescriptionH,
 long numberOfSamples,short sampleFlags,TimeValue *sampleTime); 
F_QTML( OSErr ) AddMediaSampleReference(Media m,long dataOffset,unsigned long size,
 TimeValue durationPerSample,SampleDescriptionHandle sampleDescriptionH,
 long numberOfSamples,short sampleFlags,TimeValue *sampleTime); 
F_QTML( OSErr ) GetMediaSample(Media m,Handle dataOut,long maxSizeToGrow,long *size,
 TimeValue time,TimeValue *sampleTime,TimeValue *durationPerSample,SampleDescriptionHandle sampleDescriptionH,
 long *sampleDescriptionIndex,long maxNumberOfSamples,long *numberOfSamples,
 short *sampleFlags); 
F_QTML( OSErr ) GetMediaSampleReference(Media m,long *dataOffset,long *size,
 TimeValue time,TimeValue *sampleTime,TimeValue *durationPerSample,SampleDescriptionHandle sampleDescriptionH,
 long *sampleDescriptionIndex,long maxNumberOfSamples,long *numberOfSamples,
 short *sampleFlags); 

/*  High level editing Routines  */
F_QTML( Movie ) CutMovieSelection(Movie m); 
F_QTML( Movie ) CopyMovieSelection(Movie m); 
F_QTML( void ) PasteMovieSelection(Movie m,Movie src); 
F_QTML( void ) AddMovieSelection(Movie m,Movie src); 
F_QTML( void ) ClearMovieSelection(Movie m); 

/*  Middle-level Editing Routines  */
F_QTML( OSErr ) InsertTrackSegment(Track srcTrack,Track dstTrack,TimeValue srcIn,
 TimeValue srcDuration,TimeValue dstIn); 
F_QTML( OSErr ) InsertMovieSegment(Movie srcMovie,Movie dstMovie,TimeValue srcIn,
 TimeValue srcDuration,TimeValue dstIn); 
F_QTML( OSErr ) InsertEmptyTrackSegment(Track dstTrack,TimeValue dstIn,TimeValue dstDuration); 
F_QTML( OSErr ) InsertEmptyMovieSegment(Movie dstMovie,TimeValue dstIn,TimeValue dstDuration); 
F_QTML( OSErr ) DeleteTrackSegment(Track t,TimeValue dstIn,TimeValue duration); 
F_QTML( OSErr ) DeleteMovieSegment(Movie m,TimeValue dstIn,TimeValue duration); 
F_QTML( OSErr ) ScaleTrackSegment(Track t,TimeValue dstIn,TimeValue oldDuration,
 TimeValue newDuration); 
F_QTML( OSErr ) ScaleMovieSegment(Movie m,TimeValue dstIn,TimeValue oldDuration,
 TimeValue newDuration); 

F_QTML( OSErr ) CopyTrackSettings(Track srcTrack,Track dstTrack); 
F_QTML( OSErr ) CopyMovieSettings(Movie srcMovie,Movie dstMovie); 

/*  low-level Editing Routines  */
F_QTML( OSErr ) InsertMediaIntoTrack(Track t,TimeValue trackStart,TimeValue mediaTime,
 TimeValue mediaDuration,Fixed mediaRate); 

F_QTML( OSErr ) BeginMediaEdits(Media m); 
F_QTML( OSErr ) EndMediaEdits(Media m); 

/*  movie & track edit state Routines  */
F_QTML( MovieEditState ) NewMovieEditState(Movie m); 
F_QTML( OSErr ) UseMovieEditState(Movie m,MovieEditState toState); 
F_QTML( OSErr ) DisposeMovieEditState(MovieEditState state); 

F_QTML( TrackEditState ) NewTrackEditState(Track t); 
F_QTML( OSErr ) UseTrackEditState(Track t,TrackEditState state); 
F_QTML( OSErr ) DisposeTrackEditState(TrackEditState state); 

/*  Miscellaneous Routines  */
F_QTML( TimeValue ) TrackTimeToMediaTime(TimeValue value,Track t); 

F_QTML( long ) GetMovieDataSize(Movie m,TimeValue startTime,TimeValue duration); 
F_QTML( long ) GetTrackDataSize(Track t,TimeValue startTime,TimeValue duration); 
F_QTML( long ) GetMediaDataSize(Media m,TimeValue startTime,TimeValue duration); 

F_QTML( Boolean ) PtInMovie(Movie m,Point pt); 
F_QTML( Boolean ) PtInTrack(Track t,Point pt); 

F_QTML( OSErr ) UpdateMovie(Movie m); 

/*  Alternate Group Selection Routines  */
F_QTML( void ) SetMovieLanguage(Movie m,long language); 

F_QTML( void ) SetAutoTrackAlternatesEnabled(Movie m,Boolean enable); 
F_QTML( void ) SelectMovieAlternates(Movie m); 

/*  UserData Routines  */
F_QTML( OSErr ) GetUserData(UserData u,Handle data, OSType udType,long index); 
F_QTML( OSErr ) AddUserData(UserData u,Handle data,OSType udType); 
F_QTML( OSErr ) RemoveUserData(UserData u,OSType udType,long index); 
F_QTML( OSType ) GetNextUserDataType(UserData u, OSType udType); 
F_QTML( short ) CountUserDataType(UserData u,OSType udType); 
F_QTML( OSErr ) AddUserDataText(UserData u,Handle data,OSType udType,long index,
 short itlRegionTag); 
F_QTML( OSErr ) GetUserDataText(UserData u,Handle data,OSType udType,long index,
 short itlRegionTag); 
F_QTML( OSErr ) RemoveUserDataText(UserData u,OSType udType,long index,short itlRegionTag); 

/*  interesting time Routines  */
F_QTML( void ) GetMediaNextInterestingTime(Media m,short interestingTimeFlags,
 TimeValue time,Fixed rate,TimeValue *interestingTime,TimeValue *interestingDuration); 
F_QTML( void ) GetTrackNextInterestingTime(Track t,short interestingTimeFlags,
 TimeValue time,Fixed rate,TimeValue *interestingTime,TimeValue *interestingDuration); 
F_QTML( void ) GetMovieNextInterestingTime(Movie m,short interestingTimeFlags,
 short numMediaTypes,const OSType *whichMediaTypes,TimeValue time,Fixed rate,
 TimeValue *interestingTime,TimeValue *interestingDuration); 

/*  movie file Routines  */
F_QTML( OSErr ) CreateMovieFile(const FSSpec *fileSpec,OSType creator,ScriptCode scriptTag,
 long createMovieFileFlags,short *resRefNum,Movie *newMovie); 
F_QTML( OSErr ) DeleteMovieFile(const FSSpec *fileSpec); 

F_QTML( OSErr ) OpenMovieFile(const FSSpec *fileSpec,short *resRefNum, char perms); 
F_QTML( OSErr ) CloseMovieFile(short resRefNum); 
enum {

 movieInDataForkResID = -1			/* magic res ID */

};

F_QTML( OSErr ) NewMovieFromFile(Movie *m,short resRefNum,short *resId, StringPtr resName,
 short newMovieFlags,Boolean *dataRefWasChanged); 

F_QTML( OSErr ) AddMovieResource(Movie m,short resRefNum,short *resId,const StringPtr resName); 
F_QTML( OSErr ) RemoveMovieResource(short resRefNum,short resId); 
F_QTML( OSErr ) UpdateMovieResource(Movie m,short resRefNum,short resId,const StringPtr resName); 

F_QTML( Boolean ) HasMovieChanged(Movie m); 
F_QTML( void ) ClearMovieChanged(Movie m); 

F_QTML( void ) FlattenMovie(Movie m,long movieFlattenFlags,const FSSpec *theFile,
 OSType creator,ScriptCode scriptTag,long createMovieFileFlags,short *resId,
 const StringPtr resName); 
F_QTML( Movie ) FlattenMovieData(Movie m,long movieFlattenFlags,const FSSpec *theFile,
 OSType creator,ScriptCode scriptTag,long createMovieFileFlags); 
F_QTML( void ) SetMovieProgressProc(Movie m,MovieProgressProcPtr p,long refCon); 

F_QTML( OSErr ) PutMovieOnScrap(Movie m,long movieScrapFlags); 
F_QTML( Movie ) NewMovieFromScrap(long newMovieFlags); 

/*  Video Media Routines  */
F_QTML( HandlerError ) GetVideoMediaGraphicsMode(MediaHandler mh,long *graphicsMode,
 RGBColor *opColor); 
F_QTML( HandlerError ) SetVideoMediaGraphicsMode(MediaHandler mh,long graphicsMode,
 const RGBColor *opColor); 

/*  Sound Media Routines  */
F_QTML( HandlerError ) GetSoundMediaBalance(MediaHandler mh,short *balance); 
F_QTML( HandlerError ) SetSoundMediaBalance(MediaHandler mh,short balance); 


#ifndef __ERRORS__
enum {
 couldNotResolveDataRef = -2000,
 badImageDescription = -2001,
 badPublicMovieAtom = -2002,
 cantFindHandler = -2003,
 cantOpenHandler = -2004,
 badComponentType = -2005,
 noMediaHandler = -2006,
 noDataHandler = -2007,
 invalidMedia = -2008,
 invalidTrack = -2009,
 invalidMovie = -2010,
 invalidSampleTable = -2011,
 invalidDataRef = -2012,
 invalidHandler = -2013,
 invalidDuration = -2014,
 invalidTime = -2015,
 cantPutPublicMovieAtom = -2016,
 badEditList = -2017,
 mediaTypesDontMatch = -2018,
 progressProcAborted = -2019
};
enum {
 movieToolboxUnitialized = -2020,
 wfFileNotFound = -2021,
 cantCreateSingleForkFile = -2022,
 invalidEditState = -2023,
 nonMatchingEditState = -2024,
 staleEditState = -2025,
 userDataItemNotFound = -2026,
 maxSizeToGrowTooSmall = -2027,
 badTrackIndex = -2028,
 trackIDNotFound = -2029,
 trackNotInMovie = -2030,
 timeNotInTrack = -2031,
 timeNotInMedia = -2032,
 badEditIndex = -2033,
 internalQuickTimeError = -2034,
 cantEnableTrack = -2035,
 invalidRect = -2036,
 invalidSampleNum = -2037,
 invalidChunkNum = -2038,
 invalidSampleDescIndex = -2039
};
enum {
 invalidChunkCache = -2040,
 invalidSampleDescription = -2041,
 dataNotOpenForRead = -2042,
 dataNotOpenForWrite = -2043,
 dataAlreadyOpenForWrite = -2044,
 dataAlreadyClosed = -2045,
 endOfDataReached = -2046,
 dataNoDataRef = -2047,
 noMovieFound = -2048,
 invalidDataRefContainer = -2049,
 badDataRefIndex = -2050,
 noDefaultDataRef = -2051,
 couldNotUseAnExistingSample = -2052,
 featureUnsupported = -2053
};
#endif

/* TimeBase Routines */
enum {loopTimeBase = 1, palindromeLoopTimeBase = 2};
typedef unsigned char TimeBaseFlags;

enum {
 triggerTimeFwd       = 0x0001,  	/* when curTime exceeds triggerTime going forward */
 triggerTimeBwd       = 0x0002,  	/* when curTime exceeds triggerTime going backwards */
 triggerTimeEither    = 0x0003,  	/* when curTime exceeds triggerTime going either direction */
 triggerRateLT        = 0x0004,  	/* when rate changes to less than trigger value */
 triggerRateGT        = 0x0008,  	/* when rate changes to greater than trigger value */
 triggerRateEqual     = 0x0010,  	/* when rate changes to equal trigger value */
 triggerRateLTE       = triggerRateLT | triggerRateEqual,
 triggerRateGTE       = triggerRateGT | triggerRateEqual,
 triggerRateNotEqual  = triggerRateGT | triggerRateEqual | triggerRateLT,
 triggerRateChange    = 0
};
typedef unsigned char QTCallBackFlags;

enum {timeBaseBeforeStartTime = 1,timeBaseAfterStopTime = 2};
typedef unsigned char TimeBaseStatus;

enum {callBackAtTime = 1,callBackAtRate = 2,callBackAtTimeJump = 3};
#define	callBackAtInterrupt	0x8000

typedef unsigned short QTCallBackType;

typedef void (pascal * QTCallBackProc)(QTCallBack cb,long refCon);

enum {
 qtcbNeedsRateChanges = 1,
 qtcbNeedsTimeChanges = 2
};

/* CallBack equates */
struct QTCallBackHeader {
 long callBackFlags;
 long reserved1;
 char qtPrivate[40];
};

typedef struct QTCallBackHeader QTCallBackHeader;


/*  TimeBase Routines  */
F_QTML( TimeBase ) NewTimeBase(void); 
F_QTML( void ) DisposeTimeBase(TimeBase tb); 

F_QTML( TimeValue ) GetTimeBaseTime(TimeBase tb,TimeScale s,TimeRecord *out); 
F_QTML( void ) SetTimeBaseTime(TimeBase tb,const TimeRecord *tr); 
F_QTML( void ) SetTimeBaseValue(TimeBase tb,TimeValue t,TimeScale s); 

F_QTML( Fixed ) GetTimeBaseRate(TimeBase tb); 
F_QTML( void ) SetTimeBaseRate(TimeBase tb,Fixed r); 

F_QTML( TimeValue ) GetTimeBaseStartTime(TimeBase tb,TimeScale s,TimeRecord *out); 
F_QTML( void ) SetTimeBaseStartTime(TimeBase tb,const TimeRecord *tr); 
F_QTML( TimeValue ) GetTimeBaseStopTime(TimeBase tb,TimeScale s,TimeRecord *out); 
F_QTML( void ) SetTimeBaseStopTime(TimeBase tb,const TimeRecord *tr); 

F_QTML( long ) GetTimeBaseFlags(TimeBase tb); 
F_QTML( void ) SetTimeBaseFlags(TimeBase tb,long timeBaseFlags); 

F_QTML( void ) SetTimeBaseMasterTimeBase(TimeBase slave,TimeBase master,const TimeRecord *slaveZero); 
F_QTML( TimeBase ) GetTimeBaseMasterTimeBase(TimeBase tb); 
F_QTML( void ) SetTimeBaseMasterClock(TimeBase slave,Component clockMeister,
 const TimeRecord *slaveZero); 
F_QTML( ComponentInstance ) GetTimeBaseMasterClock(TimeBase tb); 

F_QTML( void ) ConvertTime(TimeRecord *inout,TimeBase newBase); 
F_QTML( void ) ConvertTimeScale(TimeRecord *inout,TimeScale newScale); 
F_QTML( void ) AddTime(TimeRecord *dst,const TimeRecord *src); 
F_QTML( void ) SubtractTime(TimeRecord *dst,const TimeRecord *src); 

F_QTML( long ) GetTimeBaseStatus(TimeBase tb,TimeRecord *unpinnedTime); 

F_QTML( void ) SetTimeBaseZero(TimeBase tb,TimeRecord *zero); 

/*  CallBack Routines  */
F_QTML( QTCallBack ) NewCallBack(TimeBase tb,short cbType); 

F_QTML( void ) DisposeCallBack(QTCallBack qtCall); 

F_QTML( short ) GetCallBackType(QTCallBack qtCall); 
F_QTML( TimeBase ) GetCallBackTimeBase(QTCallBack qtCall); 

F_QTML( OSErr ) CallMeWhen(QTCallBack qtCall,QTCallBackProc callBackProc,long refCon,
 long param1,long param2,long param3); 

F_QTML( void ) CancelCallBack(QTCallBack cb); 

/*  Clock CallBack support Routines  */
F_QTML( OSErr ) AddCallBackToTimeBase(QTCallBack cb); 
F_QTML( OSErr ) RemoveCallBackFromTimeBase(QTCallBack cb); 
F_QTML( QTCallBack ) GetFirstCallBack(TimeBase tb); 
F_QTML( QTCallBack ) GetNextCallBack(QTCallBack cb); 
F_QTML( void ) ExecuteCallBack(QTCallBack cb); 


/* Movie Controller Routines */

#define MovieControllerComponentType OST_play

enum {
	mcActionIdle = 1,						// no param
	mcActionDraw = 2,						// param is WindowPtr
	mcActionActivate = 3,					// no param
	mcActionDeactivate = 4,					// no param
	mcActionMouseDown = 5,					// param is pointer to EventRecord
	mcActionKey = 6,						// param is pointer to EventRecord
	mcActionPlay = 8,						// param is Fixed, play rate
	mcActionGoToTime = 12,					// param is TimeRecord
	mcActionSetVolume = 14,					// param is a short
	mcActionGetVolume = 15,					// param is pointer to a short
	mcActionStep = 18,						// param is number of steps (short)
	mcActionSetLooping = 21,				// param is Boolean
	mcActionGetLooping = 22,				// param is pointer to a Boolean
	mcActionSetLoopIsPalindrome = 23,		// param is Boolean
	mcActionGetLoopIsPalindrome = 24,		// param is pointer to a Boolean
	mcActionSetGrowBoxBounds = 25,			// param is a Rect
	mcActionControllerSizeChanged = 26,		// no param
	mcActionSetSelectionBegin = 29,			// param is TimeRecord
	mcActionSetSelectionDuration = 30,		// param is TimeRecord, action only taken on set-duration
	mcActionSetPosterTime = 31,				// param is TimeRecord
	mcActionSetKeysEnabled = 32,			// param is Boolean
	mcActionGetKeysEnabled = 33,			// param is pointer to Boolean
	mcActionSetPlaySelection = 34,			// param is Boolean
	mcActionGetPlaySelection = 35,			// param is pointer to Boolean
	mcActionSetUseBadge = 36,				// param is Boolean
	mcActionGetUseBadge = 37,				// param is pointer to Boolean
	mcActionSetFlags = 38,					// param is long of flags
	mcActionGetFlags = 39,					// param is pointer to a long of flags
	mcActionSetPlayEveryFrame = 40,			// param is Boolean
	mcActionGetPlayEveryFrame = 41,			// param is pointer to Boolean
	mcActionGetPlayRate = 42,				// param is pointer to Fixed
	mcActionShowBalloon = 43,				// param is a pointer to a boolean. set to false to stop balloon
	mcActionBadgeClick = 44					// no param
};
typedef unsigned short mcAction;

enum {mcFlagSuppressMovieFrame = 1<<0,mcFlagSuppressStepButtons = 1<<1,
 mcFlagSuppressSpeakerButton = 1<<2,mcFlagsUseWindowPalette = 1<<3};

typedef Boolean (pascal * MCActionFilter)(MovieController mc,short *action,long param);

enum {mcTopLeftMovie = 1<<0,mcScaleMovieToFit = 1<<1,mcWithBadge = 1<<2,
 mcNotVisible = 1<<3,mcWithFrame = 1<<4};
typedef unsigned char mcCreationFlagsEnum;

enum {mcInfoUndoAvailable = 1<<0,mcInfoCutAvailable = 1<<1,mcInfoCopyAvailable = 1<<2,
 mcInfoPasteAvailable = 1<<3,mcInfoClearAvailable = 1<<4,mcInfoHasSound = 1<<5,
 mcInfoIsPlaying = 1<<6,mcInfoIsLooping = 1<<7,mcInfoIsInPalindrome = 1<<8,
 mcInfoEditingEnabled = 1<<9};
typedef unsigned short mcInfoFlagsEnum;

/*  target management  */
F_QTML( MovieController ) NewMovieController(Movie m,const Rect *movieRect,
 long controllerCreationFlags); 
F_QTML( void ) DisposeMovieController(MovieController mc); 

F_QTML( ComponentResult ) MCSetMovie(MovieController mc,Movie m,WindowPtr movieWindow,
 Point where); 
F_QTML( Movie ) MCGetMovie(MovieController mc); 
F_QTML( ComponentResult ) MCRemoveMovie(MovieController mc); 

/*  event handling etc.  */
F_QTML( ComponentResult ) MCIsPlayerEvent(MovieController mc,const EventRecord *e); 
F_QTML( ComponentResult ) MCSetActionFilter(MovieController mc,MCActionFilter filter); 
F_QTML( ComponentResult ) MCDoAction(MovieController mc,short action,void *params); 

/*  state type things  */
F_QTML( ComponentResult ) MCSetControllerAttached(MovieController mc,Boolean attach); 
F_QTML( ComponentResult ) MCIsControllerAttached(MovieController mc); 
F_QTML( ComponentResult ) MCSetControllerPort(MovieController mc,CGrafPtr gp); 
F_QTML( CGrafPtr ) MCGetControllerPort(MovieController mc); 

F_QTML( ComponentResult ) MCSetVisible(MovieController mc,Boolean show); 
F_QTML( ComponentResult ) MCGetVisible(MovieController mc); 

F_QTML( ComponentResult ) MCGetControllerBoundsRect(MovieController mc,Rect *bounds); 
F_QTML( ComponentResult ) MCSetControllerBoundsRect(MovieController mc,const Rect *bounds); 
F_QTML( RgnHandle ) MCGetControllerBoundsRgn(MovieController mc); 
F_QTML( RgnHandle ) MCGetWindowRgn(MovieController mc,WindowPtr w); 

/*  other stuff  */
F_QTML( ComponentResult ) MCMovieChanged(MovieController mc,Movie m); 
F_QTML( ComponentResult ) MCSetDuration(MovieController mc,TimeValue duration); 
F_QTML( TimeValue ) MCGetCurrentTime(MovieController mc,TimeScale *scale); 
F_QTML( ComponentResult ) MCNewAttachedController(MovieController mc,Movie m,
 WindowPtr w,Point where); 
F_QTML( ComponentResult ) MCPositionController(MovieController mc,Rect *movieRect,
 Rect *controllerRect,long controllerCreationFlags); 

/*  direct event handlers  */
F_QTML( ComponentResult ) MCDraw(MovieController mc,WindowPtr w); 
F_QTML( ComponentResult ) MCActivate(MovieController mc,WindowPtr w,Boolean activate); 
F_QTML( ComponentResult ) MCIdle(MovieController mc); 
F_QTML( ComponentResult ) MCKey(MovieController mc,char key,long modifiers); 
F_QTML( ComponentResult ) MCClick(MovieController mc,WindowPtr w,Point where,
 long when,long modifiers); 

/*   calls for editing  */
F_QTML( ComponentResult ) MCEnableEditing(MovieController mc,Boolean enabled); 
F_QTML( long ) MCIsEditingEnabled(MovieController mc); 
F_QTML( Movie ) MCCopy(MovieController mc); 
F_QTML( Movie ) MCCut(MovieController mc); 
F_QTML( ComponentResult ) MCPaste(MovieController mc,Movie srcMovie); 
F_QTML( ComponentResult ) MCClear(MovieController mc); 
F_QTML( ComponentResult ) MCUndo(MovieController mc); 

/*  menu related stuff  */
F_QTML( ComponentResult ) MCGetControllerInfo(MovieController mc,long *mcInfoFlags); 

F_QTML( ComponentResult ) MCSetClip(MovieController mc, RgnHandle theClip, RgnHandle movieClip);
F_QTML( ComponentResult ) MCGetClip(MovieController mc, RgnHandle *theClip, RgnHandle *movieClip);

F_QTML( ComponentResult ) MCDrawBadge(MovieController mc, RgnHandle movieRgn, RgnHandle *badgeRgn);

#ifndef __ERRORS__
enum {
/* movie controller error codes */
 cannotMoveAttachedController = -9999,
 controllerHasFixedHeight = -9998,
 cannotSetWidthOfAttachedController = -9997,
 controllerBoundsNotExact = -9996,
 editingNotAllowed = -9995,
 badControllerHeight = -9994
};
#endif

enum {
	kMCSetMovieSelect = 2,
	kMCRemoveMovieSelect = 3,
	kMCIsPlayerEventSelect = 7,
	kMCSetActionFilterSelect = 8,
	kMCDoActionSelect = 9,
	kMCSetControllerAttachedSelect = 10,
	kMCIsControllerAttachedSelect = 11,
	kMCSetControllerPortSelect = 12,
	kMCGetControllerPortSelect = 13,
	kMCGetVisibleSelect = 14,
	kMCSetVisibleSelect = 15,
	kMCGetControllerBoundsRectSelect = 16,
	kMCSetControllerBoundsRectSelect = 17,
	kMCGetControllerBoundsRgnSelect = 18,
	kMCGetWindowRgnSelect = 19,
	kMCMovieChangedSelect = 20,
	kMCSetDurationSelect = 21,
	kMCGetCurrentTimeSelect = 22,
	kMCNewAttachedControllerSelect = 23,
	kMCDrawSelect = 24,
	kMCActivateSelect = 25,
	kMCIdleSelect = 26,
	kMCKeySelect = 27,
	kMCClickSelect = 28,
	kMCEnableEditingSelect = 29,
	kMCIsEditingEnabledSelect = 30,
	kMCCopySelect = 31,
	kMCCutSelect = 32,
	kMCPasteSelect = 33,
	kMCClearSelect = 34,
	kMCUndoSelect = 35,
	kMCPositionControllerSelect = 36,
	kMCGetControllerInfoSelect = 37,
	kMCNewControllerFromFileSelect = 38,
	kMCSetBadgePictSelect = 39,
	kMCSetClipSelect = 40,
	kMCGetClipSelect = 41,
	kMCDrawBadgeSelect = 42
};

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
