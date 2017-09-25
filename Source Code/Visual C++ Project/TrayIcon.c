#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"

extern LONG_PTR					sIsPriorTo67 = 0;
extern char					debugStr[255];
extern HANDLE				hSubclassMutex;  // MJG 3/26/04
static pTI					startPtr;

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

extern struct		ACTIVECALLS
{
	BOOL		bPrinterCapture;
	BOOL		bTrayIcons;
	BOOL		b4DMaximize; //01/21/03
} activeCalls;

//added 01/17/03 see 4DPlugin082102.c
extern struct		TOOLBARRESTRICT
{
	LONG			toolBarOnDeck; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG			top; // WJF 6/24/16 Win-21 LONG_PTR -> LONG
	LONG			left; // WJF 6/24/16 Win-21 LONG_PTR -> LONG
	LONG			right; // WJF 6/24/16 Win-21 LONG_PTR -> LONG
	LONG			bottom; // WJF 6/24/16 Win-21 LONG_PTR -> LONG
	LONG			topProcessNbr; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG			leftProcessNbr; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG			rightProcessNbr; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG			bottomProcessNbr; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG			trackingRestriction; // WJF 6/24/16 Win-21 LONG_PTR -> LONG
	LONG_PTR		appBeingMaxed;
	LONG_PTR		appWindowState;
	RECT		origWindowRect;
	LONG		clientOffsetx; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG		clientOffsety;  // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	char		minimizedWindows[SMLBUF][SMLBUF]; // REB 8/11/08 #16207
	RECT		previousWindowRect; // REB 3/26/10
} toolBarRestrictions;

