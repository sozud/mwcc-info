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

#ifndef __fwscanf_h__
#define __fwscanf_h__

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

int 	fwscanf(FILE *  , const wchar_t * format, ...);  /* 990715 mf */
int 	wscanf(const wchar_t * format, ...);
int 	swscanf(const wchar_t * s, const wchar_t * format, ...);
int 	__vswscanf(const wchar_t * s, const wchar_t * format, va_list arg);
int 	__vfwscanf(FILE *  , const wchar_t * format_str, va_list arg); /* 990715 */
int 	vwscanf(const wchar_t * format, va_list arg);

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

#endif /* ifndef __fwscanf_h__ */

/*  Change Record
 *	980121 JCM  First code release.
 *  990715 mf	Remove arg names in prorotypes
 */
 