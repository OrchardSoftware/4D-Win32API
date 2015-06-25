
#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include <shlwAPI.h>
#include <commctrl.h>
#include <tchar.h>
#include <time.h> 
#include <sys/timeb.h> 

#include "utilities.h"
#include "PrivateTypes.h"
#include "EntryPoints.h"

WNDPROC				g_wpOrigMDIProc;
LONG_PTR					g_displayedTTId; // REB 3/30/11 #25290 Was UINT
extern char		g_methodText[255];
BOOL					g_bTriggerMethod;
//LONG_PTR					g_exitExtProc = 0;
extern BOOL		g_bDragFull;
extern LONG_PTR		sIsPriorTo67;

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


extern struct		HOOKHANDLES
{
	HHOOK		openSaveHookHndl;
	HHOOK		printSettingsHookHndl;
	HHOOK		printHookHndl;
	HHOOK		postProcHookHndl;
	HHOOK		systemMsgHook;
} hookHandles;

extern struct		ACTIVECALLS
{
	BOOL		bPrinterCapture;
	BOOL		bTrayIcons;
	BOOL		b4DMaximize; //01/21/03
} activeCalls;

// ------------------------------------------------
// 
//  FUNCTION: sys_FileCheck( PA_PluginParameters params )
//
//  PURPOSE:	Checks to see if a file exists or is already open.  It expects to find the file.
//						Checks by trying to create file.  If file is created, it is deleted.
//
//  COMMENTS:	returns error code to 4D
//						PARAMS
//						1. complete text path & filename
//
//	DATE:			dcc 01/08/02

void sys_FileCheck( PA_PluginParameters params )
{
	HANDLE				hFile;
	char					fileName[MAX_PATH];
	LONG_PTR					fileName_len;
	DWORD					lastError;

	fileName_len	= PA_GetTextParameter( params, 1, fileName );
	fileName[fileName_len] = '\0';

	hFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 
								FILE_FLAG_DELETE_ON_CLOSE, NULL);
	lastError = GetLastError();
	if (hFile != INVALID_HANDLE_VALUE) { //close it if we created it.
		CloseHandle(hFile);
		DeleteFile(fileName);
	}

	PA_ReturnLong( params, lastError );
}


// ------------------------------------------------
// 
//  FUNCTION: gui_ToolTipCreate( PA_PluginParameters params )
//
//  PURPOSE:	Creates tool tip control that houses one or more tool tips
//
//  COMMENTS:	Requires variables 
//						1. style (LONG_PTR) TT_BALLOON or TT_RECTANGLE;
//							default is balloon  
//						2. option window handle for window to get tip 
//
//	DATE:			dcc 12/17/01 
//
void gui_ToolTipCreate( PA_PluginParameters params )
{
	LONG_PTR									style = TT_BALLOON, returnValue = 0;
	LONG_PTR									uId = 0;
	LONG_PTR*									puId = &uId;
	INITCOMMONCONTROLSEX	iccex;
	HWND									hwndTT, hwndTarget;
	HINSTANCE							hAppInst;
	DWORD									dwStyle = WS_POPUP | TTS_ALWAYSTIP;

	if(GetDllVersion(TEXT("Comctl32.dll")) < PACKVERSION(4,70)) {
   	PA_ReturnLong( params, 0 );
		return;
	}

	style				= PA_GetLongParameter( params, 1 ); 
	hwndTarget	= (HWND)PA_GetLongParameter( params, 2 ); // optionally supplied if tool tip 
						// to be associated with window not having focus

	if (hwndTarget == 0) {
		hwndTarget = (HWND)PA_GetHWND(PA_GetWindowFocused());
	} else if (!IsWindow(hwndTarget)) {
		PA_ReturnLong( params, 0 );
		return;
	}

	if (windowHandles.hwndTT == 0) {
		//activeCalls.bToolTips = TRUE; 01/21/03
		iccex.dwICC		= ICC_WIN95_CLASSES;
		iccex.dwSize	= sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&iccex);
		hAppInst = GetModuleHandle(NULL);
	
		if (style == 0) {
			dwStyle |= TTS_BALLOON; 
		}

		hwndTT = CreateWindow(TOOLTIPS_CLASS, TEXT(""), dwStyle,
					0, 0, 0, 0, hwndTarget, NULL, hAppInst, NULL);
		//set global for callback hook procedure
		windowHandles.hwndTT = hwndTT;
		// set global message hook
		hookHandles.systemMsgHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc,
				(HINSTANCE) NULL, GetCurrentThreadId());
		if (hookHandles.systemMsgHook == (HHOOK) NULL) {
			PA_ReturnLong( params, 0 );
			return;
		} else {
			returnValue = 1;
		}
	}

	PA_ReturnLong( params, returnValue );
}


