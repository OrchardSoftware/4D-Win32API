/*
	File:		Drag.h

	Contains:	C Interface to the Drag Manager

	Copyright:	© 1992-1993 by Apple Computer, Inc., all rights reserved.
*/

#ifndef __DRAG__
#define __DRAG__

#ifndef __APPLEEVENTS__
#if LONG_FILENAMES
#include <AppleEvents.h>
#else
#include <AppleEve.h>
#endif
#endif

#ifndef __TEXTEDIT__
#include <TextEdit.h>
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
/* Gestalt Selector and Response Constants (will move to GestaltEqu.h) */

#define	gestaltDragMgrAttr			OST_drag		// Drag Manager attributes
#define	gestaltDragMgrPresent		0			// Drag Manager is present

#define	gestaltTEAttr				OST_teat		// TextEdit attributes
#define gestaltTEHasGetHiliteRgn	0			// TextEdit has TEGetHiliteRgn


/* Flavor Flags */

enum {
	flavorSenderOnly				= 0x00000001L,	// flavor is available to sender only
	flavorSenderTranslated			= 0x00000002L,	// flavor is translated by sender
	flavorNotSaved					= 0x00000004L,	// flavor should not be saved
	flavorSystemTranslated			= 0x00000100L	// flavor is translated by system
};

typedef unsigned long FlavorFlags;


/* Drag Attributes */

enum {
	dragHasLeftSenderWindow			= 0x00000001L,	// drag has left the source window since TrackDrag
	dragInsideSenderApplication		= 0x00000002L,	// drag is occurring within the sender application
	dragInsideSenderWindow			= 0x00000004L	// drag is occurring within the sender window
};

typedef unsigned long DragAttributes;

enum {
	kDragHasLeftSenderWindow	= (1L << 0),					/* drag has left the source window since TrackDrag*/
	kDragInsideSenderApplication = (1L << 1),					/* drag is occurring within the sender application*/
	kDragInsideSenderWindow		= (1L << 2)						/* drag is occurring within the sender window*/
};

/* Special Flavor Types */

#define flavorTypeHFS				OST_hfsSpc		// flavor type for HFS data
#define flavorTypePromiseHFS		OST_phfs		// flavor type for promised HFS data
#define flavorTypeDirectory			OST_diry		// flavor type for AOCE directories


/* Drag Tracking Handler Messages */

enum {
	dragTrackingEnterHandler		= 1,		// drag has entered handler
	dragTrackingEnterWindow			= 2,		// drag has entered window
	dragTrackingInWindow			= 3,		// drag is moving within window
	dragTrackingLeaveWindow			= 4,		// drag has exited window
	dragTrackingLeaveHandler		= 5			// drag has exited handler
};

typedef short DragTrackingMessage;


enum {
	kDragTrackingEnterHandler	= 1,							/* drag has entered handler*/
	kDragTrackingEnterWindow	= 2,							/* drag has entered window*/
	kDragTrackingInWindow		= 3,							/* drag is moving within window*/
	kDragTrackingLeaveWindow	= 4,							/* drag has exited window*/
	kDragTrackingLeaveHandler	= 5								/* drag has exited handler*/
};

/* Drag Drawing Procedure Messages */

enum {
	dragRegionBegin					= 1,		// initialize drawing
	dragRegionDraw					= 2,		// draw drag feedback
	dragRegionHide					= 3,		// hide drag feedback
	dragRegionIdle					= 4,		// drag feedback idle time
	dragRegionEnd					= 5			// end of drawing
};

typedef short DragRegionMessage;


/* Zoom Acceleration */

enum {
	zoomNoAcceleration				= 0,		// use linear interpolation
	zoomAccelerate					= 1,		// ramp up step size
	zoomDecelerate					= 2			// ramp down step size
};

typedef short ZoomAcceleration;


/* Drag Manager Data Types */

typedef unsigned long DragReference;
typedef unsigned long ItemReference;

typedef ResType FlavorType;

/* HFS Flavors */

// WARNING: This struct is larger now on Windows because the FSSpec struct
//  has been grown to handle long file names. Make sure you don't save this
//  struct to disk as it will be different sizes on the 2 platforms.

struct HFSFlavor {
	OSType			fileType;					// file type
	OSType			fileCreator;				// file creator
	unsigned short	fdFlags;					// Finder flags
	FSSpec			fileSpec;					// file system specification
};
typedef struct HFSFlavor HFSFlavor;

