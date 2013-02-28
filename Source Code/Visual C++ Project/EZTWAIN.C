// EZTWAIN.C - Easy TWAIN DLL main module
//
// This code is free sample code, provided without charge, and you use it
// entirely at your own risk.
// No rights or ownership is claimed by the author, or by any company
// or organization.  There are no restrictions on use or (re)distribution.
//
// Download from:    www.dosadi.com/eztwain1.htm
//

// REB 6/26/09 #14151 Adapted for integration win Win32API


#define VERSION 119		// version number, times 100.

//------------ Includes

#include <assert.h>
#include <memory.h>		// memset, memcpy

#include "windows.h"

#ifdef _WIN32
#ifndef _INC_VFW
#include <vfw.h>
#endif
#else
#include "commdlg.h"
#endif

#include "twain.h"

#ifdef _WIN32
  #define EZTAPI WINAPI
#else
  #define EZTAPI __export FAR PASCAL
#endif

#include "eztwain.h"

//------------ Constants and Macros

#define STATIC static

#ifdef _WIN32
  #define INT32 INT_PTR
  #define DATAGROUP unsigned
  #define DSM_FILENAME "TWAIN_32.DLL"
  #define DSM_ENTRYPOINT "DSM_Entry"
  #define IsValidHandle(h) (h!=NULL)
  #define HUGEWRITE(fh, pb, bc) ((INT32)_lwrite(fh, pb, bc))
  #define FMEMSET(p, v, n) memset(p, v, n);
  #define FMEMCPY(p, q, n) memcpy(p, q, n);
#else
  #define INT32 LONG_PTR
  #define DATAGROUP ULONG_PTR
  #define DSM_FILENAME "TWAIN.DLL"
  #define DSM_ENTRYPOINT "DSM_ENTRY"
  #define VALID_HANDLE 32
  #define IsValidHandle(h) ((h!=0) && ((h) >= 32))
  #define HUGEWRITE(fh, pb, bc) _hwrite(fh, pb, bc)
  #define FMEMSET(p, v, n) _fmemset(p, v, n);
  #define FMEMCPY(p, q, n) _fmemcpy(p, q, n);
#endif

typedef enum {
	ED_NONE,
	ED_START_TRIPLET_ERRS,
	ED_CAP_GET,				// MSG_GET triplet on a capability failed
	ED_CAP_SET,				// MSG_SET triplet on capability failed
	ED_DSM_FAILURE,			// TWAIN DSM returned TWRC_FAILURE
	ED_DS_FAILURE,			// source returned TWRC_FAILURE
	ED_END_TRIPLET_ERRS,
	ED_NOT_STATE_4,			// operation invoked in wrong state
	ED_NULL_HCON,			// MSG_GET returned a null container handle
	ED_BAD_HCON,			// MSG_GET returned an invalid/unlockable container handle
	ED_BAD_CONTYPE,			// returned container ConType is not valid.
	ED_BAD_ITEMTYPE,		// returned container ItemType is not valid.
	ED_CAP_GET_EMPTY,		// returned container has 0 items.
	ED_CAP_SET_EMPTY,		// trying to restrict a cap to empty set
} ErrorDetail;

const char *pszErrDescrip[] = 
	{	"[no details available]",
		"",
		"DAT_CAPABILITY/MSG_GET failed",
		"DAT_CAPABILITY/MSG_SET failed",
		"Source Manager operation failed",
		"DataSource operation failed",
		"",
		"TWAIN session not in State 4 (Source Open)",
		"MSG_GET returned a NULL container handle",
		"MSG_GET returned an invalid container handle",
		"Returned container is not valid type",
		"Returned container has invalid ItemType",
		"Returned container is empty",
		"App and source found NO values in common",
	};

const char *pszRC[] = {
	"TWRC_SUCCESS",
	"TWRC_FAILURE",
	"TWRC_CHECKSTATUS ('tried hard')",
	"TWRC_CANCEL",
	"TWRC_DSEVENT",
	"TWRC_NOTDSEVENT",
	"TWRC_XFERDONE",
	"TWRC_ENDOFLIST",
   "TWRC_INFONOTSUPPORTED",
   "TWRC_DATANOTAVAILABLE"
};

const char *pszCC[] = {
	"TWCC_SUCCESS",
	"TWCC_BUMMER (Failure due to unknown causes)",
	"TWCC_LOWMEMORY",
	"TWCC_NODS (No Data Source)",
	"TWCC_MAXCONNECTIONS (DS is connected to max possible apps)",
	"TWCC_OPERATIONERROR (DS/DSM reported error, app shouldn't)",
	"TWCC_BADCAP (Unknown capability)",
	"7 (undefined)",
	"8 (undefined)",
	"TWCC_BADPROTOCOL (Unrecognized triplet)",
	"TWCC_BADVALUE (Data parameter out of range)",
	"TWCC_SEQERROR (Triplet out of sequence)",
	"TWCC_BADDEST (Unknown dest. App/Src in DSM_Entry)",
   "TWCC_CAPUNSUPPORTED (Cap not supported by source)",
   "TWCC_CAPBADOPERATION (Operation not supported by cap)",
   "TWCC_CAPSEQERROR (Cap has dependancy on other cap)",
};

const char szInsuffMem[] = "Insufficient Memory";	// error message

#define WM_AUTOSCAN 0x7fa0       // semi-random number

//------------ Global variables

STATIC INT_PTR			iAvailable;			// TWAIN available: 0:unknown, -1:No, 1:Yes
STATIC INT_PTR			nState = 1;			// TWAIN state (per the standard)
STATIC INT_PTR			nErrDetail;			// detailed error code
STATIC unsigned   nErrRC, nErrCC;	// result code and condition code for last error
STATIC char			szMsg[256];			// scratch buffer for messages
STATIC DSMENTRYPROC	pDSM_Entry;    // entry point of Data Source Manager (TWAIN.DLL)
STATIC HANDLE		hDSMLib;          // handle of DSM
STATIC TW_IDENTITY	AppId = {      // application identity structure
	0,									// Id, filled in by DSM
	{ 1, 0, TWLG_USA, TWCY_USA, "Unknown Version"},	// Version
    TWON_PROTOCOLMAJOR,
    TWON_PROTOCOLMINOR,
    DG_IMAGE | DG_CONTROL,
    "Unspecified Developer",           // Mfg
    "TWAIN Application",               // Family
    "EZTWAIN Client"                   // Product
    };
STATIC TW_IDENTITY      SourceId;      // source identity structure
STATIC BOOL             bHideUI;       // allow source u/i to be hidden
STATIC TW_USERINTERFACE twUI;
STATIC TW_PENDINGXFERS  pendingXfers;
STATIC BOOL             bBreakModalLoop; // flag to end modal event loop
STATIC BOOL             bMultiTransfer; // if TRUE, allow multiple Acquires in sequence.
STATIC unsigned         nMemBuffer;     // memory transfer buffer counter
STATIC HANDLE           hdibLastRcvd;   // bitmap returned by native transfer
STATIC TW_INT16         rc;             // result code       
STATIC HINSTANCE        hinstLib;       // instance handle for this DLL
STATIC HWND             hwndProxy;      // proxy window

#ifdef EZTPRO_H
STATIC HWND             hwndButton;
STATIC BOOL             bAutoScan;
#endif

//------------ Forward declarations

void EZTAPI TWAIN_WaitForXfer(HWND hwnd, LONG_PTR *lErr);

static void SetState(INT_PTR n);
static HWND GetValidHwnd(HWND hwnd);
static HWND CreateProxyWindow(void);
static TW_UINT32 Intersect(TW_UINT32 wMask, TW_UINT32 nItems, TW_UINT16 far *pItem);
static unsigned FindIndex16(TW_UINT32 nItems, TW_UINT16 far *plist, TW_UINT16 uVal, unsigned nDefault);
static unsigned BitCount(unsigned W);
static INT_PTR RecordError(ErrorDetail ed);
static void ClearError(void);
static double Fix32ToFloat(TW_FIX32 fix);
static void NativeXferHandler(void);
static void FileXferHandler(void);
static void MemoryXferHandler(void);
static void DoOneTransfer(void);

#ifdef EZTPRO_H
static HWND FindScanButton(void);
static void PressButton(HWND hwnd);
#endif

//------------ Public functions


#ifdef _WIN32
/*REB
// Win32 DLL main
BOOL WINAPI DllMain(HANDLE hModule, 
                    ULONG  ulEvent,
                    LPVOID lpReserved)
{
   switch (ulEvent) {
   case DLL_PROCESS_DETACH:
      //TWAIN_UnloadSourceManager();
      break;
   case DLL_PROCESS_ATTACH:
#ifdef DEBUG
      wsprintf(szMsg, "sizeof (TW_IDENTITY) == %d\n", sizeof (TW_IDENTITY));
      OutputDebugString(szMsg);
#endif
      assert(sizeof (TW_IDENTITY) == 156);
      break;
   default:
      break;
   }
   return TRUE;
}
*/
#else

// Win16 DLL initialization and termination routines
INT_PTR CALLBACK __export LibMain(HINSTANCE hinst, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine)
{
	wDataSeg = wDataSeg; cbHeapSize = cbHeapSize; lpszCmdLine = lpszCmdLine;

	hinstLib = hinst;
   assert(sizeof (TW_IDENTITY) == 156);
	return 1;		// indicate success
} // LibMain


INT_PTR FAR PASCAL __export _WEP(INT_PTR x)
{
	x = x;				// suppress 'is never used' warning
	return 1;
} // _WEP


#endif


