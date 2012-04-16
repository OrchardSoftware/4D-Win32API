
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
void sys_GetDocumentList( PA_PluginParameters params )
{
	LONG_PTR returnValue = 0;
	LONG_PTR fileCount = 0;
	LONG_PTR arraySize = 0;
    char fullPath[MAXBUF];
	WIN32_FIND_DATA fFindData;
	DWORD ret = 0;
	DWORD lastError = 0;
    HANDLE NextFind = 0;
	BOOL getAllFiles = FALSE;

	// parameter variables
	char *pathName = NULL;
	char *filePattern = NULL;
	LONG_PTR maxFilesToReturn = 0;
	PA_Variable paReturnFileList;

	// Get the function parameters.
	pathName = getTextParameter( params, 1 );
    filePattern = getTextParameter( params, 2 );
    paReturnFileList = PA_GetVariableParameter( params, 3 );
	maxFilesToReturn = PA_GetLongParameter( params, 4 );

	// Clear out the return array.
	PA_ResizeArray(&paReturnFileList, 0);
	
	if(pathName != NULL && filePattern != NULL)
	{
		// Check if the path is valid.
		ret = GetFileAttributes(pathName);

		if ((ret & FILE_ATTRIBUTE_DIRECTORY) != 0 && ret != INVALID_DIRECTORY)
		{
			// Build the full path name.
			if(filePattern[0] == '\0')
			{
				snprintf(fullPath, MAXBUF, "%s%s*.*", pathName, pathName[strlen(pathName) - 1] == PATHCHAR ? "" : PATHSTR);
			}
			else
			{
				snprintf(fullPath, MAXBUF, "%s%s%s", pathName, pathName[strlen(pathName) - 1] == PATHCHAR ? "" : PATHSTR, filePattern);
			}
			
			
			// Get the first file.
			NextFind = FindFirstFile(fullPath, &fFindData);
			
			if(NextFind != INVALID_HANDLE_VALUE)
			{
				// Loop through the files in the directory and create the list to return.
				lastError = ERROR_NO_MORE_FILES;
				getAllFiles = (maxFilesToReturn <= 0);
				
				while (NextFind != INVALID_HANDLE_VALUE && (fileCount < maxFilesToReturn || getAllFiles)) {        
					
					if ((fFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
					{
						fileCount++;   // Count the number of files that match the pattern.
						
						if(fileCount > arraySize)
						{
							// Increase the size of the array.
							arraySize = fileCount + ARRAY_LOAD_VALUE;
							PA_ResizeArray(&paReturnFileList, arraySize);
						}
						
						PA_SetTextInArray(paReturnFileList, fileCount, fFindData.cFileName, strlen(fFindData.cFileName));          
					}           
					
					if(!FindNextFile(NextFind, &fFindData))
					{
						lastError = GetLastError();
						break;
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

	if(lastError == ERROR_NO_MORE_FILES || lastError == ERROR_FILE_NOT_FOUND)
	{
		setError(0);
		PA_ResizeArray(&paReturnFileList, fileCount);
		returnValue = 1;
	}
	else
	{	
		setError(lastError);
		PA_ResizeArray(&paReturnFileList, 0);
	}
	
	PA_SetVariableParameter( params, 3, paReturnFileList, 0 );
	
	freeTextParameter(pathName);
	freeTextParameter(filePattern);

	PA_ReturnLong( params, returnValue );
}
