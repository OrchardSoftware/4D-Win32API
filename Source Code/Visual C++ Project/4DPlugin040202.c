#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"
#include "Utilities.h" // Added by Mark De Wever #12225
#include <shellapi.h>
#include <assert.h>

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

extern	pLL	startOfList;

// ------------------------------------------------
//
//  FUNCTION: sys_GetCommandLine( PA_PluginParameters params)
//
//  PURPOSE:	Gets command line used to start 4D
//
//  COMMENTS:	Parse command line. Ends at a negative character value.
//						Returns an array of parameters.
//						Params should be passed unquoted. There can be a space or NULL
//						between parameters. 1st param is delimited by space.  Rest are NULL.
//
//	DATE:			dcc 04/03/02 dcc
//
//	MODIFICATIONS: Rewritten 06/20/02 to make more concise, less convoluted, etc.
//                 Modified 7/29/03
void sys_GetCommandLine(PA_PluginParameters params)
{
	char				commandLineStr[MAXBUF];
	char                paramElement[MAXBUF];
	char				executableString[MAXBUF];
	char				*pMarker;
	LONG				returnValue = 0, commandLine_len = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG_PTR			charsToCopy;
	LPTSTR				pCommandLineStr, pTemp;
	PA_Variable			parameters;
	BOOL				bInQuotes = FALSE, bDone = FALSE;
	LONG				paramCount = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG				action = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG

	memset(commandLineStr, 0, MAXBUF);
	memset(paramElement, 0, MAXBUF);
	memset(executableString, 0, MAXBUF);

	parameters = PA_GetVariableParameter(params, 1);
	action = PA_GetLongParameter(params, 2);

	pCommandLineStr = GetCommandLine();

	if (pCommandLineStr == NULL) {
		returnValue = 0;
	}
	else {
		pMarker = pTemp = pCommandLineStr;

		//if first char a doublequote, skip it
		if (*pMarker == '"') {
			pMarker++;
			bInQuotes = TRUE;
		}

		// Find the executable name.
		while (!((*(++pTemp) == ' ' && !bInQuotes) || (*pTemp == '"' && bInQuotes) || *pTemp == '\0'));

		charsToCopy = (pTemp - pMarker);
		strncpy(executableString, pMarker, charsToCopy);
		commandLine_len = (LONG)charsToCopy; // WJF 6/30/16 Win-21 Cast to LONG
		executableString[charsToCopy] = '\0';

		//skip next quotes and spaces if they are there
		while ((*pTemp == '"') || (*pTemp == ' ')) {
			if ((*pTemp == '"') && (action == CL_DRAGDROP)) {
				bInQuotes = !bInQuotes; // toggle flag
			}
			commandLine_len += 1;
			pTemp++;
		}

		pMarker = pTemp;

		if (action == CL_DRAGDROP) {
			if (bInQuotes) {
				while ((*pTemp != '"') || ((*pTemp != '\0') && (*(pTemp + 1) != '\0'))) {
					pTemp++;
				}
			}
			else {
				while (*pTemp != '\0') {
					pTemp++;
				}
			}

			strncpy(paramElement, pMarker, pTemp - pMarker);
			paramElement[pTemp - pMarker] = '\0';
			if (strlen(paramElement) == 0) {
				PA_ResizeArray(&parameters, 1);
				strcpy(commandLineStr, executableString);
				returnValue = 1;
			}
			else {
				PA_ResizeArray(&parameters, 2);
				PA_SetTextInArray(parameters, 2, paramElement, strlen(paramElement));
				strcpy(commandLineStr, executableString);
				strcat(commandLineStr, " ");
				strcat(commandLineStr, paramElement);
				returnValue = 2;
			}
		}
		else {
			paramCount = 1;
			strcpy(commandLineStr, executableString);
			PA_ResizeArray(&parameters, paramCount);

			while (!bDone) {
				strcpy(paramElement, "");
				while (*pTemp >= 0) {
					pTemp++;
					//two nulls in a row also end
					//replace nulls with spaces
					if ((*(pTemp) == '\0') || (*(pTemp) == ' ')) {
						if ((*(pTemp + 1) == '\0') || (*(pTemp + 1) == ' ')){
							bDone = TRUE;
							paramCount++;
							break;
						}
						else {
							paramCount++;
							break;
						}
					}
				} // end while

				strncpy(paramElement, pMarker, pTemp - pMarker);
				paramElement[pTemp - pMarker] = '\0';
				if (strlen(paramElement) > 0) {
					PA_ResizeArray(&parameters, paramCount);
					PA_SetTextInArray(parameters, paramCount, paramElement, strlen(paramElement));
					pMarker = pTemp + 1;
					strcat(commandLineStr, " ");
					strcat(commandLineStr, paramElement);
				}
				else {
					bDone = TRUE;
				}
			} // end while !done
			returnValue = PA_GetArrayNbElements(parameters);
		}

		PA_SetTextInArray(parameters, 1, executableString, strlen(executableString));
		PA_SetTextInArray(parameters, 0, commandLineStr, strlen(commandLineStr));
		PA_SetVariableParameter(params, 1, parameters, 0);
	} // (pCommandLineStr == NULL)

	PA_ReturnLong(params, returnValue);
}