//-- Record application information
void EZTAPI TWAIN_RegisterApp(
	INT_PTR   nMajorNum,     // major and incremental revision of application.
   INT_PTR   nMinorNum,     // e.g. version 4.5: nMajorNum = 4, nMinorNum = 5
	INT_PTR   nLanguage,     // (human) language (use TWLG_xxx from TWAIN.H)
	INT_PTR   nCountry,      // country (use TWCY_xxx from TWAIN.H)
	LPSTR lpszVersion,   // version info string e.g. "1.0b3 Beta release"
	LPSTR lpszMfg,       // name of mfg/developer e.g. "Crazbat Software"
	LPSTR lpszFamily,    // product family e.g. "BitStomper"
	LPSTR lpszProduct)   // specific product e.g. "BitStomper Deluxe Pro"
{
    AppId.Id = 0;						// init to 0, but Source Manager will assign real value
    AppId.Version.MajorNum = nMajorNum;
    AppId.Version.MinorNum = nMinorNum;
    AppId.Version.Language = nLanguage;
    AppId.Version.Country  = nCountry;
    lstrcpy (AppId.Version.Info,  lpszVersion);

    AppId.ProtocolMajor =    TWON_PROTOCOLMAJOR;
    AppId.ProtocolMinor =    TWON_PROTOCOLMINOR;
    AppId.SupportedGroups =  DG_IMAGE | DG_CONTROL;
    lstrcpy (AppId.Manufacturer,  lpszMfg);
    lstrcpy (AppId.ProductFamily, lpszFamily);
    lstrcpy (AppId.ProductName,   lpszProduct);

} // TWAIN_RegisterApp


INT_PTR EZTAPI TWAIN_SelectImageSource(HWND hwnd)
{
   INT_PTR fSuccess = FALSE;
   INT_PTR nEntryState = nState;
   
   if (nState >= TWAIN_SM_OPEN || TWAIN_OpenSourceManager(hwnd)) {
      TW_IDENTITY	NewSourceId;
#ifdef _WIN32
      FMEMSET(&NewSourceId, 0, sizeof NewSourceId);
#else
      // I will settle for the system default.  Shouldn't I get a highlight
      // on system default without this call?
      TWAIN_Mgr(DG_CONTROL, DAT_IDENTITY, MSG_GETDEFAULT, &NewSourceId);
#endif
      // Post the Select Source dialog
      fSuccess = TWAIN_Mgr(DG_CONTROL, DAT_IDENTITY, MSG_USERSELECT, &NewSourceId);
   } else {
      TWAIN_ErrorBox("Unable to open Source Manager (" DSM_FILENAME ")");
   }
   
   if (nEntryState < TWAIN_SM_OPEN) {
      TWAIN_CloseSourceManager(hwnd);
      if (nEntryState < TWAIN_SM_LOADED) {
         TWAIN_UnloadSourceManager();
      }
   }
   return fSuccess;
} // TWAIN_SelectImageSource


HANDLE EZTAPI TWAIN_AcquireNative(HWND hwnd, unsigned wPixTypes, LONG_PTR *lErr) // REB 7/1/09 Added third parameter to track errors.
{
	HANDLE hnative = NULL;
	ClearError();			// clear error detail
 
   hwnd = GetValidHwnd(hwnd);
   if (nState < TWAIN_SOURCE_ENABLED) {
      if (!TWAIN_OpenSourceManager(hwnd)) {
         //TWAIN_ErrorBox("Unable to open Source Manager (" DSM_FILENAME ")");
		 *lErr = -1;
      } else if (!TWAIN_OpenDefaultSource()) {
         //TWAIN_ReportLastError("Unable to open default Data Source.");
		  *lErr = -2;
      } else if (!TWAIN_NegotiatePixelTypes(wPixTypes)) {
         //TWAIN_ReportLastError("Failed to negotiate Pixel Type.");
		  *lErr = -3;
         return NULL;
      } else {
         assert(nState == TWAIN_SOURCE_OPEN);
      }
   }
   if (nState >= TWAIN_SOURCE_OPEN) {
      hnative = TWAIN_WaitForNativeXfer(hwnd, lErr);
   }

   if (!bMultiTransfer) {
       // shut everything down in the right sequence
       TWAIN_UnloadSourceManager();
   }

   // Note: hnative will be NULL if transfer failed or was cancelled.
	return hnative;
} // TWAIN_AcquireNative


HANDLE EZTAPI TWAIN_WaitForNativeXfer(HWND hwnd, LONG_PTR *lErr)
{
    HANDLE hdib = NULL;
    if (nState >= TWAIN_SOURCE_OPEN && TWAIN_XferMech() == XFERMECH_NATIVE) {
        // all good, go ahead
    } else if (!TWAIN_SetXferMech(XFERMECH_NATIVE)) {
       // TWAIN_ReportLastError("Error setting Native Transfer mode");
		*lErr = -4;
    }
    TWAIN_WaitForXfer(hwnd, lErr);
    hdib = hdibLastRcvd;
    hdibLastRcvd = NULL;
    return hdib;
} // TWAIN_WaitForNativeXfer


void EZTAPI TWAIN_WaitForXfer(HWND hwnd, LONG_PTR *lErr)
{
   //BOOL bWasEnabled;
   // Make up a valid window if we weren't given one
   hwnd = GetValidHwnd(hwnd);
   // Disable the parent window during the modal acquire
   // REB #14151 Leave the parent window alone!
   //bWasEnabled = (EnableWindow(hwnd, FALSE) == 0);

   if (nState == TWAIN_TRANSFER_READY) {
      DoOneTransfer();
   } else if (nState >= TWAIN_SOURCE_ENABLED  || TWAIN_EnableSource(hwnd)) {
      // source is enabled, wait for transfer or source closed
      TWAIN_ModalEventLoop();
	  *lErr = 1;
   } else {
      //OutputDebugString("EZTW: EnableSource failed.\n");
     // TWAIN_ReportLastError("Failed to enable Data Source.");
   }

   // Re-enable the parent window if it was enabled
   // REB #14151 Leave the parent window alone!
   //EnableWindow(hwnd, bWasEnabled);
} // TWAIN_WaitForXfer


void EZTAPI TWAIN_FreeNative(HANDLE hdib)
// Release the memory allocated to a native format image, as returned by TWAIN_AcquireNative.
// (For those coding in C or C++, this is just a call to GlobalFree.)
{
	if (hdib) GlobalFree(hdib);
} // TWAIN_FreeNative



INT_PTR EZTAPI TWAIN_AcquireToClipboard(HWND hwndApp, unsigned wPixTypes)
// Like AcquireNative, but puts the resulting image, if any, into the system clipboard.
// Useful for environments like Visual Basic where it is hard to make direct use of a DIB handle.
// A return value of 1 indicates success, 0 indicates failure.
{
	INT_PTR fOk = FALSE;
	LONG_PTR lErr = 0;
	HANDLE hDib = TWAIN_AcquireNative(hwndApp, wPixTypes, &lErr);
	if (hDib) {
		if (OpenClipboard(hwndApp)) {
			if (EmptyClipboard()) {
				SetClipboardData(CF_DIB, hDib);
				fOk = TRUE;
				hDib = NULL;
			}
			CloseClipboard();
		}
		if (hDib) {
			// something went wrong, recycle the image
			GlobalFree(hDib);
		}
	}
	return fOk;		// failed
} // TWAIN_AcquireToClipboard


INT_PTR EZTAPI TWAIN_AcquireToFilename(HWND hwndApp, LPCSTR pszFile)
// Adapted from a routine by David D. Henseler (ddh) of SOLUTIONS GmbH
{
	INT_PTR result = -1;
	LONG_PTR lErr = 0;
	HANDLE	hDib = TWAIN_AcquireNative(hwndApp, TWAIN_ANYTYPE, &lErr);
	if (hDib) {
		result = TWAIN_WriteNativeToFilename(hDib, pszFile);
		TWAIN_FreeNative(hDib);
	}
	return result;
} // TWAIN_AcquireToFile


///////////////////////////////////////////////////////////////////////
// DIB utilities

INT_PTR EZTAPI TWAIN_DibDepth(HANDLE hdib)
{
   LPBITMAPINFOHEADER pbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
   INT_PTR D = pbi->biBitCount;
   GlobalUnlock(hdib);
   return D;
} // TWAIN_DibDepth

INT_PTR EZTAPI TWAIN_DibWidth(HANDLE hdib)
{
   LPBITMAPINFOHEADER pbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
   INT_PTR W = (INT_PTR)pbi->biWidth;
   GlobalUnlock(hdib);
   return W;
} // TWAIN_DibWidth


INT_PTR EZTAPI TWAIN_DibHeight(HANDLE hdib)
{
   LPBITMAPINFOHEADER pbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
   INT_PTR H = (INT_PTR)pbi->biHeight;
   GlobalUnlock(hdib);
   return H;
} // TWAIN_DibHeight


static INT_PTR ColorCount(INT_PTR bpp)
{
   return 0xFFF & (1 << bpp);
}

       
static INT_PTR BmiColorCount(LPBITMAPINFOHEADER lpbi)
{
   if (lpbi->biSize == sizeof(BITMAPCOREHEADER)) {
      LPBITMAPCOREHEADER    lpbc = ((LPBITMAPCOREHEADER)lpbi);
      return 1 << lpbc->bcBitCount;
   } else if (lpbi->biClrUsed == 0) {
      return ColorCount(lpbi->biBitCount);
   } else {
      return (INT_PTR)lpbi->biClrUsed;
   }
} // BmiColorCount


static INT_PTR DibNumColors(VOID FAR *pv)
{
   return BmiColorCount((LPBITMAPINFOHEADER)pv);
} // DibNumColors


static size_t RowBytes(INT_PTR bpp, INT_PTR w)
{
	// bytes per image row - round up to DWORD
	return (bpp * w + 31) / 32 * 4;
}

static size_t BmiRowBytes(const LPBITMAPINFOHEADER lpbi) // REB 3/30/11 #25290 Change LONG_PTR to size_t
{
	return RowBytes(lpbi->biBitCount, lpbi->biWidth);
} // BmiRowBytes


static size_t BmiColorTableBytes(LPBITMAPINFOHEADER lpbi)
{
	return BmiColorCount(lpbi) * sizeof(RGBQUAD);
} // BmiColorTableBytes


static size_t ColorTableBytes(INT_PTR bpp)
{
	return ColorCount(bpp) * sizeof(RGBQUAD);
} // ColorTableBytes


static size_t OffsetToBits(LPBITMAPINFOHEADER lpbi)
{
	return (size_t)(lpbi->biSize + BmiColorTableBytes(lpbi));
} // OffsetToBits


static LPBYTE FindBits(LPBITMAPINFOHEADER lpbi)
{
	return (LPBYTE)lpbi + OffsetToBits(lpbi);
} // FindBits


static LPBYTE FindRow(LPBITMAPINFOHEADER lpbi, INT_PTR nRow)
{
	LPBYTE lpbits = FindBits(lpbi);

	if (lpbi->biHeight > 0) {
		// standard upside-down DIB
		nRow = (INT_PTR)lpbi->biHeight - 1 - nRow;
	}
	return lpbits + nRow * BmiRowBytes(lpbi);
}


