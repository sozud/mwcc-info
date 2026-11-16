/************************************************************************/
/*	Project...:	Standard C++ Library									*/
/*	Name......:	CPlusLib.h												*/
/*	Purpose...:	C++ specific runtime functions							*/
/*  Copyright.: Copyright © 1993-1997 Metrowerks, Inc.					*/
/************************************************************************/

#ifndef __CPLUSLIB__
#define __CPLUSLIB__

#include <stddef.h>

//
//	#define CABI_ZEROOFFSETVTABLE		1
//		
//	forces vtable pointer to point to the first virtual function pointer,
//	the RTTI header will be at offset -sizeof(RTTIVTableHeader)
//	This flag must be set in CABI.c and in MWRTTI.cp
//

//#define __INTEL__ 1	//	define to 1 to test WIN_ABI

#ifdef __INTEL__

#define CABI_ZEROOFFSETVTABLE		1

#else

#define CABI_ZEROOFFSETVTABLE		0

#endif

//
//	invisible constructor/destructor argument handling
//

#ifdef __INTEL__

#define CTORARG_TYPE			int
#define CTORARG_PARTIAL			(0)				//	construct non-virtual bases
#define CTORARG_COMPLETE		(1)				//	construct all bases	

#define CTORCALL_COMPLETE(ctor,objptr)\
	(((void (*)(void *,CTORARG_TYPE))ctor)(objptr,CTORARG_COMPLETE))

#define DTORARG_TYPE			int
#define DTORARG_DELETEFLAG		0x0001			//	delete after destruction
#define DTORARG_VECTORFLAG		0x0002			//	array destruction

/*
// 960801: Changed this to fix a bug in deleting arrays of objects
//#define DTORCALL_COMPLETE(dtor,objptr)\
//	(((void (*)(void *))dtor)(objptr))

This won't work because it will try and pass the "this"
pointer incorrectly.
//	(((void (*)(void *,DTORARG_TYPE))dtor)(objptr,0))

The following code should work but the x86 libraries shouldn't
be using DTORCALL_COMPLETE function anyway. Also you can't
have an asm block all on one line like this.

#define DTORCALL_COMPLETE(dtor,objptr)\
    asm {                             \
    	MOV ECX, objptr               \
    	MOV EAX, 0                    \
    	PUSH 0                        \
    	                              \
    	CALL dtor                     \
    	                              \
    	ADD ESP, 4                    \
                                      \
    }

*/
#define DTORCALL_PARTIAL(dtor,objptr)\
	(((void (*)(void *))dtor)(objptr))

#else

#define CTORARG_TYPE			short
#define CTORARG_PARTIAL			(0)				//	construct non-virtual bases
#define CTORARG_COMPLETE		(1)				//	construct all bases	

#if __CFM68K__

	long __getA7(void) = { 0x200F };
	void __setA7(long a7:__d0) = { 0x2E40 };

#define CTORCALL_COMPLETE(ctor,objptr)\
	{\
		long __savedA7 = __getA7();\
		(((void (*)(void *,CTORARG_TYPE))ctor)(objptr,CTORARG_COMPLETE));\
		__setA7(__savedA7);\
	}

#else

#define CTORCALL_COMPLETE(ctor,objptr)\
	(((void (*)(void *,CTORARG_TYPE))ctor)(objptr,CTORARG_COMPLETE))

#endif

#define DTORARG_TYPE			short
#define DTORARG_PARTIAL			(0)				//	destroy non-virtual bases	
#define DTORARG_COMPLETE		(-1)			//	destroy all bases	
#define DTORARG_DELETE			(1)				//	destroy all bases and delete object

#define DTORCALL_COMPLETE(dtor,objptr)\
	(((void (*)(void *,DTORARG_TYPE))dtor)(objptr,DTORARG_COMPLETE))

#define DTORCALL_PARTIAL(dtor,objptr)\
	(((void (*)(void *,DTORARG_TYPE))dtor)(objptr,DTORARG_PARTIAL))

#endif

typedef void *ConstructorDestructor;	//	constructor/destructor function	pointer

typedef struct PTMF {
	long	this_delta;					//	delta to this pointer
	long	vtbl_offset;				//	offset of virtual function pointer in vtable (<0: non-virtual function address)
	union {
		void	*func_addr;				//	non-virtual function address
		long	ventry_offset;			//	offset of vtable pointer in class
	}	func_data;
}	PTMF;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MC68K__
extern long		__ptmf_test(const PTMF *ptmf);
extern long		__ptmf_cmpr(const PTMF *ptmf1,const PTMF *ptmf2);
extern void		__ptmf_call(...);
extern void		__ptmf_scall(...);
#endif

// hh 971207  Added support for namespace
// A lot of this stuff comes from CPPRTL.C
#ifdef __INTEL__
	// Type of a destructor
	typedef void (*destructor)(void *object, int Free);
	
	// Data structure for pointers to member functions
	typedef struct MemberFuncPointerType {
		long	this_delta;				//	delta to this pointer
		long	vtbl_offset;			//	offset to vtable (-1: not a virtual function)
		union {
			void	*func_addr;			//	nonvirtual function address
			long	ventry_offset;		//	offset of virtual function entry in vtable
		}	func_data;
	} MemberFuncPointerType;
	
	int __ptmf_test(MemberFuncPointerType *ptmf);
	int __ptmf_cmpr(MemberFuncPointerType *p1, MemberFuncPointerType *p2);
	void *__ptmf_cast(int delta, MemberFuncPointerType *in, MemberFuncPointerType *out);
	void * __copy(void *dest, void *source, unsigned long len);
	void __declspec(naked) *__ptmf_call();
	void __declspec(naked) *__ptmf_scall();
#else
	extern PTMF		*__ptmf_cast(long offset,const PTMF *ptmfrom,PTMF *ptmto);
	extern void		*__copy(char *to,char *from,size_t size);
#endif

extern void		__construct_array(void *block,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n);
extern void		__destroy_arr(void *block,ConstructorDestructor dtor,size_t size,size_t n);
extern void		*__construct_new_array(void *block,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n);
extern void		__destroy_new_array(void *block,ConstructorDestructor dtor);
extern void		*__destroy_new_array2(void *block,ConstructorDestructor dtor);

#ifdef __cplusplus
}
#endif

#endif

// hh 971207 Added support for namespaces.  This involved getting everything prototyped properly