// ------------------------------------------------
//
//  FUNCTION: gui_GetWindowStyle( PA_PluginParameters params)
//
//  PURPOSE:	Get window styles
//
//  COMMENTS:	outputs in array the applicable styles for the window
//						Returns via return code the LONG_PTR representing the value
//
//	DATE:			dcc 07/18/02 dcc
//
//	MODIFICATIONS:
//
void gui_GetWindowStyle(PA_PluginParameters params, BOOL isEx)
{
	PA_Variable					styles;
	LONG						returnValue = 0, testValue = 0, i; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	HWND						hWnd;
	char						styleText[40];
	BOOL						bFoundOne;
	LONG						hWndIndex = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG

	hWndIndex = PA_GetLongParameter(params, 1); // WJF 9/1/15 #43731 We are now getting an index to an internal array

	if (isEx){ // WJF 9/16/15 #43731
		hWnd = handleArray_retrieve((DWORD)hWndIndex);
	}
	else {
		hWnd = (HWND)hWndIndex;
	}

	if (IsWindow(hWnd)) {
		styles = PA_GetVariableParameter(params, 2);

		returnValue = GetWindowLong(hWnd, GWL_STYLE);
		testValue = returnValue;

		//and all styles to see what the window uses
		i = 1;
		while ((testValue != 0) && (i <= NUMBER_STYLES))
		{
			bFoundOne = FALSE;

			if ((testValue & WS_THICKFRAME) == WS_THICKFRAME) {
				strcpy(styleText, "WS_THICKFRAME");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_THICKFRAME;
			}
			else if
				((testValue & WS_POPUP) == WS_POPUP) {
				strcpy(styleText, "WS_POPUP");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_POPUP;
			}
			else if
				((testValue & WS_CHILD) == WS_CHILD) {
				strcpy(styleText, "WS_CHILD");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_CHILD;
			}
			else if
				((testValue & WS_MINIMIZEBOX) == WS_MINIMIZEBOX) {
				strcpy(styleText, "WS_MINIMIZEBOX");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_MINIMIZEBOX;
			}
			else if
				((testValue & WS_VISIBLE) == WS_VISIBLE) {
				strcpy(styleText, "WS_VISIBLE");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_VISIBLE;
			}
			else if
				((testValue & WS_DISABLED) == WS_DISABLED) {
				strcpy(styleText, "WS_DISABLED");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_DISABLED;
			}
			else if
				((testValue & WS_CLIPSIBLINGS) == WS_CLIPSIBLINGS) {
				strcpy(styleText, "WS_CLIPSIBLINGS");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_CLIPSIBLINGS;
			}
			else if
				((testValue & WS_CLIPCHILDREN) == WS_CLIPCHILDREN) {
				strcpy(styleText, "WS_CLIPCHILDREN");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_CLIPCHILDREN;
			}
			else if
				((testValue & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX) {
				strcpy(styleText, "WS_MAXIMIZEBOX");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_MAXIMIZEBOX;
			}
			else if
				((testValue & WS_CAPTION) == WS_CAPTION) {
				strcpy(styleText, "WS_CAPTION");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_CAPTION;
			}
			else if
				((testValue & WS_BORDER) == WS_BORDER) {
				strcpy(styleText, "WS_BORDER");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_BORDER;
			}
			else if
				((testValue & WS_DLGFRAME) == WS_DLGFRAME) {
				strcpy(styleText, "WS_DLGFRAME");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_DLGFRAME;
			}
			else if
				((testValue & WS_VSCROLL) == WS_VSCROLL) {
				strcpy(styleText, "WS_VSCROLL");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_VSCROLL;
			}
			else if
				((testValue & WS_HSCROLL) == WS_HSCROLL) {
				strcpy(styleText, "WS_HSCROLL");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_HSCROLL;
			}
			else if
				((testValue & WS_SYSMENU) == WS_SYSMENU) {
				strcpy(styleText, "WS_SYSMENU");
				bFoundOne = TRUE;
				testValue = testValue ^ WS_SYSMENU;
			}

			if (bFoundOne) {
				PA_ResizeArray(&styles, i);
				PA_SetTextInArray(styles, i, styleText, strlen(styleText));
			}
			i++;
		} // end while

		if (testValue == 0) {
			strcpy(styleText, "WS_OVERLAPPED");
			PA_ResizeArray(&styles, i);
			PA_SetTextInArray(styles, i, styleText, strlen(styleText));
		}

		PA_SetVariableParameter(params, 2, styles, 0);
	}
	PA_ReturnLong(params, returnValue);
}

// ------------------------------------------------
//
//  FUNCTION: gui_restrictWindow( PA_PluginParameters params)
//
//  PURPOSE:	Prevents window from being resized, moved, etc
//
//  COMMENTS:	4D adds a resixing window/control to bottom right
//						client area that sends sends SYSCOMMAND messages.
//						So even with Window's sizing border removed, must
//						stll trap these messages with subclass procedure.
//
//	DATE:			dcc 07/15/02 dcc
//
//	MODIFICATIONS: 09/09/02 added functionality for closebox, next, & previous window disabling
//

void gui_RestrictWindow(PA_PluginParameters params, BOOL isEx)
{
	LONG				action = 0, returnValue = 0, styleChg = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	HWND				hWnd = NULL;
	HMENU				hSysMenu;
	pLL					thisLink = NULL, previousLink = NULL;
	LONG				hWndIndex = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG

	// subClass procedure must be in place
	if (processHandles.wpProToolsOrigProc == NULL) {
		PA_ReturnLong(params, -2);
		return;
	}

	hWndIndex = PA_GetLongParameter(params, 1); // WJF 9/1/15 #43731 We are now getting an index to an internal array
	action = PA_GetLongParameter(params, 2);

	if (isEx){ // WJF 9/16/15 #43731
		hWnd = handleArray_retrieve((DWORD)hWndIndex);
	}
	else {
		hWnd = (HWND)hWndIndex;
	}

	if (IsWindow(hWnd)) {  // 09/09/02 more action values now so removed restriction
		returnValue = GetWindowLong(hWnd, GWL_STYLE);

		// eliminate sizing cursor and Window's sizing border
		switch (action)
		{
		case (RW_NO_MIN) :
			if ((returnValue & WS_MINIMIZEBOX) == WS_MINIMIZEBOX) {
				styleChg = returnValue ^ WS_MINIMIZEBOX;
				returnValue = SetWindowLong(hWnd, GWL_STYLE, styleChg);
			}
			break;

		case (RW_NO_MAX) :
			if ((returnValue & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX) {
				styleChg = returnValue ^ WS_MAXIMIZEBOX;
				returnValue = SetWindowLong(hWnd, GWL_STYLE, styleChg);
			}
			break;

		case (RW_NO_CLOSE) :
			hSysMenu = GetSystemMenu(hWnd, 0);
			if (hSysMenu != NULL) {
				EnableMenuItem(hSysMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
			}
			break;

		case (RW_RESTORE_CLOSE) :
			hSysMenu = GetSystemMenu(hWnd, 0);
			if (hSysMenu != NULL) {
				EnableMenuItem(hSysMenu, SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
			}
			break;
		}

		if (startOfList == NULL) {
			init_list(&startOfList);
		}
		// insert item in linked list -- if not already there
		if (!search_list(&startOfList, &thisLink, &previousLink, LL_hWnd, LL_Restrict, (LONG_PTR *)&hWnd)) {
			thisLink = (pLL)insert_list(&startOfList);
			if (thisLink == NULL) {
				PA_ReturnLong(params, -1); // could not add to list - get out
				return;
			}
		}
		else {
			PA_ReturnLong(params, (LONG)hWnd); // return window handle if already in list // WJF 6/30/16 Win-21 Changed cast from LONG_PTR -> LONG
			return;
		}

		thisLink->hWnd = hWnd;
		thisLink->dataLong1 = action;
		thisLink->type = LL_Restrict;
		thisLink->wpProc = processHandles.wpProToolsOrigProc;
		returnValue = length_list(startOfList);
	}

	PA_ReturnLong(params, returnValue);
}

// ------------------------------------------------
//
//  FUNCTION: ProToolsProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//
//  PURPOSE:	Sub classes all MDI child 4D windows.
//
//  COMMENTS: At present only interested in certain messages for certain windows
//						Used with gui_RestrictWindow and gui_RespectToolBar.  This subclass
//						procedure is activated by a call to sys_SubClassInit.
//
//	DATE:			07/15/02 dcc
//
//  MODIFICATIONS: 01/16/03 added intercept for WM_GETMINMAXINFO to limit
//									maximize size so as to not cover a tool bar (designed in 4D) positioned
//									under the MDI window's menu bar.

LRESULT APIENTRY ProToolsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG_PTR			action = 0, command = 0, hitArea = 0;
	pLL					currentLink, prevLink;
	RECT				rect, clientRect, workingAreaRect;
	LONG				x = 0, y = 0, xFrame, yFrame; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG				mnuItemCount, i; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	HMENU				hMenu;
	BOOL				bFuncReturn;
	LPMINMAXINFO		lpmmi; //01/18/03
	static LONG_PTR			InCommand = 0; // 01/18/03
	POINT				pt; // 01/19/03
	LPWINDOWPOS			lpwp;
	LPNCCALCSIZE_PARAMS	lpcalcsize;
	static HWND			currentChildMaxed = NULL, prevChildMaxed = NULL;
	//char*				minimizedWindows[32];
	char				title[128];
	INT					n = 0, err = 0; // WJF 6/30/16 Win-21 INT_PTR -> INT
	INT					minimizedPerRow; // REB 8/11/08 #16207 // WJF 6/30/16 Win-21 INT_PTR -> INT

	currentLink = NULL;
	prevLink = NULL;

	if (toolBarRestrictions.toolBarOnDeck == 1) { // 01/17/03
		xFrame = GetSystemMetrics(SM_CXSIZEFRAME);
		yFrame = GetSystemMetrics(SM_CYSIZEFRAME);
		// REB 8/11/08 #16207
		minimizedPerRow = ((GetSystemMetrics(SM_CXVIRTUALSCREEN) - (toolBarRestrictions.clientOffsetx + toolBarRestrictions.left + toolBarRestrictions.right)) / GetSystemMetrics(SM_CXMINSPACING));
		//err = GetWindowText(hwnd, title , 128); //**DEBUG**
		switch (uMsg)
		{
		case WM_SYSCOMMAND: //this tells a subsequent WM_GETMINMAXINFO command
			// that it is okay to proceed if we are in the process of a maximize command.
			command = wParam & 0xFFF0;
			if (SC_MAXIMIZE == command) {
				InCommand = IN_SYSCOMMAND_MAX;
				GetWindowRect(hwnd, &toolBarRestrictions.origWindowRect);
				GetWindowRect(windowHandles.MDIhWnd, &rect);
				toolBarRestrictions.clientOffsetx = rect.left;
				toolBarRestrictions.clientOffsety = rect.top;
				currentChildMaxed = hwnd;
				if (IsZoomed(hwnd) == 0){ // REB 8/11/08 #16207
					//minimizePositions[toolBarRestrictions.numMinimized] = 0;
					err = GetWindowText(hwnd, title, 128);
					for (n = 0; n < SMLBUF; n++) {
						if (strcmp(toolBarRestrictions.minimizedWindows[n], title) == 0){
							strcpy(toolBarRestrictions.minimizedWindows[n], "");
							//break;
						}
					}
				}
			}
			// REB 8/11/08 #16207
			if (SC_MINIMIZE == command) {
				InCommand = IN_SYSCOMMAND_MIN;
				GetWindowRect(hwnd, &toolBarRestrictions.origWindowRect);
				GetWindowRect(windowHandles.MDIhWnd, &rect);

				//Debug
				//GetWindowRect(windowHandles.MDIs_4DhWnd, &rect);
				//GetWindowRect(windowHandles.fourDhWnd, &rect);
				//End Debug

				toolBarRestrictions.clientOffsetx = rect.left;
				toolBarRestrictions.clientOffsety = rect.top;
			}

			if (SC_RESTORE == command) { //|| (toolBarRestrictions.appBeingMaxed > 0)) {
				InCommand = IN_SYSCOMMAND_RESTORE;
				if (IsZoomed(hwnd) != 0){ // REB 8/11/08 #16207 Only clear these if we are restoring a window that was already maximized.
					currentChildMaxed = NULL;
					// REB 3/26/10 #22878 Move the previous window back.
					MoveWindow(prevChildMaxed, toolBarRestrictions.previousWindowRect.left, toolBarRestrictions.previousWindowRect.top, (toolBarRestrictions.previousWindowRect.right - toolBarRestrictions.previousWindowRect.left), (toolBarRestrictions.previousWindowRect.bottom - toolBarRestrictions.previousWindowRect.top), 1);
					prevChildMaxed = NULL;
				}
				else{
					err = GetWindowText(hwnd, title, 128);
					for (n = 0; n < SMLBUF; n++) {
						if (strcmp(toolBarRestrictions.minimizedWindows[n], title) == 0){
							strcpy(toolBarRestrictions.minimizedWindows[n], "");
							//break;
						}
					}
				}
				toolBarRestrictions.appBeingMaxed = 0;
			}
			//if (SC_NEXTWINDOW == command) {
			//toolBarRestrictions.appBeingMaxed = XCHANGING_MAX_WINDOWS;
			//}

			if (SC_CLOSE == command) {
				if (IsZoomed(hwnd) == 0){ // REB 8/11/08 #16207
					err = GetWindowText(hwnd, title, 128);
					for (n = 0; n < SMLBUF; n++) {
						if (strcmp(toolBarRestrictions.minimizedWindows[n], title) == 0){
							strcpy(toolBarRestrictions.minimizedWindows[n], "");
							//break;
						}
					}
				}
				break;
			};

			break;

		case WM_WINDOWPOSCHANGING:

			lpwp = (LPWINDOWPOS)lParam;

			GetWindowRect(windowHandles.MDIhWnd, &rect);

			if ((InCommand == IN_SYSCOMMAND_MAX) && (toolBarRestrictions.appWindowState == APP_MAXIMIZED)) {
				lpwp->x = lpwp->x - toolBarRestrictions.clientOffsetx - toolBarRestrictions.left;
				lpwp->y = lpwp->y - toolBarRestrictions.clientOffsety - toolBarRestrictions.top - yFrame;
				lpwp->cx = toolBarRestrictions.origWindowRect.right - toolBarRestrictions.origWindowRect.left;
				lpwp->cy = toolBarRestrictions.origWindowRect.bottom - toolBarRestrictions.origWindowRect.top;
				return 0;
			}
			// REB 8/11/08 #16207
			if ((InCommand == IN_SYSCOMMAND_MIN)){  // Minimizing the windows
				err = GetWindowText(hwnd, title, 128);
				for (n = 0; n < SMLBUF; n++) {
					if ((strcmp(toolBarRestrictions.minimizedWindows[n], "") == 0) || (strcmp(toolBarRestrictions.minimizedWindows[n], title) == 0)){
						strcpy(toolBarRestrictions.minimizedWindows[n], title);
						break;
					}
				}

				lpwp->x = toolBarRestrictions.clientOffsetx + toolBarRestrictions.left + ((n % minimizedPerRow)  * GetSystemMetrics(SM_CXMINSPACING));

				SystemParametersInfo(SPI_GETWORKAREA, 0, &workingAreaRect, 0);
				lpwp->y = workingAreaRect.bottom - yFrame - toolBarRestrictions.clientOffsety - toolBarRestrictions.bottom - (((n / (minimizedPerRow)) + 1) * GetSystemMetrics(SM_CYMINSPACING));

				return 0;
			}

			if ((toolBarRestrictions.appBeingMaxed == APP_MAXIMIZING) ||
				(toolBarRestrictions.appBeingMaxed == APP_RESTORING) ||
				(toolBarRestrictions.appBeingMaxed == APP_SIZING_W_CHLDMAX)) {
				lpwp->x = toolBarRestrictions.left - xFrame;
				lpwp->y = toolBarRestrictions.top - yFrame;
				lpwp->cx = rect.right - rect.left + (2 * xFrame) - toolBarRestrictions.left - toolBarRestrictions.right;
				lpwp->cy = rect.bottom - rect.top + (2 * yFrame) - toolBarRestrictions.top - toolBarRestrictions.bottom;
				return 0;
			}

			if (currentChildMaxed == hwnd) { //(toolBarRestrictions.appBeingMaxed == (LONG_PTR)hwnd) {
				lpwp->x = toolBarRestrictions.left - xFrame;
				lpwp->y = toolBarRestrictions.top - yFrame;
				lpwp->cx = rect.right - rect.left + (2 * xFrame) - toolBarRestrictions.left - toolBarRestrictions.right;
				lpwp->cy = rect.bottom - rect.top + (2 * yFrame) - toolBarRestrictions.top - toolBarRestrictions.bottom;
				return 0;
			}

			if (toolBarRestrictions.top > 0){  // AMS 8/29/14 #39693 If its a top toolbar, prevent windows from being able to be placed behind the toolbar NOTE: This fix does not work in Windows versions 8 or newer
				if (lpwp->y < toolBarRestrictions.top){
					lpwp->y = toolBarRestrictions.top - 1;
				}
			}
			break;

		case WM_GETMINMAXINFO:
			if ((InCommand == IN_SYSCOMMAND_MAX) || (toolBarRestrictions.appBeingMaxed > 0) ||
				(currentChildMaxed == hwnd)) {
				lpmmi = (LPMINMAXINFO)lParam;

				lpmmi->ptMaxSize.x = lpmmi->ptMaxSize.x - toolBarRestrictions.left - toolBarRestrictions.right;
				lpmmi->ptMaxSize.y = lpmmi->ptMaxSize.y - toolBarRestrictions.top - toolBarRestrictions.bottom;
				lpmmi->ptMaxPosition.x = -xFrame + toolBarRestrictions.left; // maxPosition x&y seem to be static values -- hmmm!
				lpmmi->ptMaxPosition.y = -yFrame + toolBarRestrictions.top;
				lpmmi->ptMaxTrackSize.x = lpmmi->ptMaxTrackSize.x - toolBarRestrictions.left - toolBarRestrictions.right;
				lpmmi->ptMaxTrackSize.y = lpmmi->ptMaxTrackSize.y - toolBarRestrictions.top - toolBarRestrictions.bottom;

				return 0; // return 0 telling OS that we have processed this command
			} // end if
			break;

		case WM_NCCALCSIZE:
			if (((currentChildMaxed != NULL) || (prevChildMaxed != NULL)) && ((BOOL)wParam == TRUE)) {
				lpcalcsize = (LPNCCALCSIZE_PARAMS)lParam;
				lpwp = lpcalcsize->lppos;
				rect = lpcalcsize->rgrc[0];
				clientRect = lpcalcsize->rgrc[1];
				toolBarRestrictions.appBeingMaxed = 0; //new
			}
			break;

		case WM_NCACTIVATE:
			if ((currentChildMaxed != NULL) && (currentChildMaxed != hwnd)) {
				//bFuncReturn = IsZoomed(currentChildMaxed);
				// REB 3/26/10 #22878 Here we need to capture the size of the newly activated window before it's resized to the max.
				// This window will become the new original window and we'll save the previous window position to be resized later.
				if (wParam){
					toolBarRestrictions.previousWindowRect = toolBarRestrictions.origWindowRect;
					GetWindowRect(hwnd, &toolBarRestrictions.origWindowRect);
					prevChildMaxed = currentChildMaxed;
					currentChildMaxed = hwnd;
				}
				toolBarRestrictions.appBeingMaxed = (LONG_PTR)currentChildMaxed; //was XCHANGING_MAX_WINDOWS
			}
			break;

		case WM_SETFOCUS: // msg received by window getting focus
			//if (prevChildMaxed == (HWND)wParam) {
			//use the following in lieu of XCHANGING.. to restore all on window change
			//SendMessage((HWND)wParam, WM_SYSCOMMAND, SC_RESTORE, 0L);
			//use this to keep maxed window when using CRTL-TAB.  But 4D has an ugy bug in window
			//  resize when called from a menu (Bring to Front).
			//toolBarRestrictions.appBeingMaxed = XCHANGING_MAX_WINDOWS;
			break;

		case WM_SIZE:
			if (toolBarRestrictions.appBeingMaxed != APP_SIZING_W_CHLDMAX) {
				toolBarRestrictions.appBeingMaxed = 0;
			}
			InCommand = 0;
			break;

		case WM_SIZING:
		case WM_MOVING: // restrict sizing or moving to prevent
			GetClientRect(windowHandles.MDIhWnd, &clientRect);
			//clientRect.right contains width of client area
			//clientRect.bottom contains height of client area
			//convert this rect to screen coordinates for comparison to movingRect
			pt.x = clientRect.left;
			pt.y = clientRect.top;
			ClientToScreen(windowHandles.MDIhWnd, &pt);
			clientRect.left = pt.x;
			clientRect.top = pt.y;
			pt.x = clientRect.right;
			pt.y = clientRect.bottom;
			ClientToScreen(windowHandles.MDIhWnd, &pt);
			clientRect.right = pt.x;
			clientRect.bottom = pt.y;

			clientRect.left += toolBarRestrictions.left;
			if (toolBarRestrictions.trackingRestriction == 0) {
				clientRect.top += toolBarRestrictions.top;
			}
			else {
				clientRect.top += toolBarRestrictions.trackingRestriction;
			}
			clientRect.right -= toolBarRestrictions.right;
			clientRect.bottom -= toolBarRestrictions.bottom;
			ClipCursor(&clientRect);
			break;

		case WM_EXITSIZEMOVE:
			rect.left = 0;
			rect.top = 0;
			rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);// REB 6/6/08 #16838 (Fix Provided by Keith White)
			rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);// REB 6/6/08 #16838 (Fix Provided by Keith White)
			ClipCursor(&rect);
			break;
		default:
			if ((uMsg != WM_ERASEBKGND) && (uMsg != WM_NCPAINT) && (uMsg != WM_PAINT) && (uMsg != WM_IME_SETCONTEXT) && (uMsg != WM_IME_NOTIFY) && (uMsg != WM_SETFOCUS) && (uMsg != WM_NCACTIVATE) && (uMsg != WM_KILLFOCUS) && (uMsg != WM_GETTEXT)){
				break;
			};
		} // end switch (uMsg)
	} // end if (toolBarRestrictions.toolBarOnDeck == 1)

	if (startOfList != NULL) {
		if ((uMsg == WM_SYSCOMMAND) || (uMsg == WM_NCHITTEST) || (uMsg == WM_INITMENU)) {
			if (search_list(&startOfList, &currentLink, &prevLink, LL_hWnd, LL_Restrict, (LONG_PTR *)&hwnd)) {
				action = (LONG)currentLink->dataLong1; // WJF 6/30/16 Win-21 Cast to LONG

				switch (uMsg)
				{
				case (WM_SYSCOMMAND) :
					command = wParam & 0xFFF0;
					switch (command)
					{
					case (SC_SIZE) :
						if (RW_NO_SIZE == (action & RW_NO_SIZE)) {
							wParam = 0x000F;
						}
						break;

					case (SC_MOVE) :
						if (RW_NO_MOVE == (action & RW_NO_MOVE)) {
							wParam = 0x000F;
						}
						break;

					case (SC_MINIMIZE) :
						if (RW_NO_MIN == (action & RW_NO_MIN)) {
							wParam = 0x000F;
						}
						break;

					case (SC_MAXIMIZE) :
						if (RW_NO_MAX == (action & RW_NO_MAX)) {
							wParam = 0x000F;
						}
						break;
					}  // end switch

					break;

				case (WM_NCHITTEST) :
					if (RW_NO_SIZE == (action & RW_NO_SIZE)) {
						hitArea = DefWindowProc(hwnd, uMsg, wParam, lParam);
						if ((hitArea >= HTLEFT) && (hitArea <= HTBOTTOMRIGHT)) {
							uMsg = HTNOWHERE;
						}
						else {
							// test for coordinates of lower right
							GetWindowRect(hwnd, &rect);
							x = LOWORD(lParam);
							y = HIWORD(lParam);
							if ((x <= rect.right) && (x >= (rect.right - 25)) &&
								(y <= rect.bottom) && (y >= (rect.bottom - 25))) {
								uMsg = HTNOWHERE;
							}
						}
					}
					break;

				case (WM_INITMENU) :

					hMenu = GetSystemMenu(hwnd, FALSE);

					if (RW_NO_SIZE == (action & RW_NO_SIZE)) {
						bFuncReturn = EnableMenuItem(hMenu, SC_SIZE, MF_BYCOMMAND | MF_GRAYED);
					}
					if (RW_NO_MOVE == (action & RW_NO_MOVE)) {
						bFuncReturn = EnableMenuItem(hMenu, SC_MOVE, MF_BYCOMMAND | MF_GRAYED);
					}
					if (RW_NO_MIN == (action & RW_NO_MIN)) {
						bFuncReturn = EnableMenuItem(hMenu, SC_MINIMIZE, MF_BYCOMMAND | MF_GRAYED);
					}
					if (RW_NO_MAX == (action & RW_NO_MAX)) {
						bFuncReturn = EnableMenuItem(hMenu, SC_MAXIMIZE, MF_BYCOMMAND | MF_GRAYED);
					}
					if (RW_NO_NEXT == (action & RW_NO_NEXT)) {
						mnuItemCount = GetMenuItemCount(hMenu);
						for (i = (mnuItemCount - 1); i >= 0; i--)
						{
							if (GetMenuItemID(hMenu, i) == SC_NEXTWINDOW) {
								bFuncReturn = DeleteMenu(hMenu, SC_NEXTWINDOW, MF_BYCOMMAND);
								// is next higher menu item a separator line?
								if (GetMenuItemID(hMenu, i - 1) == 0) {
									bFuncReturn = DeleteMenu(hMenu, i - 1, MF_BYPOSITION);
								}
							}
							else if (GetMenuItemID(hMenu, i) == SC_PREVWINDOW) {
								bFuncReturn = DeleteMenu(hMenu, SC_PREVWINDOW, MF_BYCOMMAND);
								// is next higher menu item a separator line?
								if (GetMenuItemID(hMenu, i - 1) == 0) {
									bFuncReturn = DeleteMenu(hMenu, i - 1, MF_BYPOSITION);
								}
							}
						}
					} // end if (RW_NO_NEXT == (action & RW_NO_NEXT))

					break;

				case (WM_DESTROY) :
					delete_list(&startOfList, LL_hWnd, LL_Restrict, (LONG_PTR *)&hwnd);
					break;
				} // end switch (uMsg)
			} // end if (search_list( &startOfList, &currentLink, &prevLink, LL_hWnd, LL_Restrict, (LONG_PTR *) &hwnd))
		} // end if ((uMsg == WM_SYSCOMMAND) || (uMsg == WM_NCHITTEST) || (uMsg == WM_INITMENU))
	} // end if (startOfList != NULL)

	return CallWindowProc(processHandles.wpProToolsOrigProc, hwnd, uMsg, wParam, lParam);
}

// ------------------------------------------------
//
//  FUNCTION: gui_SubClassInit( PA_PluginParameters params )
//
//  PURPOSE:	Initializes the subclass for the entire class ProToolsSubMDIWndClass
//
//  COMMENTS:	Requires some positive value to initialize.  RW_SUBCLASS_INIT
//						provided as constant in resource file
//						Can be released using parameter RW_RELEASE passed to this function
//
//	DATE:			dcc 07/27/02 dcc
//
//	MODIFICATIONS:
//			REB 3/24/10 Commented out unrequired logic and variables.
//

void gui_SubClassInit(PA_PluginParameters params)
{
	LONG				returnValue = 0, action = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	//WNDCLASSEX			wndClassEx;
	//LPWNDCLASSEX		lpWndClassEx = &wndClassEx;
	//char				className[] = "ProToolsSubMDIWndClass";
	//HINSTANCE			hInst;

	// Put NDEBUF flag back into Preprocessor definitions
	action = PA_GetLongParameter(params, 1);
	toolBarRestrictions.toolBarOnDeck = 0; // added for gui_RespectToolBar 01/16/03

	if (action != RW_RELEASE) {
		if (processHandles.wpProToolsOrigProc == NULL) {
			processHandles.wpProToolsOrigProc = (WNDPROC)SetClassLongPtr(windowHandles.MDIs_4DhWnd, GCLP_WNDPROC, (LONG_PTR)ProToolsProc);
			returnValue = (LONG)processHandles.wpProToolsOrigProc; // WJF 6/30/16 Win-21 Casting to LONG

			/*
			//hInst = (HINSTANCE) PA_Get4DHInstance(); // this doesn't work for 4D 6.5
			hInst = (HINSTANCE) GetWindowLong(windowHandles.fourDhWnd, GWL_HINSTANCE); // 09/10/02
			//hInst = (HINSTANCE)GetWindowLongPtr(windowHandles.MDIhWnd, GWL_HINSTANCE);

			wndClassEx.cbSize = sizeof(WNDCLASSEX);
			if (GetClassInfoEx(hInst, className, lpWndClassEx)) {
			wndClassEx.lpfnWndProc = &ProToolsProc;
			processHandles.wpProToolsOrigProc = (WNDPROC) SetClassLongPtr(windowHandles.MDIs_4DhWnd, GCLP_WNDPROC, (LONG_PTR)ProToolsProc);
			returnValue = (LONG_PTR) processHandles.wpProToolsOrigProc;
			}
			*/
		}
	}
	else {
		if (processHandles.wpProToolsOrigProc != NULL) {
			//SetClassLongPtr(windowHandles.fourDhWnd, GCLP_WNDPROC, (LONG_PTR)processHandles.wpProToolsOrigProc);
			SetClassLongPtr(windowHandles.MDIs_4DhWnd, GCLP_WNDPROC, (LONG_PTR)processHandles.wpProToolsOrigProc);
			processHandles.wpProToolsOrigProc = NULL;
			returnValue = 1;
		}
		clear_list(&startOfList);
	}
	PA_ReturnLong(params, returnValue);
}

// ------------------------------------------------
//
//  FUNCTION: gui_GetWindowState( PA_PluginParameters params)
//
//  PURPOSE:	Determine if window is minimized or maximized
//
//  COMMENTS:	Returns 0 if window is normal, 1 if minimized
//						and 2 if maximized.
//
//	DATE:			dcc 07/20/02 dcc
//
//	MODIFICATIONS:
//
void gui_GetWindowState(PA_PluginParameters params, BOOL isEx)
{
	HWND				hWnd;
	LONG			hWndIndex; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG			returnValue = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG

	hWndIndex = PA_GetLongParameter(params, 1); // WJF 9/1/15 #43731 We are now getting an index to an internal array

	if (isEx){ // WJF 9/16/15 #43731
		hWnd = handleArray_retrieve((DWORD)hWndIndex);
	}
	else {
		hWnd = (HWND)hWndIndex;
	}

	if (IsWindow(hWnd)) {
		if (IsIconic(hWnd)) {
			returnValue = IS_ICONIC;
		}
		else if (IsZoomed(hWnd)) {
			returnValue = IS_ZOOMED;
		}
	}

	PA_ReturnLong(params, returnValue);
}

// ------------------------------------------------
//
//  FUNCTION: gui_SetWindowStyle( PA_PluginParameters params)
//
//  PURPOSE:	Enables disables toolbox icons
//
//  COMMENTS:	based on gui_RestrictWindow but can also enable the icons
//              uses the RW_ENABLE/DISABLE group
//
//	DATE:		2006/03/03 by Mark de Wever #12225
//
//  0 oke
// -1 style change failed
// -2 subclass proc not defined
// -3 failed to add to list

void gui_SetWindowStyle(PA_PluginParameters params, BOOL isEx)
{
	LONG				StyleCurr = 0, StyleNew = 0, action = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	HWND				hWnd = NULL;
	HMENU				hSysMenu;
	pLL					thisLink = NULL, previousLink = NULL;
	LONG				hWndIndex = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG

	hWndIndex = PA_GetLongParameter(params, 1); // WJF 9/1/15 #43731 We are now getting an index to an internal handle array
	action = PA_GetLongParameter(params, 2);

	if (isEx){ // WJF 9/16/15 #43731
		hWnd = handleArray_retrieve((DWORD)hWndIndex);
	}
	else {
		hWnd = (HWND)hWndIndex;
	}

	if (IsWindow(hWnd)) {
		StyleCurr = GetWindowLong(hWnd, GWL_STYLE);
		StyleNew = StyleCurr;

		if (0 != (action & RW_DISABLE_MIN)){
			StyleNew = StyleNew & ~WS_MINIMIZEBOX;
		}

		if (0 != (action & RW_ENABLE_MIN)){
			StyleNew = StyleNew | WS_MINIMIZEBOX;
		}

		if (0 != (action & RW_DISABLE_MAX)){
			StyleNew = StyleNew & ~WS_MAXIMIZEBOX;
		}

		if (0 != (action & RW_ENABLE_MAX)){
			StyleNew = StyleNew | WS_MAXIMIZEBOX;
		}

		if (0 != (action & RW_DISABLE_CLOSE)){
			hSysMenu = GetSystemMenu(hWnd, 0);
			if (hSysMenu != NULL) {
				EnableMenuItem(hSysMenu, SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
			}
		}

		if (0 != (action & RW_ENABLE_CLOSE)){
			hSysMenu = GetSystemMenu(hWnd, 0);
			if (hSysMenu != NULL) {
				EnableMenuItem(hSysMenu, SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
			}
		}

		if (0 != (action & RW_DISABLE_RESIZE)){
			// subClass procedure must be in place
			if (processHandles.wpProToolsOrigProc == NULL) {
				PA_ReturnLong(params, -2);
				return;
			}

			if (startOfList == NULL) {
				init_list(&startOfList);
			}

			// insert item in linked list -- if not already there
			if (!search_list(&startOfList, &thisLink, &previousLink, LL_hWnd, LL_Restrict, (LONG_PTR *)&hWnd)) {
				thisLink = (pLL)insert_list(&startOfList);
				if (thisLink == NULL) {
					PA_ReturnLong(params, -2);
					return;
				}
				else {
					thisLink->hWnd = hWnd;
					thisLink->dataLong1 = RW_NO_SIZE; // sub proc uses other number
					thisLink->type = LL_Restrict;
					thisLink->wpProc = processHandles.wpProToolsOrigProc;
				}
			}
		}

		if (0 != (action & RW_ENABLE_RESIZE)){
			// no subclass or no list head, we're done
			if ((NULL != processHandles.wpProToolsOrigProc) && (NULL != startOfList)) {
				delete_list(&startOfList, LL_hWnd, LL_Restrict, (LONG_PTR *)&hWnd);
			}
		}

		if (StyleNew != StyleCurr){
			if (0 == SetWindowLong(hWnd, GWL_STYLE, StyleNew)){
				PA_ReturnLong(params, -2);
				return;
			}
			else {
				// notify windows we changed the style
				SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
	}
	// when we end up here all is oke
	PA_ReturnLong(params, 0);
}

// ------------------------------------------------
//
//  FUNCTION: sys_FileExists( PA_PluginParameters params)
//
//  PURPOSE:	Checks whether a file exits
//
//	DATE:		2006/07/21 by Mark de Wever #12225
//
//  0 file doesn't exist (or error)
//  1 file exists

// 4D has problem with files and directories starting
// with a dot. So this routine is a replacement of
// Test path name. Note if this routine says the file
// exists and Test path name not then the file exists.
// Unfortunatly some 4D commando's can use these files
// some don't. So beware when using this function.
//
// ------------------------------------------------

void sys_FileExists(PA_PluginParameters params){
	char *file = NULL;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile;
	BOOL ret = FALSE; // assume file doesn't exist  // WJF 6/30/16 Win-21 LONG_PTR -> BOOL

	file = getTextParameter(params, 1);

	if (NULL != file){
		hFindFile = FindFirstFile(file, &FindFileData);

		// if a valid handle we should test whether it's not a directory
		if (INVALID_HANDLE_VALUE != hFindFile){
			ret = (0 == (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)); // set exists
			FindClose(hFindFile);	// close handle
		}

		freeTextParameter(file);
	}

	PA_ReturnLong(params, ret);
}

// ------------------------------------------------
//
//  FUNCTION: sys_DirectoryExists( PA_PluginParameters params)
//
//  PURPOSE:	Checks whether a directory exits
//
//	DATE:		2006/07/21 by Mark de Wever #12225
//
//  0 directory doesn't exist (or error)
//  1 directory exists
//  See notes at sys_FileExists
//
// ------------------------------------------------

void sys_DirectoryExists(PA_PluginParameters params){
	char *directory = NULL;
	LONG_PTR length;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile;
	LONG ret = 0; // assume directory doesn't exist // WJF 6/30/16 Win-21 LONG_PTR -> LONG

	// retrieve the directory manual
	length = PA_GetTextParameter(params, 1, 0L);

	if (length > 0) {
		directory = malloc(length + 1); // WJF 7/13/16 Win-21 Removed typecasting on malloc to follow C best practices

		if (directory != NULL) {
			memset(directory, 0, length + 1);
			PA_GetTextParameter(params, 1, directory);
		}
	}

	// if the directory ends with a backslash it will fail and needs to be removed
	// if the directory is a disk root it will fail handle here in special case
	if (NULL != directory){
		if ((':' == directory[1]) && (length <= 3)) { // REB 8/2/10 #24474 Changed ( 3 == length) to (length <= 3)
			// drive since has only 3 chars, handle here
			ret = GetFileAttributes(directory);
			if (ret != INVALID_FILE_ATTRIBUTES){ // REB 8/2/10 #24474 Added check for invalid attributes.
				ret = (0 != (GetFileAttributes(directory) & FILE_ATTRIBUTE_DIRECTORY)); // set exists
				free(directory);
				directory = NULL; // we have a result so avoid to normal handler
			}
		}
		else if ('\\' == directory[length - 1]){
			directory[length - 1] = '\0';
		}
	}

	if (NULL != directory){
		hFindFile = FindFirstFile(directory, &FindFileData);

		// if a valid handle we should test whether it's a directory
		if (INVALID_HANDLE_VALUE != hFindFile){
			ret = (0 != (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)); // set exists
			FindClose(hFindFile);	// close handle
		}
		freeTextParameter(directory);
	}

	PA_ReturnLong(params, ret);
}