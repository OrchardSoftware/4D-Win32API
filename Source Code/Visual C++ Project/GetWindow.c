/* REB 2/20/09 #19122
These two functions are used to retrieve the main MDI window and
the client MDI window.
*/

#include "4DPluginAPI.h"
#include "PrivateTypes.h"
#include "EntryPoints.h"
#include <Windows.h> 
#include "asiextrn.h"

HWND GetMainWindow()
{
 M2W* m2w=(M2W*)ASI_SystemValue(ASISV_CMD_GET,ASISV_gl_M2W,0);
 return m2w->m2w_hMainWnd;
}

HWND GetMDIClientWindow()
{
 M2W* m2w=(M2W*)ASI_SystemValue(ASISV_CMD_GET,ASISV_gl_M2W,0);
 return m2w->m2w_hDeskWnd;
}