// ------------------------------------------------
// 
//  FUNCTION: gui_ToolTipShowOnObject( PA_PluginParameters params )
//
//  PURPOSE:	Displays tool tip on an object (window, control, etc
//
//  COMMENTS:	Requires variables 
//						1.	Application supplied ID number - should be target window handle
//								if window without focus is to get tip
//						2.	Tip Text 
//						3.	Constant indicating location on object 
//						4.	Constant indicating to close on click or not
//						5.	Title text
//						6.	Method name to execute on click
//						7.	Left coordinate of object (if coordinates not supplied
//									the plugin expects that predefined variables named TT_Left,
//									TT_TOP, etc have been defined in 4D and used in a 
//									Get Object Rect call 
//						8.	Top coordinate
//						9.	Right coordinate
//						10. Bottom coordinate
//						11. message box width -- forces word wrapping
//        
//	DATE:			dcc 12/17/01 
//
void gui_ToolTipShowOnObject( PA_PluginParameters params )
{
	LONG_PTR									cx = 0, cy = 0, returnValue = 0, location = 0, freeStack = 0;
	HWND									hwndTarget;
	TOOLINFO							toolInfo;
	LONG_PTR									uId = 0, howToClose = 0, sendMsgReturn;
	LONG_PTR*									puId = &uId;
	char									paramMessage[255], title[255], method[80]; // REB 8/1/08 #17556 Increased size of title from 40 to 255.
	LPTSTR								lpTitle = title;
	LONG_PTR									paramMessage_len	= strlen(paramMessage);
	LONG_PTR									title_len					= strlen(title);
	LONG_PTR									method_len				= strlen(method);
	LPTSTR								lpParamMessage		= paramMessage;
	RECT									rect;
	LONG_PTR									left, top, right, bottom, balloonWidth, icon = 0, captionHeight, frameHeight;
	PA_Variable						PALeft, PATop, PARight, PABottom;

	if ((sIsPriorTo67)) { // does not work with 6.5 plugin
		PA_ReturnLong( params,  -1 );
		return;
	}
	
	g_bTriggerMethod = FALSE;

	uId								= PA_GetLongParameter( params, 1 );
	paramMessage_len	= PA_GetTextParameter( params, 2, paramMessage );
	paramMessage[paramMessage_len] = '\0';
	location					= PA_GetLongParameter( params, 3 );
	howToClose				= PA_GetLongParameter( params, 4 );
	title_len					= PA_GetTextParameter( params, 5, title ); 
	title[title_len] = '\0';
	method_len				= PA_GetTextParameter( params, 6, method );
	method[method_len] = '\0';
	left							= PA_GetLongParameter( params, 7 );
	top								= PA_GetLongParameter( params, 8 );
	right							= PA_GetLongParameter( params, 9 );
	bottom						= PA_GetLongParameter( params, 10 );
	balloonWidth			= PA_GetLongParameter( params, 11 ); // ignore setting if zero

	if ((left == 0) && (top == 0) && (right == 0) && (bottom == 0)) {
		//check for process variables used in Get Object Rect in4D
		PALeft		= PA_GetVariable("TT_LEFT");
		PATop			= PA_GetVariable("TT_TOP");
		PARight		= PA_GetVariable("TT_RIGHT");
		PABottom	= PA_GetVariable("TT_BOTTOM");
	
		left		= PA_GetLongintVariable(PALeft);
		top			= PA_GetLongintVariable(PATop);
		right		= PA_GetLongintVariable(PARight);
		bottom	= PA_GetLongintVariable(PABottom);
	}

	if ((left == 0) && (top == 0) && (right == 0) && (bottom == 0)) {
		PA_ReturnLong( params, 0 );
		return;
	}
	if (uId <= 500) {  //uId's that are less than 500 it is assumed target
			// window is window with focus
		hwndTarget = (HWND)PA_GetHWND(PA_GetWindowFocused());
	} else {
		hwndTarget = (HWND)uId;
	}
	toolInfo.cbSize		= sizeof(toolInfo);
	toolInfo.uFlags		= TTF_ABSOLUTE | TTF_TRACK; 
	toolInfo.hwnd			= hwndTarget;
	toolInfo.uId			= uId;
	toolInfo.hinst		= 0;
	toolInfo.lpszText = lpParamMessage;
	GetWindowRect(hwndTarget, &rect);

	captionHeight = GetSystemMetrics( SM_CYCAPTION );
	frameHeight = GetSystemMetrics(SM_CYFIXEDFRAME);

	switch (location)
	{
		case TT_TOPRIGHT :
			cx = rect.left	+ right;
			cy = frameHeight + captionHeight + rect.top	+ top;
			break;
		case TT_TOPLEFT :
			cx = rect.left	+ left;
			cy = frameHeight + captionHeight + rect.top	+ top;
			break;
		case TT_BOTTOMRIGHT :
			cx = rect.left	+ right;
			cy = frameHeight + captionHeight + rect.top	+ bottom;
			break;
		case TT_BOTTOMLEFT :
			cx = rect.left	+ left;
			cy = frameHeight + captionHeight + rect.top	+ bottom;			
			break;
		default :
			cx = rect.left + left + ((right - left) / 2);
			cy = frameHeight + captionHeight + rect.top + top + ((bottom	- top)  / 2);
			break;
	}
	if (title_len != 0) {
		switch (title[0]) // leading 1, 2, 0r 3 causes addition of icon
		{
			case '1' :
				lpTitle = &title[1];
				if (*lpTitle != '\0') {
					strcpy(title, lpTitle); // remove the number indicating the icon
					lpTitle = title;
					icon = NIIF_INFO;
				}
				break;
				
			case '2' :
				lpTitle = &title[1];
				if (*lpTitle != '\0') {
					strcpy(title, lpTitle);
					lpTitle = title;
					icon = NIIF_WARNING;
			}
				break;
				
			case '3' :
				lpTitle = &title[1];
				if (*lpTitle != '\0') {
					strcpy(title, lpTitle);
					lpTitle = title;
					icon = NIIF_ERROR;
				}
				break;
			default :
				icon = NIIF_NONE;
		}
	}
	if (howToClose == TT_CLOSE_ON_CLICK) {
		windowHandles.displayedTTOwnerhwnd = hwndTarget;
		if (method_len != 0) {
			strcpy(g_methodText, method); 
			returnValue = PA_NewProcess(createNewProcess, 1024 * 32, "$New_Process");
		}
	} else {
		windowHandles.displayedTTOwnerhwnd = NULL;
	}
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_ADDTOOL, 0, (LPARAM) &toolInfo);
	if (balloonWidth != 0) {
		SendMessage(windowHandles.hwndTT, TTM_SETMAXTIPWIDTH, 0, balloonWidth);
	}

	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_TRACKPOSITION, (WPARAM)0, (LPARAM) (DWORD) MAKELONG(cx, cy));
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_SETTITLE, (WPARAM)icon, (LPARAM)(LPCTSTR)lpTitle);
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM) &toolInfo);
	g_displayedTTId = uId;
	
	PA_ReturnLong( params, sendMsgReturn );

}

