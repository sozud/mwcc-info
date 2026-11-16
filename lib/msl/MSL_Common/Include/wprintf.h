/*  Metrowerks Standard Library  */

/*  $Date: 1999/08/03 23:13:17 $ 
 *  $Revision: 1.8 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	wchar_io.h
 */

#ifndef __fwprintf_h__
#define __fwprintf_h__

#include <ansi_parms.h>
#include <cstdio>

#pragma options align=native
#if defined(__CFM68K__) && !defined(__USING_STATIC_LIBS__)
	#pragma import on
#endif

#ifdef __cplusplus
	#ifdef _MSL_USING_NAMESPACE
		namespace std {
	#endif
	extern "C" {
#endif

/* extern prototypes */

int 	fwprintf(FILE *  , const wchar_t * format, ...);  /*  990715  mf */
int 	wprintf(const wchar_t * format, ...);
int 	swprintf(wchar_t * s, size_t n, const wchar_t * format, ...);
int 	vfwprintf(FILE *  , const wchar_t * format_str, va_list arg); /* 990715 mf */
int 	vswprintf(wchar_t * s, size_t n, const wchar_t * format, va_list arg);
int 	vwprintf(const wchar_t * format, va_list arg);

#ifdef __cplusplus
	}
	#ifdef _MSL_USING_NAMESPACE
		}
	#endif
#endif

#if defined(__CFM68K__) && !defined(__USING_STATIC_LIBS__)
	#pragma import reset
#endif
#pragma options align=reset

#endif /* ifndef __fwprintf_h__ */

/*     Change record
 * 990715 mf	remove arg names in prototypes
 */