INT_PTR EZTAPI TWAIN_DibNumColors(HANDLE hdib)
// given a DIB handle, return the number of palette entries: 0,2,16, or 256
{
	VOID FAR *pv = GlobalLock(hdib);
	INT_PTR nColors = DibNumColors(pv);
	GlobalUnlock(hdib);
    return nColors;
}


size_t EZTAPI DIB_RowBytes(HANDLE hdib) // REB 3/30/11 #25290 Change LONG_PTR to size_t
{
	size_t s = BmiRowBytes((LPBITMAPINFOHEADER)GlobalLock(hdib)); // REB 3/30/11 #25290 Change LONG_PTR to size_t
	GlobalUnlock(hdib);
	return s;
}


void EZTAPI DIB_ReadRow(HANDLE hdib, INT_PTR nRow, BYTE* prow)
{
   LPBITMAPINFOHEADER pbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
   FMEMCPY(prow, FindRow(pbi, nRow), (size_t)BmiRowBytes(pbi));
   GlobalUnlock(hdib);
}


//---------------------------------------------------------------------------

const PALETTEENTRY peStock256[256] = {
   // from 0 .. 9 are Windows fixed colors
     0,   0,   0, 0,	// 000 always black
   128,   0,   0, 0,	// 001 dark red
     0, 128,   0, 0,	// 002 dark green
   128, 128,   0, 0,	// 003 dark brown
     0,   0, 128, 0,	// 004 dark blue
   128,   0, 128, 0,	// 005 dark purple
     0, 128, 128, 0,	// 006 dark teal
   192, 192, 192, 0,	// 007 light gray
   192, 220, 192, 0,	// 008 pale green
   166, 202, 240, 0,	// 009 sky blue
     4,   4,   4, 4,	// 010 dark gray ramp
     8,   8,   8, 4,	// 011
    12,  12,  12, 4,	// 012
    17,  17,  17, 4,	// 013
    22,  22,  22, 4,	// 014
    28,  28,  28, 4,	// 015
    34,  34,  34, 4,	// 016
    41,  41,  41, 4,	// 017
    85,  85,  85, 4,	// 018
    77,  77,  77, 4,	// 019
    66,  66,  66, 4,	// 020
    57,  57,  57, 4,	// 021
   255, 124, 128, 4,	// 022
   255,  80,  80, 4,	// 023
   214,   0, 147, 4,	// 024
   204, 236, 255, 4,	// 025
   239, 214, 198, 4,	// 026
   231, 231, 214, 4,	// 027
   173, 169, 144, 4,	// 028
    51,   0,   0, 4,	// 029
   102,   0,   0, 4,	// 030
   153,   0,   0, 4,	// 031 
   204,   0,   0, 4,	// 032
     0,  51,   0, 4,	// 033
	51,  51,   0, 4,	// 034
   102,  51,   0, 4,	// 035
   153,  51,   0, 4,	// 036
   204,  51,   0, 4,	// 037
   255,  51,   0, 4,	// 038
     0, 102,   0, 4,	// 039
    51, 102,   0, 4,	// 040
   102, 102,   0, 4,	// 041
   153, 102,   0, 4,	// 042
   204, 102,   0, 4,	// 043
   255, 102,   0, 4,	// 044
     0, 153,   0, 4,	// 045     
	51, 153,   0, 4,	// 046
   102, 153,   0, 4,	// 047
   153, 153,   0, 4,	// 048
   204, 153,   0, 4,	// 049
   255, 153,   0, 4,	// 050
     0, 204,   0, 4,	// 051
    51, 204,   0, 4,	// 052
   102, 204,   0, 4,	// 053
   153, 204,   0, 4,	// 054
   204, 204,   0, 4,	// 055
   255, 204,   0, 4,	// 056
   102, 255,   0, 4,	// 057
   153, 255,   0, 4,	// 058
   204, 255,   0, 4,	// 059
     0,   0,  51, 4,	// 060
    51,   0,  51, 4,	// 061
   102,   0,  51, 4,	// 062
   153,   0,  51, 4,	// 063
   204,   0,  51, 4,	// 064
   255,   0,  51, 4,	// 065
     0,  51,  51, 4,	// 066
    51,  51,  51, 4,	// 067
   102,  51,  51, 4,	// 068
   153,  51,  51, 4,	// 069
   204,  51,  51, 4,	// 070
   255,  51,  51, 4,	// 071
     0, 102,  51, 4,	// 072
    51, 102,  51, 4,	// 073
   102, 102,  51, 4,	// 074
   153, 102,  51, 4,	// 075
   204, 102,  51, 4,	// 076
   255, 102,  51, 4,	// 077
     0, 153,  51, 4,	// 078
    51, 153,  51, 4,	// 079
   102, 153,  51, 4,	// 080
   153, 153,  51, 4,	// 081
   204, 153,  51, 4,	// 082
   255, 153,  51, 4,	// 083
     0, 204,  51, 4,	// 084
    51, 204,  51, 4,	// 085
   102, 204,  51, 4,	// 086
   153, 204,  51, 4,	// 087
   204, 204,  51, 4,	// 088
   255, 204,  51, 4,	// 089  
    51, 255,  51, 4,	// 090
   102, 255,  51, 4,	// 091
   153, 255,  51, 4,	// 092
   204, 255,  51, 4,	// 093
   255, 255,  51, 4,	// 094
     0,   0, 102, 4,	// 095
    51,   0, 102, 4,	// 096
   102,   0, 102, 4,	// 097
   153,   0, 102, 4,	// 098
   204,   0, 102, 4,	// 099
   255,   0, 102, 4,	// 100
     0,  51, 102, 4,	// 101
    51,  51, 102, 4,	// 102
   102,  51, 102, 4,	// 103
   153,  51, 102, 4,	// 104
   204,  51, 102, 4,	// 105
   255,  51, 102, 4,	// 106
     0, 102, 102, 4,	// 107
    51, 102, 102, 4,	// 108
   102, 102, 102, 4,	// 109
   153, 102, 102, 4,	// 110
   204, 102, 102, 4,	// 111
     0, 153, 102, 4,	// 112
    51, 153, 102, 4,	// 113
   102, 153, 102, 4,	// 114
   153, 153, 102, 4,	// 115
   204, 153, 102, 4,	// 116
   255, 153, 102, 4,	// 117
     0, 204, 102, 4,	// 118
    51, 204, 102, 4,	// 119
   153, 204, 102, 4,	// 120
   204, 204, 102, 4,	// 121
   255, 204, 102, 4,	// 122
     0, 255, 102, 4,	// 123
    51, 255, 102, 4,	// 124
   153, 255, 102, 4,	// 125
   204, 255, 102, 4,	// 126
   255,   0, 204, 4,	// 127
   204,   0, 255, 4,	// 128
     0, 153, 153, 4,	// 129
   153,  51, 153, 4,	// 130
   153,   0, 153, 4,	// 131
   204,   0, 153, 4,	// 132
     0,   0, 153, 4,	// 133
    51,  51, 153, 4,	// 134
   102,   0, 153, 4,	// 135
   204,  51, 153, 4,	// 136
   255,   0, 153, 4,	// 137
     0, 102, 153, 4,	// 138
    51, 102, 153, 4,	// 139
   102,  51, 153, 4,	// 140
   153, 102, 153, 4,	// 141
   204, 102, 153, 4,	// 142
   255,  51, 153, 4,	// 143
    51, 153, 153, 4,	// 144
   102, 153, 153, 4,	// 145
   153, 153, 153, 4,	// 146
   204, 153, 153, 4,	// 147
   255, 153, 153, 4,	// 148
     0, 204, 153, 4,	// 149
    51, 204, 153, 4,	// 150
   102, 204, 102, 4,	// 151
   153, 204, 153, 4,	// 152
   204, 204, 153, 4,	// 153
   255, 204, 153, 4,	// 154
     0, 255, 153, 4,	// 155
    51, 255, 153, 4,	// 156
   102, 204, 153, 4,	// 157
   153, 255, 153, 4,	// 158
   204, 255, 153, 4,	// 159
   255, 255, 153, 4,	// 160
     0,   0, 204, 4,	// 161
    51,   0, 153, 4,	// 162
   102,   0, 204, 4,	// 163
   153,   0, 204, 4,	// 164
   204,   0, 204, 4,	// 165
     0,  51, 153, 4,	// 166
    51,  51, 204, 4,	// 167
   102,  51, 204, 4,	// 168
   153,  51, 204, 4,	// 169
   204,  51, 204, 4,	// 170
   255,  51, 204, 4,	// 171
     0, 102, 204, 4,	// 172
    51, 102, 204, 4,	// 173
   102, 102, 153, 4,	// 174
   153, 102, 204, 4,	// 175
   204, 102, 204, 4,	// 176
   255, 102, 153, 4,	// 177
     0, 153, 204, 4,	// 178
    51, 153, 204, 4,	// 179
   102, 153, 204, 4,	// 180
   153, 153, 204, 4,	// 181
   204, 153, 204, 4,	// 182
   255, 153, 204, 4,	// 183
     0, 204, 204, 4,	// 184
    51, 204, 204, 4,	// 185
   102, 204, 204, 4,	// 186
   153, 204, 204, 4,	// 187
   204, 204, 204, 4,	// 188
   255, 204, 204, 4,	// 189
     0, 255, 204, 4,	// 190
    51, 255, 204, 4,	// 191
   102, 255, 153, 4,	// 192
   153, 255, 204, 4,	// 193
   204, 255, 204, 4,	// 194
   255, 255, 204, 4,	// 195
    51,   0, 204, 4,	// 196
   102,   0, 255, 4,	// 197
   153,   0, 255, 4,	// 198
     0,  51, 204, 4,	// 199
    51,  51, 255, 4,	// 200
   102,  51, 255, 4,	// 201
   153,  51, 255, 4,	// 202
   204,  51, 255, 4,	// 203
   255,  51, 255, 4,	// 204 
     0, 102, 255, 4,	// 205
    51, 102, 255, 4,	// 206
   102, 102, 204, 4,	// 207
   153, 102, 255, 4,	// 208
   204, 102, 255, 4,	// 209
   255, 102, 204, 4,	// 210
     0, 153, 255, 4,	// 211
    51, 153, 255, 4,	// 212
   102, 153, 255, 4,	// 213
   153, 153, 255, 4,	// 214
   204, 153, 255, 4,	// 215
   255, 153, 255, 4,	// 216
     0, 204, 255, 4,	// 217
    51, 204, 255, 4,	// 218
   102, 204, 255, 4,	// 219
   153, 204, 255, 4,	// 220
   204, 204, 255, 4,	// 221
   255, 204, 255, 4,	// 222
    51, 255, 255, 4,	// 223
   102, 255, 204, 4,	// 224
   153, 255, 255, 4,	// 225
   204, 255, 255, 4,	// 226
   255, 102, 102, 4,	// 227
   102, 255, 102, 4,	// 228
   255, 255, 102, 4,	// 229
   102, 102, 255, 4,	// 230
   255, 102, 255, 4,	// 231
   102, 255, 255, 4,	// 232
   165,   0,  33, 4,	// 233
    95,  95,  95, 4,	// 234
   119, 119, 119, 4,	// 235 
   134, 134, 134, 4,	// 236
   150, 150, 150, 4,	// 237
   203, 203, 203, 4,	// 238
   178, 178, 178, 4,	// 239
   215, 215, 215, 4,	// 240
   221, 221, 221, 4,	// 241
   227, 227, 227, 4,	// 242
   234, 234, 234, 4,	// 243
   241, 241, 241, 4,	// 244
   248, 248, 248, 4,	// 245
   // 246 - 255 are Windows fixed colors    
   255, 251, 240, 0,	// 246
   160, 160, 164, 0,	// 247
   128, 128, 128, 0,	// 248
   255,   0,   0, 0,	// 249
     0, 255,   0, 0,	// 250
   255, 255,   0, 0,	// 251
     0,   0, 255, 0,	// 252
   255,   0, 255, 0,	// 253
     0, 255, 255, 0,	// 254
   255, 255, 255, 0,	// 255 always white 
};

