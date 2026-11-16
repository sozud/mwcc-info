/*
 *	newnothrow.cpp
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

	nothrow_t nothrow;           // blc 981230 added nothrow

#ifndef _MSL_NO_CPP_NAMESPACE
	}
#endif


void * operator new(size_t size, const nothrow_t&) throw()
{
	try
	{
		return operator new(size);
	}
	catch(...)
	{
	}
	return 0;
}

/* Change History
 * 1998-12-30      moved std::nothrow defintion from newop.cpp to prevent 
 *                 link errors with MFC
 * 1999-08-30      the nothrow version of new now calls the other new,
 *                 catching any thrown exceptions (IR9907-1360-1)
 */
