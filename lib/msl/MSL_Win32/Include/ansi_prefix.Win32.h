/*  Metrowerks Standard Library  */

/*  $Date: 1999/04/08 22:16:31 $ 
 *  $Revision: 1.10 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*
 *	ansi_prefix.win32.h
 *	
 */

#ifndef __ansi_prefix__
#define __ansi_prefix__

#include <os_enum.h>
#define __dest_os	__win32_os
#include <x86_prefix.h>
#define _MT
#ifndef _MWMT
# define _MWMT 1 
#endif 
 /* need this to pick up prototypes for _begin/_endthreadex 
    in the win32 SDK header process.h */


#define STOP_PROGRAM_BEFORE_EXIT 0  /* will stop console apps from exiting
                                       before you get a chance to see it, set to 0 
                                       if you don't want this behavior

                                    */

/* hh 980121 Protect ourselves from overly helpful Microsoft headers */
#define _WCHAR_T_DEFINED
#define _SIZE_T_DEFINED

#define __MSL_LONGLONG_SUPPORT__
/* #define _MSL_MALLOC_0_RETURNS_NON_NULL */
/* #define _MSL_PRO4_MALLOC */
/* hh 980217 
	__ANSI_OVERLOAD__ controls whether or not the prototypes in the C++ standard
	section 26.5 get added to <cmath> and <math.h> or not.  If __ANSI_OVERLOAD__
	is defined, and a C++ compiler is used, then these functions are available,
	otherwise not.
	
	There is one exception to the above rule:  double abs(double); is available
	in <cmath> and <math.h> if the C++ compiler is used.  __ANSI_OVERLOAD__ has
	no effect on the availability of this one function.
	
	There is no need to recompile the C or C++ libs when this switch is flipped.
*/
/* hh 990201 turned on */
#define __ANSI_OVERLOAD__ 

#endif
/*	Change Log
960711 KO		Added this include for the WinNT headers
960829 bkoz	need to define modena lib for c++ libraries
hh 980121 Protect ourselves from overly helpful Microsoft headers by telling them that
          we will take care of the C lib.
mf 980621  added #define _MWMT          
hh 990201 turned __ANSI_OVERLOAD__ on because we now have foo(int) support
hh 990228 added _MSL_MALLOC_0_RETURNS_NON_NULL flag
mf 990301 -- yikes to the above ^, commented out STOP_PROGRAM_BEFORE_EXIT to attempt new scheme
             w/Pro 5 ide, also added logic for single threaded lib.
mf  990309       use _MWMT instead of _MT for single threaded lib             
blc 990408  turned off STOP_PROGRAM_BEFORE_EXIT; not needed with Pro5 IDE

*/
