// WJF 6/17/16 Win-18 

#include "4DPlugin.h"
#include "utilities.h"

#define _SECOND ((ULONGLONG) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)

HANDLE hLogFile = NULL;
BOOL bLogIsOpen = FALSE;

// Open the log file
void openLogFile() {
	char logFilePath[MAX_PATH];
	char commandName[256] = { '\0' };
	char cName[256] = { '\0' };
	PA_Unistring unistring;
	PA_Variable logsPath;
	PA_Unistring uniLogs;
	char * szLogsPath = NULL;
	SYSTEMTIME lt;
	char buffer[8];
	HRESULT hr = 0;

	PA_GetCommandName(485, commandName);

	int j = 0;

	// Get the full command name. A for loop is needed because PA_GetCommandName returns the command name with a null character between each character. (Ex. - "D,\0,O,\0,C,\0..). 
	// The for loop extracts the null character. Without the for loop, you will be unable to use the string returned by PA_GetCommand, as only the first character will be returned since the next character is null.
	for (int i = 0; i < sizeof(commandName); i++) // WJF 6/29/15 #43134 Changed <= to <
	{
		if (commandName[i] != '\0')
		{
			cName[j] = commandName[i];
			j++;
		}
	}

	cName[(strlen(cName))] = '\0';

	strcat_s(cName, 256, "(Logs folder)");

	unistring = CStringToUnistring(cName);

	logsPath = PA_ExecuteFunction(&unistring);

	uniLogs = PA_GetStringVariable(logsPath);

	szLogsPath = UnistringToCString(&uniLogs);

	if (szLogsPath != NULL) {
		strcpy_s(logFilePath, MAX_PATH, szLogsPath);
		free(szLogsPath);
		szLogsPath = NULL;

		strcat_s(logFilePath, MAX_PATH, "Win32API\\");

		CreateDirectory(logFilePath, NULL);

		logMaintenance(logFilePath);

		strcat_s(logFilePath, MAX_PATH, "Win32API_");

		GetLocalTime(&lt);

		_itoa_s(lt.wYear, buffer, 8, 10);
		strcat_s(logFilePath, MAX_PATH, buffer);

		_itoa_s(lt.wMonth, buffer, 8, 10);
		strcat_s(logFilePath, MAX_PATH, buffer);

		_itoa_s(lt.wDay, buffer, 8, 10);
		strcat_s(logFilePath, MAX_PATH, buffer);

		strcat_s(logFilePath, MAX_PATH, ".log");

		hLogFile = CreateFile(logFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hLogFile != INVALID_HANDLE_VALUE) {
			bLogIsOpen = TRUE;
		}
	}
	
}

// Close the log file
void closeLogFile(){
	if (hLogFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hLogFile);
	}
}

// Write to the log file
void writeLogFile(const char * szLog){
	DWORD dwPos = 0;
	DWORD dwBytesWritten = 0;
	SYSTEMTIME localTime;
	char szOutput[1024];
	size_t logLength = 0;
	char buffer[8];

	GetLocalTime(&localTime);

	_itoa_s(localTime.wMonth, buffer, 8, 10);
	strcpy_s(szOutput, 1024, buffer);
	strcat_s(szOutput, 1024, "/");
	_itoa_s(localTime.wDay, buffer, 8, 10);
	strcat_s(szOutput, 1024, buffer);
	strcat_s(szOutput, 1024, "/");
	_itoa_s(localTime.wYear, buffer, 8, 10);
	strcat_s(szOutput, 1024, buffer);
	strcat_s(szOutput, 1024, " ");
	_itoa_s(localTime.wHour, buffer, 8, 10);
	strcat_s(szOutput, 1024, buffer);
	strcat_s(szOutput, 1024, ":");
	_itoa_s(localTime.wMinute, buffer, 8, 10);
	strcat_s(szOutput, 1024, buffer);
	strcat_s(szOutput, 1024, ":");
	_itoa_s(localTime.wSecond, buffer, 8, 10);
	strcat_s(szOutput, 1024, buffer);
	strcat_s(szOutput, 1024, ":");
	_itoa_s(localTime.wMilliseconds, buffer, 8, 10);
	strcat_s(szOutput, 1024, buffer);
	strcat_s(szOutput, 1024, " - ");
	strcat_s(szOutput, 1024, szLog);

	logLength = strlen(szOutput);

	if ((hLogFile != INVALID_HANDLE_VALUE) && (bLogIsOpen)) {
		dwPos = SetFilePointer(hLogFile, 0, NULL, FILE_END);
		LockFile(hLogFile, dwPos, 0, logLength, 0);
		WriteFile(hLogFile, szOutput, logLength, &dwBytesWritten, NULL);
		UnlockFile(hLogFile, dwPos, 0, logLength, 0);
	}
}

// Delete log files older than 30 days
void logMaintenance(const char * szLogDir){
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER fileSize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = ERROR_SUCCESS;
	FILETIME ftCurrent;
	SYSTEMTIME stCurrent;
	FILETIME ftFile;
	ULONGLONG qwCurrent = 0;
	ULONGLONG qwFile = 0;
	ULONGLONG qwResult = 0;
	char searchPath[MAX_PATH];
	char deletePath[MAX_PATH];

	strcpy_s(searchPath, MAX_PATH, szLogDir);
	strcat_s(searchPath, MAX_PATH, "*.*");

	GetSystemTimeAsFileTime(&ftCurrent);

	qwCurrent = (((ULONGLONG)ftCurrent.dwHighDateTime) << 32) + ftCurrent.dwLowDateTime;

	hFind = FindFirstFile(searchPath, &ffd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			ftFile = ffd.ftCreationTime;
			qwFile = (((ULONGLONG)ftFile.dwHighDateTime) << 32) + ftFile.dwLowDateTime;
			qwResult = qwCurrent - qwFile;

			// Delete files older than 30 days
			if (qwResult >= (30 * _DAY)){
				strcpy_s(deletePath, MAX_PATH, szLogDir);
				strcat_s(deletePath, MAX_PATH, ffd.cFileName);
				DeleteFile(deletePath);
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);
}