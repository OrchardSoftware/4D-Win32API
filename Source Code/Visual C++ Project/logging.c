// WJF 6/17/16 Win-18

#include "4DPlugin.h"
#include "utilities.h"

#define _SECOND ((ULONGLONG) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)

HANDLE hLogFile = NULL;
BOOL bLogIsOpen = FALSE;
CHAR logFilePath[MAX_PATH];
CHAR szLogsPath[MAX_PATH]; // WJF 7/11/16 Win-20 NULL pointer -> MAX_PATH array and made global
LONG lNumDays = 0; // WJF 7/11/16 Win-20
CHAR dateOpened[16]; // WJF 7/11/16 Win-20

//  FUNCTION:   sys_LoggingStart(PA_PluginParameters params)
//
//  PURPOSE:	Start logging Win32API commands
//
//  COMMENTS:
//
//	DATE:		7/11/16 Win-20
void sys_LoggingStart(PA_PluginParameters params) {
	LONG returnValue = 0; // WJF 7/11/16 Win-20

	PA_GetTextParameter(params, 1, szLogsPath);
	lNumDays = PA_GetLongParameter(params, 2);
	
	returnValue = logOpenFile();

	PA_ReturnLong(params, returnValue); // WJF 7/11/16 Win-20
}

//  FUNCTION:   sys_LoggingStop(PA_PluginParameters params)
//
//  PURPOSE:	Stop logging Win32API commands
//
//  COMMENTS:
//
//	DATE:		WJF 7/11/16 Win-20
void sys_LoggingStop(PA_PluginParameters params){
	PA_ReturnLong(params, logCloseFile()); // WJF 7/11/16 Win-20 Return long and moved to common method
}

