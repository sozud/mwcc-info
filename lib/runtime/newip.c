/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *		newip.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */
 
// The new function that news a guy at a specify address

void *__nw__FUiPv(unsigned int size, void *ptr);        /* hh 971207 Added prototype */
void *__nw__FUiPv(unsigned int size, void *ptr)
{
    return ptr;
}

/*
	Change Record
 * hh 971207 Added __nw__FUiPv prototype
*/
