// C++ exception handling runtime

#include <ansi_parms.h> 
#include <cwchar>
#include <cstdarg>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <exception>

#define CPP_EXCEPTION_CODE		(0xE06D7363)
#define MAGIC_NUMBER			(0x19930520)

// Any of these bits means handler is called in unwinding mode

#define UNWINDING_MASK			0x66

// Flags on exception types

#define TYPE_CONST				1
#define TYPE_VOLATILE			2
#define TYPE_REFERENCE			8

// Flags on the subtype structure for throws

#define SUBTYPE_NON_CLASS		1
#define SUBTYPE_VBASE			4


// This is the RTTI typeid structure

typedef struct TypeId
{
	void 		*vtab;
	char		*tid;
	char		name[];
} TypeId;

// This structure identifies a type that may be caught by
// for this thrown exception.  If the type is a base class,
// the offset, vbtab and vbptr fields tell us how to get to
// this base class from the thrown type.

typedef struct ThrowSubType
{
	long		flags;		// CATCH_TYPE flags
	TypeId		*tid;		// pointer to typeid struct for type
	long		offset;		// offset of base class
	long		vbtab;		// offset of vb table or -1
	long		vbptr;		// offset of vb ptr in vb table
	long		size;		// size of catch type data
	void		*cctor;		// copy constructor or 0
} ThrowSubType;

// This structure identifies all of the types of catchers that
// apply to a thrown object

typedef struct SubTypeArray
{
    long			count;
    ThrowSubType	*subtypes[0];
} SubTypeArray;	



// This structure identifies the type of a thrown object

typedef struct ThrowType
{
	long		flags;		// TYPE_CONST TYPE_VOLATILE
	void		*dtor;		// Destuctor address or 0
	long		unknown;	// ??
	SubTypeArray *subtypes;	// Pointer to array of catch types for this throw
} ThrowType;


typedef struct Handler Handler;
typedef struct State State;
typedef struct Catcher Catcher;

// This is the header for the catch frame data for C++ exceptoin handling

typedef struct HandlerHeader
{
	long		magic;
	long		state_count;
	State		*states;
	long		handler_count;
	Handler		*handlers;
	long		unknown1;
	long		unknown2;
} HandlerHeader;


// This struct is an entry in the state table

struct State
{
	long		next;		// index of next state
	void		*callback;	// callback to unwind this state
};

// This struct defines a specific try-catch sequence

struct Handler
{
	long		first_state;// first state handler applies to
	long		last_state;	// last state handler applies to
	long		new_state;	// state to set if handling
	long		catch_count;// number of catchers
	Catcher		*catches;	// pointer to catcher
};


// This struct defines a single catcher in a try..catch sequence

struct Catcher
{
    long		flags;		// TYPE_CONST TYPE_VOLATILE TYPE_REFERENCE
    TypeId		*tid;		// pointer to typeid or 0 or catch (...)
    long		offset;		// offset from EBP of catch argument
	void 		*catchcode;	// code of catch block
};


// This is used to link togehter all the frame handlers

typedef struct FrameHandler FrameHandler;

struct FrameHandler
{
	FrameHandler	*next;	// link to frame handler
	void 			*code;	// pointer to handler code
	long			state;	// handler state (index into state table or -1)
};


// This struct is used to establish a hadnler for the frame handler funciton

typedef struct HandlerHandler
{
	FrameHandler	*next;	// link to frame handler
	void 			*code;	// pointer to handler code
	FrameHandler	*fh;	// frame handler for C++ code that invoked us
	void			*dtor;	// pointer to destrucor for exception being handled
	char			*ThrowData; // pointer to the object thrown
	long			state;	// handler state (index into state table or -1)
	long			ebp;	// saved ebp of the Handler function
	long			esp;	// saved esp of the handler function
	long			ebx;	// saved ebx
	long			esi;	// saved esi
	long			edi;	// saved edi
	void			*rethrow; // address to continue for a rethrow
	char			terminate;// terminate if an exception is thrown
} HandlerHandler;


// Undocumented Win32 API for unwinding from an exception

extern "C" int __stdcall RtlUnwind(
	FrameHandler	*fh,	// handler in frame to unwind to
	void * retaddr,			// return address ???
	LPEXCEPTION_RECORD exc,	// the exception being handled
	long unused				// always zero for my purposes anyway
	);


// This is the handler that is in effect when the FrameHandler is running