struct PromiseHFSFlavor {
	OSType			fileType;					// file type
	OSType			fileCreator;				// file creator
	unsigned short	fdFlags;					// Finder flags
	FlavorType		promisedFlavor;				// promised flavor containing an FSSpec
};
typedef struct PromiseHFSFlavor PromiseHFSFlavor;

#ifdef __cplusplus
extern "C" {
#endif

/* Application-Defined Drag Handler Routines */
#if MACVER

typedef OSErr pascal (*DragTrackingHandlerProcPtr) (DragTrackingMessage message, WindowPtr theWindow,
											 void *handlerRefCon, DragReference theDragRef);

typedef OSErr pascal (*DragReceiveHandlerProcPtr) (WindowPtr theWindow, void *handlerRefCon,
											DragReference theDragRef);

/* Application-Defined Routines */

typedef OSErr pascal (*DragSendDataProcPtr) (FlavorType theType, void *dragSendRefCon,
										  ItemReference theItemRef, DragReference theDragRef);

typedef OSErr pascal (*DragInputProcPtr) (Point *mouse, short *modifiers, void *dragInputRefCon,
									   DragReference theDragRef);

typedef OSErr pascal (*DragDrawingProcPtr) (DragRegionMessage message,
										 RgnHandle showRegion, Point showOrigin,
										 RgnHandle hideRegion, Point hideOrigin,
										 void *dragDrawingRefCon, DragReference theDragRef);

/* Installing and Removing Drag Handlers */

OSErr pascal InstallTrackingHandler
								(DragTrackingHandler trackingHandler,
								 WindowPtr theWindow,
								 void *handlerRefCon)
	= { 0x7001, 0xABED };

OSErr pascal InstallReceiveHandler
								(DragReceiveHandler receiveHandler,
								 WindowPtr theWindow,
								 void *handlerRefCon)
	= { 0x7002, 0xABED };

OSErr pascal RemoveTrackingHandler
								(DragTrackingHandler trackingHandler,
								 WindowPtr theWindow)
	= { 0x7003, 0xABED };

OSErr pascal RemoveReceiveHandler
								(DragReceiveHandler receiveHandler,
								 WindowPtr theWindow)
	= { 0x7004, 0xABED };


/* Creating and Disposing Drag References */

OSErr pascal NewDrag			(DragReference *theDragRef)
	= { 0x7005, 0xABED };

OSErr pascal DisposeDrag		(DragReference theDragRef)
	= { 0x7006, 0xABED };


/* Adding Drag Item Flavors */

OSErr pascal AddDragItemFlavor	(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 void *dataPtr,
								 Size dataSize,
								 FlavorFlags theFlags)
	= { 0x7007, 0xABED };

OSErr pascal SetDragItemFlavorData
								(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 const void *dataPtr,
								 Size dataSize,
								 unsigned long dataOffset)
	= { 0x7009, 0xABED };


/* Providing Drag Callback Procedures */

OSErr pascal SetDragSendProc	(DragReference theDragRef,
								 DragSendDataProc sendProc,
								 void *dragSendRefCon)
	= { 0x700A, 0xABED };

OSErr pascal SetDragInputProc	(DragReference theDragRef,
								 DragInputProc inputProc,
								 void *dragInputRefCon)
	= { 0x700B, 0xABED };

OSErr pascal SetDragDrawingProc	(DragReference theDragRef,
								 DragDrawingProc drawingProc,
								 void *dragDrawingRefCon)
	= { 0x700C, 0xABED };


/* Performing a Drag */

OSErr pascal TrackDrag			(DragReference theDragRef,
								 const EventRecord *theEvent,
								 RgnHandle theRegion)
	= { 0x700D, 0xABED };


/* Getting Drag Item Information */

OSErr pascal CountDragItems		(DragReference theDragRef,
								 unsigned short *numItems)
	= { 0x700E, 0xABED };

OSErr pascal GetDragItemReferenceNumber
								(DragReference theDragRef,
								 unsigned short index,
								 ItemReference *theItemRef)
	= { 0x700F, 0xABED };

OSErr pascal CountDragItemFlavors
								(DragReference theDragRef,
								 ItemReference theItemRef,
								 unsigned short *numFlavors)
	= { 0x7010, 0xABED };

OSErr pascal GetFlavorType		(DragReference theDragRef,
								 ItemReference theItemRef,
								 unsigned short index,
								 FlavorType *theType)
	= { 0x7011, 0xABED };

OSErr pascal GetFlavorFlags		(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 FlavorFlags *theFlags)
	= { 0x7012, 0xABED };

OSErr pascal GetFlavorDataSize	(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 Size *dataSize)
	= { 0x7013, 0xABED };

OSErr pascal GetFlavorData		(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 void *dataPtr,
								 Size *dataSize,
								 unsigned long dataOffset)
	= { 0x7014, 0xABED };

OSErr pascal GetDragItemBounds	(DragReference theDragRef,
								 ItemReference theItemRef,
								 Rect *itemBounds)
	= { 0x7015, 0xABED };

OSErr pascal SetDragItemBounds	(DragReference theDragRef,
								 ItemReference theItemRef,
								 const Rect *itemBounds)
	= { 0x7016, 0xABED };

OSErr pascal GetDropLocation	(DragReference theDragRef,
								 AEDesc *dropLocation)
	= { 0x7017, 0xABED };

OSErr pascal SetDropLocation	(DragReference theDragRef,
								 const AEDesc *dropLocation)
	= { 0x7018, 0xABED };


/* Getting Information About a Drag */

OSErr pascal GetDragAttributes	(DragReference theDragRef,
								 DragAttributes *flags)
	= { 0x7019, 0xABED };

OSErr pascal GetDragMouse		(DragReference theDragRef,
								 Point *mouse,
								 Point *pinnedMouse)
	= { 0x701A, 0xABED };

OSErr pascal SetDragMouse		(DragReference theDragRef,
								 Point pinnedMouse)
	= { 0x701B, 0xABED };

OSErr pascal GetDragOrigin		(DragReference theDragRef,
								 Point *initialMouse)
	= { 0x701C, 0xABED };

OSErr pascal GetDragModifiers	(DragReference theDragRef,
								 short *modifiers,
								 short *mouseDownModifiers,
								 short *mouseUpModifiers)
	= { 0x701D, 0xABED };


/* Drag Highlighting */

OSErr pascal ShowDragHilite		(DragReference theDragRef,
								 RgnHandle hiliteFrame,
								 Boolean inside)
	= { 0x701E, 0xABED };

OSErr pascal HideDragHilite		(DragReference theDragRef)
	= { 0x701F, 0xABED };

OSErr pascal DragPreScroll		(DragReference theDragRef,
								 short dH,
								 short dV)
	= { 0x7020, 0xABED };

OSErr pascal DragPostScroll		(DragReference theDragRef)
	= { 0x7021, 0xABED };

OSErr pascal UpdateDragHilite	(DragReference theDragRef,
								 RgnHandle updateRgn)
	= { 0x7022, 0xABED };


/* Drag Manager Utilities */

Boolean pascal WaitMouseMoved	(Point initialMouse)
	= { 0x7023, 0xABED };

OSErr pascal ZoomRects			(const Rect *fromRect,
								 const Rect *toRect,
								 short zoomSteps,
								 ZoomAcceleration acceleration)
	= { 0x7024, 0xABED };

OSErr pascal ZoomRegion			(RgnHandle region,
								 Point zoomDistance,
								 short zoomSteps,
								 ZoomAcceleration acceleration)
	= { 0x7025, 0xABED };

// Will move to TextEdit.h
OSErr pascal TEGetHiliteRgn		(RgnHandle region, TEHandle hTE)
	= { 0x3F3C, 0x000F, 0xA83D };

#else

typedef F_PASCAL_PTR(OSErr, *DragTrackingHandlerProcPtr) (DragTrackingMessage message, WindowPtr theWindow,
											 void *handlerRefCon, DragReference theDragRef);

typedef F_PASCAL_PTR(OSErr, *DragReceiveHandlerProcPtr) (WindowPtr theWindow, void *handlerRefCon,
											DragReference theDragRef);


/* Application-Defined Routines */

typedef F_PASCAL_PTR(OSErr, *DragSendDataProcPtr) (FlavorType theType, void *dragSendRefCon,
										  ItemReference theItemRef, DragReference theDragRef);

typedef F_PASCAL_PTR(OSErr, *DragInputProcPtr) (Point *mouse, short *modifiers, void *dragInputRefCon,
									   DragReference theDragRef);

typedef F_PASCAL_PTR(OSErr, *DragDrawingProcPtr) (DragRegionMessage message,
										 RgnHandle showRegion, Point showOrigin,
										 RgnHandle hideRegion, Point hideOrigin,
										 void *dragDrawingRefCon, DragReference theDragRef);


/*
 *	Drag Manager Routines
 */


/* Installing and Removing Drag Handlers */

F_PASCAL(OSErr) InstallTrackingHandler
								(DragTrackingHandlerProcPtr trackingHandler,
								 WindowPtr theWindow,
								 void *handlerRefCon);

F_PASCAL(OSErr) InstallReceiveHandler
								(DragReceiveHandlerProcPtr receiveHandler,
								 WindowPtr theWindow,
								 void *handlerRefCon);

F_PASCAL(OSErr) RemoveTrackingHandler
								(DragTrackingHandlerProcPtr trackingHandler,
								 WindowPtr theWindow);

F_PASCAL(OSErr) RemoveReceiveHandler
								(DragReceiveHandlerProcPtr receiveHandler,
								 WindowPtr theWindow);


/* Creating and Disposing Drag References */

F_PASCAL(OSErr) NewDrag			(DragReference *theDragRef);

F_PASCAL(OSErr) DisposeDrag		(DragReference theDragRef);


/* Adding Drag Item Flavors */

F_PASCAL(OSErr) AddDragItemFlavor	(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 void *dataPtr,
								 Size dataSize,
								 FlavorFlags theFlags);

F_PASCAL(OSErr) SetDragItemFlavorData
								(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 const void *dataPtr,
								 Size dataSize,
								 unsigned long dataOffset);


/* Providing Drag Callback Procedures */

F_PASCAL(OSErr) SetDragSendProc	(DragReference theDragRef,
								 DragSendDataProcPtr sendProc,
								 void *dragSendRefCon);

F_PASCAL(OSErr) SetDragInputProc	(DragReference theDragRef,
								 DragInputProcPtr inputProc,
								 void *dragInputRefCon);

F_PASCAL(OSErr) SetDragDrawingProc	(DragReference theDragRef,
								 DragDrawingProcPtr drawingProc,
								 void *dragDrawingRefCon);


/* Performing a Drag */

F_PASCAL(OSErr) TrackDrag			(DragReference theDragRef,
								 const EventRecord *theEvent,
								 RgnHandle theRegion);


/* Getting Drag Item Information */

F_PASCAL(OSErr) CountDragItems		(DragReference theDragRef,
								 unsigned short *numItems);

F_PASCAL(OSErr) GetDragItemReferenceNumber
								(DragReference theDragRef,
								 unsigned short index,
								 ItemReference *theItemRef);

F_PASCAL(OSErr) CountDragItemFlavors
								(DragReference theDragRef,
								 ItemReference theItemRef,
								 unsigned short *numFlavors);

F_PASCAL(OSErr) GetFlavorType		(DragReference theDragRef,
								 ItemReference theItemRef,
								 unsigned short index,
								 FlavorType *theType);

F_PASCAL(OSErr) GetFlavorFlags		(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 FlavorFlags *theFlags);

F_PASCAL(OSErr) GetFlavorDataSize	(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 Size *dataSize);

F_PASCAL(OSErr) GetFlavorData		(DragReference theDragRef,
								 ItemReference theItemRef,
								 FlavorType theType,
								 void *dataPtr,
								 Size *dataSize,
								 unsigned long dataOffset);

F_PASCAL(OSErr) GetDragItemBounds	(DragReference theDragRef,
								 ItemReference theItemRef,
								 Rect *itemBounds);

F_PASCAL(OSErr) SetDragItemBounds	(DragReference theDragRef,
								 ItemReference theItemRef,
								 const Rect *itemBounds);

F_PASCAL(OSErr) GetDropLocation	(DragReference theDragRef,
								 AEDesc *dropLocation);

F_PASCAL(OSErr) SetDropLocation	(DragReference theDragRef,
								 const AEDesc *dropLocation);


/* Getting Information About a Drag */

F_PASCAL(OSErr) GetDragAttributes	(DragReference theDragRef,
								 DragAttributes *flags);

F_PASCAL(OSErr) GetDragMouse		(DragReference theDragRef,
								 Point *mouse,
								 Point *pinnedMouse);

F_PASCAL(OSErr) SetDragMouse		(DragReference theDragRef,
								 Point pinnedMouse);

F_PASCAL(OSErr) GetDragOrigin		(DragReference theDragRef,
								 Point *initialMouse);

F_PASCAL(OSErr) GetDragModifiers	(DragReference theDragRef,
								 short *modifiers,
								 short *mouseDownModifiers,
								 short *mouseUpModifiers);


/* Drag Highlighting */

F_PASCAL(OSErr) ShowDragHilite		(DragReference theDragRef,
								 RgnHandle hiliteFrame,
								 Boolean inside);

F_PASCAL(OSErr) HideDragHilite		(DragReference theDragRef);

F_PASCAL(OSErr) DragPreScroll		(DragReference theDragRef,
								 short dH,
								 short dV);

F_PASCAL(OSErr) DragPostScroll		(DragReference theDragRef);

F_PASCAL(OSErr) UpdateDragHilite	(DragReference theDragRef,
								 RgnHandle updateRgn);


/* Drag Manager Utilities */

F_PASCAL(Boolean) WaitMouseMoved	(Point initialMouse);

F_PASCAL(OSErr) ZoomRects			(const Rect *fromRect,
								 const Rect *toRect,
								 short zoomSteps,
								 ZoomAcceleration acceleration);

F_PASCAL(OSErr) ZoomRegion			(RgnHandle region,
								 Point zoomDistance,
								 short zoomSteps,
								 ZoomAcceleration acceleration);

// Will move to TextEdit.h
F_PASCAL(OSErr) TEGetHiliteRgn		(RgnHandle region, TEHandle hTE);

// ITEDO
typedef  DragTrackingHandlerProcPtr DragTrackingHandler;
typedef  DragReceiveHandlerProcPtr DragReceiveHandler;
typedef DragSendDataProcPtr DragSendDataProc;
typedef DragInputProcPtr DragInputProc;
typedef DragDrawingProcPtr DragDrawingProc;

typedef DragTrackingHandler		DragTrackingHandlerUPP;
typedef DragReceiveHandler		DragReceiveHandlerUPP;
typedef DragSendDataProc		DragSendDataUPP;
typedef DragInputProc			DragInputUPP;
typedef DragDrawingProc			DragDrawingUPP;

enum {
	uppDragTrackingHandlerProcInfo = kPascalStackBased
		 | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(DragTrackingMessage)))
		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(WindowPtr)))
		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void*)))
		 | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(DragReference)))
};

