

/*
	File:		ContextualMenu.h

	Contains:	<contents>

	Written by:	Guy Fullerton

	Copyright:	<copyright>

	Change History (most recent first):

         <6>     3/18/97    GBF     Add CMM trap number.
         <5>     3/11/97    GBF     Add the Contextual Menu gestalt selector codes.
         <4>     2/27/97    GBF     Adding trapwords to the declarations.
         <3>     2/13/97    GBF     Remove DisplayContextualMenu.
         <2>     1/14/97    GBF     Fixing the API for Backwards-compatibility INIT as well as the
                                    display of the proper guide file name.
        <0+>    10/15/96    GBF     Moved here from the Assistance tree.
*/


/*
	File:		ContextualMenu.h

	Contains:	The public ContextualMenu routines

	Written by:	deeje
				Guy Fullerton

	Copyright:	© 1995 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

        <15>     7/29/96    GBF     Adding some #pragma import directives
        <14>     7/28/96    GBF     Trying to make this thing Universal Headers compatible for
                                    Harmony. Moving the plugin specific stuff to another file.
                                    Removed a bunch of unnecessary #includes.
        <13>     7/12/96    GBF     The last change comment (12) got screwed up; I'm fixing it.
        <12>     7/12/96    GBF     Making the DisplayContextualMenu interface more modern.
                                    replacing kShowBalloon and kShowHelp with some better enums.
                                    Added some more enums.
        <11>     4/11/96    GBF     Putting kExamineContextNoTimeout back, cuz I'm an idiot.
        <10>     4/11/96    GBF     Moved kExamineContextNoTimeout out of here.
         <9>      4/9/96    GBF     Add IsShowContextualMenuClick function. Cleaned up the API.
         <8>     3/22/96    GBF     Added Timeout constant
         <7>     3/14/96    GBF     Removing the AddContextualMenuItem routine. Adding typedef for
                                    ContextualCommandPair. Adding some enums.
         <6>      3/6/96    GBF     Adding the selection code parameter to AddContextualMenuItem
         <5>      3/6/96    GBF     Added the AddContextualMenuItem routine.
         <4>     2/28/96    GBF     DisplayContextualMenu now returns a SInt32
         <3>     2/27/96    GBF     allow hierarchical menus within a contextual menu
         <2>      2/7/96    arno    Changed Point to Point* to follow standard Maxwell conventions
         <5>     12/5/95    djc     changing file type
		 <4>	 8/29/95	djc		adding FindContextPopupArea
        <3+>     8/29/95    djc     adding FindContextPopupArea()
         <3>      8/1/95    djc     adding Balloon help and AppleGuide
		 <2>	 7/31/95	djc		cleanup
        <1+>     7/31/95    djc     cleanup
         <1>     7/31/95    djc     initial check-in
*/



#ifndef	__CONTEXTUALMENU__
#define	__CONTEXTUALMENU__

#include	<AppleEvents.h>
#include	<Events.h>
#include	<Menus.h>



/*
 *
 *	 High Level Interfaces 
 *
 */

#ifdef __cplusplus
extern "C"
{
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


enum {
	// Contextual Menu Trap Number
	_ContextualMenuDispatch				= 0xAA72
};

enum
{
	// Gestalt Selectors
#ifdef WIN32
	gestaltContextualMenuAttr			= 'cmnu',
#else
	gestaltContextualMenuAttr =  OST  ('c', 'm', 'n', 'u'),
#endif
	gestaltContextualMenuPresent		= 0,
	gestaltContextualMenuTrapAvailable	= 1
};



enum
{
	// Values indicating what kind of help the application supports
	kCMHelpItemNoHelp			= 0,
	kCMHelpItemAppleGuide,
	kCMHelpItemOtherHelp,
	
	// Values indicating what was chosen from the menu
	kCMNothingSelected			= 0,
	kCMMenuItemSelected,
	kCMShowBalloonSelected,
	kCMShowHelpSelected
};

typedef signed long OSStatus;


	// This registers your application as a client of contextual menus.
	// This is necessary so that the CMM can register your app as a
	// real client; the CMM Enabler will use this registry as a means of
	// deciding whether to provide the compatibility for a particular
	// application.
F_PASCAL(OSStatus) InitContextualMenus(void)
 TWOWORDINLINE(0x7001, 0xAA72);

	// This determines if the given event represents an action to invoke
	// a Contextual Menu.  Pass every mouseUp or mouseDown event to this
	// function.  When it returns true, the event represents a Contextual
	// Menu invocation action.  When it returns false, the event doesn't
	// represent an invocation.
F_PASCAL(Boolean) IsShowContextualMenuClick(const EventRecord* inEvent)
 TWOWORDINLINE(0x7002, 0xAA72);

	// This will draw and track a Contextual Menu.  Pass in the MenuRef to
	// draw (or NULL if you just want the default, mostly empty one), the
	// location of the mouse on the last Contextual Menu invocation event,
	// a Boolean indicating whether there is Balloon Help content
	// available for the item clicked upon, values indicating the
	// guide content available for the item clicked, and an AEDesc
	// which describes the item clicked (this can also be NULL).  Also
	// pass pointers to a UInt32, and SInt16 and a UInt16 which will give
	// you the results of the Contextual Menu selection.
	//
	// If the user doesn't select anything from the Contextual Menu, this
	// returns userCanceledErr, sets outUserSelectionType to
	// kCMNothingSelected, and sets outMenuID and outMenuItem to zero.
	//
	// If the user selects the Show Balloon item, this returns noErr,
	// passes kCMShowBalloonSelected in outUserSelectionType, and
	// sets both outMenuID and outMenuItem to zero.  You should then
	// display a help balloon appropriate for the item clicked.
	//
	// If the user selects the Guide related item, this returns noErr,
	// passes kCMShowHelpSelected in outUserSelectionType, and
	// sets both outMenuID and outMenuItem to zero.  You should then
	// open up a Guide file to a place appropriate for the item clicked.
	//
	// If the user selects one of your menu items, it returns noErr,
	// passes kCMMenuItemSelected, and sets outMenuID and outMenuItem
	// appropriately based upon the item chosen.
	//
	// If some other error occurred, an appropriate OSStatus will be
	// returned.
F_PASCAL(OSStatus) ContextualMenuSelect(MenuRef inMenuRef,
										Point inGlobalLocation,
										Boolean inBalloonAvailable,
										UInt32 inHelpType,
										ConstStr255Param inHelpItemString,
										const AEDesc* inSelection,
										UInt32* outUserSelectionType,
										SInt16* outMenuID,
										UInt16* outMenuItem)
 TWOWORDINLINE(0x7003, 0xAA72);

										  
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


