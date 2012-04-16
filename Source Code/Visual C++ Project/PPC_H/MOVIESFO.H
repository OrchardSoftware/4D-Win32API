/************************************************************

Created: Wednesday, August 28, 1991 at 1:56 AM
 MoviesFormat.h
 C Interface to the Macintosh Libraries


  Copyright Apple Computer, Inc. 1991
  All rights reserved

************************************************************/


#ifndef __MOVIESFORMAT__
#define __MOVIESFORMAT__

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

#ifndef __MOVIES__
#include <Movies.h>
#endif

#define kMovieVersion	(0)				/* version number of the format here described */

struct MoviesUserData {
	long		size;						/* size of this user data */
	long		type;						/* type of user data */
	char 		data[1];					/* the user data */
};
typedef struct MoviesUserData MoviesUserData;

struct UserDataAtom {
	long 			size;
	long			type;
	MoviesUserData	userData[1];
};
typedef struct UserDataAtom UserDataAtom;

/*
struct SampleDescription {
	long		size;
	long		type;
	long		resvd1;
	short		resvdA;
	short		dataRefBlobId;
	};
typedef struct SampleDescription SampleDescription;
*/

struct SampleDescriptionAtom {
	long		size;
	long		type;							/* = 'stsd' */
	long		flags;							/* 1 byte of version / 3 bytes of flags */
	long		numEntries;
	SampleDescription	sampleDescTable[1];
};
typedef struct SampleDescriptionAtom SampleDescriptionAtom;

struct TimeToSampleNum {
	long		sampleCount;
	TimeValue	sampleDuration;
};
typedef struct TimeToSampleNum TimeToSampleNum;

struct TimeToSampleNumAtom {
	long		size;
	long		type;							/* = 'stts' */
	long		flags;							/* 1 byte of version / 3 bytes of flags */
	long		numEntries;
	TimeToSampleNum	timeToSampleNumTable[1];
};
typedef struct TimeToSampleNumAtom TimeToSampleNumAtom;

struct SyncSampleAtom{
	long		size;
	long		type;								/* = 'stss' */
	long		flags;								/* 1 byte of version / 3 bytes of flags */
	long		numEntries;
	long		syncSampleTable[1];
};
typedef struct SyncSampleAtom SyncSampleAtom;

struct SampleToChunk {
	long		firstChunk;
	long		samplesPerChunk;
	long		sampleDescriptionID;
};
typedef struct SampleToChunk SampleToChunk;

struct SampleToChunkAtom {
	long		size;
	long		type;								/* = 'stsc' */
	long		flags;								/* 1 byte of version / 3 bytes of flags */
	long		numEntries;
	SampleToChunk	sampleToChunkTable[1];
};
typedef struct SampleToChunkAtom SampleToChunkAtom;

struct ChunkOffsetAtom {
	long		size;
	long		type;								/* = 'stco' */
	long		flags;								/* 1 byte of version / 3 bytes of flags */
	long		numEntries;
	long		chunkOffsetTable[1];
};
typedef struct ChunkOffsetAtom ChunkOffsetAtom;

struct SampleSizeAtom {
	long		size;
	long		type;								/* = 'stsz' */
	long		flags;								/* 1 byte of version / 3 bytes of flags */
	long		sampleSize;
	long		numEntries;
	long		sampleSizeTable[1];
};
typedef struct SampleSizeAtom SampleSizeAtom;

struct SampleTableAtom {
	long		size;
	long		type;			/* = 'stbl' */

	SampleDescriptionAtom	sampleDescription;
	TimeToSampleNumAtom		timeToSampleNum;
	SampleToChunkAtom		sampleToChunk;
	SyncSampleAtom			syncSample;
	SampleSizeAtom			sampleSize;
	ChunkOffsetAtom			chunkOffset;
	
};
typedef struct SampleTableAtom SampleTableAtom;

	
struct PublicHandlerInfo {
	long				flags;								/* 1 byte of version / 3 bytes of flags */
	long				componentType;
	long				componentSubType;
	long				componentManufacturer;
	long				componentFlags;
	long				componentFlagsMask;
	char				componentName[1];
	
};
typedef struct PublicHandlerInfo PublicHandlerInfo;
	
struct HandlerAtom {
	long				size;
	long				type;							/* = 'hdlr' */
	
	PublicHandlerInfo	hInfo;	
};
typedef struct HandlerAtom HandlerAtom;
	
	
struct DataRefAtom {
	long				size;
	long				type;
	
	long				flags;					/* 1 byte of version / 3 bytes of flags (contains the dataRef attributes) */
	char				dataRefData[1];			/* for the current data handlers, this is an alias */
};
typedef struct DataRefAtom DataRefAtom;

struct DataRefContainerAtom {
	long				size;
	long				type;
	
	long				flags;							/* 1 byte of version / 3 bytes of flags */
	long				numEntries;
	DataRefAtom			dataRefs[1];
};
typedef struct DataRefContainerAtom DataRefContainerAtom;