static int HandlerForHandler(LPEXCEPTION_RECORD exc, HandlerHandler *fh, LPCONTEXT context, void *mystery)
{
	int IsCppExc;
	
	IsCppExc = 0;
	if (exc->ExceptionCode == CPP_EXCEPTION_CODE &&
		exc->NumberParameters == 3 &&
		exc->ExceptionInformation[0] == MAGIC_NUMBER)
	{
	    IsCppExc = 1;
	}
	
	if (exc->ExceptionFlags & UNWINDING_MASK)
	{
		if (IsCppExc && exc->ExceptionInformation[1] != 0)
		{
			// New C++ excpeption is unwinding us, Call the destructor
				
			if (fh->dtor)
			{
				asm
				{
					mov		eax, fh
					mov		ecx, HandlerHandler.ThrowData[eax]
					call	HandlerHandler.dtor[eax]
				}
			}
		}
	    return 1;
	}

	// Check for a rethrow
			
	if (IsCppExc &&	
		exc->ExceptionInformation[1] == 0 && 
		exc->ExceptionInformation[2] == 0)
	{
		// Rethrow.  Restore state and go back to the original handler

		asm
		{
			mov		eax, fh
			mov		ebp, HandlerHandler.ebp[eax]
			;mov		esp, HandlerHandler.esp[eax]
			mov		ebx, HandlerHandler.ebx[eax]
			mov		esi, HandlerHandler.esi[eax]
			mov		edi, HandlerHandler.edi[eax]
			mov		HandlerHandler.dtor[eax], 0
			jmp		HandlerHandler.rethrow[eax]
		}		
	}

	// Don't allow any other exceptions during exception handling that
	// are not handled before we get back here
	
	if (IsCppExc && fh->terminate)
	{	
	    terminate();
	    
	    // In case terminate() does not
	    
	    ExitProcess(0);
	}

	// continue

	return 1;
}


// This function is called by ___CxxFrameHandler after pushing EAX, which is the HanderHeader pointer
//
// in exc->ExceptionInformation[0] is 0x19930520 magic number
// in exc->ExceptionInformation[1] is pointer to thrown data
// in exc->ExceptionInformation[2] is poitner to throw type structure
	

