/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/
/*											
	©Apple Computer, Inc.  2/21/91 			
	      All Rights Reserved.				

*/

#ifndef __AEOBJECTS__
#define __AEOBJECTS__

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
#ifndef __MEMORY__
#if MACVER
#include <Memory.H>
#else
#include <MemoryM.H>
#endif
#endif


#ifndef __OSUTILS__
#include <OSUtils.h>
#endif

#ifndef __EVENTS__
#include <Events.h>
#endif

#ifndef __EPPC__
#include <EPPC.h>
#endif

#ifndef __APPLEEVENTS__
#if LONG_FILENAMES
#include <AppleEvents.h>
#else
#include <AppleEve.h>
#endif
#endif

#if Macintosh	|| WIN32
#define	OST_ANDSpc	'AND '
#define	OST_NOTSpc	'NOT '
#define	OST_ORSpcSpc	'OR  '
#define	OST_abso	'abso'
#define	OST_adjm	'adjm'
#define	OST_allSpc	'all '
#define	OST_anySpc	'any '
#define	OST_ccnt	'ccnt'
#define	OST_cmpd	'cmpd'
#define	OST_cmpr	'cmpr'
#define	OST_exmn	'exmn'
#define	OST_firs	'firs'
#define	OST_form	'form'
#define	OST_from	'from'
#define	OST_indc	'indc'
#define	OST_inde	'inde'
#define	OST_indx	'indx'
#define	OST_kidx	'kidx'
#define	OST_ktst	'ktst'
#define	OST_last	'last'
#define	OST_logc	'logc'
#define	OST_logi	'logi'
#define	OST_mark	'mark'
#define	OST_midd	'midd'
#define	OST_mkid	'mkid'
#define	OST_next	'next'
#define	OST_objSpc	'obj '
#define	OST_obj1	'obj1'
#define	OST_obj2	'obj2'
#define	OST_prev	'prev'
#define	OST_rang	'rang'
#define	OST_relSpc	'rel '
#define	OST_rele	'rele'
#define	OST_relo	'relo'
#define	OST_seld	'seld'
#define	OST_star	'star'
#define	OST_stop	'stop'
#define	OST_test	'test'
#define	OST_toke	'toke'
#define	OST_want	'want'
#define	OST_whos	'whos'
#define	OST_wrng	'wrng'
#define	OST_wstp	'wstp'
#define	OST_wstr	'wstr'
#define	OST_xtok	'xtok'
#else
#define	OST_ANDSpc	0x414E4420L
#define	OST_NOTSpc	0x4E4F5420L
#define	OST_ORSpcSpc	0x4F522020L
#define	OST_abso	0x6162736FL
#define	OST_adjm	0x61646A6DL
#define	OST_allSpc	0x616C6C20L
#define	OST_anySpc	0x616E7920L
#define	OST_ccnt	0x63636E74L
#define	OST_cmpd	0x636D7064L
#define	OST_cmpr	0x636D7072L
#define	OST_exmn	0x65786D6EL
#define	OST_firs	0x66697273L
#define	OST_form	0x666F726DL
#define	OST_from	0x66726F6DL
#define	OST_indc	0x696E6463L
#define	OST_inde	0x696E6465L
#define	OST_indx	0x696E6478L
#define	OST_kidx	0x6B696478L
#define	OST_ktst	0x6B747374L
#define	OST_last	0x6C617374L
#define	OST_logc	0x6C6F6763L
#define	OST_logi	0x6C6F6769L
#define	OST_mark	0x6D61726BL
#define	OST_midd	0x6D696464L
#define	OST_mkid	0x6D6B6964L
#define	OST_next	0x6E657874L
#define	OST_objSpc	0x6F626A20L
#define	OST_obj1	0x6F626A31L
#define	OST_obj2	0x6F626A32L
#define	OST_prev	0x70726576L
#define	OST_rang	0x72616E67L
#define	OST_relSpc	0x72656C20L
#define	OST_rele	0x72656C65L
#define	OST_relo	0x72656C6FL
#define	OST_seld	0x73656C64L
#define	OST_star	0x73746172L
#define	OST_stop	0x73746F70L
#define	OST_test	0x74657374L
#define	OST_toke	0x746F6B65L
#define	OST_want	0x77616E74L
#define	OST_whos	0x77686F73L
#define	OST_wrng	0x77726E67L
#define	OST_wstp	0x77737470L
#define	OST_wstr	0x77737472L
#define	OST_xtok	0x78746F6BL
#endif