// ------------------------------------------------
// 
//  FUNCTION: gui_ToolTipShowOnCoord( PA_PluginParameters params )
//
//  PURPOSE:	Displays tool tip at specific coordinates
//
//  COMMENTS:	Requires variables 
//						1.	Application supplied ID number - should be target window handle
//								if window without focus is to get tip
//						2.	Tip Text 
//						3.	Constant indicating horizontal position
//						4.	Constant indicating vertical position
//						5.  Constant indicating to close on click or not
//						6.	Title text
//						7.	Method name to execute on click
//						8.	Balloon width 
//        
//	DATE:			dcc 12/17/01 
//
void gui_ToolTipShowOnCoord( PA_PluginParameters params )
{
	LONG_PTR									cx = 0, cy = 0, returnValue = 0;
	HWND									hwndTarget;
	TOOLINFO							toolInfo;
	LONG_PTR									uId = 0, howToClose = 0, sendMsgReturn;
	LONG_PTR*									puId = &uId;
	char									paramMessage[255], title[40], method[80];
	LPTSTR								lpTitle = title;
	LONG_PTR									paramMessage_len	= strlen(paramMessage);
	LONG_PTR									title_len					= strlen(title);
	LONG_PTR									method_len				= strlen(method);
	LPTSTR								lpParamMessage		= paramMessage;
	LONG_PTR									balloonWidth, icon = 0;

	g_bTriggerMethod = FALSE;

	uId									= PA_GetLongParameter( params, 1 );
	paramMessage_len		= PA_GetTextParameter( params, 2, paramMessage );
	paramMessage[paramMessage_len] = '\0';
	cx									= PA_GetLongParameter( params, 3 );
	cy									= PA_GetLongParameter( params, 4 );
	howToClose					= PA_GetLongParameter( params, 5 );
	title_len						= PA_GetTextParameter( params, 6, title ); 
	title[title_len]		= '\0';
	method_len					= PA_GetTextParameter( params, 7, method );
	method[method_len]	= '\0';
	balloonWidth				= PA_GetLongParameter( params, 8 ); // ignore setting if zero

	hwndTarget = (HWND)PA_GetHWND(PA_GetWindowFocused());

	toolInfo.cbSize		= sizeof(toolInfo);
	toolInfo.uFlags		= TTF_ABSOLUTE | TTF_TRACK; 
	toolInfo.hwnd			= hwndTarget;
	toolInfo.uId			= uId;
	toolInfo.hinst		= 0;
	toolInfo.lpszText = lpParamMessage;
	
	if (title_len != 0) {
		switch (title[0]) // leading 1, 2, 0r 3 causes addition of icon
		{
			case '1' :
				lpTitle = &title[1];
				if (*lpTitle != '\0') {
					strcpy(title, lpTitle); // remove the number indicating the icon
					lpTitle = title;
					icon = NIIF_INFO;
				}
				break;
				
			case '2' :
				lpTitle = &title[1];
				if (*lpTitle != '\0') {
					strcpy(title, lpTitle);
					lpTitle = title;
					icon = NIIF_WARNING;
			}
				break;
				
			case '3' :
				lpTitle = &title[1];
				if (*lpTitle != '\0') {
					strcpy(title, lpTitle);
					lpTitle = title;
					icon = NIIF_ERROR;
				}
				break;
			default :
				icon = NIIF_NONE;
		}
	}
	if (howToClose == TT_CLOSE_ON_CLICK) {
		windowHandles.displayedTTOwnerhwnd = hwndTarget;
		if (method_len != 0) {
			strcpy(g_methodText, method);
			returnValue = PA_NewProcess(createNewProcess, 1024 * 32, "$New_Process");
		}
	} else {
		windowHandles.displayedTTOwnerhwnd = NULL;
	}
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_ADDTOOL, 0, (LPARAM) &toolInfo);
	if (balloonWidth != 0) {
		SendMessage(windowHandles.hwndTT, TTM_SETMAXTIPWIDTH, 0, balloonWidth);
	}
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_TRACKPOSITION, (WPARAM)0, (LPARAM) (DWORD) MAKELONG(cx, cy));
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_SETTITLE, (WPARAM)icon, (LPARAM)(LPCTSTR)lpTitle);
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM) &toolInfo);
	g_displayedTTId = uId;
		
	PA_ReturnLong( params, sendMsgReturn );

}

// ------------------------------------------------
// 
//  FUNCTION: gui_ToolTipHide( PA_PluginParameters params )
//
//  PURPOSE:	Hides tool tip
//
//  COMMENTS:	Requires variables 
//						1.	Application supplied ID number - should be target window handle
//								if window without focus is to get tip
//
//	DATE:			dcc 12/17/01 
//

void gui_ToolTipHide( PA_PluginParameters params )
{
	LONG_PTR									returnValue = 0;
	HWND									hwndTarget;
	TOOLINFO							toolInfo;
	LONG_PTR									uId = 0, sendMsgReturn;
	LONG_PTR*									puId = &uId;
	char									ttMessage[255];
	LPTSTR								lpttMessage	= ttMessage;

	uId								= PA_GetLongParameter( params, 1 );
	
	hwndTarget = (HWND)PA_GetHWND(PA_GetWindowFocused());

	toolInfo.cbSize		= sizeof(toolInfo);
	toolInfo.hwnd			= hwndTarget;
	toolInfo.uId			= uId;
	toolInfo.hinst		= 0;
	toolInfo.lpszText = lpttMessage;
	strcpy(g_methodText, "");
	
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM) &toolInfo);
	returnValue		= SendMessage(windowHandles.hwndTT, TTM_GETTOOLINFO, 0, (LPARAM) &toolInfo);

	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_POP, 0, 0);
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM) &toolInfo);
	sendMsgReturn = SendMessage(windowHandles.hwndTT, TTM_DELTOOL, 0, (LPARAM) &toolInfo);
	windowHandles.displayedTTOwnerhwnd = NULL;

	PA_ReturnLong( params, sendMsgReturn );
}

// ------------------------------------------------
// 
//  FUNCTION: gui_ToolTipDestroyControl( PA_PluginParameters params )
//
//  PURPOSE:	Destroys ToolTip control
//
//  COMMENTS:	Requires variables 
//						1.	Application supplied ID number 
//						Use this when application will no longer be displaying tool tips
//
//	DATE:			dcc 12/17/01 
//