//
//  FUNCTION: gui_SetTrayIcon( PA_PluginParameters params )
//
//  PURPOSE:	Put an icon in system tray
//
//  COMMENTS:	Flags and action determine what happens: Add, modify, delete, tool tip etc
//						Not available for pre-6.7 4D
//	IMPORTANT	NOTE: This and sys_GetPrintJob use the same subclassed window procedure.
//									You cannot arbitrarily delete the function newProc
//									without breaking sys_GetPrintJob.
//
//	DATE:			dcc 08/04/01
//
void gui_SetTrayIcon(PA_PluginParameters params)
{
	LONG_PTR							iconHndl = 0; // WJF 6/24/16 Win-21 UINT -> LONG_PTR
	NOTIFYICONDATA		nid;
	PNOTIFYICONDATA		pnid;
	LONG						returnValue = 0, flags = 0, action = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	UINT							iconID = 0; // WJF 6/30/16 Win-21 LONG_PTR -> UINT
	char							szTipParam[60], szBalloonInfo[255], szBalloonTitle[60];
	char							*pBalloonIconFlag;
	LONG							procNbr = 0, storedProcNbr = 0, nbrParams = 0;
	LONG							index; // WJF 6/30/16 Win-21
	BOOL							bFuncReturn = FALSE;
	BOOL							shellOK = FALSE;
	//HWND							hWnd;
	LONG_PTR count = -10;

	activeCalls.bTrayIcons = TRUE;

	pnid = &nid;

	count = count % 5;

	if ((sIsPriorTo67)) { // does not work with 6.5 plugin
		PA_ReturnLong(params, -1);
		return;
	}

	//hWnd = (HWND)PA_GetHWND(PA_GetWindowFocused());  // 3/2/04 Unnecessary

	nbrParams = getTrayIconParams(params, &action, &flags, &iconID, &procNbr,
		&iconHndl, szTipParam, szBalloonInfo, szBalloonTitle);
	index = findIconID(&startPtr, iconID, &storedProcNbr);
	if (index == 0) { // not found
		if (isEmpty(startPtr)) {
			//processHandles.wpFourDOrigProc = (WNDPROC) SetWindowLong(windowHandles.fourDhWnd, GWL_WNDPROC, (LONG) newProc);
			// MJG 3/26/04 Replaced code above with function call.
			subclass4DWindowProcess();
		}

		//add element to array
		bFuncReturn = insertIcon(&startPtr, iconID, procNbr);
	}
	else {
		if ((action == NIM_MODIFY) & (storedProcNbr != procNbr)) {
			// process nbr changed and modify request has been explicitly made
			bFuncReturn = updateIconIdProcNbr(&startPtr, iconID, procNbr);
		}
	} //end if (index == 0)

	// must have version 5 of shell 32 for balloon feature
	// NOTIFYICONDATA structure is larger for balloon feature
	// also must be W2K for balloons
	if (GetDllVersion(TEXT("shell32.dll")) >= PACKVERSION(5, 00))
	{
		shellOK = TRUE;
	}

	if (shellOK) {
		if ((action >= 0) & (flags >= 0x010)) {
			nid.dwInfoFlags = 0;
			strcpy_s(nid.szInfo, sizeof(nid.szInfo), szBalloonInfo);  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s

			switch (szBalloonTitle[0]) // leading 1, 2, 0r 3 causes addition of icon
			{
			case '1':
				pBalloonIconFlag = &szBalloonTitle[1];
				if (*pBalloonIconFlag != '\0') {
					strcpy_s(nid.szInfoTitle, sizeof(nid.szInfoTitle), pBalloonIconFlag);  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s
					nid.dwInfoFlags = NIIF_INFO;
				}
				break;

			case '2':
				pBalloonIconFlag = &szBalloonTitle[1];
				if (*pBalloonIconFlag != '\0') {
					strcpy_s(nid.szInfoTitle, sizeof(nid.szInfoTitle), pBalloonIconFlag);  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s
					nid.dwInfoFlags = NIIF_WARNING;
				}
				break;

			case '3':
				pBalloonIconFlag = &szBalloonTitle[1];
				if (*pBalloonIconFlag != '\0') {
					strcpy_s(nid.szInfoTitle, sizeof(nid.szInfoTitle), pBalloonIconFlag);  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s
					nid.dwInfoFlags = NIIF_ERROR;
				}
				break;
			default:
				strcpy_s(nid.szInfoTitle, sizeof(nid.szInfoTitle), szBalloonTitle);  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s
			}

			nid.uTimeout = 10;

			if (flags & NIF_HIDE) {
				flags = flags & 0x001F;
				flags = flags | NIF_STATE;
				nid.dwState = NIS_HIDDEN;
				nid.dwStateMask = NIS_HIDDEN;
			}
			else {
				if (flags & NIF_SHOW) {
					flags = flags & 0x001F;
					flags = flags | NIF_STATE;
					nid.dwState = 0;
					nid.dwStateMask = NIS_HIDDEN;
				}
			}
		}
	}
	else {
		flags = (flags & 0xF); // must not send balloon flag when version not Win2K or above
	}

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = windowHandles.fourDhWnd;
	nid.uID = iconID;
	strcpy_s(nid.szTip, sizeof(nid.szTip), szTipParam); // can use this if balloon feature not available or not wanted  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s
	nid.uVersion = 0; // REB 3/3/09 #16207
	nid.uFlags = flags; // REB 2/18/10 #22656

	switch (action)
	{
	case NIM_ADD:
	case NIM_MODIFY:
	case NIM_SETFOCUS:
	case NIM_SETVERSION:
		nid.uFlags = flags;
		nid.hIcon = (HICON)iconHndl;
		if (flags & NIF_MESSAGE) {
			nid.uCallbackMessage = WM_USER + 0x0021; // hex 21 is purely arbitrary
		}
		else {
			nid.uCallbackMessage = WM_NULL;
		}
		break;

	case NIM_DELETE:
		//if (index != 0) { // MJG 3/2/04 The element will still exist even if index is zero.
		returnValue = deleteIcon(&startPtr, iconID);
		//}
	}

	//bFuncReturn = Shell_NotifyIcon(NIM_SETVERSION,  pnid); // REB 3/3/09 #16207 Force Win95 icon handling.
	bFuncReturn = Shell_NotifyIcon(action, pnid);

	//errCode = GetLastError();
	//PA_ReturnLong( params, errCode );
	PA_ReturnLong(params, (LONG_PTR)bFuncReturn);
}