/* Logical operators: look for them in descriptors of type typeLogicalDescriptor
  with keyword keyAELogicalOperator */
#define kAEAND						OST_ANDSpc
#define kAEOR						OST_ORSpcSpc
#define kAENOT						OST_NOTSpc

/* Absolute ordinals: look for them in descriptors of type typeAbsoluteOrdinal.
  Possible use is as keyAEKeyData in an object specifier whose keyAEKeyForm
  field is formAbsolutePosition. */
#define kAEFirst					OST_firs
#define kAELast						OST_last
#define kAEMiddle					OST_midd
#define kAEAny						OST_anySpc
#define kAEAll						OST_allSpc

/*  Relative ordinals: look for them in descriptors of type formRelativePosition. */
#define kAENext						OST_next
#define kAEPrevious					OST_prev

/********** Keywords for getting fields out of object specifier records **********/
#define keyAEDesiredClass			OST_want
#define keyAEContainer				OST_from
#define keyAEKeyForm				OST_form
#define keyAEKeyData	 		 	OST_seld


/********** Keywords for getting fields out of Range specifier records **********/
#define keyAERangeStart				OST_star		/* These are the only two fields in the range desc */
#define keyAERangeStop				OST_stop
	
/********** Possible values for the keyAEKeyForm field of an object specifier **********/
/* Remember that this set is an open-ended one.  The OSL makes assumptions about some of them,
  but nothing in the grammar says you can't define your own */
#define formAbsolutePosition 		OST_indx	 	/* e.g., 1st, -2nd ( 2nd from end) */
#define formRelativePosition		OST_rele 		/* next, previous */
#define formTest					OST_test 		/* A logical or a comparison */
#define formRange					OST_rang 		/* Two arbitrary objects and everything in between */
#define formPropertyID				OST_prop 		/* Key data is a 4-char property name */
#define formName					OST_name 		/* Key data may be of type 'TEXT' */

/************** Various relevant types ****************/
/* Some of these tend to be paired with certain of the forms above.  Where this
  is the case comments indicating the form(s) follow. */
#define typeObjectSpecifier 	 	OST_objSpc		/* keyAEContainer will often be one of these */
#define typeObjectBeingExamined 	OST_exmn		/* Another possible value for keyAEContainer */
#define typeCurrentContainer		OST_ccnt		/* Another possible value for keyAEContainer */
#define typeToken					OST_toke		/* Substituted for 'ccnt' before accessor called */

#define typeRelativeDescriptor 		OST_relSpc		/* formRelativePosition */
#define typeAbsoluteOrdinal 		OST_abso		/* formAbsolutePosition */
#define typeIndexDescriptor			OST_inde		/* formAbsolutePosition */
#define typeRangeDescriptor 	 	OST_rang		/* formRange */
#define typeLogicalDescriptor	 	OST_logi		/* formTest is this or typeCompDescriptor */
#define typeCompDescriptor			OST_cmpd		/* formTest is this or typeLogicalDescriptor */
	
/************** various relevant keywords ****************/
#define keyAECompOperator			OST_relo		/* Relates two terms: '', '<', etc. */
#define keyAELogicalTerms			OST_term		/* An AEList of terms to be related by 'logc' below */
#define keyAELogicalOperator		OST_logc		/* kAEAND,  kAEOR or kAENOT */
#define keyAEObject1				OST_obj1		/* One of two objects in a term must be object specifier */
#define keyAEObject2				OST_obj2		/* The other object may be a simple descriptor or obj. spec. */


/************ Special Handler selectors for OSL Callbacks ***************/
/* You don't need to use these unless you are not using AESetObjectCallbacks. */
#define keyDisposeTokenProc			OST_xtok
#define keyAECompareProc 		 	OST_cmpr
#define keyAECountProc 			 	OST_cont
#define keyAEMarkTokenProc 		 	OST_mkid
#define keyAEMarkProc 			 	OST_mark
#define keyAEAdjustMarksProc 	 	OST_adjm
#define keyAEGetErrDescProc 	 	OST_indc

