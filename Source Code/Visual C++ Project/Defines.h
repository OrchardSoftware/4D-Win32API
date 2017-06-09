// --- defines etc

typedef BOOL(CALLBACK* LPFNDLLFUNC1) (PFLASHWINFO);
typedef DWORD(CALLBACK* LPFNDLLFUNC2) (PFIXED_INFO, PULONG);
typedef DWORD(CALLBACK* LPFNDLLFUNC3) (PMIB_IPFORWARDTABLE, PULONG, BOOL);
typedef BOOL(CALLBACK* LPFNDLLFUNC4) (LPDWORD, DWORD);

//
typedef BOOL(CALLBACK* LPFNENUMPROC) (DWORD*, DWORD, DWORD*);
typedef BOOL(CALLBACK* LPFNENUMPROCMODS) (HANDLE, HMODULE*, DWORD, LPDWORD);
typedef DWORD(CALLBACK* LPFNGETMODFNAME) (HANDLE, HMODULE, char *, DWORD);
typedef HANDLE(CALLBACK* LPFNCREATETOOLHELPSNAP) (DWORD, DWORD);
typedef BOOL(CALLBACK* LPFNPROCFIRST) (HANDLE, VOID *);
typedef BOOL(CALLBACK* LPFNPROCNEXT) (HANDLE, VOID *);

// number of array elements for region settings
#ifndef NBR_ELEMENTS
#define NBR_ELEMENTS 18
#endif

#define WIN32API_VERSION 8.1.3
#define NUM_COMMANDS 138 // WJF 7/11/16 Win-20

// constants for open file dialog options
#define FD_OVERWRITE_PROMPT					2
#define FD_FILES_ONLY								4
#define FD_NO_CHANGE_DIR						8
#define FD_DISABLE_LOOKIN_FIELD			256
#define FD_HIDE_TOOLBAR_BUTTONS			512
#define FD_HIDE_NEWDIRECTORY_BUTTON 1024
#define FD_SELECT_DIRECTORY					2048
#define FD_FILE_MUST_EXIST					4096
#define FD_CREATE_PROMPT						8192
#define FD_DISABLE_EDIT_FIELD				16384
#define FD_HIDE_UP_BUTTON						32768

// openfile dialog control IDs
#define UPONELEVELBUTTON  40961
#define NEWBUTTONFOLDER		40962
#define LOOK_IN_COMBO			1137
#define EDIT_FIELD				1152
#define LAST_LISTVIEW_MSG 4146

// more string constants
// these are used in processCDError when uncommented for debugging
#define IDS_DIALOGFAILURE     1
#define IDS_STRUCTSIZE        2
#define IDS_INITIALIZATION    3
#define IDS_NOTEMPLATE        4
#define IDS_NOHINSTANCE       5
#define IDS_LOADSTRFAILURE    6
#define IDS_FINDRESFAILURE    7
#define IDS_LOADRESFAILURE    8
#define IDS_LOCKRESFAILURE    9
#define IDS_MEMALLOCFAILURE  10
#define IDS_MEMLOCKFAILURE   11
#define IDS_NOHOOK           12
#define IDS_SETUPFAILURE     13
#define IDS_PARSEFAILURE     14
#define IDS_RETDEFFAILURE    15
#define IDS_LOADDRVFAILURE   16
#define IDS_GETDEVMODEFAIL   17
#define IDS_INITFAILURE      18
#define IDS_NODEVICES        19
#define IDS_NODEFAULTPRN     20
#define IDS_DNDMMISMATCH     21
#define IDS_CREATEICFAILURE  22
#define IDS_PRINTERNOTFOUND  23
#define IDS_NOFONTS          24
#define IDS_SUBCLASSFAILURE  25
#define IDS_INVALIDFILENAME  26
#define IDS_BUFFERTOOSMALL   27
#define IDS_FILTERSTRING     28
#define IDS_UNKNOWNERROR     29

// this is open file dialog extra reources
#define IDD_COMDLG32				100
#define IDE_SELECTED				1022
#define IDE_PATH					102
#define	IDC_BUTTON1					1023

// OS Constants
#define OS_WIN95		1
#define OS_WIN98		2
#define OS_ME			3
#define OS_NT351		351
#define OS_NT4			400
#define OS_W2K			500
#define	OS_XP			510
#define OS_WIN03		520
#define OS_VISTA_LONGHORN 600 // MWD 10/2/06 #10204
#define OS_SERVER2K8	601 // REB 7/15/09 #20729
#define OS_WIN7			610 // REB 7/15/09 #20729
#define OS_SERVER2K8R2	611 // REB 7/15/09 #20729
#define OS_WIN8			620 // REB 10/31/12 #34333
#define OS_SERVER2012	621 // REB 10/31/12 #34333
#define OS_WIN81		630 // AMS2 9/26/14 #37816
#define OS_SERVER2012R2	631 // AMS2 9/26/14 #37816
#define OS_WIN10		1000 // WJF 9/21/15 #43601
#define OS_SERVER2016	1001 // WJF 11/16/16 Win-42

#define BM_TILE				1
#define BM_SCALE			2
#define BM_SCALETOMAXCLIENT 3

// print dialog control IDs
#define ID_CB_PRINTER			1139
#define ID_CB_OK				1
#define ID_CB_PAPERSIZE			1137
#define	ID_CB_SOURCE			1138
#define ID_BTN_PORTRAIT			1056
#define ID_BTN_LANDSCAPE		1057
#define ID_BTN_PRINTTOFILE		1040
#define ID_BTN_PRINTPREVIEW		12856  //01/03/02 corrected value
#define ID_EDIT_COPIES			1154

