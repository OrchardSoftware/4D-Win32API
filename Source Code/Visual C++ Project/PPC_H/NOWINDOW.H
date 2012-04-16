// QDDefs.h needs these for every file so don't worry about them
// #define NOGDI	      
// #define NOTEXTMETRIC      
// #define NOCOLOR	      
// #define NOMSG	      

// NT wants this for whenever WINDOWSX.H is included
// #define NOWINMESSAGES     

#define NOGDICAPMASKS     
#define NOVIRTUALKEYCODES 
#define NOWINSTYLES       
#define NOSYSMETRICS      
#define NOMENUS	      
#define NOICONS	      
#define NOKEYSTATES       
#define NOSYSCOMMANDS     
#define NORASTEROPS       
#define NOSHOWWINDOW      
#define OEMRESOURCE       
#define NOATOM	      
#define NOCLIPBOARD       
#if 0
#define NOCTLMGR
#endif	      
#define NODRAWTEXT	      
#define NOKERNEL	      
#define NOMB	      
#define NOMEMMGR
#define NOMETAFILE	      
#define NOMINMAX	      
#define NOOPENFILE	      
#define NOSCROLL	      
#define NOSOUND	      
#define NOWH	      
#define NOWINOFFSETS      
#define NOCOMM	      
#define NOKANJI	      
#define NOHELP            
#define NOPROFILER	      
#define NODEFERWINDOWPOS
// rid of winnls.h 'CompareString' : macro redefinition warning  
#define NONLS

#if Compile4DLL
//#define NewHandle newpoigneeAltura
#endif