/************ Error codes ***************/
/* OSL error codes: AEM proper uses up to -1719*/
#define errAEImpossibleRange		-1720 		/* A range like 3rd to 2nd, or 1st to all. */
#define errAEWrongNumberArgs		-1721 		/* Logical op kAENOT used with other than 1 term */

#define errAEAccessorNotFound 		-1723 		/* Accessor proc matching wantClass and containerType
												...or wildcards not found */
#define errAENoSuchLogical			-1725 	 	/* Something other than AND, OR, or NOT */
#define errAEBadTestKey				-1726 		/* Test is neither typeLogicalDescriptor
												...nor typeCompDescriptor */
#define errAENotAnObjSpec			-1727 	 	/* Param to AEResolve not of type 'obj ' */
#define errAENoSuchObject			-1728  		/* e.g.,: specifier asked for the 3rd, but there are only 2.
												...Basically, this indicates a run-time resolution error. */
#define errAENegativeCount			-1729 		/* CountProc returned negative value */
#define errAEEmptyListContainer		-1730 		/* Attempt to pass empty list as container to accessor */
	
/* Possible values for flags parameter to AEResolve.  They're additive */
#define kAEIDoMinimum				0x0000 
#define kAEIDoWhose					0x0001 
#define kAEIDoMarking				0x0004 


/* You only care about the constants that follow if you're doing your own whose
  clause resolution */
#define typeWhoseDescriptor			OST_whos
#define formWhose					OST_whos
#define typeWhoseRange				OST_wrng
#define keyAEWhoseRangeStart		OST_wstr
#define keyAEWhoseRangeStop			OST_wstp
#define keyAEIndex					OST_kidx
#define keyAETest					OST_ktst

struct ccntTokenRecord {		/* used for rewriting tokens in place of 'ccnt' descriptors		*/
		DescType tokenClass ;	/* This record is only of interest to those who, when they...	*/
		AEDesc token ;			/* ...get ranges as key data in their accessor procs, choose	*/
	} ;							/* ...to resolve them manually rather than call AEResolve again.	*/
	
typedef struct ccntTokenRecord ccntTokenRecord, *ccntTokenRecPtr, **ccntTokenRecHandle ;

typedef AEDesc *DescPtr, **DescHandle ;

/* typedefs providing type checking for procedure pointers */

#ifndef THINK_C
	typedef OSErr (pascal *accessorProcPtr) ( DescType desiredClass,
			const AEDesc *container, DescType containerClass, DescType form,
			const AEDesc *selectionData, AEDesc *value, long LongInt ) ;

	typedef OSErr (pascal *compareProcPtr)( DescType oper, const AEDesc *obj1,
				const AEDesc *obj2, Boolean *result ) ;
	typedef OSErr (pascal *countProcPtr)( DescType desiredType, DescType containerClass,
				const AEDesc *container, long *result ) ;
	typedef OSErr (pascal *disposeTokenProcPtr)( AEDesc *unneededToken ) ;

	typedef OSErr (pascal *getMarkTokenProcPtr)( const AEDesc *dContainerToken, DescType containerClass,
				AEDesc *result ) ;


	typedef OSErr (pascal *getErrDescProcPtr)( DescPtr *appDescPtr ) ;

	/* Note: app is supposed to dispose of dToken after marking */
	typedef OSErr (pascal *markProcPtr)( const AEDesc *dToken, const AEDesc *markToken, long index ) ;
	typedef OSErr (pascal *adjustMarksProcPtr)( long newStart, long newStop, const AEDesc *markToken ) ;
#endif

#ifdef THINK_C
	/* THINK C 4.0 doesn't understand the above constructs, so we'll use these instead */
	typedef 	ProcPtr	accessorProcPtr;
	typedef 	ProcPtr	compareProcPtr;
	typedef 	ProcPtr	countProcPtr;
	typedef 	ProcPtr	disposeTokenProcPtr;
	typedef 	ProcPtr	getMarkTokenProcPtr;
	typedef 	ProcPtr	getErrDescProcPtr;
	typedef 	ProcPtr	markProcPtr;
	typedef 	ProcPtr	adjustMarksProcPtr;

	#define const
	/* THINK doesn't understand "const" either... */
