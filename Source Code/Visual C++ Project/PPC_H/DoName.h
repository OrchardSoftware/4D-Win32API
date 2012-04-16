#undef SetPort
#define SetPort SetPor

#ifdef GetClassName
#undef GetClassName  
#endif

#ifdef GetObject
#undef GetObject 
#endif

#define Polygon Polygo
#define ResizePalette ResizePalett

#ifdef AppendMenu
#undef AppendMenu
#endif
#define AppendMenu AppendMen
#define AnimatePalette AnimatePalett
#define CloseWindow CloseWindo 
#define DeleteMenu DeleteMen
#define DrawMenuBar DrawMenuBa
#ifdef DrawText
#undef DrawText
#endif
#define DrawText DrawTex
#define EqualRect EqualRec
#define EqualRgn EqualRg
#define FillRect FillRec
#define FillRgn FillRg
#ifdef FindWindow
#undef FindWindow
#endif
#define FindWindow FindWindo
#define FrameRect FrameRec
#define FrameRgn FrameRg
#define GetMenu GetMen
#ifdef InsertMenu
#undef InsertMenu
#endif
#define InsertMenu InsertMen
#define InsetRect InsetRec
#define InsetRgn InsetRg
#define InvertRect InvertRec
#define InvertRgn InvertRg
#define LineTo LineT
#define LoadResource LoadResourc
#define MoveTo MoveT
#define MoveWindow MoveWindo
#define OffsetRect OffsetRec
#define OffsetRgn OffsetRg
#define PaintRgn PaintRg
#define PtInRect PtInRec
#define SetCursor SetCurso
#define SetRect SetRec
#define SetRectRgn SetRectRg
#define ShowCursor ShowCurso
#define ShowWindow ShowWindo
#define UnionRect UnionRec

#if !PPC_HEADERS
#define DisposeHandle DisposHandle
#define DisposePtr DisposPtr
#define	DisposCTable DisposeCTable
#endif
#define Get1IxResource Get1IndResource
#define Get1IxType Get1IndType
#ifdef ReplaceText
#undef ReplaceText
#endif
#define ReplaceText ReplaceTex
#define OpenDriver OpenDrive
#define CloseDriver CloseDrive
#define CopyRgn CopyRg
#define UnionRgn UnionRg
#define XorRgn XorRg
#define FlushInstructionCache FlushInstructionCach
#define Sleep Slee
#define GetCursor GetCurso
#define GetCurrentProcess GetCurrentProces
#define GetPixel GetPixe
#define IsWindowVisible IsWindowVisibl