HPALETTE EZTAPI TWAIN_CreateDibPalette (HANDLE hdib)
// given a pointer to a locked DIB, returns a handle to a plausible logical
// palette to be used for rendering the DIB
// For 24-bit dibs, returns a default palette.
{
    HPALETTE            hPalette = NULL;

	LPBITMAPINFOHEADER	lpbmi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

    if (lpbmi) {
	    WORD nColors = TWAIN_DibNumColors(hdib);			// size of DIB palette
		WORD nEntries = nColors ? nColors : 256;			// size of palette to create
		// allocate logical palette structure
	    LOGPALETTE *pPal = (LOGPALETTE*)LocalAlloc(LPTR,
					sizeof(LOGPALETTE) + nEntries * sizeof(PALETTEENTRY));
		if (pPal) {
			// Fill in the palette entries
			pPal->palNumEntries = nEntries;
			pPal->palVersion    = 0x300;			// Windows 3.0 or later
			if (nColors) {
				// from the DIB color table
				// Get a pointer to the color table
				RGBQUAD FAR *pRgb = (RGBQUAD FAR *)((LPSTR)lpbmi + (WORD)lpbmi->biSize);
			    WORD i;	
				// copy from DIB palette (triples, by the way) into the LOGPALETTE
				for (i = 0; i < nEntries; i++) {
					pPal->palPalEntry[i].peRed   = pRgb[i].rgbRed;
					pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
					pPal->palPalEntry[i].peBlue  = pRgb[i].rgbBlue;
					pPal->palPalEntry[i].peFlags = (BYTE)0;
				} // for
			} else {
				// Deep Dib: Synthesize halftone palette
				memcpy(pPal->palPalEntry, peStock256, sizeof peStock256);
			}
	
			// create a logical palette
			hPalette = CreatePalette(pPal);
			LocalFree((HANDLE)pPal);
		}
	
		GlobalUnlock(hdib);
	}
    return hPalette;
} // TWAIN_CreateDibPalette


LPBYTE DibBits(LPBITMAPINFOHEADER lpdib)
// Given a pointer to a locked DIB, return a pointer to the actual bits (pixels)
{
    DWORD dwColorTableSize = (DWORD)(DibNumColors(lpdib) * sizeof(RGBQUAD));
    LPBYTE lpBits = (LPBYTE)lpdib + lpdib->biSize + dwColorTableSize;

    return lpBits;
} // end DibBits


void EZTAPI TWAIN_DrawDibToDC(HDC hDC, INT_PTR dx, INT_PTR dy, INT_PTR w, INT_PTR h,
									 HANDLE hdib, INT_PTR sx, INT_PTR sy)
{
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
	if (lpbmi) {
#ifdef _WIN32
		HDRAWDIB hdd = DrawDibOpen();
		if (hdd) {
			DrawDibDraw(hdd, hDC,
						dx, dy, w, h,
						lpbmi, DibBits(lpbmi),
						sx, sy, w, h,
						0	
						);
			DrawDibClose(hdd);
		}
#else
		StretchDIBits(hDC,
					dx, dy,			// x,y destination
					w, h,			// pixel width & height in destination
					sx, sy,			// x,y source
					w, h,			// width & height in source
					DibBits(lpbmi), // pointer to 'bits' - the pixels
					(LPBITMAPINFO)lpbmi,
					DIB_RGB_COLORS,	// DIB palette is (presumably?) RGB entries
					SRCCOPY);		// raster operation (copy)
#endif
		GlobalUnlock(hdib);
	}
} // TWAIN_DrawDibToDC


///////////////////////////////////////////////////////////////////////
// TWAIN general


INT_PTR EZTAPI TWAIN_IsAvailable(void)
// return 1 if TWAIN services are available, 0 if 'TWAIN-less' system
{
	if (pDSM_Entry) return TRUE;		// SM currently loaded

	if (iAvailable == 0) {
		if (TWAIN_LoadSourceManager()) {
			iAvailable = 1;
			TWAIN_UnloadSourceManager();
		} else {
			iAvailable = -1;
		}
	}
	return (iAvailable > 0);
} // TWAIN_IsAvailable


INT_PTR EZTAPI TWAIN_EasyVersion(void)
// Returns the version number of EZTWAIN.DLL, multiplied by 100.
// So e.g. version 2.01 will return 201 from this call.
{
	return VERSION;
} // TWAIN_EasyVersion


INT_PTR EZTAPI TWAIN_State(void)
// Returns the TWAIN Protocol State per the spec.
{
	return nState;
} 

INT_PTR EZTAPI TWAIN_GetHideUI(void)
{
	return bHideUI;
} // TWAIN_GetHideUI

void EZTAPI TWAIN_SetHideUI(INT_PTR fHide)
{
	bHideUI = (fHide != 0);
} // TWAIN_SetHideUI


///////////////////////////////////////////////////////////////////////
// TWAIN State Changers

INT_PTR EZTAPI TWAIN_LoadSourceManager(void)
{
	char		szSMDir[128];
	INT_PTR			cc;
	OFSTRUCT	of;

	if (nState >= TWAIN_SM_LOADED) return TRUE;			// DSM already loaded

	GetWindowsDirectory(szSMDir, sizeof(szSMDir));
	cc = lstrlen(szSMDir);
	if (cc && szSMDir[cc-1] != ':') {
		lstrcat(szSMDir, "\\");
	}
	lstrcat(szSMDir, DSM_FILENAME);			// could crash!
	if (OpenFile(szSMDir, &of, OF_EXIST) != -1) {
		hDSMLib = LoadLibrary(szSMDir);
	} else {
		hDSMLib = NULL;
	}
	if (IsValidHandle(hDSMLib)) {
		pDSM_Entry = (DSMENTRYPROC) GetProcAddress(hDSMLib, DSM_ENTRYPOINT);
		if (pDSM_Entry) {
			iAvailable = 1;
			SetState(TWAIN_SM_LOADED);
		} else {
			FreeLibrary(hDSMLib);
			hDSMLib = NULL;
		}
	} else {
		pDSM_Entry = NULL;
	}
	
	return (nState >= TWAIN_SM_LOADED);
} // TWAIN_LoadSourceManager


static TW_INT32 hwnd32SM;

INT_PTR EZTAPI TWAIN_OpenSourceManager(HWND hwnd)
{
   hwnd32SM = (TW_INT32)GetValidHwnd(hwnd);
   if (nState < TWAIN_SM_OPEN) {
	   if (TWAIN_LoadSourceManager() &&
		   TWAIN_Mgr(DG_CONTROL, DAT_PARENT, MSG_OPENDSM, &hwnd32SM)) {
		   assert(nState == TWAIN_SM_OPEN);
	   }
   }
   return (nState >= TWAIN_SM_OPEN);
} // TWAIN_OpenSourceManager


INT_PTR EZTAPI TWAIN_OpenDefaultSource(void)
{
   static TW_IDENTITY twid;
   if (nState < TWAIN_SOURCE_OPEN) {
      if (nState < TWAIN_SM_OPEN && !TWAIN_OpenSourceManager(NULL)) {
         return FALSE;
      }

	   // open the system default source
      FMEMSET(&twid, 0, sizeof twid);
	   if (TWAIN_Mgr(DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, &twid)) {
		   assert(nState == TWAIN_SOURCE_OPEN);
           TWAIN_SetXferMech(TWSX_NATIVE);
	   }
   }

	return (nState == TWAIN_SOURCE_OPEN);
} // TWAIN_OpenDefaultSource


INT_PTR EZTAPI TWAIN_EnableSource(HWND hwnd)
{
   if (nState < TWAIN_SOURCE_OPEN && !TWAIN_OpenDefaultSource()) {
      return FALSE;
   }
	twUI.ShowUI = !bHideUI;
	twUI.hParent = (TW_HANDLE)GetValidHwnd(hwnd);
	TWAIN_DS(DG_CONTROL, DAT_USERINTERFACE, MSG_ENABLEDS, &twUI);
	return (nState == TWAIN_SOURCE_ENABLED);
} // TWAIN_EnableSource


INT_PTR EZTAPI TWAIN_DisableSource(void)
{
	if (nState == TWAIN_SOURCE_ENABLED &&
		TWAIN_DS(DG_CONTROL, DAT_USERINTERFACE, MSG_DISABLEDS, &twUI)) {
		assert(nState == TWAIN_SOURCE_OPEN);
	}
	return (nState <= TWAIN_SOURCE_OPEN);
} // TWAIN_DisableSource


