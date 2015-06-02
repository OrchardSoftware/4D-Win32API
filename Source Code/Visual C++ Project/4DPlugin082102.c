
#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"
#include <shellapi.h>
#include <wininet.h>
#include <Windows.h>
#include <stdlib.h>

#include "psapi.h"  // for enumerating processes in NT & 2000
#include "utilities.h"
#include "language.h" // Added by Mark De Wever #12225
#include <tlhelp32.h>

extern struct		WINDOWHANDLES
{
	HWND		fourDhWnd;
	HWND		prtSettingshWnd;
	HWND		prthWnd;
	HWND		MDIhWnd;
	HWND		hwndTT;
	HWND		displayedTTOwnerhwnd;
	HWND		openSaveTBhwnd;
	HWND		MDIs_4DhWnd; 
} windowHandles;

extern struct		PROCESSHANDLES
{
	WNDPROC		wpFourDOrigProc;
	WNDPROC		wpPrintSettingsDlgOrigProc;
	WNDPROC		wpPrintDlgOrigProc;
	WNDPROC		wpProToolsOrigProc;
} processHandles;

	struct		TOOLBARRESTRICT
{
	LONG_PTR		toolBarOnDeck;
	LONG_PTR		top;
	LONG_PTR		left;
	LONG_PTR		right;
	LONG_PTR		bottom;
	INT_PTR			topProcessNbr;
	INT_PTR			leftProcessNbr;
	INT_PTR			rightProcessNbr;
	INT_PTR			bottomProcessNbr;
	LONG_PTR		trackingRestriction;
	LONG_PTR		appBeingMaxed;
	LONG_PTR		appWindowState;
	RECT		origWindowRect;
	LONG_PTR		clientOffsetx;
	LONG_PTR		clientOffsety;
	char		minimizedWindows[SMLBUF][SMLBUF]; // REB 8/11/08 #16207 
	RECT		previousWindowRect; // REB 3/26/10
} toolBarRestrictions;



// ------------------------------------------------
// 
//  FUNCTION: sys_ShellExecute( PA_PluginParameters params)
//
//  PURPOSE:	Launch a program or open a folder
//
//  COMMENTS:	Pretty much a direct mapping to ShellExecute.  
//						Returns value >32 if successful.  <32 if error.
//        
//	DATE:			dcc 08/21/02
//
//	MODIFICATIONS: 10/28/02 replace forward slashes in a provided UNC with backslashes (3.5.2)
//				   11/21/01 made parameters longer for LONG_PTR urls etc (3.5.3)
//                 08/08/04 Removed the limit on the length of the parameters.
//				   03/03/14 Rewrote method to accept Unicode parameters // AMS #38727

