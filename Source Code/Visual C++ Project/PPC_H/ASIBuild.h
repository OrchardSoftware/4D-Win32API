/* 
*	ASIBuild.h
*	Copyright (c) 1998 Altura Software, Inc. All Rights Reserved.
*
*	cre 01/05/98 DDK - Created
*/

#define ASI_BuildVersion		353

#define ASI_BuildVersionStr		"353\0"


typedef struct
{
	long	size;
	short	version;
	char	datetimestr[64];

} BuildInfo;

short ASI_GetBuildVersion();
void  ASI_GetBuildInfo(BuildInfo* bi);
