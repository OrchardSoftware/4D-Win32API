// Mark De Wever #12225
/* 
** Definition of the messages used.
*/

struct message_list {
	char *InvalidOperation;
	char *InvalidShowConstant;
	char *FileNotFound;
	char *PathNotFound;
	char *BadFormat;
	char *AccessDenied;
	char *AssocIncomplete;
	char *DDEFail;
	char *DDETimeOut;
	char *DLLNotFound;
	char *NoAssoc;
	char *OOM;
	char *ShareViolation;
	char *Default;
};

/* messages in English */
static struct message_list message_list_english = { 
	"Invalid Operation",								// InvalidOperation
	"Invalid HowToShow Constant",						// InvalidShowConstant
	"File Not Found",									// FileNotFound
	"Path Not Found",									// PathNotFound
	".EXE File is Invalid",								// BadFormat
	"OS Denied Access to File",							// AccessDenied
	"File Name Association is Incomplete or Invalid",	// AssocIncomplete
	"DDE Transaction Could Not be Completed",			// DDEFail
	"DDE Request Timed Out",							// DDETimeOut
	"DLL Libray Not Found",								// DLLNotFound
	"No Application Associated with File Extension",	// NoAssoc
	"Insufficient Memory",								// OOM
	"Sharing Violation Occurred",						// ShareViolation
	"Unknown error occurred"							// Default
};

/* messages in Dutch */
static struct message_list message_list_dutch = { 
	"Onjuiste actie",										// InvalidOperation
	"Onjuiste toon constante",								// InvalidShowConstant
	"Bestand niet gevonden",								// FileNotFound
	"Pad niet gevonden",									// PathNotFound
	".EXE bestand is beschadigd of onjuist",				// BadFormat
	"Toegang tot bestand is geweigerd",						// AccessDenied
	"Bestand heeft onjuiste programma associatie",			// AssocIncomplete
	"DDE transactie kon niet volledig uitgevoerd worden",	// DDEFail
	"DDE aanvraag timeout",									// DDETimeOut
	"DLL niet gevonden",									// DLLNotFound
	"Bestand heeft geen geassoceered programma",			// NoAssoc
	"Onvoldoende geheugen",									// OOM
	"Gedeelde bestand wordt al gebruikt",					// ShareViolation
	"Een onbekende fout heeft plaatsgevonden"				// Default
};

/* pointer to the proper message struct, default english */
struct message_list *message = &message_list_english;
