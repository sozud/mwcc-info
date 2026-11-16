/************************************************************************/
/*	Project...:	Standard C++ Library									*/
/*	Name......:	ExcpetionX86.c											*/
/*	Purpose...:	C++ specific runtime functions							*/
/*  Copyright.: ©Copyright 1993-95 by metrowerks inc					*/
/************************************************************************/

#pragma exceptions on

#define __NOSTRING__	//	do not include <string>

#include <stdlib.h>
#include "CPlusLib.h"
#include <exception.h>
#include "exceptionx86.h"
#include "NMWException.h"

#define EXPORTALLPCOFFSETS		0			//	1: export each pc-offset

// Define the three regs that might be saved in the frame

#define REG_EBX 	3
#define REG_ESI		6
#define REG_EDI		7

#if 1
#define INLINE			static				//	static for debugging
#else
#define INLINE			inline				//	inline for shipping code
#endif

//	typedefs

typedef struct ThrowContext {
	long		EBX, ESI, EDI;				//  space for EBX, ESI and EDI
	char 		*EBP;						
	char		*returnaddr;				//	return address
	char		*throwtype;					//	throw type argument (0L: rethrow: throw; )
	void		*location;					//	location argument (0L: rethrow: throw; )
	void		*dtor;						//	dtor argument
	CatchInfo	*catchinfo;					//	pointer to rethrow CatchInfo (or 0L)
}	ThrowContext;

typedef struct ExceptionInfo {
	ExceptionRecord	*exception_record;		//	pointer to exception record
	char			*current_function;		//	pointer to current function
	char			*action_pointer;		//	pointer to action
}	ExceptionInfo;

typedef struct ActionIterator {
	ExceptionInfo	info;					//	pointer to exception record
	char			*current_bp;			//	current A6 frame
}	ActionIterator;


// Head of list of exception table headers
static ExceptionTableHeader *FirstExceptionTable = 0;

// Register an exception table

void _RegisterExceptionTables(ExceptionTableHeader *header)
{
    ExceptionTableHeader *prev, *next;
    
    // Link the tables together in order of decreasing Pc, that way, the first in the list with a Pc less
    // than the one we are looking for must be the right one.
    
    // First, find position in table
    prev = 0;
    next = FirstExceptionTable;
    while (next && next->First->Pc > header->First->Pc)
    {
        prev = next;
        next = next->Next;
    }
    
    // Now link it in there
    header->Next = next;
    if (prev)
    {
        prev->Next = header;
    }
    else
    {
        FirstExceptionTable = header;
    }
}

// Unregister an exception table when a DLL terminates

void _UnRegisterExceptionTables(ExceptionTableHeader *header)
{
    ExceptionTableHeader *prev, *next;
    
    
    // Find the table
    prev = 0;
    next = FirstExceptionTable;
	while (next && next != header)
    {
        prev = next;
        next = next->Next;
    }
    
    // If not found, we have a problem!
    if (!next)
    {
        terminate();
    }
    
    // Unlink it
    if (prev)
    {
        prev->Next = next->Next;
    }
    else
    {
        FirstExceptionTable = next->Next;
    }
    
}

extern void _UnRegisterExceptionTables(ExceptionTableHeader *header);