struct DataInfoAtom {
	long					size;
	long					type;							/* = 'dinf' */
	DataRefContainerAtom	dataRef;
};
typedef struct DataInfoAtom DataInfoAtom;

struct MatteCompressedAtom {
	long				size;
	long				type;
	
	long				flags;							/* 1 byte of version / 3 bytes of flags */
	ImageDescription	matteImageDescription;
	char				matteData[1];
};
typedef struct MatteCompressedAtom MatteCompressedAtom;

struct MatteAtom {
	long				size;
	long				type;
	
	MatteCompressedAtom	aCompressedMatte;
};
typedef struct MatteAtom MatteAtom;

struct RgnAtom {
	long		size;
	long		type;
	
	short		rgnSize;								/* this is the contents of a region */
	Rect		rgnBBox;
	char		data[1];
};
typedef struct RgnAtom RgnAtom;

struct ClippingAtom {
	long		size;
	long		type;
	
	RgnAtom		aRgnClip;
};
typedef struct ClippingAtom ClippingAtom;
	
struct VideoMediaInfoHeader {
	long				flags;							/* 1 byte of version / 3 bytes of flags */
	short				graphicsMode;					/* for QD - transfer mode */
	short				opColorRed;						/* opcolor for transfer mode */
	short				opColorGreen;
	short				opColorBlue;
	
};
typedef struct VideoMediaInfoHeader VideoMediaInfoHeader;


struct VideoMediaInfoHeaderAtom {
	long				size;							/* size of Media info */
	long				type;							/* = 'vmhd' */
	VideoMediaInfoHeader	vmiHeader;
};
typedef struct VideoMediaInfoHeaderAtom VideoMediaInfoHeaderAtom;

struct VideoMediaInfo {
	long				size;							/* size of Media info */
	long				type;							/* = 'minf' */
	VideoMediaInfoHeaderAtom	header;
	HandlerAtom			dataHandler;
	DataInfoAtom		dataInfo;
	SampleTableAtom		sampleTable;
};
typedef struct VideoMediaInfo VideoMediaInfo;

		
struct SoundMediaInfoHeader {
	long				flags;							/* 1 byte of version / 3 bytes of flags */

 	short				balance;				
	short				rsrvd;
	
};
typedef struct SoundMediaInfoHeader SoundMediaInfoHeader;

struct SoundMediaInfoHeaderAtom {
	long				size;							/* size of Media info */
	long				type;							/* = 'vmhd' */
	
	SoundMediaInfoHeader	smiHeader;	
};
typedef struct SoundMediaInfoHeaderAtom SoundMediaInfoHeaderAtom;

struct SoundMediaInfo {
	long				size;							/* size of Media info */
	long				type;							/* = 'minf' */
	
	SoundMediaInfoHeaderAtom	header;
	
	HandlerAtom			dataHandler;
	
	DataRefAtom			dataReference;
	
	SampleTableAtom		sampleTable;
};
typedef struct SoundMediaInfo SoundMediaInfo;

struct MediaInfo {
	long			size;
	long			type;
	
	/* whatever data the media handler needs goes here */
};
typedef struct MediaInfo MediaInfo;


/***********************
* Media Directory Structures 
***********************/	

struct MediaHeader {
	long				flags;							/* 1 byte of version / 3 bytes of flags */
	
	long				creationTime;					/* seconds since Jan 1904 when directory was created */
	long				modificationTime;				/* seconds since Jan 1904 when directory was appended */

	TimeValue			timeScale;						/* start time for Media (Media time) */
	TimeValue			duration;						/* length of Media (Media time) */

	short				language;
	short				quality;
};
typedef struct MediaHeader MediaHeader;

struct MediaHeaderAtom {
	long 		size;
	long		type;			
	
	MediaHeader	header;
};
typedef struct MediaHeaderAtom MediaHeaderAtom;

struct MediaDirectory {
	long				size;
	long				type;							/* = 'mdia' */
	
	MediaHeaderAtom			mediaHeader;				/* standard Media information */

	HandlerAtom			mediaHandler;

	MediaInfo			mediaInfo;
} ;
typedef struct MediaDirectory MediaDirectory;


/***********************
* Track Structures 
***********************/	
	
enum {
	TrackEnable = 1<<0,
	TrackInMovie = 1<<1,
	TrackInPreview = 1<<2,
	TrackInPoster = 1<<3
	};
	
struct TrackHeader {
	long				flags;							/* 1 byte of version / 3 bytes of flags */

	long				creationTime;					/* seconds since Jan 1904 when directory was created */
	long				modificationTime;				/* seconds since Jan 1904 when directory was appended */

	long				trackID;

	long				reserved1;

	TimeValue			duration;						/* length of track (track time) */

	long				reserved2;
	long				reserved3;

	short				layer;
	short				alternateGroup;

