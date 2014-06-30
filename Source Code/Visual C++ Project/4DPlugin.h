/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.h
 #	source generated by 4D Plugin Wizard
 #	Project : Win32API
 #	author :  dcompton & dschilling
 #	6/21/2001
 #
 #  
 # --------------------------------------------------------------------------------*/


#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#ifndef __4DPLUGIN_H__
#define __4DPLUGIN_H__

#include <Windows.h>
#include <TlHelp32.h> // for snapshot of process in win95/98 etc
#include <stdio.h> // for snapshot
#include "Defines.h"

// MWD 10/21/05 #9246 Define Function for DLL entrypoint.
#ifdef _cplusplus
  extern "C" __declspec(dllexport) { 
#endif
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
#ifdef _cplusplus
  }
#endif

// --- Win32API Commands
void gui_GetWindow( PA_PluginParameters params, HWND hWnd );
void gui_GetWndRect( PA_PluginParameters params );
void gui_SetWndRect( PA_PluginParameters params );
void gui_ShowWindow( PA_PluginParameters params );
void sys_GetUserName( PA_PluginParameters params );
void gui_SetWindowTitle( PA_PluginParameters params );
void sys_IsMultiByte( PA_PluginParameters params );
void gui_DisableCloseBox( PA_PluginParameters params );
void gui_SetWindowLong( PA_PluginParameters params );
void gui_WinHelp( PA_PluginParameters params );
void sys_GetDefPrinter( PA_PluginParameters params );
void sys_SetDefPrinter( PA_PluginParameters params );
void sys_EnumPrinters( PA_PluginParameters params );
void gui_DelMenuItem( PA_PluginParameters params );
void gui_GetOpenFileName( PA_PluginParameters params );
void gui_GetSaveFileName( PA_PluginParameters params );
void gui_LoadIcon( PA_PluginParameters params );
void gui_SetIcon( PA_PluginParameters params );
void gui_GetWindowFrom4DWin( PA_PluginParameters params );
void sys_GetRegionSettings( PA_PluginParameters params, BOOL arraySupplied );
void sys_GetTimeZone( PA_PluginParameters params );
void sys_GetUTCOffset( PA_PluginParameters params );
void gui_GetDisplayFontDPI( PA_PluginParameters params);
void sys_GetPrintJob( PA_PluginParameters params ); // added 7/16/01 -- rewritten 10/23/01
void sys_GetGUID( PA_PluginParameters params ); //added 7/30/01
void gui_FlashWindow( PA_PluginParameters params ); //added 08/04/01 filled unused case (10)
void sys_GetRoutes ( PA_PluginParameters params ); //added 08/08/01
void sys_GetNetworkInfo( PA_PluginParameters params ); //added 08/09/01
LONG_PTR sys_GetOSVersion( BOOL bInternalCall, PA_PluginParameters params ); //added as plugin call 0828/01 was internal only
void sys_PlayWav( PA_PluginParameters params ); //add 10/16/01
void sys_GetWindowMetrics( PA_PluginParameters params ); // added 11/09/01
void gui_LoadBackground( PA_PluginParameters params, BOOL DeInit ); // added 11/09/01
void sys_SetClientTime( PA_PluginParameters params ); // added 11/30/01
void sys_SetClientDate( PA_PluginParameters params ); //added 11/30/01
void gui_ToolTipCreate( PA_PluginParameters params ); // added 12/06/01
void gui_ToolTipShowOnObject( PA_PluginParameters params ); // added 12/17/01
void gui_ToolTipShowOnCoord( PA_PluginParameters params ); // added 12/17/01
void gui_ToolTipHide( PA_PluginParameters params ); // added 12/17/01
void gui_ToolTipDestroyControl( PA_PluginParameters params ); // added 12/17/01
void gui_SetTrayIcon( PA_PluginParameters params ); // added/revised 12/18/01
void sys_FileCheck( PA_PluginParameters params ); //added 01/08/02
void sys_GetCommandLine( PA_PluginParameters params ); //added 04/02/02
void gui_GetWindowStyle	( PA_PluginParameters params ); //added 07/18/02
void gui_RestrictWindow( PA_PluginParameters params ); //added 07/15/02
void gui_GetWindowState( PA_PluginParameters params ); // added 07/22/02
void gui_SubClassInit( PA_PluginParameters params ); // added 07/26/02
void sys_ShellExecute( PA_PluginParameters params ); // added 08/21/02
void sys_IsAppLoaded( PA_PluginParameters params ); // added 08/22/02
void gui_SelectColor( PA_PluginParameters params ); //added 11/25/02 (3.5.3)
void gui_RespectToolBar( PA_PluginParameters params ); // added 01/16/03 (3.5.4)
void sys_IsConnectedToInternet(PA_PluginParameters params );  // added 08/14/03 (3.5.5)
void gui_GetSysColor( PA_PluginParameters params ); // added 12/02/03 (3.5.6)
void gui_SetSysColor( PA_PluginParameters params ); // added 12/02/03 (3.5.6)
void sys_GetEnv( PA_PluginParameters params ); // added 12/02/03 (3.5.6)
void sys_SetEnv( PA_PluginParameters params ); // added 12/02/03 (3.5.6)
void sys_GetRegKey( PA_PluginParameters params ); // added 12/04/03 (3.5.6)
void sys_GetRegType( PA_PluginParameters params ); // added 12/14/03 (3.5.6)
void sys_GetRegEnum( PA_PluginParameters params ); // added 12/14/03 (3.5.6)
void sys_GetDocumentList( PA_PluginParameters params ); // added 6/4/04 (3.6)
void sys_SetPluginLanguage( PA_PluginParameters params ); // MWD & Mark De Wever 10/2/06 #12225
void gui_SetWindowStyle( PA_PluginParameters params ); // MWD & Mark De Wever 10/2/06 #12225
void sys_FileExists( PA_PluginParameters params ); // MWD & Mark De Wever 10/2/06 #12225
void sys_DirectoryExists( PA_PluginParameters params ); // MWD & Mark De Wever 10/2/06 #12225
void sys_PrintDirect2Driver ( PA_PluginParameters params ); // MWD 8/15/06 #10250
void sys_KillProcessByName ( PA_PluginParameters params ); // MWD 11/30/06 #12572
void sys_KillProcessByID ( PA_PluginParameters params ); // MWD 11/30/06 #12572
void sys_EnumProcesses ( PA_PluginParameters params ); // MWD 11/30/06 #12572
void sys_LogonUser(PA_PluginParameters params); // MWD 1/8/07 #6982
void sys_GetTimeZoneList( PA_PluginParameters params ); // REB 4/6/09 #19472
void TWAIN_GetSources ( PA_PluginParameters params ); //REB 6/23/09 #14151
void TWAIN_SetSource( PA_PluginParameters params ); // REB 6/23/09 #14151
void TWAIN_AcquireImage( PA_PluginParameters params ); // REB 6/23/09 #14151
void sys_IsAppFrontmost( PA_PluginParameters params ); // REB 8/21/09
void gui_MessageBox (PA_PluginParameters params); // DJD 2008-09-12
void gui_HideTaskBar (PA_PluginParameters params); // REB 1/8/10 #22389
void gui_ShowTaskBar (PA_PluginParameters params); // REB 1/8/10 #22389
void gui_HideTitleBar (PA_PluginParameters params); // REB 1/8/10 #22389
void gui_ShowTitleBar (PA_PluginParameters params); // REB 1/8/10 #22389
void gui_SetMDIOpaque (PA_PluginParameters params); // REB 1/8/10 #22389
void gui_SetMDITransparent (PA_PluginParameters params); // REB 1/8/10 #22389
void gui_MaximizeMDI (PA_PluginParameters params); // REB 1/8/10 #22389
void gui_MinimizeMDI (PA_PluginParameters params); // REB 1/8/10 #22389
void gui_RestoreMDI (PA_PluginParameters params); // REB 1/8/10 #22389
void sys_DisableTaskManager (PA_PluginParameters params); // REB 1/8/10 #22389
void sys_EnableTaskManager (PA_PluginParameters params); // REB 1/8/10 #22389
void sys_SetRegKey( PA_PluginParameters params, LONG_PTR selector ); // REB 11/17/10 #25402
void sys_IsAppRunningAsService( PA_PluginParameters params ); // REB 1/12/11 #25587
void sys_CompareBLOBs( PA_PluginParameters params); // REB 11/9/12 TESTING
// ----- Other modules -------
//window background-related
LRESULT APIENTRY BkgrndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