void gui_ToolTipDestroyControl( PA_PluginParameters params )
{
	LONG_PTR			returnValue = 0;

	if (hookHandles.systemMsgHook != NULL) {
		windowHandles.hwndTT = NULL;
		windowHandles.displayedTTOwnerhwnd = NULL;
		strcpy(g_methodText, ""); 
		g_displayedTTId = 0;
		//activeCalls.bToolTips = FALSE; 01/21/03
		UnhookWindowsHookEx(hookHandles.systemMsgHook);
		returnValue = 1;
	}
	
	PA_ReturnLong( params, returnValue );
}


// ------------------------------------------------
// 
//  FUNCTION: sys_SetClientDate( PA_PluginParameters params )
//
//  PURPOSE:	Sets system clock date for workstation to date passed in as param
//
//  COMMENTS:	Function returns immediately if not run on client.  Also returns -1 if a time provider is
//							enabled on the client, i.e., running XP on an XP domain.
//						
//        
//	DATE:			dcc 11/30/01 
//
//  MODIFICATIONS:  05/29/02 added second param to force date change even with XP time provider.
//									XP will again alter this but it may be used to get date changed immediately
//										without waiting fo domain server to take action.
//									09/23/02 changed from system time to local time functions and corrected 
//										if statement before SetLocalTime

void sys_SetClientDate( PA_PluginParameters params )
{
	LONG_PTR					returnValue = 0, action = 0;
	WORD					day, year, month;
	SYSTEMTIME		st;
	
	if (PA_Is4DClient()) {
		action = PA_GetLongParameter(params, 2);
		if (checkTimeProvider()) {
			returnValue = -1; // -1 indicates that a time provider is enabled
		}
		if (((action == 0) && (returnValue == 0)) || ((returnValue == -1) && (action > 0))) { 
			PA_GetDateParameter( params, 1, &day, &month, &year );
			//GetSystemTime(&st);
			GetLocalTime(&st);
			st.wMonth = month;
			st.wDay = day;
			st.wYear = year;
			if ((returnValue != -1) || ((returnValue == -1) && (action > 0))) {
				//returnValue = SetSystemTime(&st);
				returnValue = SetLocalTime(&st);
			}
		}
	} else {
		returnValue = -2;
	}

	PA_ReturnLong( params, returnValue );
}

// ------------------------------------------------
// 
//  FUNCTION: sys_SetClientTime( PA_PluginParameters params )
//
//  PURPOSE:	Sets system clock for workstation to time passed in as param
//
//  COMMENTS:	Function returns immediately if not run on client.
//						
//        
//	DATE:			dcc 11/30/01 
//
//  MODIFICATIONS:  05/29/02 added second param to force date change even with XP time provider.
//									XP will again alter this but it may be used to get date changed immediately
//										without waiting fo domain server to take action.
//									09/23/02 changed from system time to local time functions

void sys_SetClientTime( PA_PluginParameters params )
{
	LONG_PTR					serverTime, returnValue = 0, action = 0;
	WORD					hour, minute;
	SYSTEMTIME		st;
	//struct _timeb tstruct;
	
	//if (PA_Is4DClient()) {
		action = PA_GetLongParameter(params, 2);
		if (checkTimeProvider()) { 
			// -1 indicates that a time provider is enabled
			returnValue = -1;
		}
		if (((action == 0) && (returnValue == 0)) || ((returnValue == -1) && (action > 0))) { 
			serverTime = PA_GetLongParameter( params, 1 );
			//_ftime( &tstruct ); // adjust for UTC offset
			//serverTime = serverTime + (tstruct.timezone * 60);
			//GetSystemTime(&st);
			GetLocalTime(&st);
			hour = (serverTime / 3600);
			minute = (serverTime - (hour * 3600)) / 60;
			st.wHour = hour;
			st.wMinute = minute;
			if ((returnValue != -1) || ((returnValue == -1) && (action > 0))){
				//returnValue = SetSystemTime(&st);
				returnValue = SetLocalTime(&st);
			}
		}
	//} else {
		//returnValue = -2;
	//}

	PA_ReturnLong( params, returnValue );
}



