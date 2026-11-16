/*
 *	newarrnothrow.cpp
 *	
 *		Copyright © 1996 Metrowerks, Inc.
 *		All rights reserved.
 *
 */

#include <new>
#include <NewHand.h>
#include <stdlib.h>

#pragma exceptions on

/************************************************************************/
/*	Purpose..: 	Array allocation/deallocation functions					*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
#if __MWERKS__>=0x2020

extern void *operator new[](size_t size,const nothrow_t& nt) throw()
{
	return operator new(size,nt);
}
#endif

