#ifndef LOGGING_H
#define LOGGING_H

#include "4DPlugin.h"

// WJF 6/17/16 Win-18 Logging
// WJF 7/8/16 Win-20 Moved to their own header
void sys_LoggingStart(PA_PluginParameters params);
void sys_LoggingStop(PA_PluginParameters params);
void writeLogFile(const char * strLog);
LONG logMaintenance(); // WJF 7/11/16 Win-20
LONG logOpenFile(); // WJF 7/11/16 Win-20
LONG logCloseFile(); // WJF 7/11/16 Win-20

#endif