void sys_ShellExecute( PA_PluginParameters params )
{
	LONG_PTR returnValue = 0;
	LONG_PTR len = 0;
	char returnText[255]; // MWD & Mark De Wever #12225
	INT_PTR	 howToShow;
	//char *pChar;

	//char *operation = NULL;
	//char *file = NULL;
	//char *parameters = NULL;
	//char *directory = NULL;

	PA_Unistring *UnistringFile;
	PA_Unistring *UnistringOperation; 
	PA_Unistring *UnistringParameters; 
	PA_Unistring *UnistringDirectory; 

	PA_Unichar *file; 
	PA_Unichar *operation; 
	PA_Unichar *parameters; 
	PA_Unichar *directory; 
	PA_Unichar *pChar; 

	// Get the function parameters.
	//operation = getTextParameter(params, 1);
	//file = getTextParameter(params, 2);
	//parameters = getTextParameter(params, 3);
	//directory = getTextParameter(params, 4);

	UnistringOperation = PA_GetStringParameter( params, 1); 
	UnistringFile = PA_GetStringParameter( params, 2); 
	UnistringParameters = PA_GetStringParameter( params, 3); 
	UnistringDirectory = PA_GetStringParameter( params, 4); 
	howToShow = PA_GetLongParameter( params, 5 ); 

	operation = PA_GetUnistring(UnistringOperation); // AMS /28/14
	file = PA_GetUnistring(UnistringFile); // AMS 2/28/14
	len = (PA_GetUnistringLength(UnistringFile) + 1); // AMS 2/28/14
	parameters = PA_GetUnistring(UnistringParameters); // AMS 2/28/14
	directory = PA_GetUnistring(UnistringDirectory); // AMS 2/28/14

	//if ((strcmp(_strlwr(operation), "open")			!= 0) &&
	//	 (strcmp(_strlwr(operation),  "explore")	!= 0) &&
	//	 (strcmp(_strlwr(operation),  "print")		!= 0) &&
	//	 (file == NULL || strlen(file) == 0)) {
	//	//strcpy(returnText, "Invalid Operation");
	//	strncpy(returnText, message->InvalidOperation, 255); // Mark De Wever #12225 replaced the line above
	//}
	if((lstrcmpiW(operation, L"open")    != 0) &&
	   (lstrcmpiW(operation, L"explore") != 0) &&
	   (lstrcmpiW(operation, L"print")   != 0) &&
	   (file == NULL || len == 0)) {
		strncpy(returnText, message->InvalidOperation, 255); 
	}
	else if (howToShow > 11) {
		//strcpy(returnText, "Invalid HowToShow Constant");
		strncpy(returnText, message->InvalidShowConstant, 255); // Mark De Wever #12225 replaced the line above
	}
	else
	{
		pChar = file; // added 10/28/02 shellExecute wants backslashes
		do {
			if (*pChar == '/') {
				*pChar = '\\';
			}
		} while (*pChar++ != '\0') ;
		
		if (directory != NULL) {
			pChar = directory;
			do  {
				if (*pChar == '/') {
					*pChar = '\\';
				}
			} while (*pChar++ != '\0');
		}
		
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE); // AMS 5/6/14 #38727

		returnValue = (LONG_PTR) ShellExecuteW(NULL, operation, file, parameters, directory, howToShow);
		
		strcpy(returnText, "");
		if (returnValue <= 32) { // error occurred
			switch (returnValue)
			{
			case ERROR_FILE_NOT_FOUND :
				//strcpy(returnText, "File Not Found");		
				strncpy(returnText, message->FileNotFound, 255); // Mark De Wever #12225 replaced line above
				break;
				
			case ERROR_PATH_NOT_FOUND :
				//strcpy(returnText, "Path Not Found");
				strncpy(returnText, message->PathNotFound, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case ERROR_BAD_FORMAT :
				//strcpy(returnText, ".EXE File is Invalid");		
				strncpy(returnText, message->BadFormat, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case SE_ERR_ACCESSDENIED :
				//strcpy(returnText, "OS Denied Access to File");		
				strncpy(returnText, message->AccessDenied, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case SE_ERR_ASSOCINCOMPLETE  :
				//strcpy(returnText, "File Name Association is Incomplete or Invalid");		
				strncpy(returnText, message->AssocIncomplete, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case SE_ERR_DDEBUSY  :
			case SE_ERR_DDEFAIL   :
				//strcpy(returnText, "DDE Transaction Could Not be Completed");
        strncpy(returnText, message->DDEFail, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case SE_ERR_DDETIMEOUT   :
				//strcpy(returnText, "DDE Request Timed Out");		
				strncpy(returnText, message->DDETimeOut, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case SE_ERR_DLLNOTFOUND    :
				//strcpy(returnText, "DLL Libray Not Found");
        strncpy(returnText, message->DLLNotFound, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case SE_ERR_NOASSOC   :
				//strcpy(returnText, "No Application Associated with File Extenstion");
        strncpy(returnText, message->NoAssoc, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case SE_ERR_OOM :
				//strcpy(returnText, "Insufficient Memory");
        strncpy(returnText, message->OOM, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			case SE_ERR_SHARE    :
				//strcpy(returnText, "Sharing Violation Occurred");
        strncpy(returnText, message->ShareViolation, 255); // Mark De Wever #12225 Replaced line above
				break;
				
			default:
				//strcpy(returnText, "Unknown error occurred");	
				strncpy(returnText, message->Default, 255); // Mark De Wever #12225 Replaced line above
				break;
			}
		}
	}

	//freeTextParameter(operation);
	//freeTextParameter(file);
	//freeTextParameter(parameters);
	//freeTextParameter(directory);

	PA_ReturnText( params, returnText, strlen(returnText));
}

// ------------------------------------------------
// 
//  FUNCTION: sys_IsAppLoaded( PA_PluginParameters params )
//
//  PURPOSE:	Check if a program is loaded
//
//  COMMENTS:	Win98 uses different approach than NT4 and up
//						Function pointers used for OS specific functions.
//        
//	DATE:			dcc 08/23/02
//
//	MODIFICATIONS: 
//
void sys_IsAppLoaded( PA_PluginParameters params )
{
	LPFNENUMPROC				lpfnEnumProc = NULL;
	LPFNENUMPROCMODS		lpfnEnumProcMods = NULL;
	LPFNGETMODFNAME			lpfnGetModFName = NULL;
	HINSTANCE						hPSapiDLL;
	BOOL								bFuncReturn, bUseToolHelp = FALSE;
	char								appName[100]; 
	LONG_PTR								appName_len;

	appName_len						 = PA_GetTextParameter( params, 1, appName );
	appName[appName_len]	 = '\0';

	hPSapiDLL = (HINSTANCE) getPSapiPointers(&lpfnEnumProc, &lpfnEnumProcMods, &lpfnGetModFName);

	bFuncReturn = reviewProcesses(hPSapiDLL, lpfnEnumProc, lpfnEnumProcMods, lpfnGetModFName,
								appName, bUseToolHelp);

	PA_ReturnLong( params, bFuncReturn);
}

// ------------------------------------------------
// 
//  FUNCTION: getPSapiPointers(LPFNENUMPROC *ppEnumProc,	LPFNENUMPROCMODS *ppEnumProcMods,
//									LPFNGETMODFNAME *ppGetModFName, char *pAppName,
//									LONG_PTR osver, BOOL bUseToolHelp)
//
//  PURPOSE:	Tests if psAPI library is available
//
//  COMMENTS:	Win98 uses different approach than NT4 and up
//						
//        
//	DATE:			dcc 08/23/02
//
//	MODIFICATIONS: 
//
HINSTANCE getPSapiPointers(LPFNENUMPROC *ppEnumProc,	LPFNENUMPROCMODS *ppEnumProcMods,
											LPFNGETMODFNAME *ppGetModFName)
{
	HINSTANCE			hPSapiDLL = (HINSTANCE)0;

	//load psapi.dll to get address pointers for enumProcesses etc
	
	hPSapiDLL = LoadLibrary("psAPI.dll");
	if (hPSapiDLL != NULL) {
		*ppEnumProc = (LPFNENUMPROC)GetProcAddress(hPSapiDLL, "EnumProcesses");
		*ppEnumProcMods = (LPFNENUMPROCMODS)GetProcAddress(hPSapiDLL, "EnumProcessModules");
		*ppGetModFName = (LPFNGETMODFNAME)GetProcAddress(hPSapiDLL, "GetModuleFileNameExA");
	}

	return hPSapiDLL;
}

// ------------------------------------------------
// 
//  FUNCTION: reviewProcesses(HANDLE hPSapiDLL, LPFNENUMPROC lpfnEnumProc,	LPFNENUMPROCMODS lpfnEnumProcMods,
//										 LPFNGETMODFNAME lpfnGetModFName, char *pAppName, BOOL bUseToolHelp)
//
//  PURPOSE:	Looks for pAppName as running app
//
//  COMMENTS:	
//						
//        
//	DATE:			dcc 08/23/02
//
//
BOOL reviewProcesses(HANDLE hPSapiDLL, LPFNENUMPROC lpfnEnumProc,	LPFNENUMPROCMODS lpfnEnumProcMods,
										 LPFNGETMODFNAME lpfnGetModFName, char *pAppName, BOOL bUseToolHelp)
{
	LONG_PTR					returnValue = 0;
	DWORD					aProcesses[2048], cbNeeded, cProcesses, cMods; // AMS 7/7/14 #39738 Changed aProcesses value from 1024 to 2048 
	UINT					i, j;
	char					szProcessName[MAX_PATH] = "Unknown";
	char					szModName[MAX_PATH];
	HMODULE					hMods[2048]; // AMS 7/7/14 #39738 Changed 1024 to 2048
	HANDLE					hProcess	 = NULL, hProcessSnap = NULL;
	PROCESSENTRY32			pe32			 = {0};
	BOOL					bFuncReturn = FALSE;
	MODULEENTRY32			me32			 = {0};
	HINSTANCE				hKernelDLL = (HINSTANCE)0;
	LPFNCREATETOOLHELPSNAP	lpfnCreateToolHelpSnap = NULL;
	LPFNPROCFIRST			lpfnProcessFirst = NULL;
	LPFNPROCNEXT			lpfnProcessNext = NULL;

	if (bUseToolHelp || hPSapiDLL == NULL) { // win95/98/Me or NT351 // 5/6/04 Use win95 way if the psAPI.dll is not found. 
		hKernelDLL = LoadLibrary("kernel32.dll");
		if (hKernelDLL == NULL) {
			return 0;
		} else {

			lpfnCreateToolHelpSnap = (LPFNCREATETOOLHELPSNAP) GetProcAddress(hKernelDLL, "CreateToolhelp32Snapshot");
			lpfnProcessFirst = (LPFNPROCFIRST) GetProcAddress(hKernelDLL, "Process32First");
			lpfnProcessNext  = (LPFNPROCNEXT) GetProcAddress(hKernelDLL, "Process32Next");

			if ((lpfnCreateToolHelpSnap == NULL) || (lpfnProcessFirst == NULL) || (lpfnProcessNext == NULL)) {
				return 0;
			} else {
				//take a snapshot of all processes
				hProcessSnap = lpfnCreateToolHelpSnap(TH32CS_SNAPPROCESS, 0);
				if (hProcessSnap == INVALID_HANDLE_VALUE) return 0;
				//fill in size of structure
				pe32.dwSize = sizeof(PROCESSENTRY32);

				if (lpfnProcessFirst(hProcessSnap, &pe32)) {
					if(strstr(_strlwr(pe32.szExeFile), _strlwr(pAppName)) != NULL) {
						returnValue = 1;
					} else {

						while(lpfnProcessNext(hProcessSnap, &pe32))
						{
							if(strstr(_strlwr(pe32.szExeFile), pAppName) != NULL) {
								returnValue = 1;
								break;
							}
						}
					}
					CloseHandle(hProcessSnap);
				} else {
					returnValue = 0;
					CloseHandle(hProcessSnap);
				} //end if (Process32First(hProcessSnap, &pe32))
			} //end if ((lpfnCreateToolhelpSnap == NULL) || (lpfnProcessFirst == NULL) || (lpfnProcessNext == NULL))
		} //end if (hKernelDLL == NULL) 

		return returnValue;

	} else { // otherwise can use process enumeration

		// get list of process identifiers
		// lpfnEnumProc - EnumProcesses function
		bFuncReturn = lpfnEnumProc( aProcesses, sizeof(aProcesses), &cbNeeded ); 
		if (!bFuncReturn) return returnValue;
		//calculate how many process identifies were returned
		cProcesses = cbNeeded / sizeof(DWORD);
		//check if one of these is Win32API.4DX or ASIPPC.DLL
		// app exe name can be changed so cannot rely on it being 4D anything
		for ( i = 0; i < cProcesses; i++ )
		{
			//get a handle to the process
			hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i] );
			//get process name
			if (hProcess) {
				// lpfnEnumProcMods - EnumProcessModules
				if (lpfnEnumProcMods( hProcess, hMods, sizeof(hMods), & cbNeeded)) {
					cMods = cbNeeded / sizeof(HMODULE);
					for ( j = 0; j < cMods; j++ ) {
						// lpfnGetModFName - GetModuleFileNameEx
						if ( lpfnGetModFName (hProcess, hMods[j], szModName, sizeof(szModName))) {
							//does name contain what we are looking for?
							if (strstr(_strlwr(szModName), _strlwr(pAppName)) == NULL) {
								returnValue = 0; 
							} else { //found it so keep it going
								return 1;
								break;
							}
						}
					}
				} //end if (EnumProcessModules( hProcess, &hMod, sizeof(hMod), & cbNeeded))
			} // end if (hProcess)
			CloseHandle( hProcess );
		} // end for ( i = 0; i < cProcesses; i++ )
	} // end if (bUseToolHelp)

	//if (returnValue == -1) {
		FreeLibrary(hPSapiDLL);
	//}
	return returnValue;
}

// ------------------------------------------------
// 
//  FUNCTION: gui_SelectColor( PA_PluginParameters params)
//
//  PURPOSE:  Displays the color common dialog
//
//  COMMENTS:	
//						
//        
//	DATE:	  dcc 11/25/02 (3.5.3)
//
//	MODIFICATIONS:
//
void gui_SelectColor( PA_PluginParameters params)
{
	CHOOSECOLOR			cColor;
	static COLORREF		acrCustColor[16];
	static DWORD		rgbCurrent;
	LONG_PTR				rParam, gParam, bParam, returnValue = 0, i, hasCustom;
	PA_Variable			custColorArray;
	
	rParam = PA_GetLongParameter( params, 1); 
	gParam = PA_GetLongParameter( params, 2); 
	bParam = PA_GetLongParameter( params, 3);
	hasCustom   = PA_GetLongParameter( params, 4);

	if (rParam > 255) rParam = 0;
	if (gParam > 255) gParam = 0;
	if (bParam > 255) bParam = 0;

	if (hasCustom == 1) {
		custColorArray = PA_GetVariableParameter( params, 5 );
		if(PA_GetVariableKind(custColorArray) == eVK_ArrayLongint) {
			for (i = 0; i < PA_GetArrayNbElements(custColorArray); i++)
			{
				acrCustColor[i] = PA_GetLongintInArray(custColorArray, i + 1);
			}
		}
	}
	ZeroMemory(&cColor, sizeof(CHOOSECOLOR));
	cColor.lStructSize		= sizeof(CHOOSECOLOR);
	cColor.hwndOwner			= windowHandles.fourDhWnd;
	cColor.lpCustColors		= (LPDWORD) acrCustColor;
	cColor.rgbResult			= rgbCurrent;

	if ((rParam > 0) || (gParam > 0) || (bParam > 0)) {
		cColor.rgbResult = RGB(rParam, gParam, bParam);
		cColor.Flags = CC_FULLOPEN | CC_RGBINIT;
	} else {
		cColor.Flags = CC_FULLOPEN;
	}
	
	if (ChooseColor(&cColor)== TRUE) {
		rgbCurrent = cColor.rgbResult;

		rParam = GetRValue(rgbCurrent);
		gParam = GetGValue(rgbCurrent);
		bParam = GetBValue(rgbCurrent);
		
		PA_SetLongParameter( params, 1, rParam );
		PA_SetLongParameter( params, 2, gParam );
		PA_SetLongParameter( params, 3, bParam );

		if (hasCustom == 1) {
			PA_ResizeArray(&custColorArray, 16);
			for (i = 0; i < 16; i++)
			{
				PA_SetLongintInArray(custColorArray, i + 1, acrCustColor[i]);
			}
			PA_SetVariableParameter( params, 5, custColorArray, 0);
		}
		returnValue = 1;
	}

	PA_ReturnLong( params, returnValue);

}

// ------------------------------------------------
// 
//  FUNCTION: gui_RespectToolBar( PA_PluginParameters params )
//
//  PURPOSE:  Function to intecept maximize event in the 
//						ProToolsProc.  There the window maximized 
//						size is reduced by the number of pixels
//						passed into the function. 
//
//  COMMENTS:	This has been experimented with but should be tested
//						more thoroughly.  Some situations still cause a max'd
//						window to resize under a toolbar. Noticed in particular
//						when a new process window pops up over another process's 
//						max'd window.  This is left for an excersize for some adventurous
//						soul.
//        
//	DATE:			dcc 01/16/03 (3.5.4)
//
//	MODIFICATIONS:
//
void gui_RespectToolBar( PA_PluginParameters params )
{
	LONG_PTR					position_len = 0, tbRestriction = 0;
	LONG_PTR					trackingRestriction = 0, processNbr;
	char					position[2];
	INT_PTR						theChar;


	tbRestriction	 = PA_GetLongParameter( params, 1); 
	position_len			 = PA_GetTextParameter( params, 2, position );
	position[position_len]	 = '\0';
	if (position_len > 1) {
		position[1] = '\0';
	}
	_strlwr(position);

	if (position_len == 0) {	// pass a negative in and turns it all off
		toolBarRestrictions.toolBarOnDeck = 0;
		//restoreOrig4DWindowProcess(); // MJG 3/26/04 The 4D window will remain subclassed until the plug-in is unloaded.
		PA_ReturnLong( params, toolBarRestrictions.toolBarOnDeck);
		return;
	}

	trackingRestriction = PA_GetLongParameter( params, 3);
	toolBarRestrictions.trackingRestriction  = trackingRestriction;

	if ((tbRestriction != 0) &&  (strpbrk(position, "ltrb") != NULL)) {
		toolBarRestrictions.toolBarOnDeck = 1;
		theChar = (INT_PTR) position[0];
	}

	processNbr = PA_GetCurrentProcessNumber();

	switch (theChar)
	{
		case 'l' :
			toolBarRestrictions.left = tbRestriction;
			toolBarRestrictions.leftProcessNbr = processNbr;
			break;

		case 't' :
			toolBarRestrictions.top = tbRestriction;
			toolBarRestrictions.topProcessNbr = processNbr;
			break;
		case 'r' :
			toolBarRestrictions.right = tbRestriction;
			toolBarRestrictions.rightProcessNbr = processNbr;
			break;

		case 'b' :
			toolBarRestrictions.bottom = tbRestriction;
			toolBarRestrictions.bottomProcessNbr = processNbr;
			break;
	}

	toolBarRestrictions.appBeingMaxed = 0;
  
	//if (processHandles.wpFourDOrigProc == NULL) {
	//	processHandles.wpFourDOrigProc = (WNDPROC) SetWindowLong(windowHandles.fourDhWnd, GWL_WNDPROC, (LONG) newProc);
	//}
	// MJG 3/26/04 Replaced code above with function call.
	subclass4DWindowProcess();

	PA_ReturnLong( params, toolBarRestrictions.toolBarOnDeck);
}

// ------------------------------------------------
// 
//  FUNCTION: sys_IsConnectedToInternet( PA_PluginParameters params )
//
//  PURPOSE:  Determine if a connection exists to the internet. 
//
//  COMMENTS: Original source provided by Michael Bond.
//        
//	DATE:	  08/14/03 (3.5.5)
//
//	MODIFICATIONS:  MJG 3/25/04 Reworked to handle case when Internet
//                  Explore is not installed.
//
void sys_IsConnectedToInternet( PA_PluginParameters params )
{
	short returnValue;
	LONG_PTR connectionType;

	LPFNDLLFUNC4	lpfnDllFunc4;
	HINSTANCE		hDLL;

	connectionType = 0;
	returnValue = 0;
	lpfnDllFunc4 = NULL;
	hDLL = NULL;

	hDLL = LoadLibrary("wininet.dll");

	if (hDLL != NULL) {
		lpfnDllFunc4 = (LPFNDLLFUNC4)GetProcAddress(hDLL, "InternetGetConnectedState");
		if ( lpfnDllFunc4 != NULL){
			if(lpfnDllFunc4(&connectionType, 0)){
				returnValue = 1;
			}
		} 

		FreeLibrary(hDLL);
	}


	PA_ReturnShort( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: gui_GetSysColor( PA_PluginParameters params )
//
//  PURPOSE:  Get the current color of the specified display element.
//        
//	DATE:	  MJG 12/1/03 (3.5.6)
//
void gui_GetSysColor( PA_PluginParameters params )
{
	LONG_PTR returnValue;
	LONG_PTR displayElement;
	LONG_PTR retRValue, retGValue, retBValue;
	DWORD dwResult;

	returnValue = 0;
	displayElement = PA_GetLongParameter( params, 1); 
	retRValue = retGValue = retBValue = 0;

	if(displayElement >= COLOR_SCROLLBAR && displayElement <= COLOR_GRADIENTINACTIVECAPTION && displayElement != COLOR_UNUSED){

		dwResult = GetSysColor(displayElement); 

		retRValue = GetRValue(dwResult);
		retGValue = GetGValue(dwResult);
		retBValue = GetBValue(dwResult); 

		returnValue = 1;
	}
    
	PA_SetLongParameter( params, 2, retRValue );
	PA_SetLongParameter( params, 3, retGValue );
	PA_SetLongParameter( params, 4, retBValue );

 
	PA_ReturnLong( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: gui_SetSysColor( PA_PluginParameters params )
//
//  PURPOSE:  Set the color of the specified display element.
//        
//	DATE:	  MJG 12/1/03 (3.5.6)
//
void gui_SetSysColor( PA_PluginParameters params )
{
	LONG_PTR returnValue;
	INT_PTR displayElement[1];
	COLORREF rgbValue[1];
	BYTE rValue, gValue, bValue;

	returnValue = 0;
	displayElement[0] = (INT_PTR) PA_GetLongParameter( params, 1); 
	rValue = (BYTE) PA_GetLongParameter( params, 2);
	gValue = (BYTE) PA_GetLongParameter( params, 3);
    bValue = (BYTE) PA_GetLongParameter( params, 4);

	rgbValue[0] = RGB(rValue, gValue, bValue);

    returnValue = SetSysColors(1, displayElement, rgbValue);

	if(returnValue != 0)
		returnValue = 1;
 
	PA_ReturnLong( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: sys_GetEnv( PA_PluginParameters params )
//
//  PURPOSE:  Get the value of an environment variable.
//        
//	DATE:	  MJG 12/2/03 (3.5.6)
//
void sys_GetEnv( PA_PluginParameters params )
{
	LONG_PTR returnValue;
	LONG_PTR envLength;
	char envName[MAXBUF], *envValue;

	envValue = NULL;
	memset(envName, 0, MAXBUF);

	envLength = PA_GetTextParameter( params, 1, envName );
	
	if(envLength > 0)
		envValue = getenv(envName);

	if(envValue == NULL)
	{
		returnValue = 0;
		PA_SetTextParameter(params, 2, "", 0);
	}
	else {
		returnValue = 1;
		PA_SetTextParameter(params, 2, envValue, strlen(envValue));
	}

	PA_ReturnLong( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: sys_GetEnv( PA_PluginParameters params )
//
//  PURPOSE:  Set the value of an environment variable.
//        
//	DATE:	  MJG 12/2/03 (3.5.6)
//
void sys_SetEnv( PA_PluginParameters params )
{
	LONG_PTR returnValue;
	char envName[MAXBUF], envValue[MAXBUF];

	returnValue = 0;
	memset(envName, 0, MAXBUF);
	memset(envValue, 0, MAXBUF);

	PA_GetTextParameter( params, 1, envName );
	PA_GetTextParameter( params, 2, envValue );
	
	strncat(envName, "=", MAXBUF);
	strncat(envName, envValue, MAXBUF);
	
	returnValue = !(putenv(envName));

	PA_ReturnLong( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: sys_GetRegKey( PA_PluginParameters params )
//
//  PURPOSE:  Get a key from the registry.
//        
//	DATE:	  MJG 12/4/03 (3.5.6)
//
void sys_GetRegKey( PA_PluginParameters params )
{
	LONG_PTR returnValue, regKey, retErr, dataSize, arraySize, expandDataSize;
	LONG_PTR i, len;
	char regSub[MAXBUF];
	char regName[MAXBUF];
	char *returnDataBuffer, *ptrData;
	HKEY hRootKey;
	HKEY hOpenKey;
	DWORD dwDataType;
	DWORD dwReturnLong;
	PA_Variable	paReturnArray;

	// AMS2 12/9/14 #41400 Initalized the dataSize variable. In 64 bit environments this can be randomly initalized to a size in bytes 
	// that is larger than malloc can allot, causing it to return null and crash when returning to 4D. Remember to always initialize your size variables.
	dataSize = 0;
	returnValue = regKey = retErr = arraySize = expandDataSize = 0; 
	hRootKey = hOpenKey = 0;
	ptrData = returnDataBuffer = NULL;
	memset(regSub, 0, MAXBUF);
	memset(regName, 0, MAXBUF);

	// Get the function parameters.
	regKey = PA_GetLongParameter( params, 1 );
	PA_GetTextParameter( params, 2, regSub );
	PA_GetTextParameter( params, 3, regName );

	// Convert the 4d registry constant into a Windows registry key.
	hRootKey = getRootKey( regKey );

	// Open the registry key.
	retErr = RegOpenKeyEx(hRootKey, regSub, 0, KEY_READ, &hOpenKey);
	
	if(retErr == ERROR_SUCCESS){

		// Get the value type and size.
		retErr = RegQueryValueEx(hOpenKey, regName, NULL, &dwDataType, NULL, &dataSize);

		if(retErr == ERROR_SUCCESS){

			switch(dwDataType){
			case REG_BINARY:
				returnDataBuffer = malloc(dataSize);
				retErr = RegQueryValueEx(hOpenKey, regName, NULL, NULL, (LPBYTE) returnDataBuffer, &dataSize);

				if(retErr == ERROR_SUCCESS){
					PA_SetBlobParameter(params, 4, returnDataBuffer, dataSize);
					returnValue = 1;
				}

				free(returnDataBuffer);
				break;

			case REG_DWORD:
			case REG_DWORD_BIG_ENDIAN:
				dataSize = sizeof(dwReturnLong);
				retErr = RegQueryValueEx(hOpenKey, regName, NULL, NULL, (LPBYTE) &dwReturnLong, &dataSize);

				if(retErr == ERROR_SUCCESS){
					PA_SetLongParameter(params, 4, dwReturnLong);
					returnValue = 1;
				}
				break;

			case REG_EXPAND_SZ:
				returnDataBuffer = malloc(dataSize);
				retErr = RegQueryValueEx (hOpenKey, regName, NULL, NULL, returnDataBuffer, &dataSize);
				
				if(retErr == ERROR_SUCCESS)
				{
					regExpandStr(&returnDataBuffer);
					PA_SetTextParameter(params, 4, returnDataBuffer, strlen(returnDataBuffer));
					returnValue = 1;
				}

				free(returnDataBuffer);
				break;


			case REG_MULTI_SZ:
				returnDataBuffer = malloc(dataSize);
				paReturnArray = PA_GetVariableParameter( params, 4 );

				retErr = RegQueryValueEx(hOpenKey, regName, NULL, NULL, returnDataBuffer, &dataSize);

				if(retErr == ERROR_SUCCESS)
				{
					arraySize = regGetNumElements(returnDataBuffer);
					PA_ResizeArray(&paReturnArray, arraySize);

			
					for(i = 1, ptrData = returnDataBuffer; i <= arraySize; i++)
					{
						len = strlen(ptrData);
						PA_SetTextInArray(paReturnArray, i, ptrData, len);
						ptrData+=len+1;
					} 

					returnValue = 1;
					PA_SetVariableParameter( params, 4, paReturnArray, 0 );
				}

				free(returnDataBuffer);
				break;


			case REG_SZ:
				returnDataBuffer = (char*)malloc(dataSize);
				retErr = RegQueryValueEx(hOpenKey, regName, NULL, NULL, returnDataBuffer, &dataSize);

				if(retErr == ERROR_SUCCESS){
					PA_SetTextParameter(params, 4, returnDataBuffer, dataSize-1);
					returnValue = 1;
				}

				free(returnDataBuffer);
				break;
			} 
		}
	}

	RegCloseKey( hOpenKey );
	PA_ReturnLong( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: sys_GetRegType( PA_PluginParameters params )
//
//  PURPOSE:  Get a key from the registry.
//        
//	DATE:	  MJG 12/4/03 (3.5.6)
//
void sys_GetRegType( PA_PluginParameters params )
{
	LONG_PTR returnValue, regKey, retErr;
	char regSub[MAXBUF];
	char regName[MAXBUF];
	char *returnDataBuffer;
	HKEY hRootKey;
	HKEY hOpenKey;
	DWORD dwDataType;

	returnValue = regKey = retErr = 0;
	hRootKey = hOpenKey = 0;
	returnDataBuffer =  NULL;
	memset(regSub, 0, MAXBUF);
	memset(regName, 0, MAXBUF);

	// Get the function parameters.
	regKey = PA_GetLongParameter( params, 1 );
	PA_GetTextParameter( params, 2, regSub );
	PA_GetTextParameter( params, 3, regName );

	// Convert the 4d registry constant into a Windows registry key.
	hRootKey = getRootKey( regKey );

	// Open the registry key.
	retErr = RegOpenKeyEx(hRootKey, regSub, 0, KEY_READ, &hOpenKey);
	
	if(retErr == ERROR_SUCCESS){

		// Get the value type and size.
		retErr = RegQueryValueEx(hOpenKey, regName, NULL, &dwDataType, NULL, NULL);

		if(retErr == ERROR_SUCCESS){

			returnValue = get4dRegType(dwDataType);

		}
	}

	RegCloseKey( hOpenKey );
	PA_ReturnLong( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: sys_GetRegEnum( PA_PluginParameters params )
//
//  PURPOSE:  Get a key from the registry.
//        
//	DATE:	  MJG 12/4/03 (3.5.6)
//
void sys_GetRegEnum( PA_PluginParameters params )
{
	LONG_PTR returnValue, regKey, retErr;
	char regSub[MAXBUF];
	LONG_PTR regBufSize = MAX_REG_SIZE;
	CHAR regBuf[MAX_REG_SIZE]; 

	FILETIME ftLastWriteTime;
	HKEY hRootKey;
	HKEY hOpenKey;
	DWORD dwSubKeys;
	DWORD dwValues;
	DWORD i, j;
	PA_Variable paReturnArray1;
	PA_Variable paReturnArray2;

	dwSubKeys = dwValues = 0;
	returnValue = regKey = retErr = 0;
	hRootKey = hOpenKey = 0;
	memset(regSub, 0, MAXBUF);

	// Get the function parameters.
	regKey = PA_GetLongParameter( params, 1 );
	PA_GetTextParameter( params, 2, regSub );
	paReturnArray1 = PA_GetVariableParameter( params, 3 );
	paReturnArray2 = PA_GetVariableParameter( params, 4 );
	
	PA_ResizeArray(&paReturnArray1, 0);
	PA_ResizeArray(&paReturnArray2, 0);

	// Convert the 4d registry constant into a Windows registry key.
	hRootKey = getRootKey( regKey );

	// Open the registry key.
	retErr = RegOpenKeyEx(hRootKey, regSub, 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE , &hOpenKey);
	
	if(retErr == ERROR_SUCCESS){
		
		retErr = RegQueryInfoKey(hOpenKey, NULL, NULL, NULL, &dwSubKeys, NULL, NULL, &dwValues, NULL, NULL, NULL, NULL); 
		
		if( retErr == ERROR_SUCCESS)
		{
			// Enumerate the subkey names.
			
			if (dwSubKeys)
			{
				retErr = ERROR_SUCCESS;
				PA_ResizeArray(&paReturnArray1, dwSubKeys);
				
				for (i=0,j=0; i<dwSubKeys; i++) 
				{ 
					regBufSize = MAX_REG_SIZE;
					retErr = RegEnumKeyEx(hOpenKey, i, regBuf, &regBufSize, NULL, NULL, NULL, &ftLastWriteTime); 
					
					if (retErr == ERROR_SUCCESS) 
					{
						PA_SetTextInArray(paReturnArray1, ++j, regBuf, regBufSize);
					}
				}
			} 
			
			// Enumerate the key value names. 
			if (dwValues) 
			{
				retErr = ERROR_SUCCESS;
				PA_ResizeArray(&paReturnArray2, dwValues);
				
				for (i=0,j=0; i<dwValues; i++) 
				{ 
					regBufSize = MAX_REG_SIZE;
					regBuf[0] = '\0'; 
					retErr = RegEnumValue(hOpenKey, i, regBuf, &regBufSize, NULL, NULL, NULL, NULL);
					
					if (retErr == ERROR_SUCCESS ) 
					{ 
						PA_SetTextInArray(paReturnArray2, ++j, regBuf, regBufSize);
					} 
				}
			}

			returnValue = 1;
		}
	} 
	
	PA_SetVariableParameter( params, 3, paReturnArray1, 0 );
	PA_SetVariableParameter( params, 4, paReturnArray2, 0 );
	
	RegCloseKey( hOpenKey );

	PA_ReturnLong( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: sys_SetPluginLanguage( PA_PluginParameters params )
//
//  PURPOSE:  Set the language for the returning message of
//            sys_ShellExecute
//        
//	DATE:	  20/02/06 Mark De Wever #12225
//

void sys_SetPluginLanguage( PA_PluginParameters params )
{
	LONG_PTR language;
		// Get the function parameters.
	language = PA_GetLongParameter( params, 1 );

	switch( language )
	{
		case LANG_ENGLISH :
			message = &message_list_english;
			break;

		case LANG_DUTCH: 
			message = &message_list_dutch;
			break;

		default :
			language = LANG_ENGLISH;
			message = &message_list_english;
			break;
	}

	PA_ReturnLong( params, language );
}

// ------------------------------------------------
// 
//  FUNCTION: sys_PrintDirect2Driver(PA_PluginParameters params) 
//
//  PURPOSE:  Takes a printer name and data for a label, and prints the label to the given printer name.
//						     
//	DATE: MWD 8/9/06 #10250
//		  REB 8/11/09 Removed support for Windows versions older than 2000.
//
//-------------------------------------------------

void sys_PrintDirect2Driver( PA_PluginParameters params )
{
	PRINTDLG pd;                      // Structure to hold information about printer
	DOCINFO di;                       // Structure to hold "document" information
	char printerName[MAXBUF] = "";    // String to hold the printerName param ($1)
	char data[MAXLABELBUF] = "";      // String to hold the data param ($2) REB 6/5/08 #17022 Changed MAXBUF to MAXLABELBUF which is twice as big.
	char *origDefault;                // String to hold the original default printer
	INT_PTR printerName_len;              // Int to hold maximum length of printer name
	INT_PTR ret;                          // Int to hold return value of functions                 
	INT_PTR iErrCode = 0;                 // Int to hold the error code.
	ULONG_PTR ulBytesNeeded;      // Holds size information

	// Set needed bytes to default value
	ulBytesNeeded = MAXLABELBUF; // REB 6/5/08 #17022 Changed MAXBUF to MAXLABELBUF

	// Set this to 255.
	printerName_len = 255;

	// Get the function parameters.
	PA_GetTextParameter(params, 1, printerName);
	PA_GetTextParameter(params, 2, data);

	// Allocate memory for Storing string for Original Default Printer & pBuf
	origDefault = (char *)malloc(ulBytesNeeded);
	memset(origDefault, 0, ulBytesNeeded);
	
    // Get name of current Default Printer
	GetDefaultPrinter(origDefault, &ulBytesNeeded);
    
	// Set the new Default Printer to our label printer, with the name obtained from the registry
	ret = SetDefaultPrinter((char *)printerName);
	
	// We set the default printer just fine, now let's do the printing.
	if (ret != 0)
	{
		            
  	// Allocate memory for PRINTDLG structure
	  memset( &pd, 0, sizeof( pd ) );

	  // Define properties of the PRINTDLG structure
	  pd.lStructSize = sizeof( pd );

	  // PD_RETURNDEFAULT causes the PrintDlg function to automatically use the properties from
	  // the default printer.  PD_RETURNDC causes the function to return the device context
	  // for the printer.  This device context allows us to print a label
	  pd.Flags = PD_RETURNDEFAULT | PD_RETURNDC;

		// These two structures must be NULL to use the PD_RETURNDC flag.
		// Do this explicitly, just in case
		pd.hDevMode = NULL;
		pd.hDevNames = NULL;
			
	  // Retrieve the Device Context.  It will be accessible as a member of the PRINTDLG structure
	  if(!PrintDlg( &pd ))
		{
			// Get the error from the common dialog box
			// Error code will not work properly with FormatMessage, so use this instead.
		  iErrCode = CommDlgExtendedError();
		}
		
		if(iErrCode == 0)
		{
      // Initialize the DOCINFO structure
      memset(&di, 0, sizeof(di));
      di.cbSize = sizeof(DOCINFO);
	  	di.lpszDocName = "Label"; 
      di.lpszOutput = (LPTSTR) NULL; 
      di.lpszDatatype = "raw";
      di.fwType = 0; 
    	
	  	// Start a document in the print spooler
	  	if(!StartDoc(pd.hDC, &di))
			{
	  	  iErrCode = GetLastError();
			} // end if
		}
					
		if(iErrCode == 0)
		{
		  // Start a new page in the print spooler
	    if(!StartPage(pd.hDC))
			{
			  iErrCode = GetLastError();
			} // end if !
		}

		if(iErrCode == 0)
		{
		  if(!TextOut(pd.hDC, 1, 1, data, strlen(data)))
			{
			  iErrCode = GetLastError();
			}
		}

		// De-allocate commandList
		
		// Let the print spooler know that the page is done
		EndPage(pd.hDC);
			
    // Let the print spooler know that the document is done
	  EndDoc(pd.hDC);
			
		
		// Delete the Device Context
		DeleteDC(pd.hDC);
			
		
		// Now reset our default printer.
    
		// Set the Default Printer back to the original.
  		ret = SetDefaultPrinter(origDefault);

		PA_ReturnLong(params, (LONG_PTR)iErrCode); 
	} else {
	  PA_ReturnLong(params, (LONG_PTR)GetLastError());
	}	// end if
}//end function

// ------------------------------------------------
// 
//  FUNCTION: sys_KillProcessByName(PA_PluginParameters params) 
//
//  PURPOSE:  Kills a process or processes with the given name.
//						     
//	DATE: MWD 11/30/06 #12572
//
//-------------------------------------------------

void sys_KillProcessByName( PA_PluginParameters params )
{
	HANDLE hProcessSnap;              // Handle to the process snapshot
	HANDLE hProcess;                  // Handle to the process itself
	PROCESSENTRY32 pe32;              // ProcessEntry to get info about processes
  char processName[MAXBUF] = "";    // String to hold the printerName param ($1)
	LONG_PTR lMode = 1;                   // Long to hold the working mode ($2)
	                                  // 1 = just first process matching name
	                                  // 2 = all processes matching name
	BOOL bCleanFirst = FALSE;         // Boolean to see if we should try to cleanly close the app
	                                  // before killing it mercilessly
	BOOL bOrigCleanFirst = FALSE;     // Keeps track of original bCleanFirst value
	                                  // to reset between loop iterations ($3)
	BOOL bDone = FALSE;               // This will keep track of whether or not we are finished
	                                  // Looping through processes.
	// Get the function parameters.
	PA_GetTextParameter(params, 1, processName);
	lMode = PA_GetLongParameter(params, 2);
	bOrigCleanFirst = (PA_GetLongParameter(params, 3) == 1 ? TRUE : FALSE);

  // Take a snapshot of all processes in the system.
	// If we fail, return the error code
  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if(hProcessSnap == INVALID_HANDLE_VALUE)
  {
    PA_ReturnLong(params, (LONG_PTR)GetLastError());
		return;
  }

	// Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // If we can't do it, then return the error code
  if(!Process32First( hProcessSnap, &pe32))
  {
    CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
    PA_ReturnLong(params, (LONG_PTR)GetLastError());
		return;
  }

  // Now walk the snapshot of processes, and
  // display information about each process in turn
  do
  {
    
		// Check the name
		if (strcmp(pe32.szExeFile, processName) == 0)
		{

			bCleanFirst = bOrigCleanFirst;
		  // Get the process
		  // We need to make sure that we have the TERMINATE right
      hProcess = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
			
			// Couldn't get the process
			// Clean up the handle
			// and return the error
      if(hProcess == NULL) {
				CloseHandle(hProcessSnap);
        PA_ReturnLong(params, (LONG_PTR)GetLastError());
			  return;
			}

			if(bCleanFirst)
			{
        // TerminateClean() posts WM_CLOSE to all windows whose PID
        // matches your process's.
        EnumWindows((WNDENUMPROC)TerminateClean, (LPARAM) pe32.th32ProcessID) ;

				if(WaitForSingleObject(hProcess, 500)!=WAIT_OBJECT_0)
				{
          bCleanFirst = TRUE;
				}
			}

			if(!bCleanFirst)
			{
        // Kill the process
        if(TerminateProcess(hProcess, 1)) {;
			    // Success!
			    // If we're in mode 1 then we are finished
			    // If not, then we will need to keep going
  			  if(lMode == 1) {
	  			  bDone = TRUE;
					} // end 
				} else {
          // Fail!
				  // Clean up and return the error
          CloseHandle(hProcess);
				  CloseHandle(hProcessSnap);
				  PA_ReturnLong(params, (LONG_PTR)GetLastError());
				  return;
				}
			}

			// Close our handle
      CloseHandle(hProcess);
    } // end if
		Process32Next(hProcessSnap, &pe32); // WJF 6/2/15 #42839 Moved out of while condition

  } while(GetLastError()!=ERROR_NO_MORE_FILES || !bDone); // WJF 6/2/15 #42839 Added GetLastError Check, corrected logical or syntax, and added inversion to bDone

	// Close the handle and return success
  CloseHandle(hProcessSnap);
  PA_ReturnLong(params, -1 * (LONG_PTR)bCleanFirst);
}
	
// ------------------------------------------------
// 
//  FUNCTION: sys_killProcessByID(PA_PluginParameters params) 
//
//  PURPOSE:  Kills the process with the given ID.
//						     
//	DATE: MWD 11/30/06 #12572
//
//-------------------------------------------------

void sys_KillProcessByID( PA_PluginParameters params )
{
	HANDLE hProcess;                  // Handle to the process itself
	INT_PTR lPID;                         // Integer to hold the processID ($1)
	BOOL bCleanFirst = FALSE;         // Boolean to see if we should try to cleanly close the app
	                                  // before killing it mercilessly ($2)
	
	// Get the function parameters.
	lPID = PA_GetLongParameter(params, 1);
	bCleanFirst = (PA_GetLongParameter(params, 2) == 1 ? TRUE : FALSE);

	// Open a handle to the process
  hProcess = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, lPID); 
  
  // Check to see if we got it, and return an error if we didn't
	if(hProcess == NULL) {
    PA_ReturnLong(params, (LONG_PTR)GetLastError());
		return;
	}


	if(bCleanFirst)
	{
    // TerminateClean() posts WM_CLOSE to all windows whose PID
    // matches your process's.
    EnumWindows((WNDENUMPROC)TerminateClean, (LPARAM) lPID) ;

		if(WaitForSingleObject(hProcess, 500)!=WAIT_OBJECT_0)
		{
      bCleanFirst = TRUE;
			PA_ReturnLong(params, -1);
		}
	} // end if bCleanFirst

	if(!bCleanFirst)
	{    
	  // Kill the process
    if (TerminateProcess(hProcess, 1))
		{
	  	// Success! Return 0
	  	PA_ReturnLong(params, 0);
		} else {
	  	// Fail! Return the error
      PA_ReturnLong(params, (LONG_PTR)GetLastError());
		}
	} // end if !bCleanFirst

	// Clean up
	CloseHandle(hProcess);
}

// ------------------------------------------------
//
//  FUNCTION: TerminateClean(hWND, lparam)
//
//  Callback method to post the WM_CLOSE message for
//  cleanly exiting an app
//
//  DATE: MWD 1/8/07 #5421
//
// ------------------------------------------------

BOOL CALLBACK TerminateClean(HWND hWnd, LPARAM lParam)
{
  DWORD dwID ;

	// Get the ID of the window that this was called on
  GetWindowThreadProcessId(hWnd, &dwID) ;

	// Make sure that it matches our PID
	// If it does, post a WM_CLOSE message
	// Posting this message to a window is the
	// same as hitting its close button
	if(dwID == (DWORD)lParam)
  {
    PostMessage(hWnd, WM_CLOSE, 0, 0) ;
  }

  return TRUE ;
}

// ------------------------------------------------
// 
//  FUNCTION: sys_EnumProcesses(PA_PluginParameters params) 
//
//  PURPOSE:  Fills parallel arrays with process names and ids
//						     
//	DATE: MWD 11/30/06 #12572
//
// ------------------------------------------------

void sys_EnumProcesses( PA_PluginParameters params )
{
	HANDLE hProcessSnap;              // Handle to the process snapshot
	PROCESSENTRY32 pe32;              // ProcessEntry to get info about processes
  
	PA_Variable paNameArray;          // Array to store process names ($1)
	PA_Variable paIDArray;            // Array to store process IDs ($2)

	INT_PTR iSize = 0;                    // Int to control size of array

  // Get variables representing passed in arrays
	paNameArray = PA_GetVariableParameter(params, 1);
	paIDArray = PA_GetVariableParameter(params, 2);
	
	// Resize arrays to 0
	PA_ResizeArray(&paNameArray, iSize);
	PA_ResizeArray(&paIDArray, iSize);

  // Take a snapshot of all processes in the system.
	// If we fail, return the error code
  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if(hProcessSnap == INVALID_HANDLE_VALUE)
  {
    PA_ReturnLong(params, (LONG_PTR)GetLastError());
		return;
  }

	// Set the size of the structure before using it.
  pe32.dwSize = sizeof( PROCESSENTRY32 );

  // Retrieve information about the first process,
  // If we can't do it, then return the error code
  if(!Process32First( hProcessSnap, &pe32))
  {
    CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
    PA_ReturnLong(params, (LONG_PTR)GetLastError());
		return;
  }

  // Now walk the snapshot of processes, and
  // Insert an array element in each array, one for name and one for ID.
  do
  {
    iSize++; 
		PA_ResizeArray(&paNameArray, iSize);
	  PA_ResizeArray(&paIDArray, iSize);
		PA_SetTextInArray(paNameArray, iSize, pe32.szExeFile, strlen(pe32.szExeFile));
		PA_SetLongintInArray(paIDArray, iSize, pe32.th32ProcessID);
		
	} while(Process32Next(hProcessSnap, &pe32));

	// Close the handle and return success
  CloseHandle(hProcessSnap);

	// Set return arrays
	PA_SetVariableParameter(params, 1, paNameArray, 0);
	PA_SetVariableParameter(params, 2, paIDArray, 0);

	// Return error code.
	PA_ReturnLong(params, 0);
}

// ------------------------------------------------
// 
//  FUNCTION: sys_LogonUser(PA_PluginParameters params) 
//
//  PURPOSE:  Authenticates a user against at the domain level
//						     
//	DATE: MWD 1/8/07 #6982
//
//  Code provided by DAS on behalf of TEB.
//  Re-worked to be included in Win32API
//
//-------------------------------------------------

void sys_LogonUser(PA_PluginParameters params)
{
	
	HANDLE hToken = NULL;
	BOOL bStatus = FALSE;
	char chUserID[MAXBUF] = "";   // String to hold the username
	char chDomain[MAXBUF] = "";   // String to hold the domain
	char chPassword[MAXBUF] = ""; // String to hold the password
	
	// Make sure that we've cleared out any previous errors
	SetLastError(0);

	// Get our parameters
	PA_GetTextParameter(params, 1, chUserID);
	PA_GetTextParameter(params, 2, chDomain);
	PA_GetTextParameter(params, 3, chPassword);

  // Authenticate the user
  bStatus = LogonUser(chUserID, chDomain, chPassword, LOGON32_LOGON_NETWORK, 
                      LOGON32_PROVIDER_DEFAULT, &hToken);

  if(bStatus)
	{
    // Authenticated
    // Close the handle
    if (hToken != NULL)
		{
      CloseHandle(hToken);
      hToken = NULL;
    }
  }

  PA_ReturnLong(params, (LONG_PTR)bStatus);
}

/* end of NTAuth.c */