INT_PTR EZTAPI TWAIN_CloseSource(void)
{
	rc = TWRC_SUCCESS;

	if (nState == TWAIN_SOURCE_ENABLED) TWAIN_DisableSource();
	if (nState == TWAIN_SOURCE_OPEN &&
		TWAIN_Mgr(DG_CONTROL, DAT_IDENTITY, MSG_CLOSEDS, &SourceId)) {
		assert(nState == TWAIN_SM_OPEN);
	}	
	return (nState <= TWAIN_SM_OPEN);
} // TWAIN_CloseSource


INT_PTR EZTAPI TWAIN_CloseSourceManager(HWND hwnd)
{


	if (nState > TWAIN_SM_OPEN) {
		TWAIN_CloseSource();
	}
	if (nState == TWAIN_SM_OPEN) {
		hwnd32SM = (TW_INT32)GetValidHwnd(hwnd);
		rc = TWRC_SUCCESS;
		if (TWAIN_Mgr(DG_CONTROL, DAT_PARENT, MSG_CLOSEDSM, &hwnd32SM)) {
			assert(nState == TWAIN_SM_LOADED);
		}
	}
	// REB 2/28/13 #35165 Let's try clearing this here.
	DestroyWindow(hwndProxy);
	hwndProxy = NULL; 

	return (nState <= TWAIN_SM_LOADED);
} // TWAIN_CloseSourceManager


INT_PTR EZTAPI TWAIN_UnloadSourceManager(void)
{
   TWAIN_CloseSourceManager(NULL);
	if (nState == TWAIN_SM_LOADED) {
		if (hDSMLib) {
			FreeLibrary(hDSMLib);
			hDSMLib = NULL;
		}
		pDSM_Entry = NULL;
		SetState(TWAIN_PRESESSION);
	}
	return (nState == TWAIN_PRESESSION);
} // TWAIN_UnloadSourceManager


void EZTAPI TWAIN_ModalEventLoop(void)
{
	MSG msg;
   // Clear global breakout flag
   bBreakModalLoop = FALSE;

#ifdef EZTPRO_H
   if (bAutoScan) {
      PostMessage(NULL, WM_AUTOSCAN, 0, 0);
   }
#endif

	while ((nState >= TWAIN_SOURCE_ENABLED)
       && !bBreakModalLoop
       && GetMessage((LPMSG)&msg, NULL, 0, 0)) {

      if (!TWAIN_MessageHook ((LPMSG)&msg)) {
			TranslateMessage ((LPMSG)&msg);
			DispatchMessage ((LPMSG)&msg);
		}
   } // while
   bBreakModalLoop = FALSE;
} // TWAIN_ModalEventLoop


void EZTAPI TWAIN_BreakModalLoop(void)
{
   bBreakModalLoop = TRUE;
} // TWAIN_BreakModalLoop


static void NativeXferHandler(void)
{
    TW_UINT32		hNative = 0;
    
    hdibLastRcvd = NULL;
    assert(nState == TWAIN_TRANSFER_READY);
    if (TWAIN_DS(DG_IMAGE, DAT_IMAGENATIVEXFER, MSG_GET, &hNative)) {
        hdibLastRcvd = (HANDLE)hNative;
    }
} // NativeXferHandler


#ifndef EZTPRO_H

static void FileXferHandler(void)
{
   TWAIN_ErrorBox("EZTW: File Transfer mode not supported.");
} // FileXferHandler

static void MemoryXferHandler(void)
{
   TWAIN_ErrorBox("EZTW: Memory Transfer mode not supported.");
} // MemoryXferHandler

#endif

static void DoOneTransfer(void)
{
   switch (TWAIN_XferMech()) {
   case XFERMECH_NATIVE:
      NativeXferHandler();
      break;
   case XFERMECH_FILE:
      FileXferHandler();
      break;
   case XFERMECH_MEMORY:
      MemoryXferHandler();
      break;
   } // switch

   // If inside ModalEventLoop, break out
   TWAIN_BreakModalLoop();

   // Acknowledge transfer
   TWAIN_EndXfer();
	assert(nState == TWAIN_TRANSFER_READY ||
          nState == TWAIN_SOURCE_ENABLED);
} // DoOneTransfer


INT_PTR EZTAPI TWAIN_MessageHook(LPMSG lpmsg)
// returns TRUE if msg processed by TWAIN (source)
{
	INT_PTR   bProcessed = FALSE;

#ifdef EZTPRO_H
   if (lpmsg->message == WM_AUTOSCAN && lpmsg->hwnd == NULL) {
      // Time to try auto-button-press again
      hwndButton = FindScanButton();
      if (hwndButton) {
         PressButton(hwndButton);
         bAutoScan = FALSE;
      }
      if (bAutoScan) {
         PostMessage(NULL, WM_AUTOSCAN, 0, 0);
      }
      return TRUE;
   }
#endif

   if (nState >= TWAIN_SOURCE_ENABLED) {
      // source enabled
		TW_EVENT	twEvent;
		twEvent.pEvent = (TW_MEMREF)lpmsg;
		twEvent.TWMessage = MSG_NULL;
		// see if source wants to process (eat) the message
		TWAIN_DS(DG_CONTROL, DAT_EVENT, MSG_PROCESSEVENT, &twEvent);
		bProcessed = (rc == TWRC_DSEVENT);
		switch (twEvent.TWMessage) {
			case MSG_XFERREADY:
				assert(nState == TWAIN_TRANSFER_READY);
            DoOneTransfer();
				break;
			case MSG_CLOSEDSREQ:
				TWAIN_DisableSource();
				break;
			case MSG_NULL:
				// no message returned from DS
				break;
		} // switch
    }
	return bProcessed;
} // TWAIN_MessageHook


INT_PTR EZTAPI TWAIN_EndXfer(void)
{
   if (nState == TWAIN_TRANSFERRING) {
		TWAIN_DS(DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER, &pendingXfers);
	}
   return (nState < TWAIN_TRANSFERRING);
} // TWAIN_EndXfer


INT_PTR EZTAPI TWAIN_AbortAllPendingXfers(void)
{
   TWAIN_EndXfer();
   if (nState == TWAIN_TRANSFER_READY) {
		TWAIN_DS(DG_CONTROL, DAT_PENDINGXFERS, MSG_RESET, &pendingXfers);
	}
	return (nState <= TWAIN_SOURCE_ENABLED);
} // TWAIN_AbortAllPendingXfers


///////////////////////////////////////////////////////////////////////
// DIB/BMP File I/O

INT_PTR EZTAPI TWAIN_WriteNativeToFilename(HANDLE hdib, LPCSTR pszFile)
// Writes a DIB handle to a .BMP file
//
// hdib		= DIB handle, as returned by TWAIN_AcquireNative
// pszFile	= far pointer to NUL-terminated filename
// If pszFile is NULL or points to a null string, prompts the user
// for the filename with a standard file-save dialog.
//
// Return values:
//	 0	success
//	-1	user cancelled File Save dialog
//	-2	file open error (invalid path or name, or access denied)
//	-3	(weird) unable to lock DIB - probably an invalid handle.
//	-4	writing BMP data failed, possibly output device is full
{
	INT_PTR result;
	char szFile[256];
	HFILE fh;
	OFSTRUCT ofs;

	if (!pszFile || !*pszFile) {
		// prompt for filename
		OPENFILENAME ofn;
		INT_PTR nExt;
		
		FMEMSET(&ofn, 0, sizeof ofn);
		szFile[0] = '\0';
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "Windows Bitmap (*.bmp)\0*.bmp\0\0";
		ofn.lpstrFile= szFile;
		ofn.nMaxFile = sizeof(szFile) - 5;
		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY |
					OFN_NOREADONLYRETURN;
	
		if (!GetSaveFileName(&ofn)) {
			return -1;					// user cancelled dialog
		}
		// supply default extension - GetSaveFileName doesn't seem to do it!
		nExt = ofn.nFileExtension;
		if (nExt && !szFile[nExt]) {
			// no extension
			lstrcat(szFile, ".bmp");
		}
		pszFile = szFile;
	}

	result = -2;
	fh = OpenFile(pszFile , &ofs, OF_CREATE | OF_WRITE | OF_SHARE_EXCLUSIVE);
	if (fh != HFILE_ERROR) {
		result = TWAIN_WriteNativeToFile(hdib, fh);
		_lclose(fh);
	}
	return result;
} // TWAIN_WriteNativeToFilename


INT_PTR WriteDibToFile(LPBITMAPINFOHEADER lpDIB, HFILE fh)
{
	BITMAPFILEHEADER		bfh;
	INT_PTR						fOk = FALSE;
	INT_PTR						nBPP = lpDIB->biBitCount;
	INT_PTR						nColors = (INT_PTR)lpDIB->biClrUsed;

	// figure out actual size of color table
	if (nColors == 0 && nBPP <= 8) {
		nColors = (1 << nBPP);
	}
	if (lpDIB->biCompression == BI_RGB) {
		// uncompressed bitmap, image size might not be set
		DWORD dwBytesPerRow = (((lpDIB->biWidth * nBPP) + 31) / 32) * 4;
		lpDIB->biSizeImage = dwBytesPerRow * lpDIB->biHeight;
	} else if (lpDIB->biSizeImage == 0) {
		// compressed bitmap, image size had damn well better be set!
		return FALSE;
		// This could be hacked around with something like this:
		//lpDIB->biSizeImage = GlobalSize((HANDLE)GlobalHandle(HIWORD(lpDIB)));
	}

	// Set up BMP header.
	bfh.bfType = 0x4d42;                // "BM"
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) +
					sizeof(BITMAPINFOHEADER) +
					sizeof(RGBQUAD) * nColors;
	bfh.bfSize = bfh.bfOffBits + lpDIB->biSizeImage;

	if (_lwrite(fh, (LPCSTR)&bfh, sizeof bfh) == sizeof bfh) {

		INT32 towrite = bfh.bfSize - (INT32)sizeof bfh;

		if (HUGEWRITE(fh, (LPCSTR)lpDIB, towrite) == towrite) {
			fOk = TRUE;
		}
	}

	return fOk;

} // WriteDibToFile