// Write to the log file
void writeLogFile(const char * szLog){
	DWORD dwPos = 0;
	DWORD dwBytesWritten = 0;
	SYSTEMTIME lt; // WJF 7/11/16 Win-20 localTime -> lt
	CHAR szOutput[128]; // WJF 7/11/16 Win-20 1024 -> 128
	DWORD logLength = 0; // WJF 6/30/16 Win-21 size_t -> DWORD
	// LPSTR dateString = NULL; // WJF 7/11/16 Win-20  // ZRW 2/13/17 WIN-39 Initialized but not referenced
	CHAR dateComp[16]; // WJF 7/11/16 Win-20

	if (bLogIsOpen) { // WJF 7/8/16 Win-20 Don't bother doing this if the log isn't open
		GetLocalTime(&lt);

		sprintf_s(dateComp, 16, "%04u%02u%02u", lt.wYear, lt.wMonth, lt.wDay);

		// WJF 7/11/16 Win-20 If the current date doesn't match the date of the open log file, close it and make a new one
		if (strcmp(dateComp, dateOpened) != 0) {
			logCloseFile();
			logOpenFile();
		}

		if (bLogIsOpen) { // WJF 7/11/16 Win-20 This might change if the day rolled over
			// MM/DD/YYYY H:M:S:MS - 
			// WJF 7/11/16 Win-20 Rewrote to be simpler
			sprintf_s(szOutput, 128, "%02u/%02u/%04u %02u:%02u:%02u:%03u - ", lt.wMonth, lt.wDay, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
			strcat_s(szOutput, sizeof(szOutput), szLog);  // ZRW 4/5/17 WIN-39 strcat -> strcat_s

			logLength = (DWORD)strlen(szOutput); // WJF 6/30/16 Win-21 Cast to DWORD

			//if ((hLogFile != INVALID_HANDLE_VALUE) && (bLogIsOpen)) { // WJF 7/8/16 Win-20 Removed unneccessary check
			dwPos = SetFilePointer(hLogFile, 0, NULL, FILE_END);
			LockFile(hLogFile, dwPos, 0, logLength, 0);
			WriteFile(hLogFile, szOutput, logLength, &dwBytesWritten, NULL);
			UnlockFile(hLogFile, dwPos, 0, logLength, 0);
			// }

		}
	}
}

//  FUNCTION:   logMaintenance(PA_PluginParameters params)
//
//  PURPOSE:	Deletes log files older than the specified number of days
//
//  COMMENTS:
//
//	DATE:		WJF 6/17/16 Win-18
//  WJF 7/8/16 Win-20 Now returns the number of deleted files
LONG logMaintenance() {
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	FILETIME ftCurrent;
	FILETIME ftFile;
	ULONGLONG qwCurrent = 0;
	ULONGLONG qwFile = 0;
	ULONGLONG qwResult = 0;
	char searchPath[MAX_PATH];
	char deletePath[MAX_PATH];
//	LONG lTextLength = 0;  // ZRW 2/13/17 WIN-39 Initialized but not referenced
	LONG lNumDeleted = 0;

	strcpy_s(searchPath, sizeof(searchPath), szLogsPath); // WJF 7/11/16 Win-20 parameter -> global  // ZRW 3/23/17 WIN-39 MAX_PATH -> sizeof(searchPath)
	strcat_s(searchPath, sizeof(searchPath), "*.*");  // ZRW 4/5/17 WIN-39 MAX_PATH -> sizeof(searchPath)

	GetSystemTimeAsFileTime(&ftCurrent);

	qwCurrent = (((ULONGLONG)ftCurrent.dwHighDateTime) << 32) + ftCurrent.dwLowDateTime;

	hFind = FindFirstFile(searchPath, &ffd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return lNumDeleted;
	}

	do {
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			ftFile = ffd.ftCreationTime;
			qwFile = (((ULONGLONG)ftFile.dwHighDateTime) << 32) + ftFile.dwLowDateTime;
			qwResult = qwCurrent - qwFile;

			// Delete files older than the specified number of days
			if (qwResult > (lNumDays * _DAY)){ // WJF 7/8/16 Win-20 30 -> lNumDays, >= -> >
				strcpy_s(deletePath, sizeof(deletePath), szLogsPath);  // ZRW 3/23/17 WIN-39 MAX_PATH -> sizeof(deletePath)
				strcat_s(deletePath, sizeof(deletePath), ffd.cFileName);  // ZRW 4/5/17 WIN-39 MAX_PATH -> sizeof(deletePath)
				if (DeleteFile(deletePath)) {
					lNumDeleted++; // WJF 7/11/16 Win-20
				}
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	return lNumDeleted;
}

// WJF 7/11/16 Win-20 Internal function so that we can switch files when the day rolls over
LONG logOpenFile() {
	SYSTEMTIME lt;
	LONG returnValue = 0;

	if (szLogsPath != NULL) {
		strcpy_s(logFilePath, sizeof(logFilePath), szLogsPath);  // ZRW 3/23/17 WIN-39 strcpy -> strcpy_s

		// strcat_s(logFilePath, MAX_PATH, "Win32API\\"); 

		CreateDirectory(logFilePath, NULL);

		if (lNumDays > 0) {
			logMaintenance();
		}

		strcat_s(logFilePath, sizeof(logFilePath), "Win32API_"); // ZRW 4/5/17 WIN-39 MAX_PATH -> sizeof(logFilePath)

		GetLocalTime(&lt);

		sprintf_s(dateOpened, 16, "%04u%02u%02u", lt.wYear, lt.wMonth, lt.wDay);
		strcat_s(logFilePath, sizeof(logFilePath), dateOpened);  // ZRW 4/5/17 WIN-39 MAX_PATH -> sizeof(logFilePath)

		strcat_s(logFilePath, sizeof(logFilePath), ".log");  // ZRW 4/5/17 WIN-39 MAX_PATH -> sizeof(logFilePath)

		hLogFile = CreateFile(logFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hLogFile != INVALID_HANDLE_VALUE) {
			bLogIsOpen = TRUE;
			returnValue = 1; // WJF 7/11/16 Win-20
		}
	}

	return returnValue;
}

// WJF 7/11/16 Win-20 Internal function so that we can switch files when the day rolls over
LONG logCloseFile() {
	LONG lReturnValue = -1; 

	if (hLogFile != INVALID_HANDLE_VALUE) {
		if (CloseHandle(hLogFile)) {
			lReturnValue = 0; 
			bLogIsOpen = FALSE;
		}
	}
	else { 
		lReturnValue = -2;
	}

	return lReturnValue;
}