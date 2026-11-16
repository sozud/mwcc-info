/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *		arrcontdest.cp
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */
// Array constructor and destructor code

#include "cpluslib.h"

#pragma exceptions on

/************************************************************************/
/*	Purpose..: 	Initialize a new allocated array of objects				*/
/*	Input....:	pointer to allocated memory (+8 bytes) (0L: error)		*/
/*	Input....:	pointer to default constructor function	(or 0L)			*/
/*	Input....:	pointer to destructor function (or 0L)					*/
/*	Input....:	size of one array element								*/
/*	Input....:	number of array elements								*/
/*	Return...:	pointer to first object in array						*/
/************************************************************************/
extern void *__construct_new_array(void *block,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n)
{
	char	*ptr;

	if((ptr=(char *)block)!=0L)
	{
		//	store number of allocated objects and size of one object at the beginnig of the allocated block
		*(size_t *)ptr=size;
		*(size_t *)(ptr+sizeof(size_t))=n;
		ptr+=sizeof(size_t)*2;

		if(ctor)
		{	//	call constructor to initialize array
			char	*p;
			size_t	i;

			#pragma exception_arrayinit
			//	this #pragma adds partial array construction exception action (ptr,dtor,size,i)

			for(i=0,p=ptr; i<n; i++,p+=size)
			{
#if __INTEL__
				asm
				{
		        	mov		ecx, p
		        	push	1
		        	call	dword ptr ctor
		        	add		esp, 4
				}
#else
			    ctor(p,1);
#endif
			}
		}
	}
	return ptr;	//	return pointer to first object;
}



/************************************************************************/
/*	Purpose..: 	Construct an array of objects							*/
/*	Input....:	pointer to array memory									*/
/*	Input....:	pointer to default constructor function					*/
/*	Input....:	pointer to destructor function (or 0L)					*/
/*	Input....:	size of one array element								*/
/*	Input....:	number of array elements								*/
/*	Return...:	---														*/
/************************************************************************/
extern void __construct_array(void *ptr,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n)
{
	char	*p;
	size_t	i;

	#pragma exception_arrayinit
	//	this #pragma adds partial array construction exception action (ptr,dtor,size,i)

	for(i=0,p=(char *)ptr; i<n; i++,p+=size)
	{
#if __INTEL__
				asm
				{
		        	mov		ecx, p
		        	call	dword ptr ctor
				}
#else
			    ctor(p,1);
#endif
	}
}

/************************************************************************/
/*	Purpose..: 	Destroy an array of objects								*/
/*	Input....:	pointer to array memory									*/
/*	Input....:	pointer to destructor function							*/
/*	Input....:	size of one object										*/
/*	Input....:	number of objects										*/
/*	Return...:	---														*/
/************************************************************************/
extern void __destroy_arr(void *block,ConstructorDestructor dtor,size_t size,size_t n)
{
	char	*p;

	for(p=(char *)block+size*n; n>0; n--)
	{
		p-=size;
#if __INTEL__
		asm
		{
			mov		ecx, p
			call	dword ptr dtor
		}
#else
		dtor(p,-1);
#endif
	}
}

/************************************************************************/
/*	Purpose..: 	Delete an array of objects								*/
/*	Input....:	pointer to first object	(can be 0L)						*/
/*	Input....:	pointer to destructor function							*/
/*	Return...:	---														*/
/************************************************************************/
extern void __destroy_new_array(void *block,ConstructorDestructor dtor)
{
	if(block)
	{
		if(dtor)
		{
			size_t	i,objects,objectsize;
			char	*p;

			objectsize=*(size_t *)((char *)block-2*sizeof(size_t));
			objects=*(size_t *)((char *)block-sizeof(size_t));
			p=(char *)block+objectsize*objects;
			for(i=0; i<objects; i++)
			{
				p-=objectsize;
#if __INTEL__
				asm
				{
					mov		ecx, p
					push	-1
					call	dword ptr dtor
					add		esp, 4
				}
#else
				dtor(p,-1);
#endif
			}
		}
		::delete ((char *)block-2*sizeof(size_t));
	}
}