//print job-related 
LRESULT APIENTRY newProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // added 10/24/01
LRESULT APIENTRY newPrtSettingsDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // added 10/24/01
LRESULT CALLBACK printSettingsDlgHook( INT_PTR hCode, WPARAM wParam, LPARAM lParam); //added 10/24/01
BOOL CALLBACK EnumChildProc2(HWND hWnd, LPARAM lParam); //added 10/24/01
LRESULT APIENTRY newPrtDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // added 10/24/01
LRESULT CALLBACK printDlgHook( INT_PTR hCode, WPARAM wParam, LPARAM lParam); //added 10/24/01
LRESULT CALLBACK keyboardLLHook(INT_PTR code, WPARAM wParam, LPARAM lParam); // REB 1/8/10 #22389

//file open/save-related
BOOL CALLBACK ComDlg32DlgProc (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam); // added 7/27/01
LRESULT CALLBACK theHook( INT_PTR hCode, WPARAM wparam, LPARAM lParam); //added 07/27/01
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam); //added 07/27/01
BOOL NEAR PASCAL TestNotify(HWND hDlg, LPOFNOTIFY pofn);
void ProcessCDError(DWORD dwErrorCode, HWND hWnd);
LRESULT CALLBACK postHook( INT_PTR hCode, WPARAM wParam, LPARAM lParam); //added 07/30/01

