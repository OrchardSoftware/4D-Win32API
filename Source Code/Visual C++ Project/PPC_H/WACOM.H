/* ********** Wacom Serial Tablet Driver Interface Definitions ********* */
/*                                                                       */
/*                        Copyright (C) 1988, 1989                       */
/*                      Evergreen Technologies, Inc.                     */
/*                          All Rights Reserved                          */
/*                                                                       */
/*   mod 6 26 95 tsh - remove "NOERROR" due to conflict					 */
/*   MOD 04-04-95 MAZ - fix transducer flags							 */
/*   mod 11 25 91 tsh - add NOTRANS bit to general flags                 */
/*                                                                       */
/* ********************************************************************* */


/* ***** Structures for associating stylus buttons with keys ***** */

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

typedef struct button {			/* definition for a single button */
    Boolean ignored;			/* this button is ignored by the driver */
    short	key;				/* key representation of what is generated */
} BUTTON;    					/* ...when the button is pressed - this is */
    							/* ...meant to be a function key, but a */
    							/* ...developer can put anything here that */
    							/* ...they want to;  the CDEV only puts */
    							/* ...function key press data here; if key */
    							/* ...is zero then this button is mapped to */
    							/* ...the mouse button */
    							
typedef struct bmap {					/* structure containing all buttons */
    BUTTON	penDown;			/* point button in the pens */
    BUTTON	sideSwitch;			/* side button of the standard pen */
    BUTTON	cursor[4];			/* each of the 4 cursor buttons */
} BMAP;



/* ***** Standard Data Structure For Tablet Access *****
 *
 *  Based on:
 *            "Proposed ADB Tablet Memory Structure"
 *                        Dec. 19, 1988
 *                 Dave Fleck, GTCO Corporation
 *
 */

typedef struct tabletRecord {

    /* *** Header section *** */
    
    char	version;			/* version of this data format */
    char	semaphore;			/* for future use -- tells if drvr is enabled */
    char	cursors;			/* number of cursors with tablet */
    char	updateFlags;		/* flags used when updating structure */
    short	angleRes;			/* metric bit & angular resolution */
    short	spaceRes;			/* spatial resolution of the tablet */
    long	xDimension;			/* x dimension in resolution units */
    long	yDimension;			/* y dimension in resolution units */
    long	zDimension;			/* z dimension in resolution units */
    long	xDisplace;			/* x displacement - minimum x value */
    long	yDisplace;			/* y displacement - minimum y value */
    long	zDisplace;			/* z displacement - minimum z value */
    long	resvPtr;			/* reserved pointer for future use */
    long	tabletID;			/* contains 'TBLT' identifying the device */
    
    /* *** Cursor section *** */
    
    char	DOFFlag;			/* degrees of freedom and transducer type */
    char	orientFlag;			/* type of orientation information */
    short	pressLevels;		/* pressure support and number of levels */
    short	xScale;				/* x scale factor for screen mapping */
    short	xTrans;				/* x translation factor for screen */
    short	yScale;				/* y scale factor for screen mapping */
    short	yTrans;				/* y translation factor for screen */
    char	flags;				/* proximity, update flag, and # buttons */
    char	pressThresh;		/* pressure threshold - normally unused */
    short	buttonMask;			/* button mask of driver-reserved buttons */
    short	errorFlag;			/* error code generated */
    short	buttons;			/* buttons pressed */
    short	tangPress;			/* tangential pressure level */
    short	pressure;			/* normal pressure level */
    long	timeStamp;			/* ticks at latest update */
    long	xCoord;				/* x coordinate in resolution units */
    long	yCoord;				/* y coordinate in resolution units */
    long	zCoord;				/* z coordinate in resolution units */
    short	xTilt;				/* x tilt */
    short	yTilt;				/* y tilt */
    							/* the specification calls for 8 more */
    							/* ...integers containing an attitude */
    							/* ...matrix.  This driver does not use */
    							/* ...the attitude matrix so these 16  */
    							/* ...bytes are re-allocated as described */
    short	tabXMin;			/* minimum tablet x dimension mapped */
    short	tabYMin;			/* minimum tablet y dimension mapped */
    short	tabXMax;			/* maximum tablet x dimension mapped */
    short	tabYMax;			/* maximum tablet y dimension mapped */
    short	screenXMin;			/* minimum screen x dimension mapped */
    short	screenYMin;			/* minimum screen y dimension mapped */
    short	screenXMax;			/* maximum screen x dimension mapped */
    short	screenYMax;			/* maximum screen y dimension mapped */
    							/* finally, the additional fields were */
    							/* ...added as an addition to the */
    							/* ... proposed specification to handle */
    							/* ... Wacom-specific needs */
    BMAP	*buttonMapping;		/* pointer to button mapping structure */
    short	modelInUse;			/* the Wacom tablet model currently in use */

} TABLETRECORD;



