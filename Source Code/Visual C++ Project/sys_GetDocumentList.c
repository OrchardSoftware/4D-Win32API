#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"

#include <Windows.h>

//#include "psapi.h"  // for enumerating processes in NT & 2000
#include "utilities.h"

// ------------------------------------------------
//
//  FUNCTION: sys_GetDocumentList( PA_PluginParameters params )
//
//  PURPOSE:  Return a list of files in a directory.
//
//	DATE:	  MJG 6/4/04 (3.6)
//
//  UPDATES:  MJG 7/5/05 (3.6.2)  Updated to set the 4D variable, Error,
//								  when an error occurs.
//
//			  AMS 9/16/14 #40405 (6.4)  Updated to return the oldest entries first
//		   								 and take in a starting index for specifying
//										 where the list of returned files begins.
//
//
// ------------------------------------------------
void sys_GetDocumentList(PA_PluginParameters params)
{
	LONG lReturnValue = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG lFileCount = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG lArraySize = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG lEndIndex = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG lCount = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	const LONG lFileLimit = 1000;
	char fullPath[MAXBUF];
	WIN32_FIND_DATA fFindData;
	DWORD ret = 0;
	DWORD lastError = 0;
	HANDLE NextFind = 0;
	BOOL bGetAllFiles = FALSE;
	WIN32_FIND_DATA fileList[1000];

	// parameter variables
	char *patPathName = NULL;
	char *patFilePattern = NULL;
	LONG palMaxFilesToReturn = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG palFileSort = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	LONG palStartIndex = 0; // WJF 6/30/16 Win-21 LONG_PTR -> LONG
	PA_Variable paReturnFileList;

	// Get the function parameters.
	patPathName = getTextParameter(params, 1);
	patFilePattern = getTextParameter(params, 2);
	paReturnFileList = PA_GetVariableParameter(params, 3);
	palMaxFilesToReturn = PA_GetLongParameter(params, 4);
	palFileSort = PA_GetLongParameter(params, 5);  // AMS2 9/30/14 #40405  Moved the sort parameter to be before start index
	palStartIndex = PA_GetLongParameter(params, 6);  // AMS2 9/16/14 #40405

	// Clear out the return array.
	PA_ResizeArray(&paReturnFileList, 0);

	// AMS2 9/19/14 #40405 Passing in the start index is optional, if a value is passed in then it is assumed that the user put in a number relative to a starting index of 1 instead of 0 for c/c++ arrays
	if ((palStartIndex != 0) || (palStartIndex > 0)) // WJF 6/24/16 Win-21 NULL -> 0
	{
		palStartIndex--;
	}
	else{
		palStartIndex = 0;
	}

	// WJF 4/7/15 #41624 If they haven't chosen a valid sort option, set to default
	if ((palFileSort != 1) && (palFileSort != 2))
	{
		palFileSort = 0;
	}

	if (patPathName != NULL && patFilePattern != NULL)
	{
		// Check if the path is valid.
		ret = GetFileAttributes(patPathName);

		if ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0 && ret != INVALID_DIRECTORY)
		{
			// Build the full path name.
			if (patFilePattern[0] == '\0')
			{
				_snprintf_s(fullPath, sizeof(fullPath), MAXBUF, "%s%s*.*", patPathName, patPathName[strlen(patPathName) - 1] == PATHCHAR ? "" : PATHSTR);  // ZRW 4/12/17 WIN-39 snprintf -> _snprintf_s
			}
			else
			{
				_snprintf_s(fullPath, sizeof(fullPath), MAXBUF, "%s%s%s", patPathName, patPathName[strlen(patPathName) - 1] == PATHCHAR ? "" : PATHSTR, patFilePattern);  // ZRW 4/12/17 WIN-39 snprintf -> _snprintf_s
			}

			// Get the first file.
			NextFind = FindFirstFile(fullPath, &fFindData);

			if (NextFind != INVALID_HANDLE_VALUE)
			{
				// AMS2 9/18/14 #40405  Loop through the files in the directory and build a list to sort. Currently the max files that can be stored from a directory is 1000.
				lastError = ERROR_NO_MORE_FILES;
				bGetAllFiles = (palMaxFilesToReturn <= 0);

				while ((NextFind != INVALID_HANDLE_VALUE) && (lFileCount < lFileLimit))
				{
					if ((fFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
					{
						// AMS2 9/19/14 #40405 When a file match is found, insert it into the file list array.
						fileList[lFileCount] = fFindData;
						lFileCount++;   // Count the number of files that match the pattern.
					}
					if (!FindNextFile(NextFind, &fFindData))
					{
						lastError = GetLastError();
						break;
					}
				} // end while

				// AMS2 9/18/14 #40405 Sort the array using compareFileTimeCreation which takes in two win32 find data variables and compares their creation dates.
				if (palFileSort == 1)
				{
					qsort(fileList, lFileCount, sizeof(WIN32_FIND_DATA), (int(*)(const void*, const void*))compareFileTimeCreation);
				}
				else if (palFileSort == 2) // WJF 4/7/15 #41624 Sorty by alphabetical order
				{
					qsort(fileList, lFileCount, sizeof(WIN32_FIND_DATA), (int(*)(const void*, const void*))compareAlphabetical);
				}

				// AMS2 9/18/14 #40405 Get all of the files if 0 is passed for the max files parameter.
				if (palMaxFilesToReturn == 0)
				{
					palMaxFilesToReturn = lFileCount;
				}

				// AMS2 9/18/14 #40405 If the start index or end index is out of range, then set them to the file count.
				if (palStartIndex > lFileCount)
				{
					palStartIndex = lFileCount;
				}

				lEndIndex = palMaxFilesToReturn + palStartIndex;

				if (lEndIndex > lFileCount)
				{
					lEndIndex = lFileCount;
				}

				// AMS2 9/22/14 #40405 As long as the end index is within the file limit, insert the requested file names into the return array
				// starting at the specified start index  and end at the start index + the max number of files to return.
				if (lEndIndex <= lFileLimit)
				{
					for (int i = palStartIndex; i < lEndIndex; i++)
					{
						lCount++;
						if (lCount > lArraySize)
						{
							// Increase the size of the array.
							lArraySize = lCount + ARRAY_LOAD_VALUE;
							PA_ResizeArray(&paReturnFileList, lArraySize);
						}

						PA_SetTextInArray(paReturnFileList, lCount, fileList[i].cFileName, strlen(fileList[i].cFileName));
					}
				}
			}
			else
			{
				lastError = GetLastError();
			}

			FindClose(NextFind);
		}
		else
		{
			lastError = GetLastError();
		}
	}

	if (lastError == ERROR_NO_MORE_FILES || lastError == ERROR_FILE_NOT_FOUND)
	{
		setError(0);
		PA_ResizeArray(&paReturnFileList, lCount);  // AMS2 9/22/14 #40405 When the file selection has finished with no more files in the dir or a file has not been found, resize the return array to the number of files inserted into the array.
		lReturnValue = 0;  // SDL 11/20/17 WIN-54 Was 1. This isn't technically a function error. These errors can be interpretted by the array size. 
	}
	else
	{
		setError(lastError);
		PA_ResizeArray(&paReturnFileList, 0);
	}

	PA_SetVariableParameter(params, 3, paReturnFileList, 0);

	freeTextParameter(patPathName);
	freeTextParameter(patFilePattern);

	PA_ReturnLong(params, lReturnValue);
}