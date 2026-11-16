/*
 *	throwsbad.cpp
 *	
 *		Copyright © 1996-1999 Metrowerks, Inc.
 *		All rights reserved.
 *
 */

#include <stdlib.h>

#ifndef _MSL_NO_CPP_NAMESPACE
    namespace std {
#endif

// Global variable indicating whether a bad_alloc object should be thrown
// when a new operation failes.

char    __throws_bad_alloc = 1;

#ifndef _MSL_NO_CPP_NAMESPACE
    }
#endif

// Change History
// blc 990427  moved __throws_bad_alloc definition to throwsbad.cpp
