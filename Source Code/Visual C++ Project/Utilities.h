#ifndef _UTILITIES_H
#define _UTILITIES_H

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "PrivateTypes.h"
#include "EntryPoints.h"

char * getTextParameter(PA_PluginParameters, short);
void freeTextParameter(char *);
int compareFileTimeCreation(const void* p1, const void* p2);
int compareAlphabetical(const void* p1, const void* p2);
void setError(LONG value); // MJG 7/5/05 #8372 // WJF 6/30/16 Win-21 LONG_PTR -> LONG
LONG PA_GetTextParameter(PA_PluginParameters, short, char*); // REB 3/22/11 #25290 // WJF 6/30/16 Win-21 LONG_PTR -> LONG
void PA_SetTextParameter(PA_PluginParameters, short, char*, LONG_PTR); // REB 3/22/11 #25290
void PA_SetTextInArray(PA_Variable, LONG, char*, LONG_PTR); // REB 3/28/11 #25290 // WJF 6/30/16 Win-21 LONG_PTR -> LONG
LONG PA_GetTextInArray(PA_Variable, LONG, char*); // REB 3/28/11 #25290 // WJF 6/29/16 Win-21 LONG_PTR -> LONG
void PA_ReturnText(PA_PluginParameters, char*, LONG_PTR); // REB 3/28/11 #25290
PA_Unistring CStringToUnistring(char* text); // REB 4/20/11 #27322
char* UnistringToCString(PA_Unistring* UnistringText); // REB 4/20/11 #27322
#endif