//
//  FUNCTION: getTrayIconParams( PA_PluginParameters params, LONG_PTR *pAction, LONG_PTR *pFlags, LONG_PTR *pIconID, LONG_PTR *pProcessNbr, LONG_PTR *pIconHndl )
//
//  PURPOSE:	Gets parameters passed to plugin call and returns them via the pointers
//
//  COMMENTS:	Returns number of non-zero parameters
//
//	DATE:			dcc 08/17/01
//
// WJF 6/30/16 Win-21 Most LONG_PTR -> LONG
// ZRW 2/13/17 WIN-39 pIconID LONG -> UINT
LONG getTrayIconParams(PA_PluginParameters params, LONG *pAction, LONG *pFlags, UINT *pIconID, LONG *pProcessNbr, LONG_PTR *pIconHndl, char* szTipParam, char* szBalloonInfo, char* szBalloonTitle)
{
	LONG szTipParam_len, szBalloonInfo_len, szBalloonTitle_len, returnValue = 0;

	*pAction = PA_GetLongParameter(params, 1);
	if (*pAction != 0) returnValue++;
	*pFlags = PA_GetLongParameter(params, 2);
	if (*pFlags != 0) returnValue++;
	*pIconID = PA_GetLongParameter(params, 3);
	if (*pIconID != 0) returnValue++;
	*pProcessNbr = PA_GetLongParameter(params, 4);
	if (*pProcessNbr != 0) returnValue++;
	*pIconHndl = PA_GetLongParameter(params, 5);
	if (*pIconHndl != 0) returnValue++;

	szTipParam_len = PA_GetTextParameter(params, 6, szTipParam);
	if (szTipParam_len > 59) {
		szTipParam_len = 60;
	}
	szTipParam[szTipParam_len] = '\0';
	if (szTipParam_len) returnValue++;

	szBalloonInfo_len = PA_GetTextParameter(params, 7, szBalloonInfo);
	if (szBalloonInfo_len > 254) {
		szBalloonInfo_len = 255;
	}
	szBalloonInfo[szBalloonInfo_len] = '\0';
	if (szBalloonInfo_len) returnValue++;

	szBalloonTitle_len = PA_GetTextParameter(params, 8, szBalloonTitle);
	if (szBalloonTitle_len > 59) {
		szBalloonTitle_len = 60;
	}
	szBalloonTitle[szBalloonTitle_len] = '\0';
	if (szBalloonTitle_len) returnValue++;

	return returnValue;
}

//  FUNCTION: insertIcon ( pTI *pIcon, LONG_PTR iconID, LONG_PTR procNbr )
//
//  PURPOSE:	Inserts icon info into linked list
//
//  COMMENTS:	Returns false if memory not allocated
//
//	DATE:			dcc 09/09/01
// WJF 6/30/16 Win-21 LONG_PTR -> LONG
BOOL insertIcon(pTI *pIcon, LONG iconID, LONG procNbr)
{
	pTI			newPtr = NULL, previousPtr = NULL, currentPtr = NULL;

	newPtr = malloc(sizeof(TI));
	if (newPtr != NULL) {
		newPtr->iconID = iconID;
		newPtr->procNbr = procNbr;
		newPtr->nextPtr = NULL;

		previousPtr = NULL;
		currentPtr = *pIcon;

		while (currentPtr != NULL && iconID > currentPtr->iconID) {
			previousPtr = currentPtr;
			currentPtr = currentPtr->nextPtr;
		} //end while

		if (previousPtr == NULL) {
			newPtr->nextPtr = *pIcon;
			*pIcon = newPtr;
		}
		else {
			previousPtr->nextPtr = newPtr;
			newPtr->nextPtr = currentPtr;
		} // end if
		return TRUE;
	}
	else {
		return FALSE;
	}
}

//  FUNCTION: findIconID ( pTI* pIcon, LONG_PTR iconID, LONG_PTR *pProcessNbr )
//
//  PURPOSE:	Finds position of icon ID in linked list
//
//  COMMENTS:
//
//	DATE:			dcc 09/09/01
// WJF 6/30/16 Win-21 LONG_PTR -> LONG
LONG findIconID(pTI* pNewNode, LONG iconID, LONG *pProcessNbr)
{
	pTI			previousPtr, currentPtr;
	LONG			position = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG

	if (!isEmpty(*pNewNode)) {
		if (iconID == (*pNewNode)->iconID) {
			position = 1;
			*pProcessNbr = (*pNewNode)->procNbr;
		}
		else {
			previousPtr = *pNewNode;
			currentPtr = (*pNewNode)->nextPtr;
			while (currentPtr != NULL && currentPtr->iconID != iconID) {
				previousPtr = currentPtr;
				currentPtr = currentPtr->nextPtr;
				position += 1;
			}
			if (position != 0) {
				*pProcessNbr = currentPtr->procNbr;
			}
		}
	}

	return position;
}