INT_PTR EZTAPI TWAIN_WriteNativeToFile(HANDLE hdib, HFILE fh)
// Writes a DIB to a file in .BMP format.
//
// hdib		= DIB handle, as returned by TWAIN_AcquireNative
// fh		= file handle, as returned by C _open or Windows _lopen or OpenFile
//
// Return value as for TWAIN_WriteNativeToFilename
{
	INT_PTR result = -3;
	LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
	if (lpbmi) {
		result = -4;
		if (WriteDibToFile(lpbmi, fh)) {
			result = 0;			// success
		}
		GlobalUnlock(hdib);
	}
	return result;
} //  TWAIN_WriteNativeToFile


HANDLE EZTAPI TWAIN_LoadNativeFromFilename(LPCSTR pszFile)
// Load a .BMP file and return a DIB handle (as from AcquireNative.)
//
// pszFile	= far pointer to NUL-terminated filename
// If pszFile is NULL or points to a null string, prompts the user
// for the filename with a standard file-open dialog.
//
// Return value:
//	handle to a DIB if successful, otherwise NULL (0).
{
	HANDLE hdib = NULL;
	char szFile[256];
	HFILE fh;
	OFSTRUCT ofs;

	if (!pszFile || !*pszFile) {
		// prompt for filename
		OPENFILENAME ofn;
		INT_PTR nExt;
	
		FMEMSET(&ofn, 0, sizeof ofn);
		szFile[0] = '\0';
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "Windows Bitmaps (*.bmp)\0*.bmp\0\0";
		ofn.lpstrFile= szFile;
		ofn.nMaxFile = sizeof(szFile) - 5;
		ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY |
					OFN_NOREADONLYRETURN;
	
		if (!GetOpenFileName(&ofn)) {
			return NULL;					// user cancelled dialog
		}
		// supply default extension - GetOpenFileName doesn't seem to do it!
		nExt = ofn.nFileExtension;
		if (nExt && !szFile[nExt]) {
			// no extension
			lstrcat(szFile, ".bmp");
		}
		pszFile = szFile;
	}

	fh = OpenFile(pszFile, &ofs, OF_READ | OF_SHARE_DENY_WRITE);
	if (fh != HFILE_ERROR) {
		hdib = TWAIN_LoadNativeFromFile(fh);
		_lclose(fh);
	}
	return hdib;
} // TWAIN_LoadNativeFromFilename


HANDLE EZTAPI TWAIN_LoadNativeFromFile(HFILE fh)
// Like LoadNativeFromFilename, but takes an already open file handle.
{
	HANDLE hdib;
	LPBYTE pbi;
	BITMAPFILEHEADER bmh;
	INT32 dibSize;
	// Read BMP file header and validate
	if (_lread(fh, &bmh, sizeof bmh) != sizeof bmh ||
		bmh.bfType != 0x4d42) {
		return NULL;
	}
	// Allocate global block for DIB
	dibSize = bmh.bfSize - sizeof bmh;
	hdib = GlobalAlloc(0, dibSize);
	pbi = (LPBYTE)GlobalLock(hdib);
	if (!hdib || !pbi) {
		return NULL;
	}

	// Read DIB from file
	if (_hread(fh, pbi, dibSize) != dibSize) {
		GlobalUnlock(hdib);
		GlobalFree(hdib);
		return NULL;
	}
	GlobalUnlock(hdib);
	return hdib;
} // TWAIN_LoadNativeFromFile

///////////////////////////////////////////////////////////////////////
// TWAIN State 4 Negotiation Functions

INT_PTR EZTAPI TWAIN_NegotiateXferCount(INT_PTR nXfers)
{
	return TWAIN_SetCapOneValue(CAP_XFERCOUNT, TWTY_INT16, nXfers);
} // TWAIN_NegotiateXferCount


INT_PTR EZTAPI TWAIN_NegotiatePixelTypes(unsigned wPixTypes)
{
	TW_CAPABILITY 		cap;
	void far *			pv;
	INT_PTR					fSuccess = FALSE;

	if (nState != TWAIN_SOURCE_OPEN) {
		return RecordError(ED_NOT_STATE_4);
	}

	if (TWAIN_ANYTYPE == wPixTypes) {
		return TRUE;			// that was easy!
	}
                                                                           
                                                                           
	// Fill in capability structure
	cap.Cap = ICAP_PIXELTYPE;			// capability id
	cap.ConType = TWON_ENUMERATION;		// favorite type of container (should be ignored...)

	if (!TWAIN_DS(DG_CONTROL, DAT_CAPABILITY, MSG_GET, (TW_MEMREF)&cap)) {
		return RecordError(ED_CAP_GET);
	}
	if (!cap.hContainer) {
		return RecordError(ED_NULL_HCON);
	}
	if (!(pv = GlobalLock(cap.hContainer))) {
		// this source is invalid, further negotiation is unlikely to succeed
		return RecordError(ED_BAD_HCON);
	}

	switch (cap.ConType) {

		case TWON_ENUMERATION: {
			TW_ENUMERATION far *pcon = (TW_ENUMERATION far *)pv;
			if (pcon->NumItems < 1) {
				RecordError(ED_CAP_GET_EMPTY);
			} else if (pcon->ItemType != TWTY_UINT16 && pcon->ItemType != TWTY_INT16) {
            // We can only handle the two types
				RecordError(ED_BAD_ITEMTYPE);
			} else {
            TW_UINT16 FAR * plist = (TW_UINT16 FAR *)pcon->ItemList;
            TW_UINT32 nItems = pcon->NumItems;
            TW_UINT32 nCurrent = pcon->CurrentIndex;
            TW_UINT32 nDefault = pcon->DefaultIndex;
            TW_UINT16 uCurrentVal = plist[nCurrent < nItems ? nCurrent : 0];
            TW_UINT16 uDefaultVal = plist[nDefault < nItems ? nDefault : 0];
            nItems = Intersect(wPixTypes, nItems, plist);
            pcon->NumItems = nItems;
            pcon->CurrentIndex = FindIndex16(nItems, plist, uCurrentVal, 0);
            pcon->DefaultIndex = FindIndex16(nItems, plist, uDefaultVal, 0);
				fSuccess = (pcon->NumItems != 0);
				if (!fSuccess) RecordError(ED_CAP_SET_EMPTY);
			}
			break;
		}
		
		case TWON_ARRAY: {
			// this is technically illegal - TWAIN 1.5, p9-30, Containers for MSG_GET: TW_ENUMERATION, TW_ONEVALUE
			TW_ARRAY far *pcon = (TW_ARRAY far *)pv;
			if (pcon->NumItems < 1) {
				RecordError(ED_CAP_GET_EMPTY);
			} else if (pcon->ItemType != TWTY_UINT16 && pcon->ItemType != TWTY_INT16) {
				RecordError(ED_BAD_ITEMTYPE);
			} else {
				pcon->NumItems = Intersect(wPixTypes, (unsigned)pcon->NumItems, (TW_UINT16 far *)pcon->ItemList);
				fSuccess = (pcon->NumItems != 0);
				if (!fSuccess) RecordError(ED_CAP_SET_EMPTY);
			}
			break;
		}
		
		case TWON_ONEVALUE: {
			TW_ONEVALUE far *pcon = (TW_ONEVALUE far *)pv;
			fSuccess = ((1 << pcon->Item) & wPixTypes);
			if (!fSuccess) RecordError(ED_CAP_SET_EMPTY);
			break;
		}
		
		default:
			// something we don't understand, abandon negotiations
			RecordError(ED_BAD_CONTYPE);
			break;
	} // switch

	GlobalUnlock(cap.hContainer);

	if (fSuccess) {
		// For enums (and arrays) send intersection back, to restrict it.
		// For one vals, don't bother - could only cause confusion.
		if (cap.ConType != TWON_ONEVALUE) {
			fSuccess = TWAIN_DS(DG_CONTROL, DAT_CAPABILITY, MSG_SET, (TW_MEMREF)&cap);
			if (!fSuccess) RecordError(ED_CAP_SET);
		}
	}
	GlobalFree(cap.hContainer);

   if (fSuccess) {
      OutputDebugString("EZTW:Pixel Types negotiated.\n");
   }
	return fSuccess;
} // TWAIN_NegotiatePixelTypes


INT_PTR EZTAPI TWAIN_GetCurrentUnits(void)
{
	INT_PTR nUnits = TWUN_INCHES;
	TWAIN_GetCapCurrent(ICAP_UNITS, TWTY_UINT16, &nUnits);
	return nUnits;
} // TWAIN_GetCurrentUnits


INT_PTR EZTAPI TWAIN_SetCurrentUnits(INT_PTR nUnits)
// Negotiate the current pixel type for acquisition.
// Negotiation is only allowed in State 4 (TWAIN_SOURCE_OPEN)
// The source may select this pixel type, but don't assume it will.
{
	return TWAIN_SetCapOneValue(ICAP_UNITS, TWTY_UINT16, (TW_UINT16)nUnits);
} // TWAIN_SetCurrentUnits


INT_PTR EZTAPI TWAIN_GetBitDepth(void)
// Ask the source for the current bitdepth.
// This value depends on the current PixelType.
// Bit depth is per color channel e.g. 24-bit RGB has bit depth 8.
// If anything goes wrong, this function returns 0.
{
	INT_PTR nBits = 0;
	TWAIN_GetCapCurrent(ICAP_BITDEPTH, TWTY_UINT16, &nBits);
	return nBits;
} // TWAIN_GetBitDepth

INT_PTR EZTAPI TWAIN_SetBitDepth(INT_PTR nBits)
// (Try to) set the current bitdepth (for the current pixel type).
{
	return TWAIN_SetCapOneValue(ICAP_BITDEPTH, TWTY_UINT16, (TW_UINT16)nBits);
} // TWAIN_SetBitDepth


INT_PTR EZTAPI TWAIN_GetPixelType(void)
// Ask the source for the current pixel type.
// If anything goes wrong (it shouldn't!), this function returns 0 (TWPT_BW).
{
	INT_PTR nPixType = 0;
	TWAIN_GetCapCurrent(ICAP_PIXELTYPE, TWTY_UINT16, &nPixType);
	return nPixType;
} // TWAIN_GetPixelType