enum {
	uppDragReceiveHandlerProcInfo = kPascalStackBased
		 | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(WindowPtr)))
		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(void*)))
		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(DragReference)))
};

enum {
	uppDragSendDataProcInfo = kPascalStackBased
		 | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(FlavorType)))
		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(void*)))
		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(ItemReference)))
		 | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(DragReference)))
};

enum {
	uppDragInputProcInfo = kPascalStackBased
		 | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Point*)))
		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(short*)))
		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(void*)))
		 | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(DragReference)))
};

enum {
	uppDragDrawingProcInfo = kPascalStackBased
		 | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(DragRegionMessage)))
		 | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(RgnHandle)))
		 | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Point)))
		 | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(RgnHandle)))
		 | STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(Point)))
		 | STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(void*)))
		 | STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(DragReference)))
};

#define NewDragTrackingHandlerProc(userRoutine)		\
		(DragTrackingHandlerUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDragTrackingHandlerProcInfo, GetCurrentArchitecture())

#define CallDragTrackingHandlerProc(userRoutine, message, theWindow, handlerRefCon, theDragRef)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDragTrackingHandlerProcInfo, (message), (theWindow), (handlerRefCon), (theDragRef))

#define NewDragReceiveHandlerProc(userRoutine)		\
		(DragReceiveHandlerUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDragReceiveHandlerProcInfo, GetCurrentArchitecture())