/************************************************************************/
/* Purpose..: Get a exception record pointer							*/
/* Input....: pointer to return address									*/
/* Input....: pointer to ExceptionInfo struct for result				*/
/* Return...: ---														*/
/************************************************************************/
static void ExX86_FindExceptionRecord(char *returnaddr,ExceptionInfo *info)
{
    FunctionTableEntry *functab;
    int num, min, max, mid;
    char *Pc;
    static long def = 0;
    ExceptionTableHeader *table;
   
    // Assume not found
    info->exception_record = 0;
    info->action_pointer = 0; 
    
    // Find the correct function mapping table.  Since they are always non-overlapping,
    // and linked in order of decreasing address, the first table with a starting address
    // less than the pc we are looking for must be it
    
    table = FirstExceptionTable;
    while (table && table->First->Pc > returnaddr)
    {
        table = table->Next;
    }
    
    if (!table)
    {
        // no table found in the correct range
        return;
    }
    
    // Get pointer to the table and the number of entries (Last-First)
    functab = table->First;
    num = table->Last - functab;
    
    // binary search for the return address
    min = 0;
    max = num-1;
    while (min <= max)
    {
        mid = (max + min) / 2;
        Pc = functab[mid].Pc;
        
        // See if it is in the range
        if (returnaddr >= Pc && returnaddr < functab[mid+1].Pc)
        {
            char *ptr;
            int i;
            unsigned char *pcaddr;
            
            // Get pointer to exception record
            ptr = functab[mid].exctable;
            info->exception_record = (ExceptionRecord *)ptr;
         	
         	// Make sure we actually have actions for this guy
         	if ((info->exception_record->register_mask & 1) == 0)
         	{
	         	// Get number of offsets
	         	num = info->exception_record->num_offsets;
	         	ptr += sizeof(ExceptionRecord);
	         	
	         	// Now find the last return address pointer that
	         	// is lower than the actual return address

	         	for (i=0; i<num; i++)
	         	{
					pcaddr = *(unsigned char **)ptr;	         		
					ptr += 4;
					
					// the pcaddr is the address of the call, get
					// the address of the next instruction to match
					// the return address.  If you don't like this,
					// send complaints to Cam.
					
					if (*pcaddr == 0xe8)
					{
						// 32-bit relative call, always 5 byte
						pcaddr += 5;
					}
					else if (*pcaddr == 0xff)
					{
						int mod, rm;
						
						// indirect call, interpret the mode byte
						
						pcaddr++;
						
						mod = *pcaddr >> 6;
						rm = (*pcaddr) & 7;
						pcaddr++;
						
						// Register mode (mod=3) has no more byte, others do
						if (mod != 3)
						{
							// Watch for sib byte
							
							if (rm == 4)
							{
							    pcaddr++;
							}
							
							// and for 32-bit direct addressing
							
							if (mod == 0 && rm == 5)
							{
								// direct 32-bit skip the 4 byte addr
								pcaddr += 4;
							}
							else
							{
								// size of displacement depends on rm
								
								switch(mod)
								{
								case 1:
									pcaddr++;
									break;
								
								case 2:
									pcaddr += 4;
									break;
								}
							}
						}
					}
					else
					{
						// Error: not a call instruction.
						
						terminate();
					}
					
					if ((char *)pcaddr == returnaddr)
					{
	         	        info->action_pointer = *(char **)ptr;
					    return;
					}
	         	    ptr += 4;
	         	}
	        }
         	info->action_pointer = 0;
            return;
        }
        else if (Pc < returnaddr)
        {
            min = mid+1;
        }
        else
        {
            max = mid-1;
        }
    }
}


/************************************************************************/
/* Purpose..: Return current exception action type						*/
/* Input....: pointer to ActionIterator									*/
/* Return...: action type												*/
/************************************************************************/
static exaction_type ExX86_CurrentAction(const ActionIterator *iter)
{
	if(iter->info.action_pointer==0) return EXACTION_ENDOFLIST;
	return ((ex_destroylocal *)iter->info.action_pointer)->action&EXACTION_MASK;
}