static int MyFrameHandler(HandlerHeader *hhp, void *ret_addr, LPEXCEPTION_RECORD exc, FrameHandler *fh, LPCONTEXT context, void *mystery)
{
	int state;
	Handler *hp;
	int hi, cti, sti;
	ThrowType *ttp;
	Catcher *ctp;
	SubTypeArray *sta;
	ThrowSubType *tstp;
	char *CatchFrame, *ThrowData, *CatchData;
	void *ContinueAddress;
	HandlerHandler HH;
	HandlerHandler *UnwindTo;
	long RestoreSP;
			
	// Check magic number to make sure we were called as expected
	
	if (hhp->magic != MAGIC_NUMBER)
	{
	    terminate();
	    ExitProcess(0);
	}

	// Establish a handler to be in effect during handling
		
	HH.code = &HandlerForHandler;
	HH.state = -1;
	HH.fh = fh;
	HH.dtor = 0;
	HH.terminate = 1;
	asm
	{
		lea		eax, HH
		mov		HandlerHandler.ebp[eax], ebp
		mov		HandlerHandler.esp[eax], esp
		mov		HandlerHandler.ebx[eax], ebx
		mov		HandlerHandler.esi[eax], esi
		mov		HandlerHandler.edi[eax], edi
		mov		HandlerHandler.rethrow[eax], OFFSET rethrow
		mov		ecx, fs:[0]
		mov		HandlerHandler.next[eax], ecx

	rethrow:
		lea		eax, HH
		mov		fs:[0], eax
	}
	
	// See if we are unwinding
	
	if (exc->ExceptionFlags & UNWINDING_MASK)
	{
		// We are unwinding, time to call all our destructors
		
		state = fh->state;
		while (state != -1)
		{
			void *callback;
			
			// See if there is a callback for this state
			
			callback = hhp->states[state].callback;
			if (callback)
			{
				
				asm
				{
					mov		eax, callback	; get callback address
					push	ebp				; save current ebp
					
					mov		ebp, fh			; set up frames's ebp
					add		ebp, 12
					
					call	eax				; call the guy
					
					pop		ebp				; and restore ebp
				}
			}
			
			// go to next state
			
			state = hhp->states[state].next;
		}

		// remove our handler
		
		asm
		{
			lea		eax, HH
			mov		eax, HandlerHandler.next[eax]
			mov		fs:[0], eax
		}
				
		return 1;
	}
			
	// Check to see if the exception is a C++ exception, we won't handle anything else.
	
	if (exc->ExceptionCode != CPP_EXCEPTION_CODE)
	{

		// remove our handler
		
		asm
		{
			lea		eax, HH
			mov		eax, HandlerHandler.next[eax]
			mov		fs:[0], eax
		}
				
		// Continue search for handler
		
	    return 1;
	}

	// Check magic number on it
	
	if (exc->NumberParameters != 3 || exc->ExceptionInformation[0] != MAGIC_NUMBER)
	{
	    terminate();
	    ExitProcess(0);
	}
	
	// Shit! Now we have to do some real work

	// Get the state
	
	state = fh->state;
	
	// Get the esp to restore after we have handled
	
	asm
	{
		mov		eax, fh
		mov		eax, -4[eax]
		mov		RestoreSP, eax
	}
		
	// Get the throw data
	
	ttp = (ThrowType *)exc->ExceptionInformation[2];

	// ttp == 0 on a rethrow, do not look for a handler for it, it will be handled by
	// our handler from a prior invocation that is executing the catch clause, if one
	// exits.
	 
 	if (ttp)
 	{	
		// Loop over all handlers
		
		hp = hhp->handlers;
		for (hi=0; hi<hhp->handler_count; hi++)
		{
			if (hp->first_state <= state && state <= hp->last_state)
			{
				// This handler is active, check all of it's catch blocks
				
				ctp = hp->catches;
				for (cti=0; cti<hp->catch_count; cti++)
				{
					// check for the catch all (no, not Vicky)
					
					if (ctp->tid == 0)
					{
					    goto found_catcher;
					}
					
					// loop over all throw sub-types seeing if we find a match
					
					sta = ttp->subtypes;
					for (sti=0; sti<sta->count; sti++)
					{
						tstp = sta->subtypes[sti];
						
						// Make sure flags are compatible.  The catcher must have
						// any of TYPE_CONST and TYPE_VOLATILE set if the the thrower does
						
						if (((ttp->flags & (TYPE_CONST | TYPE_VOLATILE)) & ~(ctp->flags & (TYPE_CONST | TYPE_VOLATILE))) == 0)
						{
	
							// Quick compare of address of type id
							
							if (tstp->tid == ctp->tid)
							{
							    goto found_catcher;
							}
							
							// Do the string compare
							
							if (strcmp(ctp->tid->name, tstp->tid->name) == 0)
							{
								goto found_catcher;
							}
						}
											
						// on to next subtype
					    tstp++;
					}
					
					
					// on to next catcher
				    ctp++;
				}
			}
					
			// on to next handler
		    hp++;
		}			
	}
		
	// remove our handler
	
	asm
	{
		lea		eax, HH
		mov		eax, HandlerHandler.next[eax]
		mov		fs:[0], eax
	}
				
	// no handler return status to continue search

	return 1;	
	
found_catcher:

	// at this point:
	//
	//	hp is pointer to the handler
	//  cp is pointer to the catch block

	// Get frame pointer of the catching frame
	
	CatchFrame = (char *)fh + 12;
	
	// Set new state in the catcher frame
	
	fh->state = hp->new_state;

	// Set the lowest state handled by this handler in our handler
	
	HH.state = hp->first_state;
				
	// Find the last thing to unwind.  Walk down the stack of
	// handlers till we find a handler for a previous catch
	// in this frame at lower nesting level than the one currently
	// being caught
	
	asm
	{
		mov		eax, fs:[0]
		mov		UnwindTo, eax
	}
	
	while (UnwindTo != (HandlerHandler *)fh &&
		   (
		   	 UnwindTo == &HH ||
	         UnwindTo->code != &HandlerForHandler ||
		     UnwindTo->fh != fh ||
		     UnwindTo->state > hp->first_state
		   )
		  )
	{
		UnwindTo = (HandlerHandler *)UnwindTo->next;
	}
	
	// Unwind any other frames
	
	asm
	{
		push	0
		push	exc
		push	ret_addr
		push	UnwindTo
		xor		ebx, ebx
		xor		esi, esi
		xor		edi, edi
		call	RtlUnwind
	ret_addr:
		mov		eax, fs:[0]
	}

	// Reestablish our handler, since he got unwound
	
	HH.next = (FrameHandler *)UnwindTo;
	asm
	{
		lea		eax, HH
		mov		fs:[0], eax
	}
	
	// Now unwind this frame until the state is lower than
	// the state that established this handler
	
	while (state > hp->first_state)
	{
		void *callback;
		
		// See if there is a callback for this state
		
		callback = hhp->states[state].callback;
		if (callback)
		{
			
			asm
			{
				mov		eax, callback	; get callback address
				push	ebp				; save current ebp
				
				mov		ebp, fh			; set up frames's ebp
				add		ebp, 12
				
				call	eax				; call the guy
				
				pop		ebp				; and restore ebp
			}
		}
		
		// go to next state
		
		state = hhp->states[state].next;
	}
		
	// If catcher has a type, we need to move the argument
	
	if (ctp->tid)
	{
		long pv;
		
		// Get the throw data address
		
		ThrowData = (char *)exc->ExceptionInformation[1];
		CatchData = CatchFrame + ctp->offset;
		
		// Adjust this address to address the sub-type
		
		if (tstp->flags & SUBTYPE_VBASE)
		{
			//??
		}
		else if ((tstp->flags & SUBTYPE_NON_CLASS))
		{
			if (tstp->offset != 0)
			{
			    // pointer to class thrown and pointer to base class caught,
			    // we must adjust the pointer
			    
			    pv = *(long *)ThrowData + tstp->offset;
			    ThrowData = (char *)&pv;
			}
		}
		else
		{
			// Adjust address to point to base class
			
		    ThrowData += tstp->offset;
		}
		
		// Copy data to the argument
		
		if (ctp->flags & TYPE_REFERENCE)
		{
			// copy the address
			
			*(long *)(CatchData) = (long)ThrowData;
		}
		else if (tstp->cctor)
		{
			// Additional arg for vbase constructors?
			if (tstp->flags & 4)
			{
				asm		push	1
			}
			asm
			{
				push	ThrowData
			    mov		ecx, CatchData
			    mov		eax, tstp
			    call	ThrowSubType.cctor[eax]
			}
		}
		else
		{
			memmove(CatchFrame + ctp->offset, ThrowData, tstp->size);
		}
	}

	// Set this in our handler so we can destroy the guy if we unwind through
	
	;HH.dtor = ttp->dtor;
	HH.ThrowData = ThrowData;
	
	// Exception is now handled, don't terminate if a new exception is thrown
	
	HH.terminate = 0;	
	
	// Zap the unwinding flags
	
	exc->ExceptionFlags &= ~UNWINDING_MASK;
	
	// and call the catch block
	
	asm
	{
		mov 	eax, ctp
		push	ebp
		mov		ebp, CatchFrame
		call	Catcher.catchcode[eax]
		
		// eax now has the continue address
		
		pop		ebp
		mov		ContinueAddress, eax
	}
	
	// Destroy the exception if necessary

	if (ttp->dtor)
	{	
		// Do terminate if the destructor throws
		
		HH.terminate = 1;

	    // destroy the thrown exception
	    
	    asm
	    {
	    	mov		ecx, ThrowData
	    	mov		eax, ttp
	    	call	ThrowType.dtor[eax]
	    }
	}
	
	// And continue execution after the catcher
	
	asm
	{
		lea		eax, HH				; remove our handler
		mov		eax, HandlerHandler.next[eax]
		mov		fs:[0], eax
		
		cmp		eax, fh				; are we in a nested catch?
		jne		noloadesp			; if so, cannot yet reload esp
			
		mov		ecx, RestoreSP		; get new ESP setting
		mov		esp, ecx			; restore handled frames esp
		
	noloadesp:
	
		mov		eax, ContinueAddress; get address to continue execution
				
		mov		ebp, CatchFrame		; restore handled frames ebp
		
		jmp		eax					; and go there, as if nothing has happened
	}	
	
	// never gets here
	
}