INT_PTR EZTAPI TWAIN_SetCurrentPixelType(INT_PTR nPixType)
// Negotiate the current pixel type for acquisition.
// Negotiation is only allowed in State 4 (TWAIN_SOURCE_OPEN)
// The source may select this pixel type, but don't assume it will.
{
	return TWAIN_SetCapOneValue(ICAP_PIXELTYPE, TWTY_UINT16, (TW_UINT16)nPixType);
} // TWAIN_SetCurrentPixelType


/////////////////////////////////////////////////////////////////////////////
// ICAP_XRESOLUTION

double EZTAPI TWAIN_GetCurrentResolution(void)
{
	TW_FIX32 res;
	TWAIN_GetCapCurrent(ICAP_XRESOLUTION, TWTY_FIX32, &res);
	return Fix32ToFloat(res);
} // TWAIN_GetCurrentResolution

double EZTAPI TWAIN_GetYResolution(void)
// Returns the current vertical resolution, in dots per *current unit*.
// In the event of failure, returns 0.0.
{
	TW_FIX32 res;
	TWAIN_GetCapCurrent(ICAP_YRESOLUTION, TWTY_FIX32, &res);
	return Fix32ToFloat(res);
} // TWAIN_GetYResolution


INT_PTR EZTAPI TWAIN_SetCurrentResolution(double dRes)
// Negotiate the current resolution for acquisition.
// Negotiation is only allowed in State 4 (TWAIN_SOURCE_OPEN)
// The source may select this resolution, but don't assume it will.
{
	return TWAIN_SetCapOneValue(ICAP_YRESOLUTION, TWTY_FIX32, TWAIN_ToFix32(dRes)) &&
          TWAIN_SetCapOneValue(ICAP_XRESOLUTION, TWTY_FIX32, TWAIN_ToFix32(dRes));
} // TWAIN_SetCurrentResolution


/////////////////////////////////////////////////////////////////////////////
// ICAP_CONTRAST

INT_PTR EZTAPI TWAIN_SetContrast(double dCon)
// (Try to) set the current contrast for acquisition.
// The TWAIN standard says that the range for this cap is -1000 ... +1000
{
	return TWAIN_SetCapOneValue(ICAP_CONTRAST, TWTY_FIX32, TWAIN_ToFix32(dCon));
}


/////////////////////////////////////////////////////////////////////////////
// ICAP_BRIGHTNESS

INT_PTR EZTAPI TWAIN_SetBrightness(double dBri)
// (Try to) set the current brightness for acquisition.
// The TWAIN standard says that the range for this cap is -1000 ... +1000
{
	return TWAIN_SetCapOneValue(ICAP_BRIGHTNESS, TWTY_FIX32, TWAIN_ToFix32(dBri));
}


/////////////////////////////////////////////////////////////////////////////
// ICAP_XFERMECH

INT_PTR EZTAPI TWAIN_XferMech(void)
{
	TW_UINT16 mech = XFERMECH_NATIVE;
	TWAIN_GetCapCurrent(ICAP_XFERMECH, TWTY_UINT16, &mech);
	return mech;
} // TWAIN_XferMech

INT_PTR EZTAPI TWAIN_SetXferMech(INT_PTR mech)
{
	return TWAIN_SetCapOneValue(ICAP_XFERMECH, TWTY_UINT16, (TW_UINT16)mech);
} // TWAIN_SetXferMech


/////////////////////////////////////////////////////////////////////////////
// TW_FIX32 conversions

static void DoubleToFix32(double r, TW_FIX32* pfix)
{
   // Note 1: This round-away-from-0 is new in TWAIN 1.7
   // Note 2: ANSI C converts float to INT_PTR by truncating toward 0.
	TW_INT32 val = (TW_INT32)(r * 65536.0 + (r < 0 ? -0.5 : +0.5));
	pfix->Whole = (TW_INT16)(val >> 16);			// most significant 16 bits
	pfix->Frac = (TW_UINT16)(val & 0xffff);		// least
} // DoubleToFix32


LONG_PTR EZTAPI TWAIN_ToFix32(double r)
{
	TW_FIX32 fix;
	assert(sizeof (TW_FIX32) == sizeof (LONG_PTR));
   DoubleToFix32(r, &fix);
	return *(LONG_PTR*)&fix;
} // TWAIN_ToFix32


double EZTAPI TWAIN_Fix32ToFloat(LONG_PTR nfix)
{
   TW_FIX32 fix;
   TW_INT32 val;
   *(LONG_PTR*)&fix = nfix;
	val = ((TW_INT32)fix.Whole << 16) | ((TW_UINT32)fix.Frac & 0xffff);
	return val / 65536.0;
} // TWAIN_Fix32ToFloat


INT_PTR EZTAPI TWAIN_SetCapOneValue(unsigned Cap, unsigned ItemType, LONG_PTR ItemVal)
{
	TW_CAPABILITY	cap;
	pTW_ONEVALUE	pv;
	BOOL			bSuccess;

	if (nState != TWAIN_SOURCE_OPEN) {
		TWAIN_ErrorBox("Attempt to set capability outside State 4.");
		return FALSE;
	}

	cap.Cap = Cap;			// capability id
	cap.ConType = TWON_ONEVALUE;		// container type
	cap.hContainer = GlobalAlloc(GHND, sizeof (TW_ONEVALUE));
	if (!cap.hContainer) {
		TWAIN_ErrorBox(szInsuffMem);
		return FALSE;
	}
	pv = (pTW_ONEVALUE)GlobalLock(cap.hContainer);
	pv->ItemType = ItemType;
	pv->Item = ItemVal;
	GlobalUnlock(cap.hContainer);
	bSuccess = TWAIN_DS(DG_CONTROL, DAT_CAPABILITY, MSG_SET, (TW_MEMREF)&cap);
	GlobalFree(cap.hContainer);
	return bSuccess;
} // TWAIN_SetCapOneValue


const size_t nTypeSize[13] =
	{	sizeof (TW_INT8),
		sizeof (TW_INT16),
		sizeof (TW_INT32),
		sizeof (TW_UINT8),
		sizeof (TW_UINT16),
		sizeof (TW_UINT32),
		sizeof (TW_BOOL),
		sizeof (TW_FIX32),
		sizeof (TW_FRAME),
		sizeof (TW_STR32),
		sizeof (TW_STR64),
		sizeof (TW_STR128),
		sizeof (TW_STR255),
	};

// helper function:
INT_PTR TypeMatch(unsigned nTypeA, unsigned nTypeB)
{
	// Integral types match if they are the same size.
	// All other types match only if they are equal
	return nTypeA == nTypeB ||
		   (nTypeA <= TWTY_UINT32 &&
		    nTypeB <= TWTY_UINT32 &&
		    nTypeSize[nTypeA] == nTypeSize[nTypeB]);
} // TypeMatch



INT_PTR EZTAPI TWAIN_GetCapCurrent(unsigned Cap, unsigned ItemType, void FAR *pVal)
{
	TW_CAPABILITY 	cap;
	void far *		pv = NULL;
	BOOL			bSuccess = FALSE;

	assert(pVal != NULL);

	if (nState < TWAIN_SOURCE_OPEN) {
		TWAIN_ErrorBox("Attempt to get capability value below State 4.");
		return FALSE;
	}

	// Fill in capability structure
	cap.Cap = Cap;					// capability id
	cap.ConType = TWON_ONEVALUE;	// favorite type of container (should be ignored...)
	cap.hContainer = NULL;

	if (TWAIN_DS(DG_CONTROL, DAT_CAPABILITY, MSG_GETCURRENT, (TW_MEMREF)&cap) &&
	    cap.hContainer &&
	    (pv = GlobalLock(cap.hContainer))) {

		if (cap.ConType == TWON_ENUMERATION) {
			TW_ENUMERATION far *pcon = (TW_ENUMERATION far *)pv;
			TW_UINT32 index = pcon->CurrentIndex;
			if (index < pcon->NumItems && TypeMatch(pcon->ItemType, ItemType)) {
				LPSTR pitem = (LPSTR)pcon->ItemList + index*nTypeSize[ItemType];
				FMEMCPY(pVal, pitem, nTypeSize[ItemType]);
				bSuccess = TRUE;
			}
		} else if (cap.ConType == TWON_ONEVALUE) {
			TW_ONEVALUE far *pcon = (TW_ONEVALUE far *)pv;
			if (TypeMatch(pcon->ItemType, ItemType)) {
				FMEMCPY(pVal, &pcon->Item, nTypeSize[ItemType]);
				bSuccess = TRUE;
			}
		}
	}
	
	if (pv) GlobalUnlock(cap.hContainer);
	if (cap.hContainer) GlobalFree(cap.hContainer);

	return bSuccess;
} 

//-------------------------- The primitive functions


