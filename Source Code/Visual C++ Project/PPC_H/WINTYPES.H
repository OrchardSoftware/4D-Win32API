#ifndef EXTENDED_IS_64BIT
#ifdef WIN32	// MSVC2.0 does not support the long double type
#define	EXTENDED_IS_64BIT	1
#else
#define	EXTENDED_IS_64BIT	0
#endif
#endif

#if !EXTENDED_IS_64BIT

typedef long double	extended;
typedef long double	DOUBLE;

#ifdef __cplusplus
extern "C" {
#endif
extern F_ASIBND( extended )	ASI_Cos( extended );
extern F_ASIBND( extended )	ASI_Sin( extended );
extern F_ASIBND( extended )	ASI_Tan( extended );
extern F_ASIBND( extended )	ASI_Exp( extended );

#ifdef __cplusplus
}
#endif

#define	CEIL	ceill
#define	FLOOR	floorl
#define	FMOD	fmodl
#define	FABS	fabsl
#define	MODF	modfl
#define	COS	ASI_Cos
#define	SIN	ASI_Sin
#define	TAN	ASI_Tan
#define	ACOS	acosl
#define	ASIN	asinl
#define	ATAN	atanl
#define	ATAN2	atan2l
#define	EXP	ASI_Exp
#define	LOG	logl
#define	LOG10	log10l
#define	SQRT	sqrtl

#else		// #if !EXTENDED_IS_64BIT

typedef double	extended;
typedef double	DOUBLE;

#define	CEIL	ceil
#define	FLOOR	floor
#define	FMOD	fmod
#define	FABS	fabs
#define	MODF	modf
#define	COS	cos
#define	SIN	sin
#define	TAN	tan
#define	ACOS	acos
#define	ASIN	asin
#define	ATAN	atan
#define	ATAN2	atan2
#define	EXP	exp
#define	LOG	log
#define	LOG10	log10
#define	SQRT	sqrt

#endif		// #if/#else !EXTENDED_IS_64BIT

#if 0		// Now implemented as a function
#define	rint( x )	(FLOOR( x + 0.5 ))
#else
#define	rint( x )	Rint( x )
#endif