// ------------------------------------------------
// 
//  FUNCTION: gui_LoadBackground( PA_PluginParameters params )
//
//  PURPOSE:	Loads a bitmap as a background for 4D window.
//
//  COMMENTS:	May be scaled or tiled.  Bitmap must be of suffient size
//				for scaling so it does not appear blocky.  Tiled Bitmaps
//				should have edges that transition nicely from one to another.
//        
//	DATE:		dcc 11/09/01 
//
void gui_LoadBackground( PA_PluginParameters params, BOOL DeInit )
{
	LONG_PTR						bmpName_len;
	char						bmpName[255];  //complete path of bmp file
	static char			lastBmpName[255];
	LONG_PTR						returnValue = 0, tileOrScale = 0;
	static LONG_PTR			lastTileOrScale = 0;
	BOOL						bFuncReturn;
	HBITMAP					hBitmap;
	WPARAM					wParam = 0;
	LPARAM					lParam = 0;
	static HWND			hWnd = 0;
	RECT						rect;
	char						windowTitle[] =  "MDI"; 
	PA_Unistring		Unistring;
	char				*pathName, *charPos;
	HWND NexthWnd;
	char			WindowName[255];
	char			szClassName[255];

	//EngineBlock			Blk4D;


	
	if (DeInit) {
		if (hWnd != 0) {
			bFuncReturn = SendNotifyMessage(hWnd, (WM_USER + 0x0031), wParam, lParam);
		}
		return;
	}
	bmpName_len = PA_GetTextParameter( params, 1, bmpName );
	bmpName[bmpName_len] = '\0';  // Explicitly set the length
	
	if ((hWnd == 0) && (bmpName_len == 0))  { //if there's no bitmap provided and we don't have one loaded, return & clear
		PA_ReturnLong( params, 0 );
		hWnd = NULL;
		strcpy(lastBmpName, "");
		lastTileOrScale = 0;
		return;
	}
	
	tileOrScale = PA_GetLongParameter( params, 2 );
	switch (tileOrScale)
	{ 
	case 0 :  // assume tiled if no param
		tileOrScale = BM_TILE;
		if (g_bDragFull) { // reset to original style if changing from scaled to tiled
			SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, g_bDragFull, NULL, 0);	
		}
		break;
	case BM_SCALE :
		// set so window NOT repainted until AFTER resized -- not during resizing
		// bFuncReturn = SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0); // 1/8/04 no longer change the system preference.
		break;
	case BM_SCALETOMAXCLIENT :
		// this causes bitmap to be scaled to maximum client size
		break;
	}

	if (hWnd != 0) { //if a background has been displayed
		// and its the same bitmap and format as this call
		// don't do it again
		if ((strcmp(bmpName, lastBmpName) == 0) && (tileOrScale == lastTileOrScale)) { //this is the same bitmap as before 
			PA_ReturnLong( params, 1 );  
			return;
		}
		// if we're here it's a different bitmap or format
		// release current background
		bFuncReturn = SendNotifyMessage(hWnd, (WM_USER + 0x0031), wParam, lParam);
		bFuncReturn = GetClientRect(hWnd, &rect);
		bFuncReturn = InvalidateRect(hWnd, &rect, TRUE);
		if (bmpName_len == 0)  { //if there's no bitmap provided, return & clear
			PA_ReturnLong( params, 1 );
			hWnd = NULL;
			strcpy(lastBmpName, "");
			lastTileOrScale = 0;
			return;
		}
	} 

	if (windowHandles.MDIhWnd == NULL) {
		//Blk4D.fHandle = NULL; // 09/09/02
		//Call4D (EX_GET_HWND, &Blk4D);
		//hWnd = (HWND)Blk4D.fHandle;

		// REB 4/20/11 #27322
		//if(PA_Is4DServer()){
		//	Unistring = PA_GetApplicationFullPath();
		//	pathName = UnistringToCString(&Unistring); 
		//	charPos = strrchr(pathName,'\\');
		//	*charPos = 0;
		//	hWnd = FindWindowEx(NULL, NULL, pathName, NULL);
		//}else{
		//	hWnd = PA_GetHWND(NULL); // the current frontmost window
		//}
		////hWnd = (HWND)PA_GetHWND(0); //09/09/02
		//hWnd = getWindowHandle(windowTitle, hWnd);

		// AMS 4/29/14 #39196 Replaced the code above. It seems as if PA_GetHWND(0) is no longer working correctly in v14. 
		Unistring = PA_GetApplicationFullPath();
		pathName = UnistringToCString(&Unistring);
		charPos = strrchr(pathName, '\\');
		*charPos = 0;
		windowHandles.fourDhWnd = FindWindowEx(NULL, NULL, pathName, NULL);

		free(pathName); // WJF 6/25/15 #42792

		NexthWnd = GetWindow(windowHandles.fourDhWnd, GW_CHILD);
		do {
			if (IsWindow(NexthWnd)){
				GetWindowText(NexthWnd, WindowName, 255);
				GetClassName(NexthWnd, szClassName, 255);
				if (strcmp(_strlwr(szClassName), "mdiclient") == 0){
					windowHandles.MDIs_4DhWnd = NexthWnd;
					break;
				}
				NexthWnd = GetNextWindow(NexthWnd, GW_HWNDNEXT);
			}
		} while (IsWindow(NexthWnd));

		hWnd = windowHandles.MDIs_4DhWnd;

	} else {
		hWnd = windowHandles.MDIhWnd;
	}
	
	hBitmap = (HBITMAP)LoadImage(0,bmpName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (hBitmap == NULL) {
		returnValue = 0;
	} else {
		lParam = (LPARAM) hBitmap;
		//returnValue = lParam;
		returnValue = 1;
		wParam = tileOrScale;
		// REB 3/18/11 #25290
		(LONG_PTR)g_wpOrigMDIProc = SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR) BkgrndProc);
		//g_wpOrigMDIProc = (WNDPROC) SetWindowLong(hWnd, GWL_WNDPROC, (LONG) BkgrndProc);
		if (g_wpOrigMDIProc != 0) {
			bFuncReturn = SendNotifyMessage(hWnd, (WM_USER + 0x0030), wParam, lParam);
			bFuncReturn = GetClientRect(hWnd, &rect);
			bFuncReturn = InvalidateRect(hWnd, &rect, TRUE);
			strcpy(lastBmpName, bmpName);
			lastTileOrScale = tileOrScale;
		} else {
			returnValue = 0;
		}
	}
	PA_ReturnLong( params, returnValue );
}



// ------------------------------------------------
// 
//  FUNCTION: sys_GetWindowMetrics( PA_PluginParameters params)
//
//  PURPOSE:	Get various metrics for a window
//
//  COMMENTS:	Returns zero if failed to get metric value

//        
//	DATE:			dcc 11/09/01 
//

void sys_GetWindowMetrics( PA_PluginParameters params )
{
	LONG_PTR						metricRequest, metricValue;

	metricRequest = PA_GetLongParameter(params, 1);
	metricValue = GetSystemMetrics( (INT_PTR) metricRequest );

	PA_ReturnLong( params, (LONG_PTR) metricValue );

}


