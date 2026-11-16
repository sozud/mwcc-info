/*  Metrowerks Standard Library  */

/*  $Date: 1999/08/06 00:03:45 $ 
 *  $Revision: 1.9 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*
 *	mslGlobals.h
 */

/*
	The purpose of this file is to set up those aspects of the environment
	that are global to both the C and C++ library.  The top of the dependency
	tree for the C library is ansi_parms.h.  The top for the C++ library is
	mcompile.h.  In order to avoid duplicating code in these two files, this
	file will be included by both ansi_parms.h and mcompile.h.  Only put stuff
	in here that otherwise would have to have been duplicated in ansi_parms.h
	and mcompile.h.
*/

#ifndef __mslGlobals_h
#define __mslGlobals_h

#ifndef __ansi_prefix__

#if defined(__MC68K__)  /*  990804  beb */
	#if defined(__embedded__)
		#include <ansi_prefix.e68k.h>
	#else
		#include <ansi_prefix.mac.h>
	#endif
#elif (defined(__POWERPC__) && !defined(__PPC_EABI__))
	#include <ansi_prefix.mac.h>
#elif defined(__BEOS__)  /*  bs  990121  */
    #include <ansi_prefix.be.h>
#elif defined(__PPC_EABI__)
	#include <ansi_prefix.PPCEABI.bare.h>
#elif defined(__INTEL__)
	#if !defined(UNDER_CE)
		#include <ansi_prefix.Win32.h>
	#endif
#elif defined(__MIPS__)
	#if !defined(UNDER_CE)
		#include <ansi_prefix.MIPS_bare.h>
	#endif
#elif defined (__m56800__)              /* mm 981028 */
	#include <ansi_prefix.56800.h>
#elif defined (__mcore__) /* 990804 vss */
	#include <ansi_prefix.MCORE_EABI_bare.h>
#else
	#ifndef RC_INVOKED  /* hh 990525 */
		#error mslGlobals.h could not include prefix file
	#endif
#endif

#endif /* __ansi_prefix__ */

#if __option(longlong)
#define __MSL_LONGLONG_SUPPORT__ 
#endif

/* Turn on and off namespace std here */
#if defined(__cplusplus) && __embedded_cplusplus == 0
    #define _MSL_USING_NAMESPACE
	/* Turn on support for wchar_t as a built in type */
	/* #pragma wchar_type on */   /*  vss  not implemented yet  */
#endif

#endif


/* Change Record
 * hh  980120  created
 * mm  981028  Added #include for ansi_prefix.56800.h
 * vss 981116  MIPS doesn't want namespaces  (?)
 * mf  981118  made mips ce not include ansi_prefix.mips.bare.h
               and changed __MSL_LONGLONG_SUPPORT__  def to work with
               what is already in all msl source files
 * bs  990121  Added BEOS
 * hh 990525  Protected error from resource compiler
 * beb 990804 Added e68k
 * vss 990804 Added mcore
 */