//  FUNCTION: updateIconIdProcNbr ( pTI *pIcon, LONG_PTR iconID, LONG_PTR processNbr )
//
//  PURPOSE:	updates the process number associated with iconId
//
//  COMMENTS:
//
//	DATE:			dcc 09/14/01
// WJF 6/30/16 Win-21 LONG_PTR -> LONG
LONG updateIconIdProcNbr(pTI *pIcon, LONG iconID, LONG processNbr)
{
	pTI			previousPtr, currentPtr;
	INT			position = 0; // WJF 6/30/16 Win-21 LONG_PTR -> INT

	if (!isEmpty(*pIcon)) {
		if (iconID == (*pIcon)->iconID) {
			position = 1;
			(*pIcon)->procNbr = processNbr;
		}
		else {
			previousPtr = *pIcon;
			currentPtr = (*pIcon)->nextPtr;
			while (currentPtr != NULL && currentPtr->iconID != iconID) {
				previousPtr = currentPtr;
				currentPtr = currentPtr->nextPtr;
				position += 1;
			}
			if (position != 0) {
				currentPtr->procNbr = processNbr;
			}
		}
	}

	return position;
}

//  FUNCTION: deleteIcon ( pTI *pIcon, LONG_PTR iconID )
//
//  PURPOSE:	deletes a struct from memory
//
//  COMMENTS:	Returns iconID if deleted else returns 0
//
//	DATE:			dcc 09/09/01
// WJF 6/30/16 Win-21 LONG_PTR -> LONG
LONG deleteIcon(pTI *pIcon, LONG iconID)
{
	pTI			previousPtr, currentPtr, tempPtr;
	LONG		returnValue = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG

	if (iconID == (*pIcon)->iconID) {//first node is to be deleted
		if ((*pIcon)->nextPtr == NULL) {
			free(*pIcon);
			startPtr = NULL;
		}
		else {
			tempPtr = (*pIcon)->nextPtr;
			free(*pIcon);
			startPtr = tempPtr;
			returnValue = iconID;
		}
	}
	else {
		previousPtr = *pIcon;
		currentPtr = (*pIcon)->nextPtr;

		while (currentPtr != NULL && currentPtr->iconID != iconID) {
			previousPtr = currentPtr;
			currentPtr = currentPtr->nextPtr;
		}

		if (currentPtr != NULL) {
			tempPtr = currentPtr;
			previousPtr->nextPtr = currentPtr->nextPtr;
			free(tempPtr);
			returnValue = iconID;
		}
	}
	if (isEmpty(startPtr)) {
		activeCalls.bTrayIcons = FALSE;
		//restoreOrig4DWindowProcess(); // 01/21/03  // MJG 3/26/04 The 4D window will remain subclassed until the plug-in is unloaded.
		//if (activeCalls.bPrinterCapture == FALSE) {
		//SetWindowLong(windowHandles.fourDhWnd, GWL_WNDPROC, (LONG) processHandles.wpFourDOrigProc);
		//}
	}
	return returnValue;
}

//  FUNCTION: isEmpty ( pTI *pIcon )
//
//  PURPOSE:	Determine if there is an icon loaded to tray
//
//  COMMENTS:	returns 1 if linked list is empty, otherwise 0
//
//	DATE:			dcc 09/09/01

LONG_PTR isEmpty(pTI pIcon)
{
	return (pIcon == NULL);
}

//  FUNCTION: sizeOfTI ( pTI *pIcon )
//
//  PURPOSE:	Determine how many icons are in memory
//
//  COMMENTS:
//
//	DATE:			dcc 09/09/01

LONG_PTR sizeOfTI(pTI pIcon)
{
	LONG_PTR			strSize = 0;
	pTI				currentPtr;

	if (!isEmpty(startPtr)) {
		strSize = 1;
		currentPtr = pIcon;

		while (currentPtr != NULL) {
			currentPtr = currentPtr->nextPtr;
			strSize += 1;
		}
	}
	return strSize;
}

//  FUNCTION: readIconInfo( pTI *, LONG_PTR, LONG_PTR*, LONG_PTR*)
//
//  PURPOSE:	Gets iconID and procNbr for given index nbr
//
//  COMMENTS:
//
//	DATE:			dcc 09/10/01
// WJF 6/30/16 Win-21 LONG_PTR -> LONG
BOOL readIconInfo(pTI *pIcon, LONG index, LONG* pIconID, LONG* pProcNbr)
{
	pTI			currentPtr;
	LONG			i;

	currentPtr = *pIcon;
	for (i = 1; i <= index; i++) // WJF 6/24/16 Win-21 '=' -> '<=' as a guess...
	{
		if (index == i) {
			*pIconID = currentPtr->iconID;
			*pProcNbr = currentPtr->procNbr;
			return TRUE;
		}
		currentPtr = currentPtr->nextPtr;
	}

	return FALSE;
}