// 
//  FUNCTION: gui_FlashWindow( PA_PluginParameters params )
//
//  PURPOSE:	Flashes a window or tray window 
//
//  COMMENTS:	Can flash a number of times or until sending a stop command etc.
//						
//
//	DATE:			dcc 08/04/01 
// 
void gui_FlashWindow( PA_PluginParameters params)
{
	HWND					hWnd;
	DWORD					flags;
	LONG_PTR					returnValue = 0, flashCount, flashRate;
	FLASHWINFO		fwi;
	PFLASHWINFO		pfwi = &fwi;
	LPFNDLLFUNC1	lpfnDllFunc1;
	HINSTANCE			hDLL;

	hWnd = (HWND)PA_GetLongParameter( params, 1 );
	if (IsWindow(hWnd)) {
		flags = PA_GetLongParameter( params, 2 );
		flashCount = PA_GetLongParameter( params, 3 );
		flashRate = PA_GetLongParameter( params, 4 ); // in milliseconds. Zero uses cursor blink rate

		// Only the new User32.dll exposes the FlashWindowEx function.
		// must test for it and if not available, use FlashWindow
		hDLL = LoadLibrary("User32.dll");

		if (flags & FLASHW_TIMER) {
			flashCount = 0; // override count param if time being used
		}

		if (hDLL != NULL) {
			lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, "FlashWindowEx");

			if (lpfnDllFunc1) {
				fwi.cbSize		= sizeof(fwi);
				fwi.hwnd			= hWnd;
				fwi.dwFlags		= flags;
				fwi.uCount		= flashCount;
				fwi.dwTimeout = flashRate;
				returnValue		= lpfnDllFunc1(pfwi);
				FreeLibrary(hDLL);
			} else {
				returnValue = FlashWindow(hWnd, flags);
			}
	
		} // end if (hDLL != NULL)
	} // end if (IsWindow(hWnd))
	PA_ReturnLong( params, returnValue );

}

// 
//  FUNCTION: sys_PlayWav( PA_PluginParameters params )
//
//  PURPOSE:	play a wave file 
//
//  COMMENTS:	must pass in a fully qualified file name with path
//						
//
//	DATE:			dcc 10/16/01 
// 
void sys_PlayWav( PA_PluginParameters params )
{
	char			fileName[MAX_PATH];
	LONG_PTR			fileName_len, flag;
	BOOL			bFuncReturn;

	fileName_len = PA_GetTextParameter( params, 1, fileName );
	fileName[fileName_len] = '\0';
	flag = PA_GetLongParameter( params, 2);
	if (flag > 0) {
		MessageBeep(flag);
		PA_ReturnLong( params, flag);
	} else {
		bFuncReturn = sndPlaySound(NULL, SND_ASYNC); // stop the sound that's is playing
		if (sndPlaySound(fileName, SND_ASYNC)) {
			PA_ReturnLong( params, 1 );
		} else {
			PA_ReturnLong (params, 0);
		}
	}
}

//  FUNCTION: scaleImage(HDC hdc, HDC hdcMem, HBITMAP hOrigBitmap, INT_PTR width, INT_PTR height)
//
//  PURPOSE:  Scale a bitmap to the desired dimensions.
//
//  COMMENTS:	
//
//	DATE: MJG 12/19/03
//
HGDIOBJ scaleImage(HDC hdc, HDC hdcMem, HBITMAP hOrigBitmap, INT_PTR width, INT_PTR height)
{
	BITMAP origBitmap, scaledBitmap;
	HBITMAP hScaledBitmap;
	HDC	hdcOrigBitmap;
	HGDIOBJ returnObject;

	GetObject(hOrigBitmap, sizeof(BITMAP), &origBitmap);
			
	hdcOrigBitmap = CreateCompatibleDC (hdc);

	scaledBitmap = origBitmap;		
	scaledBitmap.bmWidth = width;
	scaledBitmap.bmHeight = height;
	scaledBitmap.bmWidthBytes = ((scaledBitmap.bmWidth + 15) / 16) * 2;
				
	hScaledBitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject (hdcOrigBitmap, hOrigBitmap);
	returnObject = SelectObject (hdcMem, hScaledBitmap);
				
	StretchBlt(hdcMem, 0, 0, width, height, hdcOrigBitmap, 0, 0, origBitmap.bmWidth, origBitmap.bmHeight, SRCCOPY);
	DeleteDC (hdcOrigBitmap);
				
	return returnObject;
}

//  FUNCTION: tileImage(HDC hdc, HDC hdcMem, HBITMAP hOrigBitmap, INT_PTR width, INT_PTR height)
//
//  PURPOSE:  Tile a bitmap to the desired dimensions.
//
//  COMMENTS:	
//
//	DATE: MJG 12/19/03
//
HGDIOBJ tileImage(HDC hdc, HDC hdcMem, HBITMAP hOrigBitmap, INT_PTR width, INT_PTR height)
{
	BITMAP origBitmap;
	HBITMAP htiledBitmap;
	HDC	hdcOrigBitmap;
	HGDIOBJ returnObject;
	INT_PTR x,y;
		
	GetObject(hOrigBitmap, sizeof(BITMAP), &origBitmap);
		
	hdcOrigBitmap = CreateCompatibleDC (hdc);

	htiledBitmap = CreateCompatibleBitmap(hdc, width, height);

	SelectObject (hdcOrigBitmap, hOrigBitmap);
	returnObject = SelectObject (hdcMem, htiledBitmap);
		
	for (y = 0 ; y < height ; y += origBitmap.bmHeight)
		for (x = 0 ; x < width ; x += origBitmap.bmWidth)
			BitBlt (hdcMem, x, y, origBitmap.bmWidth, origBitmap.bmHeight, hdcOrigBitmap, 0, 0, SRCCOPY);
			
	DeleteDC (hdcOrigBitmap);
	return returnObject;

}