#define CallDragReceiveHandlerProc(userRoutine, theWindow, handlerRefCon, theDragRef)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDragReceiveHandlerProcInfo, (theWindow), (handlerRefCon), (theDragRef))

#define NewDragSendDataProc(userRoutine)		\
		(DragSendDataUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDragSendDataProcInfo, GetCurrentArchitecture())

#define CallDragSendDataProc(userRoutine, theType, dragSendRefCon, theItemRef, theDragRef)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDragSendDataProcInfo, (theType), (dragSendRefCon), (theItemRef), (theDragRef))

#define NewDragInputProc(userRoutine)		\
		(DragInputUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDragInputProcInfo, GetCurrentArchitecture())

#define CallDragInputProc(userRoutine, mouse, modifiers, dragInputRefCon, theDragRef)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDragInputProcInfo, (mouse), (modifiers), (dragInputRefCon), (theDragRef))

#define NewDragDrawingProc(userRoutine)		\
		(DragDrawingUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppDragDrawingProcInfo, GetCurrentArchitecture())

#define CallDragDrawingProc(userRoutine, message, showRegion, showOrigin, hideRegion, hideOrigin, dragDrawingRefCon, theDragRef)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppDragDrawingProcInfo, (message), (showRegion), (showOrigin), (hideRegion), (hideOrigin), (dragDrawingRefCon), (theDragRef))

#endif	// !MACVER

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

#endif  // __DRAG__