VOID Delay(DWORD delayTime) // delay is in
{
	DWORD		currentTime, endTime;

	endTime = GetTickCount();
	endTime += delayTime;

	do
	{
		currentTime = GetTickCount();  //do nothing
	} while (currentTime <= endTime);
}

//  FUNCTION: processWindowMessage(LONG_PTR source, LONG_PTR hwnd, WPARAM wParam, LPARAM lParam)
//
//  PURPOSE:	Communicates a value by setting a global (IP) 4D Var
//						via sending outside call to specific 4D process
//
//  COMMENTS:
//
//	MODIFICATIONS: 01/22/03 added source param to distinguish what function to respond to.
//
//	DATE:			dcc 09/09/01
//

void processWindowMessage(LONG_PTR source, LONG_PTR hwnd, WPARAM wParam, LPARAM lParam)
{
	char							procVar[30] = ST_TRAYNOTIFICATION;
	LONG							procNbr = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	PA_Variable						fourDVar;
	BOOL							bFuncReturn;

	bFuncReturn = SetForegroundWindow(windowHandles.fourDhWnd);
	switch (source)
	{
	case TRAY_ICON_FUNCTION:
		if ((PA_Get4DVersion() & 0x0000FFFF) < 0x00001100){
			// REB #17503 8/8/08 in v11 we can no longer call PA_GetVariable from this subclass.
			//   Instead of setting the ST_TRAYNOTIFICATION variable, we will just do a call
			//   back to the monitoring process in 4D on double clicks.
			strcpy_s(procVar, sizeof(procVar), ST_TRAYNOTIFICATION);  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s
			fourDVar = PA_GetVariable((PA_Unichar *)procVar); // WJF 6/24/16 Win-21 Casting to PA_Unichar *
			if (PA_GetVariableKind(fourDVar) == eVK_Longint) {
				PA_SetLongintVariable(&fourDVar, (LONG)lParam); // WJF 6/30/16 Win-21 Cast to LONG
				PA_SetVariable((PA_Unichar *)procVar, fourDVar, 1); // WJF 6/24/16 Win-21 Casting to PA_Unichar *
				findIconID(&startPtr, (LONG)wParam, &procNbr); // find icon id and get process number // WJF 6/30/16 Win-21 Cast to LONG
				PA_UpdateProcessVariable(procNbr);
			}
		}
		else{
			if ((lParam == 515) | (lParam == 518)){
				findIconID(&startPtr, (LONG)wParam, &procNbr); // find icon id and get process number // WJF 6/30/16 Win-21 CAST TO LONG
				PA_UpdateProcessVariable(procNbr);
			};
		};
		break;

	case RESPECT_TOOL_BAR_FUNCTION:
		// REB #16207 1/7/09 In v11 we can no longer call PA_GetVariable from this subclass.
		// Unfortunately we won't be able to support use of <>TB_NOTIFICATION anymore.
		if ((PA_Get4DVersion() & 0x0000FFFF) < 0x00001100){
			strcpy_s(procVar, sizeof(procVar), TB_NOTIFICATION);  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s
			fourDVar = PA_GetVariable((PA_Unichar *)procVar); // WJF 6/24/16 Win-21 Casting to PA_Unichar *
			if (PA_GetVariableKind(fourDVar) == eVK_ArrayLongint) {
				if (PA_GetArrayNbElements(fourDVar) == 4) {
					if (toolBarRestrictions.leftProcessNbr > 0) {
						PA_SetLongintInArray(fourDVar, 1, (LONG)hwnd); // WJF 6/30/16 Win-21 Casting to LONG
						PA_SetVariable((PA_Unichar *)procVar, fourDVar, 0); // WJF 6/24/16 Win-21 Casting to PA_Unichar *
						PA_UpdateProcessVariable(toolBarRestrictions.leftProcessNbr);
					}
					if (toolBarRestrictions.topProcessNbr > 0) {
						PA_SetLongintInArray(fourDVar, 2, (LONG)hwnd); // WJF 6/30/16 Win-21 Casting to LONG
						PA_SetVariable((PA_Unichar *)procVar, fourDVar, 0); // WJF 6/24/16 Win-21 Casting to PA_Unichar *
						PA_UpdateProcessVariable(toolBarRestrictions.topProcessNbr);
					}
					if (toolBarRestrictions.rightProcessNbr > 0) {
						PA_SetLongintInArray(fourDVar, 3, (LONG)hwnd); // WJF 6/30/16 Win-21 Casting to LONG
						PA_SetVariable((PA_Unichar *)procVar, fourDVar, 0); // WJF 6/24/16 Win-21 Casting to PA_Unichar *
						PA_UpdateProcessVariable(toolBarRestrictions.rightProcessNbr);
					}
					if (toolBarRestrictions.bottomProcessNbr > 0) {
						PA_SetLongintInArray(fourDVar, 4, (LONG)hwnd); // WJF 6/30/16 Win-21 Casting to LONG
						PA_SetVariable((PA_Unichar *)procVar, fourDVar, 0); // WJF 6/24/16 Win-21 Casting to PA_Unichar *
						PA_UpdateProcessVariable(toolBarRestrictions.bottomProcessNbr);
					}
				}
			}
		}
		else {
			// REB 3/29/10 #22878 Since we can't update the TB_NOTIFICATION array, just do a call to the toolbar processes.
			if (toolBarRestrictions.leftProcessNbr > 0) {
				PA_UpdateProcessVariable(toolBarRestrictions.leftProcessNbr);
			}
			if (toolBarRestrictions.topProcessNbr > 0) {
				PA_UpdateProcessVariable(toolBarRestrictions.topProcessNbr);
			}
			if (toolBarRestrictions.rightProcessNbr > 0) {
				PA_UpdateProcessVariable(toolBarRestrictions.rightProcessNbr);
			}
			if (toolBarRestrictions.bottomProcessNbr > 0) {
				PA_UpdateProcessVariable(toolBarRestrictions.bottomProcessNbr);
			}
		}

		break;
	}
}