//  FUNCTION: BkgrndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//
//  PURPOSE:	subclassed window procedure
//
//  COMMENTS:	
//
//	DATE:			dcc 11/09/01
// 
//  MODIFICATIONS:  MJG 12/19/03 Improved efficiency when drawing background.
//
LRESULT APIENTRY BkgrndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	static INT_PTR cxClient, cyClient;
	static HBITMAP hOrigBitmap;
	static BITMAP origBitmap;
	static HDC hdcMem, hdc;
	static RECT	clientRect; //dcc 12/14
	static LONG_PTR	tileOrScale, fullClientWidth, fullClientHeight;
	static HGDIOBJ oldObject;
	PAINTSTRUCT	ps;
	RECT rect;
	INT_PTR x,y;
	
	switch (uMsg)
	{
	case (WM_USER + 0x0030) : // initial setup -- passing bitmap hndl and tileOrScale
		tileOrScale = wParam;
		hOrigBitmap = (HBITMAP) lParam;
		GetObject (hOrigBitmap, sizeof (BITMAP), &origBitmap);
		hdc = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdc); 

		switch(tileOrScale) {
		case BM_SCALE:
			GetClientRect(hwnd, &clientRect);
			fullClientWidth = clientRect.right;
			fullClientHeight = clientRect.bottom;
			oldObject = scaleImage(hdc, hdcMem, hOrigBitmap, fullClientWidth,fullClientHeight);
			break;
		case BM_SCALETOMAXCLIENT:
			fullClientWidth = GetSystemMetrics(SM_CXFULLSCREEN);
			fullClientHeight = GetSystemMetrics(SM_CYFULLSCREEN);
			oldObject = scaleImage(hdc, hdcMem, hOrigBitmap, fullClientWidth, fullClientHeight);
			break;
		case BM_TILE :
			fullClientWidth = GetSystemMetrics(SM_CXFULLSCREEN);
			fullClientHeight = GetSystemMetrics(SM_CYFULLSCREEN);
			oldObject = tileImage(hdc, hdcMem, hOrigBitmap, fullClientWidth, fullClientHeight);
			break;
		}
		break;
	
		
	case WM_SIZE :
		
		if(tileOrScale == BM_SCALE) {

			cxClient = LOWORD (lParam) ;
			cyClient = HIWORD (lParam) ;

			if (((fullClientWidth != cxClient) || (fullClientHeight != cyClient))
				&& (cxClient != 0 && cyClient != 0)) {

				fullClientWidth = cxClient;
				fullClientHeight = cyClient;
				DeleteObject(SelectObject(hdcMem, oldObject));
			    oldObject = scaleImage(hdc, hdcMem, hOrigBitmap, cxClient, cyClient);
				InvalidateRect(hwnd, &clientRect, FALSE);
				return 1;
			}
		} 

			
	case WM_PAINT:
			
		GetUpdateRect(hwnd, &rect, TRUE);
		cxClient = rect.right;
		cyClient = rect.bottom;
			
		BeginPaint (hwnd, &ps);

		if(cyClient < fullClientHeight && cxClient < fullClientWidth)
			BitBlt (hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
		else
			for (y = 0 ; y < cyClient ; y += fullClientHeight)
				for (x = 0 ; x < cxClient ; x += fullClientWidth)
				{
					BitBlt (hdc, x, y, fullClientWidth, fullClientHeight, hdcMem, 0, 0, SRCCOPY);
				}

		EndPaint (hwnd, &ps);
				
		return 1; // return immediately - don't let orig window proc repaint
			
		
	case (WM_USER + 0x0031) :

		ReleaseDC(hwnd, hdc);
		DeleteObject(SelectObject(hdcMem, oldObject));
		DeleteDC (hdcMem);
		DeleteObject (hOrigBitmap);
		tileOrScale = 0;
		fullClientWidth = 0;
		fullClientHeight = 0;
		cxClient = 0;
		cyClient = 0;
		// REB 3/18/11 #25290
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)g_wpOrigMDIProc);
		//SetWindowLong(hwnd, GWL_WNDPROC, (LONG) g_wpOrigMDIProc);
		break;
			
	}
	
	return CallWindowProc(g_wpOrigMDIProc, hwnd, uMsg, wParam, lParam);
	
}


DWORD GetDllVersion(LPCTSTR lpszDllName)
{
	HINSTANCE				hinstDll;
	DWORD						dwVersion = 0;
	DLLVERSIONINFO	dvi;
	HRESULT					hr;
	
	hinstDll = LoadLibrary(lpszDllName);
	if(hinstDll) {
		DLLGETVERSIONPROC pDllGetVersion; 
		pDllGetVersion = (DLLGETVERSIONPROC) GetProcAddress(hinstDll, "DllGetVersion");
		if(pDllGetVersion) { 

			ZeroMemory(&dvi, sizeof(dvi)); 
			dvi.cbSize = sizeof(dvi); 
			hr = (*pDllGetVersion)(&dvi); 
			if(SUCCEEDED(hr)) {
				dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
			}
		}
	  FreeLibrary(hinstDll);
	}
	return dwVersion;
}


LRESULT CALLBACK GetMsgProc(INT_PTR nCode, WPARAM wParam, LPARAM lParam)
{
	MSG					*pMsg;
	TOOLINFO		toolInfo;
	LONG_PTR				returnValue;
	char				ttMessage[80];
	char*				lpttMessage = ttMessage;
	pMsg = (MSG *) lParam;

	if (nCode < 0) {
		return (CallNextHookEx(hookHandles.systemMsgHook, nCode, wParam, lParam));
	}
	if (windowHandles.displayedTTOwnerhwnd != NULL) { // if not ON_CLICKED
		switch (pMsg->message)
		{
			case WM_LBUTTONDOWN :
			case WM_RBUTTONDOWN :
				if (windowHandles.hwndTT != NULL) {
					if ((windowHandles.displayedTTOwnerhwnd == pMsg->hwnd) ||
						(windowHandles.hwndTT == pMsg->hwnd)) {
						toolInfo.cbSize = sizeof(toolInfo);
						toolInfo.hwnd		= windowHandles.displayedTTOwnerhwnd;
						toolInfo.uId		= g_displayedTTId;
						toolInfo.lpszText = lpttMessage;
						SendMessage(windowHandles.hwndTT, TTM_GETTOOLINFO, 0, (LPARAM) &toolInfo);
						SendMessage(windowHandles.hwndTT, TTM_POP, 0, 0);
						returnValue = SendMessage(windowHandles.hwndTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM) &toolInfo);
						returnValue = SendMessage(windowHandles.hwndTT, TTM_DELTOOL, 0, (LPARAM) &toolInfo);
						windowHandles.displayedTTOwnerhwnd = NULL;
						g_displayedTTId = 0;
						g_bTriggerMethod = TRUE;
					}
				}
				break;
		
			default :
				break;
		} // end switch
	} //(windowHandles.displayedTTOwnerhwnd != NULL) 
	return (CallNextHookEx(hookHandles.systemMsgHook, nCode, wParam, lParam));

}