/************************************************************************/
/* Purpose..: Move to next												*/
/* Input....: pointer to ActionIterator									*/
/* Return...: next action type											*/
/************************************************************************/
static exaction_type ExX86_NextAction(ActionIterator *iter)
{
	exaction_type	action;

	for(;;)
	{
		if(	iter->info.action_pointer==0
		||	((action=((ex_destroylocal *)iter->info.action_pointer)->action)&EXACTION_ENDBIT)!=0 )
		{	//	end of action list: find next exception record
			char *return_addr;
	
			return_addr=*(char **)(iter->current_bp+4);
			ExX86_FindExceptionRecord(return_addr,&iter->info);
			if(iter->info.exception_record==0) terminate();	//	cannot find matching exception record
			iter->current_bp=*(char **)iter->current_bp;
			if(iter->info.action_pointer==0) continue;		//	no actions
		}
		else
		{
			switch(action)
			{
			case EXACTION_DESTROYLOCAL:
				iter->info.action_pointer+=sizeof(ex_destroylocal); break;
			case EXACTION_DESTROYLOCALCOND:
				iter->info.action_pointer+=sizeof(ex_destroylocalcond); break;
			case EXACTION_DESTROYLOCALPOINTER:
				iter->info.action_pointer+=sizeof(ex_destroylocalpointer); break;
			case EXACTION_DESTROYLOCALARRAY:
				iter->info.action_pointer+=sizeof(ex_destroylocalarray); break;
			case EXACTION_DESTROYPARTIALARRAY:
				iter->info.action_pointer+=sizeof(ex_destroypartialarray); break;
			case EXACTION_DESTROYMEMBER:
			case EXACTION_DESTROYBASE:
				iter->info.action_pointer+=sizeof(ex_destroymember); break;
			case EXACTION_DESTROYMEMBERCOND:
				iter->info.action_pointer+=sizeof(ex_destroymembercond); break;
			case EXACTION_DESTROYMEMBERARRAY:
				iter->info.action_pointer+=sizeof(ex_destroymemberarray); break;
			case EXACTION_DELETEPOINTER:
				iter->info.action_pointer+=sizeof(ex_deletepointer); break;
			case EXACTION_DELETEPOINTERCOND:
				iter->info.action_pointer+=sizeof(ex_deletepointercond); break;
			case EXACTION_CATCHBLOCK:
				iter->info.action_pointer+=sizeof(ex_catchblock); break;
			case EXACTION_ACTIVECATCHBLOCK:
				iter->info.action_pointer+=sizeof(ex_activecatchblock); break;
			case EXACTION_SPECIFICATION:
				iter->info.action_pointer+=
					sizeof(ex_specification)
				+	((ex_specification *)iter->info.action_pointer)->specs*sizeof(void *);
				break;
			    
			default:
				terminate();	//	error
			}
		}
		return ((ex_destroylocal *)iter->info.action_pointer)->action&EXACTION_MASK;
	}
}


/************************************************************************/
/* Purpose..: Restore registers											*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to topmost exception record						*/
/* Return...: pointer to return PC										*/
/************************************************************************/
static char *ExX86_PopStackFrame(ThrowContext *context,ExceptionInfo *info)
{
    long *frame;
    char mask;
    char *returnaddr;
    
    // pop bp
    frame = (long *)context->EBP;

	// get new return address	
    returnaddr = (char *)frame[1];
    
    // get new frame pointer
    context->EBP = (char *)*frame;
    
    // pop pushed registers
    mask = info->exception_record->register_mask;
	if (mask & (1<<REG_EBX))
	{
	    context->EBX = *(--frame);
	}
	if (mask & (1<<REG_ESI))
	{
	    context->ESI = *(--frame);
	}
	if (mask & (1<<REG_EDI))
	{
	    context->EDI = *(--frame);
	}
	
	return returnaddr;
}