void GetPlugInFullName( char *PlugInFullName); // added 07/27/01
void FormatIP(char *str, LPARAM dwIP); // added 08/09/01

HWND getWindowHandle(char* windowTitle, HWND wHnd);

DWORD GetDllVersion( LPCTSTR lpszDllName ); //added 12/08/01

LRESULT CALLBACK GetMsgProc(INT_PTR nCode, WPARAM wParam, LPARAM lParam); //added 12/08/01
LRESULT APIENTRY ProToolsProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); //added 07/15/02


VOID createNewProcess( VOID); // added 12/17/01

//tray icon related
LONG_PTR getTrayIconParams( PA_PluginParameters params, LONG_PTR *pAction, LONG_PTR *pFlags, LONG_PTR *pIconID, LONG_PTR *pProcessNbr,
											 LONG_PTR *pIconHndl, char* szTipParam, char* szBalloonInfo, char* szBalloonTitle );
LONG_PTR findIconID( pTI *, LONG_PTR iconID, LONG_PTR *pProcNbr );
LONG_PTR deleteIcon( pTI *, LONG_PTR iconID );
BOOL insertIcon( pTI *, LONG_PTR iconID, LONG_PTR procNbr );
BOOL readIconInfo( pTI *, LONG_PTR, LONG_PTR*, LONG_PTR*);
LONG_PTR updateIconIdProcNbr( pTI *pIcon, LONG_PTR iconID, LONG_PTR processNbr );
LONG_PTR sizeOfTI(pTI);
LONG_PTR isEmpty(pTI);
VOID Delay(DWORD delayTime);
void processWindowMessage(LONG_PTR source, LONG_PTR hwnd, WPARAM wParam, LPARAM lParam); // added 1st param 01/22/03, chgd 2nd to LONG_PTR from HWND

BOOL checkTimeProvider( void ); // added 01/04/02

LONG_PTR enumPrintersUsingRegistry( PA_Variable *printerArray ); // added 04/20/02
LONG_PTR enumPrintersUsingINI( PA_Variable *printerArray ); // added 04/20/02

// linked list related (this is an unordered list)
void  init_list (pLL* pStartOfList);
void  clear_list(pLL* pStartOfList);
BOOL  search_list(pLL* pSOL, pLL *thisLink, pLL *previousLink, LONG_PTR dataPosition, LONG_PTR matchType, LONG_PTR* pSearchKey); 
void* insert_list(pLL* pSOL);
BOOL  delete_list(pLL* pSOL, LONG_PTR dataPosition, LONG_PTR matchType, LONG_PTR* pSearchKey);
LONG_PTR  length_list(pLL pStartOfList);

HINSTANCE getPSapiPointers(LPFNENUMPROC *ppEnumProc,	LPFNENUMPROCMODS *ppEnumProcMods,	LPFNGETMODFNAME *ppGetModFName);


BOOL reviewProcesses(HANDLE hPSapiDLL, LPFNENUMPROC lpfnEnumProc,	LPFNENUMPROCMODS lpfnEnumProcMods,
										 LPFNGETMODFNAME lpfnGetModFName, char *pAppName, BOOL bUseToolHelp);

BOOL restoreOrig4DWindowProcess(); //added 01/21/03
VOID subclass4DWindowProcess(); // MJG 3/26/04 

// registry related
HKEY getRootKey(INT_PTR key);
INT_PTR get4dRegType(INT_PTR key);
LONG_PTR regGetNumElements(char *pValue);
void regExpandStr(char **pValue);

// Callback for Clean process termination
BOOL CALLBACK TerminateClean(HWND hWnd, LPARAM lparam); // MWD 1/8/07 #5421

#endif // __4DPLUGING_H__