// 
//  FUNCTION: createNewProcess( void )
//
//  PURPOSE:	Starts a 4D plugin process to start a 4D process
//
//  COMMENTS:	
//
//	DATE:			dcc 12/17/01

VOID createNewProcess( VOID)
{
	LONG_PTR								theProcess;
	WPARAM							wParam;
	WPARAM*							pwParam = &wParam;
	LPARAM							lParam;
	LPARAM*							plParam = &lParam;
	BOOL								bDone = FALSE;
	PA_Unistring					Unistring;

	PA_YieldAbsolute();

	theProcess = PA_GetCurrentProcessNumber();

	//Main loop
	while(!bDone)
	{
	
		PA_Yield();
		PA_PutProcessToSleep(theProcess, 40);

		bDone = (BOOL)PA_IsProcessDying();
		if (g_bTriggerMethod) {
			bDone = TRUE;
			g_bTriggerMethod = FALSE;
		}
		if (bDone) {
			
			// REB 4/20/11 #27322 Conver the C string to a Unistring
			Unistring = CStringToUnistring(&g_methodText);
			PA_ExecuteMethod(&Unistring);
			//PA_ExecuteMethod(g_methodText, strlen(g_methodText));
		}

	}
	PA_KillProcess();
}

// 
//  FUNCTION: checkTimeProvider( void )
//
//  PURPOSE:	checks registry to see if there is an active time sync
//						like a domain server.
//
//  COMMENTS:	returns boolean
//
//	DATE:			dcc 01/04/02

BOOL checkTimeProvider( ) 
{
	char			subKey[100], subKey2[100];
	LONG_PTR			errorCode;
	HKEY			hKey1, hKey2, rootKey;
  char			chKey1[MAX_PATH];
	DWORD			chKey1_len = MAX_PATH;
  char			achClass[MAX_PATH] = "";  // buffer for class name 
  DWORD			cchClassName = MAX_PATH;  // length of class string 
  DWORD			cSubKeys;                 // number of subkeys 
  DWORD			cbMaxSubKey;              // longest subkey size 
  DWORD			cchMaxClass;              // longest class string 
  DWORD			cValues;              // number of values for key 
  DWORD			cchMaxValue;          // longest value name 
  DWORD			cbMaxValueData;       // longest value data 
  DWORD			cbSecurityDescriptor; // size of security descriptor 
  FILETIME	ftLastWriteTime;      // last write time 
  
	DWORD			i, j; 
  DWORD			retCode, retValue; 
	DWORD			chData;
	DWORD			chDataBuff_len = sizeof(DWORD);
  char			achValue[80]; 
  DWORD			cchValue = 80; 
  char			achBuff[80]; 
	BOOL			bEnabled = FALSE, bInputProvider = FALSE;
 
	strcpy ( subKey, "SYSTEM\\CurrentControlSet\\Services\\W32Time\\TimeProviders");

	rootKey = HKEY_LOCAL_MACHINE;
	hKey1 = HKEY_CURRENT_USER; // will get new handle of open key
	hKey2 = HKEY_CURRENT_USER;

	errorCode = RegOpenKeyEx ( rootKey, subKey, 0, KEY_READ, &hKey1 );
	if (errorCode != ERROR_SUCCESS) {
		return FALSE;
	}

  // Enumerate the child keys, until RegEnumKeyEx fails. Then 
  // get the name of each child key and copy it into the list box. 

  for (i = 0, retCode = ERROR_SUCCESS; 
          retCode == ERROR_SUCCESS; i++) 
  { 
     retCode = RegEnumKeyEx(hKey1, 
                  i, 
                  chKey1, 
                  &chKey1_len, 
                  NULL, 
                  NULL, 
                  NULL, 
                  &ftLastWriteTime); 
     if (retCode == (DWORD)ERROR_SUCCESS) { 
			chKey1_len = MAX_PATH;
     }
		strcpy(subKey2, subKey);
		strcat(subKey2, "\\");
		strcat(subKey2, chKey1);
		errorCode = RegOpenKeyEx ( rootKey, subKey2, 0, KEY_READ, &hKey2 );
		if (errorCode != ERROR_SUCCESS) {
			return FALSE;
		}

		// Get the class name and the value count. 
		RegQueryInfoKey(hKey2,        // key handle 
				achClass,                // buffer for class name 
		    &cchClassName,           // length of class string 
				NULL,                    // reserved 
				&cSubKeys,               // number of subkeys 
				&cbMaxSubKey,            // longest subkey size 
				&cchMaxClass,            // longest class string 
				&cValues,                // number of values for this key 
				&cchMaxValue,            // longest value name 
				&cbMaxValueData,         // longest value data 
				&cbSecurityDescriptor,   // security descriptor 
				&ftLastWriteTime);       // last write time 


		// Enumerate the key values. 
		if (cValues) {
	
			for (j = 0, retValue = ERROR_SUCCESS; 
				      j < cValues; j++) 
			{ 
				cchValue = 80; 
				achValue[0] = '\0'; 
				retValue = RegEnumValue(hKey2, j, achValue, 
					  &cchValue, 
						NULL, 
						NULL,						 	// &dwType, 
						(char*) &chData,	// &bData, 
						&chDataBuff_len); // &bcData 
				if ((strcmp(_strlwr(achValue), "enabled") == 0) && (chData == 1)) { 
					bEnabled = TRUE;
				}
				if ((strcmp(_strlwr(achValue), "inputprovider") == 0) && (chData == 1)) { 
					bInputProvider = TRUE;
				}
				achBuff[0] = '\0'; 
				if ((bEnabled) && (bInputProvider)) { // found a domain time provider so bail
					errorCode = RegCloseKey( hKey2 );
					return TRUE;
				}
			} // for (j = 0, retValue = ERROR_SUCCESS; j < cValues; j++) 
		} // if (cValues)
		errorCode = RegCloseKey( hKey2 );
	} // for (i = 0, retCode = ERROR_SUCCESS; retCode == ERROR_SUCCESS; i++) 
	errorCode = RegCloseKey( hKey1 );

	return FALSE;
}
