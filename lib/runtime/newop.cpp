/*
 *	newop.cpp
 *	
 *		Copyright © 1996-1999 Metrowerks, Inc.
 *		All rights reserved.
 *
 */

#include <new>
#include <NewHand.h>
#include <stdlib.h>

#pragma exceptions on

#ifndef _MSL_NO_CPP_NAMESPACE
    namespace std {
#endif

extern char __throws_bad_alloc;

#ifndef _MSL_NO_CPP_NAMESPACE
    }
#endif

void * operator new(_CSTD::size_t size) throw(_STD::bad_alloc)
{
    // If the user does a new 0 do not try to allocate 0 or we will crash.
    // I should still return a real pointer so I am allocating 1.
    if (size == 0) {
        size = 1;
    }

    void * p;
    
    while ((p = malloc(size)) == NULL) {
        if (_STD::_new_handler_func) {
            _STD::_new_handler_func();
        } else if (_STD::__throws_bad_alloc) {
            throw (_STD::bad_alloc());
        } else {
            return NULL;
        }
    }
    
    return(p);
}



/* Change History
960830 KO	Changed the second parameter type from nothrow to nothrow_t.
			Now it matches the declaration and compiles.
960905 KO	Made this file. The original MSL file new.cpp was split
			into NewHand.cpp, NewHand.h,  newop.cpp, and delop.cpp so
			the user can overload the new and delete operators.
961216 KO	Added check for new 0. Now the flag __throws_bad_alloc is
			used.
// hh 971207 Added namespace support
// hh 980124 added nothrow
// vss 980110  remove new
// blc 981230  moved nothrow definition to newnothrow.cpp
// blc 990427  moved __throws_bad_alloc definition to throwsbad.cpp
// blc 990830  added macros to support MSL 5.1 namespace defines
*/