#define PS_PORTRAIT				1
#define PS_LANDSCAPE			2

#define PS_PRTSETTINGSDLG		1
#define PS_PRTDLG				2

#define PS_IDLE					-1
#define PS_SEARCH				0

// ToolTip constants
#define TT_BALLOON				0
#define TT_RECTANGLE			1
#define TT_CENTER				2
#define TT_TOPRIGHT				3
#define TT_TOPLEFT				4
#define TT_BOTTOMRIGHT			5
#define TT_BOTTOMLEFT			6
#define TT_CLOSE_ON_CLICK		7

// This block added by Mark De Wever
// Added these or-ing of
// negative numbers with their positive counter part
// gives strange results due to many bits set in -4
#define RW_DISABLE_MIN				64
#define RW_ENABLE_MIN				128
#define RW_DISABLE_MAX				256
#define RW_ENABLE_MAX				512
#define RW_DISABLE_CLOSE			1024 // note we clash with RW_SUBCLASS_INIT (but is undefined....)
#define RW_ENABLE_CLOSE				2048
#define RW_DISABLE_RESIZE			4096
#define RW_ENABLE_RESIZE			8192

#define PACKVERSION(major,minor) MAKELONG(minor,major)

// TrayIcon defines
#define ST_TRAYNOTIFICATION "<>ST_TrayNotification"

#define NIF_HIDE						0x0100
#define NIF_SHOW						0x0200

struct listNode {
	HWND targetWHndl;  // this may not be required
	LONG iconID; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG procNbr; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	struct listNode* nextPtr;
};

typedef struct listNode TI;
typedef TI* pTI;

struct linkedList {
	HWND			hWnd;
	WNDPROC	wpProc;
	LONG_PTR			type;
	LONG_PTR			dataLong1;
	LONG_PTR			dataLong2;
	char*		dataPtrStr;
	void*		dataPtr;
	struct	linkedList* nextPtr;
};

enum structData { LL_hWnd = 1, LL_wpProc, LL_type, LL_dataLong1, LL_dataLong2, LL_dataPtrStr, LL_dataPtr };

enum listType { LL_Restrict };

typedef struct linkedList LL;
typedef LL* pLL;

#define EP_USE_REGISTRY			0
#define EP_USE_OPEN					2
#define EP_NAMES_ONLY				1

#define CL_COMPLETE					0
#define CL_DRAGDROP					1

#define RW_RELEASE					0
#define RW_NO_SIZE					1
#define RW_NO_MOVE					2
#define RW_NO_MIN						4
#define RW_NO_MAX						8
#define RW_NO_NEXT					16
#define RW_NO_CLOSE					32
#define	RW_RESTORE_CLOSE   -32

// Window Styles
#define NUMBER_STYLES				17

#define IS_ICONIC					1
#define IS_ZOOMED					2

#define	IS_VARNAME					0
#define	IS_OBJECTNAME				1

// processWindowMessage source
#define TRAY_ICON_FUNCTION			1
#define RESPECT_TOOL_BAR_FUNCTION	2

#define IN_SYSCOMMAND_MAX			1
#define IN_SYSCOMMAND_RESTORE		2
#define IN_SHOWMAXIMIZED			3
#define XCHANGING_MAX_WINDOWS		4
#define APP_MAXIMIZING				12
#define APP_RESTORING				13
#define APP_MAXIMIZED				14
#define APP_NORM					15
#define APP_SIZING_W_CHLDMAX		16
#define APP_MINIMIZED				17 // REB 8/11/08 #16207
#define IN_SYSCOMMAND_MIN			18 // REB 8/11/08 #16207

// Tool Bar define
#define TB_NOTIFICATION			"<>TB_NOTIFICATION"

#define MAXBUF					2048
#define MAXLABELBUF				4096  // REB 6/5/08 #17022
#define MEDBUF					1024
#define SMLBUF					256

// gui_GetSysColor
#define COLOR_UNUSED			25

// sys_GetRegKey
#define GR_HKEY_CLASSES_ROOT		1
#define GR_HKEY_CURRENT_USER		2
#define GR_HKEY_DYN_DATA			3
#define GR_HKEY_LOCAL_MACHINE		4
#define GR_HKEY_USERS				5
#define GR_HKEY_CURRENT_CONFIG		6
#define GR_HKEY_PERFORMANCE_DATA	7

// get4dRegType
#define GR_TYPE_BINARY		1
#define GR_TYPE_LONGINT		2
#define GR_TYPE_TEXT		3
#define GR_TYPE_ARRAYTEXT	4

#define MAX_REG_SIZE 16383

// sys_GetDocumentList
#define INVALID_DIRECTORY 0xFFFFFFFF // Check64
#define PATHCHAR   '\\'
#define PATHSTR  "\\"
#define ARRAY_LOAD_VALUE 10

#define MAX_STRING_SIZE 255 // Mark De Wever #12225
#define snprintf     _snprintf
//#define strncpy(d, s, n) strcpy_s((d) ,(n) ,(s)) // Mark De Wever #12225

#define AES_BLOCK_SIZE 16 // WJF 5/6/15 #42665

#define MAX_PATH_PLUS 388 // WJF 9/11/15 #43727

#ifdef _WIN64
#define k64Init 0x00000000ffffffff // WJF 9/1/15 #43731/#43732 The selector value is returning this in 64-bit
#endif

#define HANDLEARRAY_CAPACITY 4096 // WJF 9/1/15 #43731 32 KB in 64-bit, 16 in 32-bit