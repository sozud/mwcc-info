/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *		runinit.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */

#include "crtl.h"
#include "NMWException.h"
#include <stdlib.h>

#ifdef __cplusplus           /* hh 971207 Added namespace support */
	extern "C" {
#endif

// Run the static init code.  The symbol __cx_a is a longword at the start of of table.  The
// table is a list of pointers to functions to call, terminated by a zero entry.  We simply
// loop through, calling each function.

void _RunInit()
{
    typedef void (**pfn)(void);
    extern char __xc_a;
    
    pfn func;
    
    func = (pfn)(&__xc_a + 4);
    while (*func)
    {
        (*func)();
        func = (pfn)((char *)func + 4);
    }

	// Set up an exit handler destruct any global constructed objects    
    atexit(__destroy_global_chain);
}

#ifdef __cplusplus         /* hh 971207 Added namespace support */
	}
#endif

/*
	Change Record
 * hh 971207 Added namespace support
*/