INT_PTR EZTAPI TWAIN_DS(ULONG_PTR dg, unsigned dat, unsigned msg, void FAR *pd)
// Call the current source with a triplet
{
   INT_PTR bOk = FALSE;
   assert(nState >= TWAIN_SOURCE_OPEN);
   rc = TWRC_FAILURE;
   if (dg == DG_IMAGE) {
      if (dat == DAT_IMAGEMEMXFER) {
         if (msg == MSG_GET && pd != NULL) {
            pTW_IMAGEMEMXFER pmxb = (pTW_IMAGEMEMXFER)pd;
            pmxb->Compression   = TWON_DONTCARE16;
            pmxb->BytesPerRow   = TWON_DONTCARE32;
            pmxb->Columns       = TWON_DONTCARE32;
            pmxb->Rows          = TWON_DONTCARE32;
            pmxb->XOffset       = TWON_DONTCARE32;
            pmxb->YOffset       = TWON_DONTCARE32;
            pmxb->BytesWritten  = TWON_DONTCARE32;
         }
      }
   }
   if (pDSM_Entry) {
      rc = (*pDSM_Entry)(&AppId, &SourceId,
						   (TW_UINT32)dg,
                     (TW_UINT16)dat,
                     (TW_UINT16)msg,
                     (TW_MEMREF)pd);
      bOk = (rc == TWRC_SUCCESS);

      if (dg == DG_CONTROL) {
         if (dat == DAT_EVENT) {
            if (msg == MSG_PROCESSEVENT) {
               if (((pTW_EVENT)pd)->TWMessage == MSG_XFERREADY) {
                  SetState(TWAIN_TRANSFER_READY);
               }
               bOk = (rc == TWRC_DSEVENT);
            }
         }
         if (dat == DAT_PENDINGXFERS) {
            if (msg == MSG_ENDXFER && bOk) {
               SetState(((pTW_PENDINGXFERS)pd)->Count ? TWAIN_TRANSFER_READY
                                                      : TWAIN_SOURCE_ENABLED);
            }
            if (msg == MSG_RESET && bOk) {
               SetState(TWAIN_SOURCE_ENABLED);
            }
         }
         if (dat == DAT_USERINTERFACE) {
            if (msg == MSG_ENABLEDS) {
               if (rc == TWRC_FAILURE) {
		            RecordError(ED_DS_FAILURE);
               } else if (rc == TWRC_CANCEL) {
				   bOk = FALSE;
			   } else {
				   // rc could be either SUCCESS or CHECKSTATUS
				   SetState(TWAIN_SOURCE_ENABLED);
				   bOk = TRUE;
               }
            }
            if (msg == MSG_DISABLEDS && bOk) {
               SetState(TWAIN_SOURCE_OPEN);
            }
         }
         if (dat == DAT_SETUPMEMXFER) {
            if (msg == MSG_GET && bOk) {
               nMemBuffer = 0;
            }
         }

      }
      if (dg == DG_IMAGE) {
         if (dat == DAT_IMAGENATIVEXFER || dat == DAT_IMAGEFILEXFER) {
            // Native and File transfers work much the same way.
            if (msg == MSG_GET) {
               bOk = (rc == TWRC_XFERDONE);
               switch (rc) {
               case TWRC_XFERDONE:
               case TWRC_CANCEL:
                  SetState(TWAIN_TRANSFERRING);
                  // Need to acknowledge end of transfer with
                  // DG_CONTROL / DAT_PENDINGXFERS / MSG_ENDXFER
                  break;
                  
               case TWRC_FAILURE:
               default:
                  // Transfer failed (e.g. insufficient memory, write-locked file)
                  // check condition code for more info
                  SetState(TWAIN_TRANSFER_READY);
                  // The image is still pending
                  break;
               } // switch
            }
         }
         if (dat == DAT_IMAGEMEMXFER) {
            if (msg == MSG_GET) {
               bOk = FALSE;
               switch (rc) {
               case TWRC_SUCCESS:
               case TWRC_XFERDONE:
                  bOk = TRUE;
                  nMemBuffer++;
                  SetState(TWAIN_TRANSFERRING);
                  break;
               case TWRC_FAILURE:
                  // "If the failure occurred during the transfer of the first
                  // buffer, the session is in State 6. If the failure occurred
                  // on a subsequent buffer, the session is in State 7."
                  SetState(nMemBuffer == 0 ? TWAIN_TRANSFER_READY : TWAIN_TRANSFERRING);
                  break;
               case TWRC_CANCEL:
                  // Transfer cancelled, no state change.
                  TWAIN_BreakModalLoop();
                  break;
               } // switch
            }
         }
      }
   }
   return bOk;
} // TWAIN_DS



INT_PTR EZTAPI TWAIN_Mgr(ULONG_PTR dg, unsigned dat, unsigned msg, void FAR *pd)
// Call the Source Manager with a triplet
{
   INT_PTR bOk = FALSE;
	rc = TWRC_FAILURE;
	if (pDSM_Entry) {
		rc = (*pDSM_Entry)(&AppId, NULL,
						   (TW_UINT32)dg,
						   (TW_UINT16)dat,
						   (TW_UINT16)msg,
						   (TW_MEMREF)pd);
   	bOk = (rc == TWRC_SUCCESS);
      if (dg == DG_CONTROL) {
         if (dat == DAT_IDENTITY) {
            if (msg == MSG_OPENDS) {
               if (bOk) {
                  // Source opened - record identity for future triplets
                  FMEMCPY(&SourceId, pd, sizeof (TW_IDENTITY));
                  SetState(TWAIN_SOURCE_OPEN);
               } else {
            		RecordError(ED_DSM_FAILURE);
               }
            }
            if (msg == MSG_CLOSEDS && bOk) {
               SetState(TWAIN_SM_OPEN);
            }
         }
         if (dat == DAT_PARENT) {
            if (msg == MSG_OPENDSM && bOk) {
               SetState(TWAIN_SM_OPEN);
            }
            if (msg == MSG_CLOSEDSM && bOk) {
               SetState(TWAIN_SM_LOADED);
            }
         }
      }
	}
	return bOk;
} // TWAIN_Mgr



unsigned EZTAPI TWAIN_GetResultCode(void)
{
	return rc;
} // TWAIN_GetResultCode



unsigned EZTAPI TWAIN_GetConditionCode(void)
{
	TW_STATUS	twStatus;
   TW_INT16    rcLast = rc;

	twStatus.ConditionCode = TWCC_BUMMER;			// what can I say. 

	if (nState >= 4) {
		// get source status if open
		TWAIN_DS(DG_CONTROL, DAT_STATUS, MSG_GET, (TW_MEMREF)&twStatus);
	} else if (nState == 3) {
		// otherwise get source manager status
		TWAIN_Mgr(DG_CONTROL, DAT_STATUS, MSG_GET, (TW_MEMREF)&twStatus);
	}
   rc = rcLast;
   return twStatus.ConditionCode;
} // TWAIN_GetConditionCode


void EZTAPI TWAIN_ReportLastError(LPCSTR pzMsg)
{
   char szMsg[256];			// scratch buffer for messages
	if (nErrDetail > ED_START_TRIPLET_ERRS && nErrDetail < ED_END_TRIPLET_ERRS) {
		wsprintf(szMsg, "%s\n%s\nRC:   %s\nCC:   %s",
			pzMsg, (LPSTR)pszErrDescrip[nErrDetail],
			(LPSTR)pszRC[nErrRC], (LPSTR)pszCC[nErrCC]);
	} else {
		wsprintf(szMsg, "%s\n%s", pzMsg, (LPSTR)pszErrDescrip[nErrDetail]);
	}
   if (nErrRC == TWRC_FAILURE && nErrCC == TWCC_OPERATIONERROR) {
      // don't report this to user - DS has already reported it.
      OutputDebugString("EZTW: TWRC_FAILURE, TWCC_OPERATIONERROR\n");
   } else {
	   TWAIN_ErrorBox(szMsg);
   }
} // ReportLastError


void EZTAPI TWAIN_ErrorBox(LPCSTR pzMsg)
{
	MessageBox(NULL, pzMsg, "TWAIN Error", MB_ICONEXCLAMATION | MB_OK);
} // TWAIN_ErrorBox


//------------ Private functions

void SetState(INT_PTR n)
{
   char szMsg[256];
   wsprintf(szMsg, "EZTW:State %d\n", n);
   OutputDebugString(szMsg);
   nState = n;
} // SetState


HWND GetValidHwnd(HWND hwnd)
// Returns a valid window handle as follows:
// If hwnd is a valid window handle, hwnd is returned.
// Otherwise a proxy window handle is created and returned.
// Once created, a proxy window handle is destroyed when
// the source manager is unloaded.
// If hwnd is an invalid window handle (other than NULL)
// an error box is displayed.
{
 	if (!IsWindow(hwnd)) {
 		if (hwnd != NULL) {
 			TWAIN_ErrorBox("EZTWAIN: window handle is invalid");
         hwnd = NULL;
 		}
      if (!hwndProxy) {
	      hwndProxy = CreateProxyWindow();
 		   if (!IsWindow(hwndProxy)) {
			   TWAIN_ErrorBox("EZTWAIN: Unable to create proxy window");
            hwndProxy = NULL;
 		   }
      }
      hwnd = hwndProxy;
 	}
   return hwnd;
} // GetValidHwnd


HWND CreateProxyWindow(void)
{
	HWND hwnd;
	hwnd = CreateWindow("STATIC",                // class
						"Acquire Proxy",              // title
						WS_POPUPWINDOW,               // style
						CW_USEDEFAULT, CW_USEDEFAULT, // x, y
						CW_USEDEFAULT, CW_USEDEFAULT, // width, height
						HWND_DESKTOP,                 // parent window
						NULL,                         // hmenu
						hinstLib,                     // hinst
						NULL);                        // lpvparam
	return hwnd;
} // CreateProxyWindow


INT_PTR RecordError(ErrorDetail ed)
{
	if (nErrDetail == ED_NONE) {
		nErrDetail = ed;
		if (ed > ED_START_TRIPLET_ERRS && ed < ED_END_TRIPLET_ERRS) {
			nErrRC = TWAIN_GetResultCode();
			nErrCC = TWAIN_GetConditionCode();
		} else {
			nErrRC = 0;
			nErrCC = 0;
		}
	}
	return FALSE;
} // RecordError


void ClearError(void)
{
	nErrDetail = ED_NONE;
} // ClearError


unsigned BitCount(unsigned W)
{
	unsigned n = 0;

	while (W) {
		n += (W & 1);
		W >>= 1;
	} // while
	return n;
} // BitCount


TW_UINT32 Intersect(TW_UINT32 wMask, TW_UINT32 nItems, TW_UINT16 far *pItem)
{
	TW_UINT32	wSet = 0;
	unsigned	i;

	// In wSet, construct set of available items.
	// Note that items that cannot be represented in wMask are also
	// unrepresentable in wSet so are implicitly discarded
	for (i = 0 ; i < nItems; i++) {
		wSet |= 1 << pItem[i];
	} // for

	// Discard anything in wMask that isn't in wSet
	wMask &= wSet;

	// Re-fill the item table with intersection set
	for (nItems = i = 0; wMask ; wMask>>=1,i++) {
		if (wMask & 1) {
			pItem[nItems++] = i;
		}
	} // for

	return nItems;
} // Intersect


unsigned FindIndex16(TW_UINT32 nItems, TW_UINT16 far *plist, TW_UINT16 uVal, unsigned nDefault)
// Find the index of uVal in the list plist which contains nItems entries.
// If not found, return nDefault.
{
   unsigned i;
   for (i = 0; i < nItems; i++) {
      if (plist[i] == uVal) {
         return i;
      }
   }
   return nDefault;
} // FindIndex16


double Fix32ToFloat(TW_FIX32 fix)
{
	TW_INT32 val = ((TW_INT32)fix.Whole << 16) | ((TW_UINT32)fix.Frac & 0xffff);
	return val / 65536.0;
} // Fix32ToFloat

//--------- END ---------
