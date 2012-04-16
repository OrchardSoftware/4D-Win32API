
#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"

// these are functions to create/maintian a singly-linked unordered list
// the struct has some extra members to handle things that crop
// up after the restrictWindow function.

void init_list (pLL *pSOL)
{
	
	*pSOL = NULL;
	return;
}


void clear_list(pLL *pSOL)
{

	pLL			pTemp;

	while(*pSOL != NULL)
	{
		pTemp = *pSOL;
		*pSOL = (*pSOL)->nextPtr;
		free(pTemp);
	}
	return;
}


BOOL search_list(pLL *pSOL, pLL *thisLink, pLL *previousLink, LONG_PTR dataPosition, LONG_PTR matchType, LONG_PTR* pSearchKey)
{
	BOOL				returnValue = FALSE;

	*previousLink = NULL;
	*thisLink = *pSOL;
	while((*thisLink != NULL) && (!returnValue))
	{
		switch (dataPosition)
		{
		case LL_hWnd :
			if (LL_Restrict == matchType) {
				if ((*thisLink)->hWnd != (HWND) *pSearchKey) {
					*previousLink = *thisLink;
					*thisLink = (*thisLink)->nextPtr;
				} else {
					returnValue = TRUE;
				}
			}
			break;
		
		case LL_wpProc :
			if (LL_Restrict == matchType) {
				if ((*thisLink)->wpProc != (WNDPROC) *pSearchKey) {
					*previousLink = *thisLink;
					*thisLink = (*thisLink)->nextPtr;
				} else {
					returnValue = TRUE;
				}
			}
			break;

		case LL_type :
			// Future but not likely.  This is here in case list stores stuff for different functions
			break;

		case LL_dataLong1 :
			// Future
			break;

		case LL_dataLong2 :
			// Future
			break;

		case LL_dataPtrStr :
			// Future
			break;

		case LL_dataPtr :
			// Future
			break;
		} // end switch
	} // end while

	return returnValue;
}


void* insert_list(pLL  *pSOL)
{
	pLL			pNewLink = NULL, thisLink = NULL, previousLink = NULL;

	thisLink = *pSOL;

	while (thisLink != NULL)
	{
		previousLink = thisLink;
		thisLink = thisLink->nextPtr;
	}

	pNewLink = malloc(sizeof(LL));
	if (pNewLink == NULL) {
		return NULL;
	} else if (previousLink == NULL) {
		pNewLink->nextPtr = thisLink;
		*pSOL = pNewLink;
		} else {
			previousLink->nextPtr = pNewLink;
			pNewLink->nextPtr = thisLink;
		}

	return pNewLink;
}



BOOL delete_list(pLL* pSOL, LONG_PTR dataPosition, LONG_PTR matchType, LONG_PTR *pSearchKey)
{
	BOOL			bFuncReturn = FALSE;
	pLL				thisLink = NULL, previousLink = NULL, deleteLink = NULL;

	bFuncReturn = search_list(pSOL, &thisLink, &previousLink, dataPosition, matchType, pSearchKey);
	if (bFuncReturn) {
		if ((*pSOL)->nextPtr == NULL) {
			free(*pSOL);
			pSOL = NULL;
		} else {
			thisLink = (*pSOL)->nextPtr;
			free(*pSOL);
		}
	} 
	return bFuncReturn;
}


LONG_PTR length_list(pLL pSOL)
{
	LONG_PTR			returnValue = 0;

	while (pSOL != NULL)
	{
		returnValue++;
		pSOL = pSOL->nextPtr;
	}

	return returnValue;
}
