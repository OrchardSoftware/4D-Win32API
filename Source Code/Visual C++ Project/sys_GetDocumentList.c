
#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"

#include <Windows.h>
//#include <stdlib.h>
//#include <vector>


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
//			  AMS2 9/16/14 #40405 (6.4)  Updated to return the oldest entries first 
//		   								 and take in a starting index for specifying 
//										 where the list of returned files begins.			
//
//
// ------------------------------------------------
void sys_GetDocumentList(PA_PluginParameters params)
{
	LONG_PTR returnValue = 0;
	LONG_PTR fileCount = 0;
	LONG_PTR arraySize = 0;
	LONG_PTR endIndex = 0;
	LONG_PTR count = 0;
	const LONG_PTR fileLimit = 1000;
	char fullPath[MAXBUF];
	WIN32_FIND_DATA fFindData;
	DWORD ret = 0;
	DWORD lastError = 0;
	HANDLE NextFind = 0;
	BOOL getAllFiles = FALSE;
	WIN32_FIND_DATA fileList[1000];
	//_vectorcall 
	// parameter variables
	char *pathName = NULL;
	char *filePattern = NULL;
	LONG_PTR maxFilesToReturn = 0;
	LONG_PTR startIndex = 0;
	PA_Variable paReturnFileList;


	// Get the function parameters.
	pathName = getTextParameter(params, 1);
	filePattern = getTextParameter(params, 2);
	paReturnFileList = PA_GetVariableParameter(params, 3);
	maxFilesToReturn = PA_GetLongParameter(params, 4);
	startIndex = PA_GetLongParameter(params, 5);  // AMS2 9/16/14 #40405

	// Clear out the return array.
	PA_ResizeArray(&paReturnFileList, 0);

	// AMS2 9/19/14 #40405 Passing in the start index is optional, if a value is passed in then it is assumed that the user put in a number relative to a starting index of 1 instead of 0 for c/c++ arrays
	if ((startIndex != NULL) || (startIndex > 0)){
		startIndex--;
	}
	else{
		startIndex = 0;
	}

	if (pathName != NULL && filePattern != NULL)
	{
		// Check if the path is valid.
		ret = GetFileAttributes(pathName);

		if ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0 && ret != INVALID_DIRECTORY)
		{
			// Build the full path name.
			if (filePattern[0] == '\0')
			{
				snprintf(fullPath, MAXBUF, "%s%s*.*", pathName, pathName[strlen(pathName) - 1] == PATHCHAR ? "" : PATHSTR);
			}
			else
			{
				snprintf(fullPath, MAXBUF, "%s%s%s", pathName, pathName[strlen(pathName) - 1] == PATHCHAR ? "" : PATHSTR, filePattern);
			}


			// Get the first file.
			NextFind = FindFirstFile(fullPath, &fFindData);

			if (NextFind != INVALID_HANDLE_VALUE)
			{
				// AMS2 9/18/14 #40405  Loop through the files in the directory and build a list to sort. Currently the max files that can be stored from a directory is 1000. 
				lastError = ERROR_NO_MORE_FILES;
				getAllFiles = (maxFilesToReturn <= 0);

				while ((NextFind != INVALID_HANDLE_VALUE) && (fileCount < fileLimit)) { 

					if ((fFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
					{
						// AMS2 9/19/14 #40405 When a file match is found, insert it into the file list array.
						fileList[fileCount] = fFindData;
						fileCount++;   // Count the number of files that match the pattern.

					}
					if (!FindNextFile(NextFind, &fFindData))
					{
						lastError = GetLastError();
						break;
					}
				} // end while

				// AMS2 9/18/14 #40405 Sort the array using compareFileTimeCreation which takes in two win32 find data variables and compares their creation dates.
				qsort(fileList, fileCount, sizeof(WIN32_FIND_DATA), (int(*)(const void*, const void*))compareFileTimeCreation);

				// AMS2 9/18/14 #40405 Get all of the files if 0 is passed for the max files parameter.
				if (maxFilesToReturn == 0)
				{
					maxFilesToReturn = fileCount;
				}

				// AMS2 9/18/14 #40405 If the start index or end index is out of range, then set them to the file count.
				if (startIndex > fileCount)
				{
					startIndex = fileCount;
				}

				endIndex = maxFilesToReturn + startIndex;

				if (endIndex > fileCount)
				{
					endIndex = fileCount;
				}

				// AMS2 9/22/14 #40405 As long as the end index is within the file limit, insert the requested file names into the return array 
				// starting at the specified start index  and end at the start index + the max number of files to return.
				if (endIndex <= fileLimit)
				{
					for (int i = startIndex; i < endIndex; i++)
					{
						count++;
						if (count > arraySize)
						{
							// Increase the size of the array.
							arraySize = count + ARRAY_LOAD_VALUE;
							PA_ResizeArray(&paReturnFileList, arraySize);
						}
						
						PA_SetTextInArray(paReturnFileList, count, fileList[i].cFileName, strlen(fileList[i].cFileName));
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
		PA_ResizeArray(&paReturnFileList, count);  // AMS2 9/22/14 #40405 When the file selection has finished with no more files in the dir or a file has not been found, resize the return array to the number of files inserted into the array.
		returnValue = 1;
	}
	else
	{
		setError(lastError);
		PA_ResizeArray(&paReturnFileList, 0);
	}

	PA_SetVariableParameter(params, 3, paReturnFileList, 0);

	freeTextParameter(pathName);
	freeTextParameter(filePattern);

	PA_ReturnLong(params, returnValue);
}