	short				volume;
	short				reserved4;

	MatrixRecord		matrix;
	Fixed				trackWidth;
	Fixed				trackHeight;
		
} ;
typedef struct TrackHeader TrackHeader;

struct TrackHeaderAtom {
	long				size;							/* size of track header */
	long				type;							/* = 'tkhd' */
		
	TrackHeader		header;
};
typedef struct TrackHeaderAtom TrackHeaderAtom;

struct EditListType {
	TimeValue		trackDuration;
	TimeValue		mediaTime;
	Fixed			mediaRate;
};
typedef struct EditListType EditListType;

struct EditListAtom {
	long				size;
	long				type;							/* = elst */
	
	long				flags;							/* 1 byte of version / 3 bytes of flags */

	long				numEntries;
	EditListType		editListTable[1];
};
typedef struct EditListAtom EditListAtom;

struct EditsAtom {
	long			size;
	long			type;								/* = edts */
	
	EditListAtom	editList;
};
typedef struct EditsAtom EditsAtom;

struct TrackDirectory {
	long				size;
	long				type;							/* = 'trak' */

	TrackHeaderAtom		trackHeader;					/* standard track information */

	ClippingAtom		trackClip;

	EditsAtom			edits;
	
	MediaDirectory		media;
	
 	UserDataAtom		userData;						/* space for extending with new data types */
} ;
typedef struct TrackDirectory TrackDirectory;

struct MovieHeader {
	long				flags;							/* 1 byte of version / 3 bytes of flags */

	long				creationTime;					/* seconds since Jan 1904 when directory was created */
	long				modificationTime;				/* seconds since Jan 1904 when directory was appended */

	/* Time specifications */
	TimeValue			timeScale;
	TimeValue			duration;
	Fixed				preferredRate;					/* rate at which to play this movie */

	short				preferredVolume;				/* volume to play movie at */
	short				reserved1;			

	/* Graphics specifications */
	long				reserved2;					
	long				reserved3;					

	MatrixRecord		matrix;
	
	TimeValue			previewTime;					/* time in track the proxy begins (track time) */
	TimeValue			previewDuration;				/* how long the proxy lasts (track time) */
	TimeValue			posterTime;						/* time in track the proxy begins (track time) */
	TimeValue			selectionTime;					/* time in track the proxy begins (track time) */
	TimeValue			selectionDuration;				/* time in track the proxy begins (track time) */
	TimeValue			currentTime;					/* time in track the proxy begins (track time) */
	long 				nextTrackID;					/* next value to use for a TrackID */

};
typedef struct MovieHeader MovieHeader;

struct MovieHeaderAtom {
	long				size;
	long				type;							/* = 'mvhd' */
	MovieHeader 		header;
};
typedef struct MovieHeaderAtom MovieHeaderAtom;

struct MovieDirectory {
	long				size;
	long				type;							/* = 'moov' */
	MovieHeaderAtom		header;
	ClippingAtom		movieClip;				
	
	/* Track Directories */
	struct {
		TrackDirectory  trackDirectory; 				/* Track directory information */	
		} 				track[1];
 		
 	/* User data for Movie */
 	UserDataAtom		userData;						/* space for user extensions */
 	
};
typedef struct MovieDirectory MovieDirectory;
/* Movie formats and tags */
	/* some system defined format IDs */
	#define	MOVIE_TYPE		OST_moov
	#define TRACK_TYPE		OST_trak
	#define MEDIA_TYPE		OST_mdia
	#define VIDEO_TYPE		OST_vide
	#define SOUND_TYPE		OST_soun


/* atom id's */
	#define MovieAID				OST_moov
	#define MovieHeaderAID			OST_mvhd
	#define ClipAID					OST_clip
	#define RgnClipAID				OST_crgn
	#define MatteAID				OST_matt
	#define MatteCompAID			OST_kmat
	#define TrackAID				OST_trak
	#define	UserDataAID				OST_udta
	#define	TrackHeaderAID			OST_tkhd
	#define EditsAID				OST_edts
	#define EditListAID				OST_elst
	#define MediaAID				OST_mdia
	#define MediaHeaderAID			OST_mdhd
	#define	MediaInfoAID			OST_minf
	#define VideoMediaInfoHeaderAID	OST_vmhd
	#define SoundMediaInfoHeaderAID	OST_smhd
	#define DataInfoAID				OST_dinf
	#define DataRefAID				OST_dref
	#define SampleTableAID			OST_stbl
	#define STSampleDescAID			OST_stsd
	#define STTimeToSampAID			OST_stts
	#define STSyncSampleAID			OST_stss
	#define STSampleToChunkAID		OST_stsc
	#define HandlerAID				OST_hdlr
	#define STSampleSizeAID			OST_stsz
	#define STChunkOffsetAID		OST_stco
	#define DataRefContainerAID 	OST_drfc
	

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

#endif // __MOVIESFORMAT__
