/************************************************************************/
/*	Project...:	Standard C++ Library									*/
/*	Name......:	GlobDest.cp												*/
/*	Purpose...:	Handle destruction of static objects					*/
/*  Copyright.: Copyright © 1993-1997 Metrowerks, Inc.					*/
/************************************************************************/


#define __NOSTRING__	//	do not include <string>

#include <ansi_parms.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdlib.h>
#include <CPlusLib.h>
#include "NMWException.h"

/************************************************************************/
/*	Purpose..: 	Register a global object for later destruction			*/
/*	Input....:	pointer to global object								*/
/*	Input....:	pointer to destructor function							*/
/*	Input....:	pointer to global registration structure				*/
/*	Return...:	pointer to global object (pass thru)					*/
/************************************************************************/
extern void *__register_global_object(void *object,void *destructor,void *regmem)
{
	((DestructorChain *)regmem)->next=__global_destructor_chain;
	((DestructorChain *)regmem)->destructor=destructor;
	((DestructorChain *)regmem)->object=object;
	__global_destructor_chain=(DestructorChain *)regmem;

	return object;
}



/************************************************************************/
/* Purpose..: Destroy all constructed global objects					*/
/* Input....: ---														*/
/* Return...: ---														*/
/************************************************************************/

DestructorChain  * __global_destructor_chain;

void __destroy_global_chain(void)
{
	DestructorChain	*gdc;

	while((gdc=__global_destructor_chain)!=0L)
	{
		__global_destructor_chain=gdc->next;
	    asm
	    {
	        	mov		eax, gdc
	        	mov		ecx, DestructorChain.object[eax]
	        	call	dword ptr DestructorChain.destructor[eax]
	    }	
	}
}

