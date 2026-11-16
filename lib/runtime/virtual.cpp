/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *		virtual.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */
 
#include <stdlib.h>

/* Visual C++ runtime function used as placeholder in pure virtual vtables
 * to cause abort during attempted execution */

extern "C" void _purecall(void);
extern "C" void _purecall(void)
{
	abort ();
}

/*
	Change Record
 * blc 980522 Created
*/