/* ***** Flags used within a TABLETRECORD *****
 *
 */

        /* updateFlags flags */
#define FRESHDATA	0x04		/* new data loaded */
#define FRESHATTR	0x08		/* new attributes loaded */
#define CHANGEDDATA	0x40		/* application has changed data */
#define CHANGEDATTR	0x80		/* application has changed attributes */

        /* tranducer types */
#define UNKNOWN		0x00
#define PEN			0x10
#define CURSOR		0x20

        /* degrees of freedom flags */
#define DOFX		0x04
#define DOFY		0x02
#define DOFZ		0x01

        /* orientation type flags */
#define NOATTITUDE	0x00		/* no transducer orientation info */
#define ATTITUDE	0x01		/* structure contains attitude matrix */
#define TILT		0x02		/* tilt information is available */
#define TILTAA		0x20		/* tilt is in azimuth and altitude form */

        /* general flags */
#define PROXIMITY	0x80		/* is the transducer within proximity */
#define TRACKING	0x40		/* is driver tracking the mouse cursor; 0 = YES */
#define NOTRANS		0x20		/* no transform of x,y data (MOUSE) */

		/* errorFlag's used by the Wacom driver */
//#define NOERROR		0x00		/* no error has occurred */
#define PORTERROR	0x01		/* error in controlling the serial port */
#define NORESPONSE	0x02		/* tablet is not responding */




/* ***** Configuration Structure Used By CDEV ***** */

typedef struct config {
    Boolean driverEnabled;		/* TRUE if driver should be enabled */
    Boolean fullTablet;			/* TRUE if full tablet should be used */
    Boolean fullScreen;			/* TRUE if full screen should be used */
    int tabX1,tabY1;			/* upper left of tablet in use */
    int tabX2,tabY2;			/* lower right of tablet in use */
    int screenX1,screenY1;		/* upper left of screen in use */
    int screenX2,screenY2;		/* lower right of screen in use */
    Boolean updateCursor;		/* TRUE if tablet should move cursor */
    Boolean pressurePen;		/* TRUE if pressure pen is used */
    int pressThresh;			/* pressure threshold of mouse button */
    BMAP buttonMapping;			/* definition of the button mapping */
    int model;					/* Wacom tablet model identification */
} CONFIG;



/*****
 *  SD Model Information
 *
 */
#define SD210		0
#define    SD210MAXX	31750
#define    SD210MAXY	23100
#define SD31X		1
#define    SD31XMAXX	21200
#define    SD31XMAXY	15000
#define SD320		2
#define    SD320MAXX	19050
#define    SD320MAXY	19050
#define SD420		3
#define    SD420MAXX	15240
#define    SD420MAXY	15240
#define SD510		4
#define    SD510MAXX	6930
#define    SD510MAXY	4620



/* ***** csCode's *****
 *
 */

#define GET_RECORD	20					/* requests the address of the */
										/* ...current tablet record */
#define FORCE_BEAT	21					/* forces a heartbeat - use for */
										/* ...forcing parameter updates */
#define START_UP	41					/* set tablet to startup defaults */
										/* ...and begin polling and processing */
#define SHUT_DOWN	42					/* stop polling and processing */



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


/* ********************************************************************* */
/*                                                                       */
/*                            End of Wacom.h                             */
/*                                                                       */
/* ********************************************************************* */
