/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *		newdelarr.cpp
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */

// Type for the constructor pointer passed to __new_arr_

typedef void (*constructor)(void *);
typedef void (*destructor)(void *, int);

typedef struct
{
    unsigned int size;
    unsigned int num;
} array_header;

extern "C" void *__new_arr(constructor c, unsigned int size, unsigned int num);  /* hh 971207 Added prototype */
extern "C" void *__new_arr(constructor c, unsigned int size, unsigned int num)
{
    char *ptr;
    char *el;
    
    ptr = new char[size * num + sizeof(array_header)];
    
    if (ptr)
    {
        int i;
        
        ((array_header *)ptr)->size = size;
        ((array_header *)ptr)->num = num;
        
        ptr += sizeof(array_header);
        
        el = ptr;
        for (i=0; i<num; i++)
        {
            //c((void *)el);
        	asm
        	{
        	    mov		ecx, el
        	    call	c
        	}
            el += size;
        }
    }
    return ptr;
}

extern "C" void __del_arr(char *ptr, destructor d);  /* hh 971207 Added prototype */
extern "C" void __del_arr(char *ptr, destructor d)
{
    array_header *header;
    char *el;
    int i;

    if (d)
    {    
        header = (array_header *)(ptr-sizeof(array_header));
        el = ptr + header->size * header->num;
	    for (i=0; i<header->num; i++)
	    {
	        el -= header->size;
	        //d((void *)el, 0);
	        asm
	        {
	        	mov		ecx, el
	        	call	d
	        }
	    }
	}    
    ptr -= sizeof(array_header);
    delete ptr;
}


/*
	Change Record
 * hh 971207 Added __new_arr prototype
 * hh 971207 Added __del_arr prototype
*/
