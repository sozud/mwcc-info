/************************************************************************/
/*  Project...: Standard C++ Library                                    */
/*  Name......: ExceptionX86.h                                          */
/*  Purpose...: C++ specific runtime functions                          */
/*  Copyright.: ©Copyright 1993-99 by metrowerks inc                    */
/************************************************************************/

#ifndef _EXCEPTX86_H
#define _EXCEPTX86_H

#define SEGMAPNAME		__segtable__
#define SEGMAPNAMESTR	"__segtable__"

//	non-volatile register restore bits

#define EXX86_DIRECTSTORE		0x8000			//	record is stored in table
#define EXX86_RESERVED			0x4000			//	reserved for future extensions
#define EXX86_REGMASK			(EXX86_DATAREGMASK|EXX86_ADDRREGMASK|EXX86_FLOATREGMASK)

#define EXX86_DATAREGMASK		0x001F			//	data register mask
#define EXX86_DATAREGFIRSTBITM	0x0001			//	first bit in mask
#define EXX86_DATAREGFIRSTBIT	0				//	first bit in mask
#define EXX86_DATAREGFIRSTREG	3				//	first bit register D3
#define EXX86_DATAREGS			5				//	number of non-volatile registers D3-D7

#define EXX86_ADDRREGMASK		0x03E0			//	address register mask
#define EXX86_ADDRREGFIRSTBITM	0x0020			//	first bit in mask
#define EXX86_ADDRREGFIRSTBIT	5				//	first bit in mask
#define EXX86_ADDRREGFIRSTREG	2				//	first bit register A2
#define EXX86_ADDRREGS			5				//	number of non-volatile registers A2-A6

#define EXX86_FLOATREGMASK		0x3C00			//	address register mask
#define EXX86_FLOATREGFIRSTBITM	0x0400			//	first bit in mask
#define EXX86_FLOATREGFIRSTBIT	10				//	first bit in mask
#define EXX86_FLOATREGFIRSTREG	4				//	first bit register FP4
#define EXX86_FLOATREGS			4				//	number of non-volatile registers FP4-FP7

#pragma pack(push, 1)

