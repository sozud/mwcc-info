/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *		stackall.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */

/* VC++ code sometimes has calls to these two functionsinserted by the compiler, 
 * so we need the entry points available. */

__declspec(naked) void * _chkstk(void);
__declspec(naked) void * _chkstk(void)
{
    asm
    {
			// save edx to restore before returning
			push    edx

    		// start edx just before return address and stored edx
    		lea		edx, [esp+8]
    		
    		// First make it round
    		// add		eax, 3
    		// and		eax, 0xfffffffc
    		
    		// Now, allocate the space in 4K chunks, probing each time, to make 
    		// sure the system allocates each of the new pages needed
    		
    next_four_k:
    		cmp		eax, 0x1000			// less than 0x1000 to go?
    		jb		last_one			// if so, see the last one
    		sub		edx, 0x1000			// if not, add a page
    		or		dword ptr[edx], 0	// touch the memory
    		sub		eax, 0x1000			// decrement memory count
    		jmp		next_four_k			// and do it again
    		
    last_one:
    		sub		edx, eax			// now, just finish off the allocation
    		mov		eax, esp			// save sp to we can restore context
    		mov		esp, edx			// store new SP into the real SP
    		mov		edx, [eax]			// restore edx
    		or		dword ptr[esp], 0	// touch the memory
			push	[eax+4]				// setup return addr
			lea		eax, [esp+4]		// store new ESP into eax for return value
    		ret							// and return to the caller
    }
}

// Stack allocation with probing every 4K so that new stack space will be allocated as needed
// blame this on Microsoft

__declspec(naked) void * _stack_alloc(void);    /* hh 971207 Added prototype */
__declspec(naked) void * _stack_alloc(void)
{
    asm
    {
    		// First make it round
    		add		eax, 3
    		and		eax, 0xfffffffc

    		jmp		_chkstk					// allocate and probe the stack
    }
}

__declspec(naked) void * _alloca_probe(void);
__declspec(naked) void * _alloca_probe(void)
{
    asm
    {
	    	jmp		_chkstk
    }
}

/*
	Change Record
 * hh 971207 Added _stack_alloc prototype
 * blc 990409  Fixed to only destroy value of EAX while in _chkstk
*/