/*
 *	delarrop.cpp
 *	
 *		Copyright © 1996 Metrowerks, Inc.
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


// Global variable indicating whether a bad_alloc object should be thrown
// when a new operation failes.
extern char	__throws_bad_alloc;

#ifndef _MSL_NO_CPP_NAMESPACE
	}
#endif

/************************************************************************/
/*	Purpose..: 	Array allocation/deallocation functions					*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
#if __MWERKS__>=0x2020

extern void operator delete[](void *ptr) throw()
{
	operator delete(ptr);
}
#endif
