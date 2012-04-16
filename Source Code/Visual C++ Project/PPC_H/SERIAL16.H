#include <windows.h>

#pragma pack (1)

typedef struct tagDCB16
{
    unsigned char  Id;
    unsigned short BaudRate;
    unsigned char  ByteSize;
    unsigned char Parity;
    unsigned char StopBits;
    unsigned short RlsTimeout;
    unsigned short CtsTimeout;
    unsigned short DsrTimeout;

    unsigned short fBinary        :1;
    unsigned short fRtsDisable    :1;
    unsigned short fParity        :1;
    unsigned short fOutxCtsFlow   :1;
    unsigned short fOutxDsrFlow   :1;
    unsigned short fDummy         :2;
    unsigned short fDtrDisable    :1;

    unsigned short fOutX          :1;
    unsigned short fInX           :1;
    unsigned short fPeChar        :1;
    unsigned short fNull          :1;
    unsigned short fChEvt         :1;
    unsigned short fDtrflow       :1;
    unsigned short fRtsflow       :1;
    unsigned short fDummy2        :1;

    char XonChar;
    char XoffChar;
    short XonLim;
    short XoffLim;
    char PeChar;
    char EofChar;
    char EvtChar;
    unsigned short TxDelay;
} DCB16;
typedef DCB16 FAR* LPDCB16;


typedef struct tagCOMSTAT16 {   /* cmst                            */
    unsigned char status;
    unsigned short cbInQue;
    unsigned short cbOutQue;
} COMSTAT16;


#ifndef CSTF_CTSHOLD
#define CSTF_CTSHOLD    0x01
#endif
#ifndef CSTF_DSRHOLD
#define CSTF_DSRHOLD    0x02
#endif

#ifndef	CSTF_RLSDHOLD
#define CSTF_RLSDHOLD   0x04
#endif

#ifndef	CSTF_XOFFHOLD
#define CSTF_XOFFHOLD   0x08
#endif

#ifndef	CSTF_XOFFSENT
#define CSTF_XOFFSENT   0x10
#endif

#ifndef	CSTF_EOF
#define CSTF_EOF        0x20
#endif

#ifndef	CSTF_TXIM
#define CSTF_TXIM       0x40
#endif


enum {OPENCOMM=1,
		SETCOMMMASK,
		CLOSECOMM,
		READCOMM,
		WRITECOMM,
		GETCOMMSTATE,
		SETCOMMSTATE,
		SETCOMMBREAK,
		CLEARCOMMBREAK,
		CLEARCOMMERROR,
		TOON_FOR_NANAR_0x10};

typedef struct SEROPEN {
	short portNumber;
	short cbInQueue;
	short cbOutQueue;
} SEROPEN;

typedef struct SERSETMASK {
	long commDevice;	// returned by OpenComm
	short mask;
} SERSETMASK;

typedef struct SERREADWRITE {
	long commDevice;	// returned by OpenComm
	void FAR *readBuff;
	short toTransmit;
} SERREADWRITE;

typedef struct SERSTATE {
	long commDevice;	// returned by OpenComm
	DCB16 FAR *dc;
} SERSTATE;

typedef struct SERCLEARERROR {
	long commDevice;	// returned by OpenComm
	COMSTAT16 FAR *stat;
} SERCLEARERROR;

#pragma pack()
