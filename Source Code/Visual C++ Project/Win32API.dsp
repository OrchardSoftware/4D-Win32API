# Microsoft Developer Studio Project File - Name="Win32API" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Win32API - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Win32API.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Win32API.mak" CFG="Win32API - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32API - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32API - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Projects/Plugins/Win32API/Source", MBGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32API - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Objs/Release"
# PROP Intermediate_Dir "Objs/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DPLUGINVCPROJECT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "4D Plugin API" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DPLUGIN_EXPORTS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rpcrt4.lib IphlpApi.lib ppc_h\asintppc.lib Winmm.lib Comctl32.lib Wininet.lib /nologo /dll /machine:I386 /out:"WIN4DX/Win32API.4DX"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Win32API - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Objs/Debug"
# PROP Intermediate_Dir "Objs/Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DPLUGINVCPROJECT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /I "4D Plugin API" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MY4DPLUGIN_EXPORTS" /D Compile4DLL=1 /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib rpcrt4.lib IphlpApi.lib ppc_h\asintppc.lib Winmm.lib Comctl32.lib Wininet.lib /nologo /dll /debug /machine:I386 /out:"C:\8.0 Databases\Plugins\Win32API.bundle\Contents\Windows\Win32API.4DX" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Win32API - Win32 Release"
# Name "Win32API - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Commands"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\sys_GetDocumentList.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\4DPlugin.c

!IF  "$(CFG)" == "Win32API - Win32 Release"

!ELSEIF  "$(CFG)" == "Win32API - Win32 Debug"

# SUBTRACT CPP /YX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\4DPlugin040202.c
# End Source File
# Begin Source File

SOURCE=.\4DPlugin082102.c
# End Source File
# Begin Source File

SOURCE=.\4DPlugin2.c
# End Source File
# Begin Source File

SOURCE=.\linkedList.c
# End Source File
# Begin Source File

SOURCE=.\Registry.c
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.c
# End Source File
# Begin Source File

SOURCE=.\Utilities.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\4DPlugin.h
# End Source File
# Begin Source File

SOURCE=.\Defines.h
# End Source File
# Begin Source File

SOURCE=.\Utilities.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\COMDLG32.RC
# End Source File
# End Group
# Begin Group "4D Plugin API"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\4D Plugin API\4DPluginAPI.c"
# End Source File
# Begin Source File

SOURCE=".\4D Plugin API\4DPluginAPI.def"
# End Source File
# Begin Source File

SOURCE=".\4D Plugin API\4DPluginAPI.h"
# End Source File
# Begin Source File

SOURCE=".\4D Plugin API\EntryPoints.h"
# End Source File
# Begin Source File

SOURCE=".\4D Plugin API\Flags.h"
# End Source File
# Begin Source File

SOURCE=".\4D Plugin API\PrivateTypes.h"
# End Source File
# Begin Source File

SOURCE=".\4D Plugin API\PublicTypes.h"
# End Source File
# End Group
# End Target
# End Project