#endif


/*ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ PUBLIC PROCEDURES ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ*/



#if MACVER
F_PASCAL( OSErr ) AEObjectInit(void);	/* THINK_C requires the "void" for prototype checking */
/* Not done by inline, but by direct linking into code.  It sets up the pack
  such that further calls can be via inline */
F_PASCAL( OSErr ) AESetObjectCallbacks(
							compareProcPtr		myCompareProc,
							countProcPtr		myCountProc,
							disposeTokenProcPtr myDisposeTokenProc,
							getMarkTokenProcPtr myGetMarkTokenProc,			/* called when mark (below) is true (all 3)*/
							markProcPtr			myMarkProc,
							adjustMarksProcPtr	myAdjustMarksProc,
							getErrDescProcPtr	myGetErrDescProcPtr )
= { 0x303C, 0x0E35, 0xA816 } ;




F_PASCAL( OSErr ) AEResolve		(	const AEDesc	*objectSpecifier,
								short			callbackFlags,		/* see above for possible values */
					  			AEDesc			*theToken )
= { 0x303C, 0x0536, 0xA816 } ;


F_PASCAL( OSErr ) AEInstallObjectAccessor(
								DescType		desiredClass,
							   	DescType		containerType,
							   	accessorProcPtr theAccessor,
							   	long			accessorRefcon,
							   	Boolean			isSysHandler )
= { 0x303C, 0x0937, 0xA816 } ;

F_PASCAL( OSErr ) AERemoveObjectAccessor(
								DescType		desiredClass,
								DescType		containerType,
								accessorProcPtr theAccessor,
								Boolean			isSysHandler )
= { 0x303C, 0x0738, 0xA816 } ;

F_PASCAL( OSErr ) AEGetObjectAccessor(
								DescType 		desiredClass,
								DescType		containerType,
								accessorProcPtr	*accessor,
								long			*accessorRefcon,
								Boolean			isSysHandler )
= { 0x303C, 0x0939, 0xA816 } ;

F_PASCAL( OSErr ) AEDisposeToken( AEDesc *theToken )
= { 0x303C, 0x023A, 0xA816 } ;

F_PASCAL( OSErr ) AECallObjectAccessor(	DescType		desiredClass,
									const AEDesc	*containerToken,
									DescType		containerClass,
									DescType		keyForm,
									const AEDesc	*keyData,
									AEDesc			*token )
= { 0x303C, 0x0C3B, 0xA816 } ;

#else

F_PASCAL( OSErr ) AEObjectInit(void);	/* THINK_C requires the "void" for prototype checking */
/* Not done by inline, but by direct linking into code.  It sets up the pack
  such that further calls can be via inline */

F_PASCAL( OSErr ) AESetObjectCallbacks(
							compareProcPtr		myCompareProc,
							countProcPtr		myCountProc,
							disposeTokenProcPtr myDisposeTokenProc,
							getMarkTokenProcPtr myGetMarkTokenProc,			/* called when mark (below) is true (all 3)*/
							markProcPtr			myMarkProc,
							adjustMarksProcPtr	myAdjustMarksProc,
							getErrDescProcPtr	myGetErrDescProcPtr );




F_PASCAL( OSErr ) AEResolve		(	const AEDesc	*objectSpecifier,
								short			callbackFlags,		/* see above for possible values */
					  			AEDesc			*theToken );


F_PASCAL( OSErr ) AEInstallObjectAccessor(
								DescType		desiredClass,
							   	DescType		containerType,
							   	accessorProcPtr theAccessor,
							   	long			accessorRefcon,
							   	Boolean			isSysHandler );

F_PASCAL( OSErr ) AERemoveObjectAccessor(
								DescType		desiredClass,
								DescType		containerType,
								accessorProcPtr theAccessor,
								Boolean			isSysHandler );

F_PASCAL( OSErr ) AEGetObjectAccessor(
								DescType 		desiredClass,
								DescType		containerType,
								accessorProcPtr	*accessor,
								long			*accessorRefcon,
								Boolean			isSysHandler ); 

F_PASCAL( OSErr ) AEDisposeToken( AEDesc *theToken );