//  FUNCTION:	restoreOrig4DWindowProcess()
//
//  PURPOSE:	checks active call flags and if all false, restores orig 4D window process
//
//  COMMENTS:	Returns True if process restored
//				A wrapper for SetWindowLong(windowHandles.fourDhWnd, GWL_WNDPROC, (LONG) processHandles.wpFourDOrigProc)
//
//	DATE:		dcc 01/21/03
//
BOOL restoreOrig4DWindowProcess()
{
	//iterate thru activeCalls and if none are active restore process
	//if ((!activeCalls.b4DMaximize) && (!activeCalls.bTrayIcons) && (!activeCalls.bPrinterCapture) && processHandles.wpFourDOrigProc != NULL) {
	if (processHandles.wpFourDOrigProc != NULL) {  // MJG 3/26/04 Replaced if-statement.
		//REB 3/18/11 #25290
		SetWindowLongPtr(windowHandles.fourDhWnd, GWLP_WNDPROC, (LONG_PTR)processHandles.wpFourDOrigProc);
		//SetWindowLong(windowHandles.fourDhWnd, GWL_WNDPROC, (LONG) processHandles.wpFourDOrigProc);
		processHandles.wpFourDOrigProc = NULL;
		return TRUE;
	}
	return FALSE;
}

//  FUNCTION:	subclassOrig4DWindowProcess()
//
//  PURPOSE:	Subclass the main 4D window in a controlled manner.
//
//  COMMENTS:
//
//	DATE:		MJG 3/26/04
//
VOID subclass4DWindowProcess()
{
	DWORD waitResult;

	if (processHandles.wpFourDOrigProc == NULL){
		waitResult = WaitForSingleObject(hSubclassMutex, INFINITE);

		if (waitResult == WAIT_OBJECT_0) {
			if (processHandles.wpFourDOrigProc == NULL){
				//REB 3/18/11 #25290
				processHandles.wpFourDOrigProc = (WNDPROC)SetWindowLongPtr(windowHandles.fourDhWnd, GWLP_WNDPROC, (LONG_PTR)newProc);
				//processHandles.wpFourDOrigProc = (WNDPROC) SetWindowLong(windowHandles.fourDhWnd, GWL_WNDPROC, (LONG) newProc);
			}

			ReleaseMutex(hSubclassMutex);
		}
	}
}