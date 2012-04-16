/*
*	ASIAsser.h
*	Copyright (c) 1998 Altura Software, Inc. All Rights Reserved.
*	Created by:     DDK                     1/23/98
*	
*/


#undef	ASSERTx
#undef	BREAKx


#ifdef NDEBUG

	#define BREAKx(str)			((void)0)

	#define ASSERTx(exp, str)	((void)0)

#else

	#ifdef __cplusplus
	extern "C" {
	#endif

	void ASI_assert(void *, void *, unsigned);

	#ifdef __cplusplus
	}
	#endif

	#define BREAKx(str)			(void)( (ASI_assert(str, __FILE__, __LINE__), 0) )

	#define ASSERTx(exp, str)	(void)( (exp) || (ASI_assert(str, __FILE__, __LINE__), 0) )

#endif	/* NDEBUG */
