/*  Metrowerks Standard Library  */

/*  $Date: 1999/03/09 17:41:32 $ 
 *  $Revision: 1.4 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	critical_regions.win32.c
 *	
 *	Routines
 *	--------
 *		__init_critical_regions
 *		__begin_critical_region
 *		__end_critical_region
 *
 *	Implementation
 *	--------------
 *
 *	Modification History
 *	--------------------
 *
 */

/* #include <x86_prefix.h>*/
#include <stdlib.h>
#include "critical_regions.h"
/*
hh 980122 Replaced <windows.h> with the following TWO includes because it is seriously
          broken.  The following 2 includes must be carefully ordered as shown, because
          they are broken too.
#include <windows.h>
*/
#include <WINDEF.H>
#include <WINBASE.H>


static CRITICAL_SECTION cs[num_critical_regions];

void __init_critical_regions(void)
{
		int i;
		for (i = 0; i < num_critical_regions; i++)
				InitializeCriticalSection(&cs[i]);
}

void __kill_critical_regions(void)
{
		int i;
		for (i = 0; i < num_critical_regions; i++)
				DeleteCriticalSection(&cs[i]);
}
#if _MWMT
void __begin_critical_region(int region)
{
		EnterCriticalSection(&cs[region]);	
}

void __end_critical_region(int region)
{
		LeaveCriticalSection(&cs[region]);
}
#endif
/*
hh 980122 Replaced <windows.h> with the following TWO includes because it is seriously
          broken.  The following 2 includes must be carefully ordered as shown, because
          they are broken too.
          
* mf 980624 don't include x86_prefix.h
*/