// This function is called by each frame as the system checks for a handler for
// a structured exception.
extern "C" int __declspec(naked) __CxxFrameHandler(LPEXCEPTION_RECORD exc, void *exc_stack, LPCONTEXT context, void *mystery);
extern "C" int __declspec(naked) __CxxFrameHandler(LPEXCEPTION_RECORD exc, void *exc_stack, LPCONTEXT context, void *mystery)
{	
	// We get passed the following:
	// In EAX is the pointer to our frame handler data
	asm
	{
		
		push	eax
		call	MyFrameHandler
				
		// If he returned, no handler, continue search
		
		add		esp, 4
	    retn
	}
}



// Throw a C++ excpeption
extern "C" __stdcall void _CxxThrowException(void *ThrowData, ThrowType *tt);
extern "C" __stdcall void _CxxThrowException(void *ThrowData, ThrowType *tt)
{
    DWORD Arguments[3];
    
    Arguments[0] = MAGIC_NUMBER;
    Arguments[1] = (DWORD)ThrowData;
    Arguments[2] = (DWORD)tt;
    
	RaiseException(CPP_EXCEPTION_CODE, EXCEPTION_NONCONTINUABLE, 3, Arguments);
}

#if 0 // dont need this anymore

static class type_info
{
public:
    type_info() { }
    virtual ~type_info() { }
} x;
#endif