#ifdef __cplusplus    // hh 971207 Added namespace support
	extern "C" {
#endif 

//	This data structue is created for each function and is pointed to by the function table
typedef struct ExceptionRecord {
	char    		register_mask;		//	register restore mask
	long			actions_offset;		//  offset from function start to the action table
	unsigned short  num_offsets;		//  number of offset/action pairs, which must follow
}	ExceptionRecord;

#pragma pack(pop)

//	action table definitions:

typedef unsigned short exaction_type;

#define EXLREFREGMASK					0xFFFFFF00	//	local register ref mask
#define EXLREFREGBITS					0x80000000	//	local register bits
//	Note:	D0...D7 = EXLREFREGBITS+<x>
//			A0...A7 = EXLREFREGBITS+8+<x>

#define EXACTION_ENDBIT					0x8000	//	action table end bit
#define EXACTION_MASK					0x00FF	//	action mask

#define EXACTION_ENDOFLIST				0x0000	//	end of action list
#define EXACTION_ERROR					0x00FF	//	error

#pragma pack(push, 2)

#define EXACTION_DESTROYLOCAL			0x0001	//	destroy local 
typedef struct ex_destroylocal {
	exaction_type	action;						//	EXACTION_DESTROYLOCAL
	long			local;						//	reference to local variable (frame offset)
	void			*dtor;						//	pointer to destructor function
}	ex_destroylocal;

#define EXACTION_DESTROYLOCALCOND		0x0002	//	destroy local 
typedef struct ex_destroylocalcond {
	exaction_type	action;						//	EXACTION_DESTROYLOCALCOND
	long			local;						//	reference to local variable (frame offset)
	long			cond;						//	reference to conditional variable (frame offset or register)
	void			*dtor;						//	pointer to destructor function
}	ex_destroylocalcond;

//#define EXACTION_DESTROYLOCALOFFSET	0x0003	//	destroy local 
//typedef struct ex_destroylocaloffset {
//	exaction_type	action;						//	EXACTION_DESTROYLOCALOFFSET
//	long			local;						//	reference to local variable
//	void			*dtor;						//	pointer to destructor function
//	long			offset;						//	offset
//}	ex_destroylocaloffset;

#define EXACTION_DESTROYLOCALPOINTER	0x0004	//	destroy local 
typedef struct ex_destroylocalpointer {
	exaction_type	action;						//	EXACTION_DESTROYLOCALPOINTER
	long			pointer;					//	reference to local pointer
	void			*dtor;						//	pointer to destructor function
}	ex_destroylocalpointer;

#define EXACTION_DESTROYLOCALARRAY		0x0005	//	destroy local 
typedef struct ex_destroylocalarray {
	exaction_type	action;						//	EXACTION_DESTROYLOCALPOINTER
	long			localarray;					//	reference to local pointer
	void			*dtor;						//	pointer to destructor function
	long			elements;					//	number of array elements
	long			element_size;				//	size of one array element
}	ex_destroylocalarray;

#define EXACTION_DESTROYPARTIALARRAY	0x0006	//	destroy local 
typedef struct ex_destroypartialarray {
	exaction_type	action;						//	EXACTION_DESTROYPARTIALARRAY
	long			arraypointer;				//	reference to array pointer
	long			arraycounter;				//	reference to array counter
	long			dtor;						//	pointer to destructor function
	long			element_size;				//	size of one array element
}	ex_destroypartialarray;

#define EXACTION_DESTROYMEMBER			0x0007	//	destroy member
#define EXACTION_DESTROYBASE			0x000C	//  destroy base class
typedef struct ex_destroymember {
	exaction_type	action;						//	EXACTION_DESTROYMEMBER
	long			objectptr;					//	pointer to complete object pointer
	void			*dtor;						//	pointer to destructor function
	long			offset;						//	offset of member in complete object
}	ex_destroymember;

#define EXACTION_DESTROYMEMBERCOND		0x0008	//	destroy member
typedef struct ex_destroymembercond {
	exaction_type	action;						//	EXACTION_DESTROYMEMBERCOND
	long			objectptr;					//	pointer to complete object pointer
	long			cond;						//	pointer to condition flag object
	void			*dtor;						//	pointer to destructor function
	long			offset;						//	offset of member in complete object
}	ex_destroymembercond;

#define EXACTION_DESTROYMEMBERARRAY		0x0009	//	destroy member array
typedef struct ex_destroymemberarray {
	exaction_type	action;						//	EXACTION_DESTROYMEMBERARRAY
	long			objectptr;					//	pointer to complete object pointer
	void			*dtor;						//	pointer to destructor function
	long			offset;						//	offset of member in complete object
	long			elements;					//	number of array elements
	long			element_size;				//	size of one array element
}	ex_destroymemberarray;

#define EXACTION_DELETEPOINTER			0x000A	//	delete pointer
typedef struct ex_deletepointer {
	exaction_type	action;						//	EXACTION_DELETEPOINTER
	long			pointerobject;				//	pointer object reference
	void			*deletefunc;				//	pointer to delete function
}	ex_deletepointer;

#define EXACTION_DELETEPOINTERCOND		0x000B	//	delete pointer
typedef struct ex_deletepointercond {
	exaction_type	action;						//	EXACTION_DELETEPOINTERCOND
	long			pointerobject;				//	pointer object reference
	void			*deletefunc;				//	pointer to delete function
	long			cond;						//	reference to conditional variable
}	ex_deletepointercond;

#define EXACTION_CATCHBLOCK				0x0010	//	catch block
typedef struct ex_catchblock {
	exaction_type	action;						//	EXACTION_CATCHBLOCK
	char			*catch_type;				//	pointer to catch type info (0L: catch(...))
	long			catch_pcoffset;				//	function relative catch label offset
	long			cinfo_ref;					//	reference to local CatchInfo struct
}	ex_catchblock;

#define EXACTION_ACTIVECATCHBLOCK		0x0011	//	active catch block
typedef struct ex_activecatchblock {
	exaction_type	action;						//	EXACTION_ACTIVECATCHBLOCK
	long			cinfo_ref;					//	reference to local CatchInfo struct
}	ex_activecatchblock;

#define EXACTION_TERMINATE				0x0012	//	active catch block
typedef struct ex_terminate {
	exaction_type	action;						//	EXACTION_TERMINATE
}	ex_terminate;


#define EXACTION_SPECIFICATION			0x0013	//	active catch block
typedef struct ex_specification {
	exaction_type	action;						//	EXACTION_SPECIFICATION
	unsigned short	specs;						//	number of specializations (0-n)
	long			pcoffset;					//	function relative offset
	long			cinfo_ref;					//	reference to local CatchInfo struct
	char			*spec[];					//	array of catch type pointers
}	ex_specification;

// Struct of the function mapping table

typedef struct FunctionTableEntry
{
    char *Pc;
    char *exctable;
} FunctionTableEntry;

// Struct used to register exception tables

typedef struct ExceptionTableHeader
{
	FunctionTableEntry *First;
	FunctionTableEntry *Last;
	struct ExceptionTableHeader *Next;
} ExceptionTableHeader;



// Register and deregister exception tables for an app or dll
void __destroy_global_chain(void);
extern void _RegisterExceptionTables(ExceptionTableHeader *header);
extern void _UnRegisterExceptionTables(ExceptionTableHeader *header);

// Pointers to the start and end of the function mapping table

extern FunctionTableEntry _PcToActionStart, _PcToActionEnd;

#ifdef __cplusplus    // hh 971207 Added namespace support
	}
#endif

#pragma pack(pop)

//	code -> action mapping tables

#define EXX86_DIRECTSTOREOFFSET	0x80000000L	//	direct store offset bit

#endif

// hh 971207 Added namespace support
