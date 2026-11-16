/*  Metrowerks Standard Library  */

/*  $Date: 1999/03/03 13:15:59 $ 
 *  $Revision: 1.2 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	wchar_t.h
 */
 
#ifndef __wchar_t__
#define __wchar_t__

#ifdef __cplusplus      
	#ifdef _MSL_USING_NAMESPACE
		namespace std {
	#endif
	extern "C" {
#endif

#if !__cplusplus || !__option(wchar_type) 
	typedef unsigned short wchar_t;
#endif

#define WCHAR_MIN	0
#define WCHAR_MAX	((wchar_t) - 1)


#ifdef __cplusplus       
	}
	#ifdef _MSL_USING_NAMESPACE
		}
	#endif
#endif


#endif /* __wchar_t__ */

/* Change record
 *  mm 990217  Recreated to avoid standard headers having to include cstddef
 *             to get the typedef for wchar_t
 */	