F_PASCAL( OSErr ) AECallObjectAccessor(	DescType		desiredClass,
									const AEDesc	*containerToken,
									DescType		containerClass,
									DescType		keyForm,
									const AEDesc	*keyData,
									AEDesc			*token );

typedef OSErr (pascal *OSLAccessorProcPtr)(DescType desiredClass, const AEDesc *container, DescType containerClass, DescType form, const AEDesc *selectionData, AEDesc *value, long accessorRefcon);
typedef OSErr (pascal *OSLCompareProcPtr)(DescType oper, const AEDesc *obj1, const AEDesc *obj2, Boolean *result);
typedef OSErr (pascal *OSLCountProcPtr)(DescType desiredType, DescType containerClass, const AEDesc *container, long *result);
typedef OSErr (pascal *OSLDisposeTokenProcPtr)(AEDesc *unneededToken);
typedef OSErr (pascal *OSLGetMarkTokenProcPtr)(const AEDesc *dContainerToken, DescType containerClass, AEDesc *result);
typedef OSErr (pascal *OSLGetErrDescProcPtr)(AEDesc **appDescPtr);
typedef OSErr (pascal *OSLMarkProcPtr)(const AEDesc *dToken, const AEDesc *markToken, long index);
typedef OSErr (pascal *OSLAdjustMarksProcPtr)(long newStart, long newStop, const AEDesc *markToken);

typedef OSLAccessorProcPtr OSLAccessorUPP;
typedef OSLCompareProcPtr OSLCompareUPP;
typedef OSLCountProcPtr OSLCountUPP;
typedef OSLDisposeTokenProcPtr OSLDisposeTokenUPP;
typedef OSLGetMarkTokenProcPtr OSLGetMarkTokenUPP;
typedef OSLGetErrDescProcPtr OSLGetErrDescUPP;
typedef OSLMarkProcPtr OSLMarkUPP;
typedef OSLAdjustMarksProcPtr OSLAdjustMarksUPP;

#define NewOSLAccessorProc(userRoutine)		\
		((OSLAccessorUPP) (userRoutine))
#define NewOSLCompareProc(userRoutine)		\
		((OSLCompareUPP) (userRoutine))
#define NewOSLCountProc(userRoutine)		\
		((OSLCountUPP) (userRoutine))
#define NewOSLDisposeTokenProc(userRoutine)		\
		((OSLDisposeTokenUPP) (userRoutine))
#define NewOSLGetMarkTokenProc(userRoutine)		\
		((OSLGetMarkTokenUPP) (userRoutine))
#define NewOSLGetErrDescProc(userRoutine)		\
		((OSLGetErrDescUPP) (userRoutine))
#define NewOSLMarkProc(userRoutine)		\
		((OSLMarkUPP) (userRoutine))
#define NewOSLAdjustMarksProc(userRoutine)		\
		((OSLAdjustMarksUPP) (userRoutine))


#endif



/*
Here are the interfaces your callback procs must be written to:

F_PASCAL( OSErr ) MyCompareProc( DescType comparisonOperator, const AEDesc *theObject,
		const AEDesc *descOrObj, Boolean *result ) ;
F_PASCAL( OSErr ) MyCountProc( DescType desiredType, DescType containerClass,
			const AEDesc *container, long *result:  ) ;
F_PASCAL( OSErr ) MyGetMarkToken( const AEDesc *containerToken, DescType containerClass,
		AEDesc *result ) ;
F_PASCAL( OSErr ) MyMark( const AEDesc *theToken, const AEDesc *markToken, long markCount ) ;
F_PASCAL( OSErr ) MyAdjustMarks( long newStart, long newStop, const AEDesc *markToken ) ;
F_PASCAL( OSErr ) MyMyDisposeToken( AEDesc *unneededToken ) ;
F_PASCAL( OSErr ) MyGetErrDesc( DescPtr *appDescPtr ) ;

F_PASCAL( OSErr ) MyObjectAccessor( DescType desiredClass, const AEDesc *containerToken,
		DescType containerClass, DescType keyForm, const AEDesc *keyData,
		AEDesc *theToken, long *theRefcon ) ;

You'll probably want to have a number of these last ones.
A proc that finds a line within a document should be installed with 'line' and 'docu'
as the desiredClass and containerClass fields in the call to AEInstallObjectHandler().
*/

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
