#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"

HKEY getRootKey(INT_PTR key)
{
	HKEY hRootKey;

	switch (key){
	case (GR_HKEY_CLASSES_ROOT) :
		hRootKey = HKEY_CLASSES_ROOT;
		break;

	case (GR_HKEY_CURRENT_USER) :
		hRootKey = HKEY_CURRENT_USER;
		break;

	case (GR_HKEY_DYN_DATA) :
		hRootKey = HKEY_DYN_DATA;
		break;

	case (GR_HKEY_LOCAL_MACHINE) :
		hRootKey = HKEY_LOCAL_MACHINE;
		break;

	case (GR_HKEY_USERS) :
		hRootKey = HKEY_USERS;
		break;

	case (GR_HKEY_CURRENT_CONFIG) :
		hRootKey = HKEY_CURRENT_CONFIG;
		break;

	case (GR_HKEY_PERFORMANCE_DATA) :
		hRootKey = HKEY_PERFORMANCE_DATA;
		break;

	default:
		hRootKey = HKEY_LOCAL_MACHINE;
	}

	return hRootKey;
}

// WJF 6/30/16 Win-21 INT_PTR -> LONG
LONG get4dRegType(INT_PTR key)
{
	switch (key) {
	case REG_BINARY:
		return GR_TYPE_BINARY;
		break;

	case REG_DWORD:
	case REG_DWORD_BIG_ENDIAN:
		return GR_TYPE_LONGINT;
		break;

	case REG_EXPAND_SZ:
	case REG_SZ:
		return GR_TYPE_TEXT;
		break;

	case REG_MULTI_SZ:
		return GR_TYPE_ARRAYTEXT;
		break;

	default:
		return 0;
	}
}

LONG regGetNumElements(char *pValue)
{
	LONG arrayElements = 0;

	do
	{
		if (*pValue == '\0')
			arrayElements++;
	} while (!(*(pValue++) == '\0' && *pValue == '\0'));

	return arrayElements;
}

void regExpandStr(char **pValue)
{
	char *tmpValue = NULL;
	DWORD expandDataSize = 0; // WJF 6/30/16 Win-21 INT_PTR -> DWORD
	DWORD retErr = 0; // WJF 6/30/16 Win-21 INT_PTR -> DWORD

	expandDataSize = ExpandEnvironmentStrings(*pValue, NULL, 0);

	if (expandDataSize > 0) {
		tmpValue = malloc(expandDataSize);
		memset(tmpValue, 0, expandDataSize);
		retErr = ExpandEnvironmentStrings(*pValue, tmpValue, expandDataSize);

		if (retErr != 0){
			free(*pValue);
			*pValue = tmpValue;
		}
	}
}