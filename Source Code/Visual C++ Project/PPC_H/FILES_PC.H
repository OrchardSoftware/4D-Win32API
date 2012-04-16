#undef PBOpen
#undef PBClose
#undef PBRead
#undef PBWrite
#undef PBGetVInfo
#undef PBGetVol
#undef PBSetVol
#undef PBFlushVol
#undef PBCreate
#undef PBDelete
#undef PBOpenDF
#undef PBOpenRF
#undef PBRename
#undef PBGetFInfo
#undef PBSetFInfo
#undef PBSetFLock
#undef PBRstFLock
#undef PBSetFVers
// #undef PBAllocate
#undef PBGetEOF
#undef PBSetEOF
#undef PBGetFPos
#undef PBSetFPos
#undef PBFlushFile
#undef PBCatSearch
#undef PBOpenWD
#undef PBCloseWD
#undef PBHSetVol
#undef PBHGetVol
#undef PBCatMove
#undef PBDirCreate
#undef PBGetWDInfo
#undef PBGetFCBInfo
#undef PBGetCatInfo
// #undef PBSetCatInfo
#undef PBAllocContig
#undef PBLockRange
#undef PBUnlockRange
#undef PBSetVInfo
#undef PBHGetVInfo
#undef PBHOpen
#undef PBHOpenRF
//#undef PBHOpenDF
#undef PBHCreate
#undef PBHDelete
#undef PBHRename
#undef PBHRstFLock
#undef PBHSetFLock
#undef PBHGetFInfo
#undef PBHSetFInfo
#undef PBMakeFSSpec
#undef PBHGetVolParms
#undef PBHGetLogInInfo
#undef PBHGetDirAccess
#undef PBHSetDirAccess
#undef PBHMapID
#undef PBHMapName
#undef PBHCopyFile
#undef PBHMoveRename
#undef PBHOpenDeny
#undef PBHOpenRFDeny
#undef PBExchangeFiles
#undef PBCreateFileIDRef
#undef PBResolveFileIDRef
#undef PBDeleteFileIDRef
#undef PBGetForeignPrivs
#undef PBSetForeignPrivs
#undef PBDTAddIcon
#undef PBDTGetIcon
#undef PBDTGetIconInfo
#undef PBDTAddAPPL
#undef PBDTRemoveAPPL
#undef PBDTGetAPPL
#undef PBDTSetComment
#undef PBDTRemoveComment
#undef PBDTGetComment
#undef PBDTFlush
#undef PBDTReset
#undef PBDTGetInfo
#undef PBDTDelete
#undef PBGetAltAccess

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

OSErr pascal PBOpen(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBClose(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBRead(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBWrite(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBGetVInfo(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBGetVol(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBSetVol(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBFlushVol(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBCreate(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBDelete(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBOpenDF(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBOpenRF(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBRename(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBGetFInfo(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBSetFInfo(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBSetFLock(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBRstFLock(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBSetFVers(ParmBlkPtr paramBlock,Boolean async); 
// OSErr pascal PBAllocate(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBGetEOF(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBSetEOF(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBGetFPos(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBSetFPos(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBFlushFile(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBCatSearch(CSParamPtr paramBlock,Boolean async); 
OSErr pascal PBOpenWD(WDPBPtr paramBlock,Boolean async); 
OSErr pascal PBCloseWD(WDPBPtr paramBlock,Boolean async); 
OSErr pascal PBHSetVol(WDPBPtr paramBlock,Boolean async); 
OSErr pascal PBHGetVol(WDPBPtr paramBlock,Boolean async); 
OSErr pascal PBCatMove(CMovePBPtr paramBlock,Boolean async); 
OSErr pascal PBDirCreate(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBGetWDInfo(WDPBPtr paramBlock,Boolean async); 
OSErr pascal PBGetFCBInfo(FCBPBPtr paramBlock,Boolean async); 
OSErr pascal PBGetCatInfo(CInfoPBPtr paramBlock,Boolean async); 
// OSErr pascal PBSetCatInfo(CInfoPBPtr paramBlock,Boolean async); 
OSErr pascal PBAllocContig(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBLockRange(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBUnlockRange(ParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBSetVInfo(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHGetVInfo(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHOpen(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHOpenRF(HParmBlkPtr paramBlock,Boolean async); 
//OSErr pascal PBHOpenDF(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHCreate(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHDelete(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHRename(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHRstFLock(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHSetFLock(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHGetFInfo(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHSetFInfo(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBMakeFSSpec(HParmBlkPtr paramBlock,Boolean async); 
OSErr pascal PBHGetVolParms(HParmBlkPtr paramBlock,Boolean async);

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