/************************************************************************/
/* Purpose..: Get a register/memory value								*/
/* Input....: pointer to throw context									*/
/* Input....: reference value											*/
/* Input....: size of value (one of: 1,2,4)								*/
/* Return...: the value (extended to long)								*/
/************************************************************************/
static long ExX86_GetValue(ThrowContext *context,long cond,short size)
{
	char	*ptr;

	if((cond&EXLREFREGMASK)==EXLREFREGBITS)
	{	//	register reference
		cond&=~EXLREFREGMASK;
		switch(cond)
		{
		case 3:
		    ptr=(char *)&(context->EBX);
		    break;
		case 6:
		    ptr=(char *)&(context->ESI);
		    break;
		case 7:
		    ptr=(char *)&(context->EDI);
		    break;
		default:
		    terminate();
		}
		switch(size)
		{
		default:	return *(char *)(ptr);
		case 2:		return *(short *)(ptr);
		case 4:		return *(long *)ptr;
		}
	}
	else
	{	//	local memory reference
		ptr=context->EBP+cond;
		switch(size)
		{
		default:	return *(char *)ptr;
		case 2:		return *(short *)ptr;
		case 4:		return *(long *)ptr;
		}
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroylocal struct								*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroylocal struct							*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DestroyLocal(ThrowContext *context,const ex_destroylocal *ex)
{
    asm
    {
      		mov		ecx, context
      		mov		ecx, ThrowContext.EBP[ecx]
      		mov		eax, ex
      		add		ecx, ex_destroylocal.local[eax]
      		call	ex_destroylocal.dtor[eax]
    }
	//((ConstructorDestructor)ex->dtor)(context->EBP+ex->local,-1);
}

/************************************************************************/
/* Purpose..: Unwind ex_destroylocalcond struct							*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroylocalcond struct						*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DestroyLocalCond(ThrowContext *context,const ex_destroylocalcond *ex)
{
	if(ExX86_GetValue(context,ex->cond,sizeof(local_cond_type)))
	{
	    asm
	    {
	      		mov		ecx, context
	      		mov		ecx, ThrowContext.EBP[ecx]
	      		mov		eax, ex
	      		add		ecx, ex_destroylocalcond.local[eax]
	      		call	ex_destroylocalcond.dtor[eax]
	    }
		//((ConstructorDestructor)ex->dtor)(context->EBP+ex->local,-1);
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroylocalpointer struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroylocalpointer struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DestroyLocalPointer(ThrowContext *context,const ex_destroylocalpointer *ex)
{
    void *location;
    
    location = (void *)ExX86_GetValue(context, ex->pointer, sizeof(void *));
    asm
    {
	        	mov		ecx, location;
	      		mov		eax, ex
	      		call	ex_destroylocalpointer.dtor[eax]
    }
	//((ConstructorDestructor)ex->dtor)((void *)ExX86_GetValue(context,ex->pointer,sizeof(void *)),-1);
}

/************************************************************************/
/* Purpose..: Unwind ex_destroylocalarray struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroylocalarray struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DestroyLocalArray(ThrowContext *context,const ex_destroylocalarray *ex)
{
	char					*pt;
	ConstructorDestructor	dtor;
	long					n,size;

	pt		= context->EBP+ex->localarray;
	dtor	= (ConstructorDestructor)ex->dtor;
	n		= ex->elements;
	size	= ex->element_size;

	for(pt=pt+size*n; n>0; n--)
	{
	    pt -= size;
		asm
		{
		    	mov			ecx, pt
		    	call		dtor
		}
	}
//	    dtor(ptr,-1);
}

/************************************************************************/
/* Purpose..: Unwind ex_destroypartialarray struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroypartialarray struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DestroyPartialArray(ThrowContext *context,const ex_destroypartialarray *ex)
{
	char					*pt;
	ConstructorDestructor	dtor;
	long					n,size;

	pt		= (char *)ExX86_GetValue(context,ex->arraypointer,sizeof(void *));
	n		= ExX86_GetValue(context,ex->arraycounter,sizeof(long));
	dtor	= (ConstructorDestructor)ExX86_GetValue(context,ex->dtor,sizeof(void *));
	size	= ExX86_GetValue(context,ex->element_size,sizeof(long));

	for(pt=pt+size*n; n>0; n--) 
	{
	    pt -= size;
		asm
		{
		    	mov			ecx, pt
		    	call		dtor
		}
	}	
	//dtor(ptr,-1);
}

/************************************************************************/
/* Purpose..: Unwind ex_destroymember struct							*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroymember struct						*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DestroyMember(ThrowContext *context,const ex_destroymember *ex, int dtor_arg)
{
    void *location;
    
    location =(char *)ExX86_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset;
    asm
    {
	        	mov		ecx, location;
	      		mov		eax, ex
	      		call	ex_destroymember.dtor[eax]
    }
	//((ConstructorDestructor)ex->dtor)((char *)ExX86_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset,-1);
}

/************************************************************************/
/* Purpose..: Unwind ex_destroymembercond struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroymembercond struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DestroyMemberCond(ThrowContext *context,const ex_destroymembercond *ex)
{
    void *location;
	if(ExX86_GetValue(context,ex->cond,sizeof(vbase_ctor_arg_type)))
	{
	    location = (char *)ExX86_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset;
	    asm
	    {
		        	mov		ecx, location;
		      		mov		eax, ex
		      		call	ex_destroymembercond.dtor[eax]
	    }
		//((ConstructorDestructor)ex->dtor)((char *)ExX86_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset,-1);
	}
}

/************************************************************************/
/* Purpose..: Unwind ex_destroymemberarray struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_destroymemberarray struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DestroyMemberArray(ThrowContext *context,const ex_destroymemberarray *ex)
{
	char					*pt;
	ConstructorDestructor	dtor;
	long					n,size;

	pt		= (char *)ExX86_GetValue(context,ex->objectptr,sizeof(void *))+ex->offset;
	dtor	= (ConstructorDestructor)ex->dtor;
	n		= ex->elements;
	size	= ex->element_size;

	for(pt=pt+size*n; n>0; n--)
	{
	    pt -= size;
		asm
		{
		    	mov			ecx, pt
		    	call		dtor
		}
	}	
	//dtor(ptr,-1);
}

/************************************************************************/
/* Purpose..: Unwind ex_deletepointer struct							*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_deletepointer struct						*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DeletePointer(ThrowContext *context,const ex_deletepointer *ex)
{
	((void (*)(void *))ex->deletefunc)((void *)ExX86_GetValue(context,ex->pointerobject,sizeof(void *)));
}

/************************************************************************/
/* Purpose..: Unwind ex_deletepointercond struct						*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to ex_deletepointercond struct					*/
/* Return...: ---														*/
/************************************************************************/
INLINE void ExX86_DeletePointerCond(ThrowContext *context,const ex_deletepointercond *ex)
{
	if(ExX86_GetValue(context,ex->cond,sizeof(local_cond_type)))
	{
		((void (*)(void *))ex->deletefunc)((void *)ExX86_GetValue(context,ex->pointerobject,sizeof(void *)));
	}
}

/************************************************************************/
/* Purpose..: Unwind stack												*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to topmost ExceptionInfo struct					*/
/* Input....: pointer to topmost action									*/
/* Input....: pointer to catcher										*/
/* Return...: ---														*/
/************************************************************************/
static void ExX86_UnwindStack(ThrowContext *context,ExceptionInfo *info, void *catcher)
{
	exaction_type	action;

#pragma exception_terminate		//	this will prevent exception exits during unwindind

	for(;;)
	{
		if(info->action_pointer==0)
		{
			char *return_addr;
	
			return_addr=ExX86_PopStackFrame(context,info);
			ExX86_FindExceptionRecord(return_addr,info);
			if(info->exception_record==0) terminate();		//	cannot find matching exception record
			continue;
		}

		action=((ex_destroylocal *)info->action_pointer)->action;
		switch(action&EXACTION_MASK)
		{
		case EXACTION_DESTROYLOCAL:
			ExX86_DestroyLocal(context,(ex_destroylocal *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroylocal); break;

		case EXACTION_DESTROYLOCALCOND:
			ExX86_DestroyLocalCond(context,(ex_destroylocalcond *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroylocalcond); break;

		case EXACTION_DESTROYLOCALPOINTER:
			ExX86_DestroyLocalPointer(context,(ex_destroylocalpointer *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroylocalpointer); break;

		case EXACTION_DESTROYLOCALARRAY:
			ExX86_DestroyLocalArray(context,(ex_destroylocalarray *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroylocalarray); break;

		case EXACTION_DESTROYPARTIALARRAY:
			ExX86_DestroyPartialArray(context,(ex_destroypartialarray *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroypartialarray); break;

		case EXACTION_DESTROYMEMBER:
			ExX86_DestroyMember(context,(ex_destroymember *)info->action_pointer, -1);
			info->action_pointer+=sizeof(ex_destroymember); break;

		case EXACTION_DESTROYBASE:
			ExX86_DestroyMember(context,(ex_destroymember *)info->action_pointer, 0);
			info->action_pointer+=sizeof(ex_destroymember); break;

		case EXACTION_DESTROYMEMBERCOND:
			ExX86_DestroyMemberCond(context,(ex_destroymembercond *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroymembercond); break;

		case EXACTION_DESTROYMEMBERARRAY:
			ExX86_DestroyMemberArray(context,(ex_destroymemberarray *)info->action_pointer);
			info->action_pointer+=sizeof(ex_destroymemberarray); break;

		case EXACTION_DELETEPOINTER:
			ExX86_DeletePointer(context,(ex_deletepointer *)info->action_pointer);
			info->action_pointer+=sizeof(ex_deletepointer); break;

		case EXACTION_DELETEPOINTERCOND:
			ExX86_DeletePointerCond(context,(ex_deletepointercond *)info->action_pointer);
			info->action_pointer+=sizeof(ex_deletepointercond); break;

		case EXACTION_CATCHBLOCK:
			if(catcher==(ex_catchblock *)info->action_pointer) return;	//	finished unwinding
			info->action_pointer+=sizeof(ex_catchblock); break;

		case EXACTION_ACTIVECATCHBLOCK:
			{
				CatchInfo		*catchinfo;

				catchinfo=(CatchInfo *)(context->EBP+((ex_activecatchblock *)info->action_pointer)->cinfo_ref);
				if(catchinfo->dtor)
				{
					// re-throwing exception from this block
					if(context->location == catchinfo->location)
					{
					    // pass destruction responsibility to re-thrown exception
					    context->dtor = catchinfo->dtor;
					}
					else
					{
						asm
						{
							mov		eax, catchinfo
							mov		ecx, CatchInfo.location[eax]
							call	CatchInfo.dtor[eax]
						}
						//DTORCALL_COMPLETE(catchinfo->dtor,catchinfo->location);
					}
				}
				
				info->action_pointer+=sizeof(ex_activecatchblock);
			}
			break;

		case EXACTION_SPECIFICATION:
			if(catcher==(void *)info->action_pointer) return;	//	finished unwinding
			info->action_pointer+=
				sizeof(ex_specification)
			+	((ex_specification *)info->action_pointer)->specs*sizeof(void *);
			break;

		default:
			terminate();			//	error
		}
		if(action&EXACTION_ENDBIT) info->action_pointer=0;
	}
}


/************************************************************************/
/* Purpose..: Check if an exception is in a specification list			*/
/* Input....: pointer to exception type string							*/
/* Input....: pointer to specification list								*/
/* Return...: ---														*/
/************************************************************************/
static int ExX86_IsInSpecification(char *extype,ex_specification *spec)
{
	long	i,offset;

	for(i=0; i<spec->specs; i++)
	{
		if(__throw_catch_compare(extype,spec->spec[i],&offset)) return 1;
	}
	return 0;
}

/************************************************************************/
/* Purpose..: Unexpected handler										*/
/* Input....: pointer to throw context									*/
/* Return...: --- (this function will never return)						*/
/************************************************************************/
extern "C" void __unexpected(CatchInfo* catchinfo)
{	
	ex_specification *unexp=(ex_specification *)catchinfo->stacktop;

#pragma exception_magic

	try {
		unexpected();
	}
	catch(...)
	{	//	unexpected throws an exception => check if the exception matches the specification
		if(ExX86_IsInSpecification((char *)((CatchInfo *)&__exception_magic)->typeinfo,unexp))
		{	//	new exception is in specification list => rethrow
			throw;
		}
#ifndef _MSL_NO_CPP_NAMESPACE
		if(ExX86_IsInSpecification("!std::bad_exception!!",unexp))   // hh 980124 added extra '!' to !bad_exception! per Andreas' instructions
		{	//	"bad_exception" is in specification list => throw bad_exception()
			throw bad_exception();
		}
#else
		if(ExX86_IsInSpecification("!bad_exception!!",unexp))   // hh 980124 added extra '!' to !bad_exception! per Andreas' instructions
		{	//	"bad_exception" is in specification list => throw bad_exception()
			throw bad_exception();
		}
#endif
		//	cannot handle exception => terminate();
	}
	terminate();
}

/************************************************************************/
/* Purpose..: Restore registers and branch to catcher					*/
/* Input....: pointer to throw context									*/
/* Input....: destination PC											*/
/* Return...: ---														*/
/************************************************************************/
static asm void ExX86_LongJump(ThrowContext *context,void *pc)
{
/*
	move.l	4(sp),a0		//	context
	move.l	8(sp),a1		//	pc
	movem.l	struct(ThrowContext.data_regs)+3*sizeof(long)(a0),d3-d7
	movem.l	struct(ThrowContext.addr_regs)+2*sizeof(long)(a0),a2-a6
//
//	Note:	we do not restore A7 because this would destroy the catch temporary.
//			A7 is restored when __end__catch() is called.
//
	jmp		(a1)			//	longjmp to catcher
*/
}

/************************************************************************/
/* Purpose..: Handle unexpected exception								*/
/* Input....: pointer to throw context									*/
/* Input....: pointer to topmost ExceptionInfo struct					*/
/* Input....: pointer to specification record							*/
/* Return...: ---														*/
/************************************************************************/
static void ExX86_HandleUnexpected(ThrowContext *context,ExceptionInfo *info,ex_specification *unexp)
{
	CatchInfo	*catchinfo;

#pragma exception_terminate		//	this will prevent exception exits during unwinding

	ExX86_UnwindStack(context,info,unexp);	//	unwind stack to failing specification

	//	initialize catch info struct
	catchinfo=(CatchInfo *)(context->EBP+unexp->cinfo_ref);
	catchinfo->location		= context->location;
	catchinfo->typeinfo		= context->throwtype;
	catchinfo->dtor			= context->dtor;
	catchinfo->stacktop		= unexp;		//	the __unexpected will never call __end_catch
											//	so we can resue this field


	asm
	{
			mov		eax, unexp
			mov		ecx, ex_specification.pcoffset[eax]
			mov		eax, context
			mov		ebx, ThrowContext.EBX[eax]
			mov		esi, ThrowContext.ESI[eax]
			mov		edi, ThrowContext.EDI[eax]
			mov		ebp, ThrowContext.EBP[eax]
			
			// reinit the floating point processor
			
			finit
			fwait
			
			jmp		ecx
	}
	//	jump to exception handler
	//ExX86_LongJump(context,unexp->pcoffset);
}


/************************************************************************/
/* Purpose..: Throw (rethrow) current exception							*/
/* Input....: pointer to throw context									*/
/* Return...: ---														*/
/************************************************************************/
static void ExX86_ThrowHandler(ThrowContext *context)
{
	ActionIterator	iter;
	ExceptionInfo	info;
	exaction_type	action;
	ex_catchblock	*catchblock;
	CatchInfo		*catchinfo;
	long			offset;

	//	find first ExceptionRecord

	ExX86_FindExceptionRecord(context->returnaddr,&info);
	if(info.exception_record==0) terminate();	//	cannot find matching exception record

	if(context->throwtype==0)
	{	//	rethrow, find most recent excpetion
		iter.info		= info;
		iter.current_bp	= context->EBP;
		for(action=ExX86_CurrentAction(&iter);; action=ExX86_NextAction(&iter))
		{
			switch(action)
			{
			case EXACTION_ACTIVECATCHBLOCK:
				break;
	
			case EXACTION_ENDOFLIST:
			case EXACTION_DESTROYLOCAL:
			case EXACTION_DESTROYLOCALCOND:
			case EXACTION_DESTROYLOCALPOINTER:
			case EXACTION_DESTROYLOCALARRAY:
			case EXACTION_DESTROYPARTIALARRAY:
			case EXACTION_DESTROYMEMBER:
			case EXACTION_DESTROYBASE:
			case EXACTION_DESTROYMEMBERCOND:
			case EXACTION_DESTROYMEMBERARRAY:
			case EXACTION_DELETEPOINTER:
			case EXACTION_DELETEPOINTERCOND:
			case EXACTION_CATCHBLOCK:
			case EXACTION_SPECIFICATION:
				continue;

			case EXACTION_TERMINATE:
			default:
				terminate();			//	cannot find find most recent excpetion
			}
			break;
		}
		catchinfo=(CatchInfo *)(iter.current_bp+((ex_activecatchblock *)iter.info.action_pointer)->cinfo_ref);
		context->throwtype	= (char *)catchinfo->typeinfo;
		context->location	= catchinfo->location;
		context->dtor		= 0;
		// original active catch block is still responsible for destruction
		context->catchinfo	= catchinfo;
	}
	else context->catchinfo=0L;

	//	find matching exception handler

	iter.info		= info;
	iter.current_bp	= context->EBP;
	for(action=ExX86_CurrentAction(&iter);; action=ExX86_NextAction(&iter))
	{
		switch(action)
		{
		case EXACTION_CATCHBLOCK:
			if(__throw_catch_compare(context->throwtype,((ex_catchblock *)iter.info.action_pointer)->catch_type,&offset))
			{
				break;
			}
			continue;

		case EXACTION_SPECIFICATION:
			if(!ExX86_IsInSpecification(context->throwtype,(ex_specification *)iter.info.action_pointer))
			{	//	unexpected specification
				ExX86_HandleUnexpected(context,&info,(ex_specification *)iter.info.action_pointer);
				//	we will never return from this function call
			}
			continue;

		case EXACTION_ENDOFLIST:
		case EXACTION_DESTROYLOCAL:
		case EXACTION_DESTROYLOCALCOND:
		case EXACTION_DESTROYLOCALPOINTER:
		case EXACTION_DESTROYLOCALARRAY:
		case EXACTION_DESTROYPARTIALARRAY:
		case EXACTION_DESTROYMEMBER:
		case EXACTION_DESTROYBASE:
		case EXACTION_DESTROYMEMBERCOND:
		case EXACTION_DESTROYMEMBERARRAY:
		case EXACTION_DELETEPOINTER:
		case EXACTION_DELETEPOINTERCOND:
		case EXACTION_ACTIVECATCHBLOCK:
			continue;

		case EXACTION_TERMINATE:
		default:
			terminate();			//	cannot find matching catch block
		}
		break;
	}

	//	we have found a matching catch block
	catchblock=(ex_catchblock *)iter.info.action_pointer;
	ExX86_UnwindStack(context,&info,catchblock);

	//	initialize catch info struct
	catchinfo=(CatchInfo *)(context->EBP+catchblock->cinfo_ref);
	catchinfo->location		= context->location;
	catchinfo->typeinfo		= context->throwtype;
	catchinfo->dtor			= context->dtor;
	if(*context->throwtype=='*')
	{	//	pointer match (create a pointer copy with adjusted offset)
		catchinfo->sublocation	= &catchinfo->pointercopy;
		catchinfo->pointercopy	= *(long *)context->location+offset;
	}
	else
	{	//	traditional or class match (directly adjust offset)
		catchinfo->sublocation	= (char *)context->location+offset;
	}

	//	jump to exception handler
	//ExX86_LongJump(context,info.current_function+catchblock->catch_pcoffset);
	asm
	{
			mov		eax, catchblock
			mov		ecx, ex_catchblock.catch_pcoffset[eax]
			mov		eax, context
			mov		ebx, ThrowContext.EBX[eax]
			mov		esi, ThrowContext.ESI[eax]
			mov		edi, ThrowContext.EDI[eax]
			mov		ebp, ThrowContext.EBP[eax]
			
			// reinit the floating point processor
			
			finit
			fwait
			
			jmp		ecx
	}
}


/************************************************************************/
/* Purpose..: Throw (rethrow) current exception							*/
/* Input....: pointer to throw type (0L: rethrow)						*/
/* Input....: pointer to complete throw object (0L: rethrow)			*/
/* Input....: pointer to throw object destructor (0L: no destructor		*/
/* Return...: ---														*/
/************************************************************************/
extern asm void __throw(char *throwtype,void *location,void *dtor)
{
	
	//	allocate and initialize ThrowContext buffer
	asm
	{
			push	ebp
			mov		ebp, esp
			
			sub		esp, ThrowContext			// allocate space for the throw context on stack
			mov		0[esp], ebx					// store the volatile registers in it
			mov		4[esp], esi
			mov		8[esp], edi
			mov		eax, 0[ebp]					// get the caller's ebp
			mov		12[esp], eax				// save in context
			mov		eax, 4[ebp]					// get the return address
			mov		16[esp], eax				// save in context
			mov		eax, throwtype				// get and save the params
			mov		20[esp], eax
			mov		eax, location
			mov		24[esp], eax
			mov		eax, dtor
			mov		28[esp], eax
			
			push	esp							// push address of throw context
			call 	ExX86_ThrowHandler			// and call never to return
			ret
	}

}

/************************************************************************/
/* Purpose..: Deinitialize CatchInfo struct								*/
/* Input....: pointer to catchinfo struct								*/
/* Return...: ---														*/
/************************************************************************/
extern asm void __end__catch(CatchInfo* catchinfo)
{
}

// hh  980124 added extra '!' to !bad_exception! per Andreas' instructions
// blc 980914 added fixes from blargg@flash.net for destructor call during rethrow