//mflag.h
//960502 bkoz to put in flag for modena library for runtime exceptions

#ifndef _MFLAG_
#define _MFLAG_
	#if 1		//	0: old CPlusLib 1: modena C++ lib
		#define	__MODENALIB__
	#else		//	CPlusLib only
		#undef	__MODENALIB__
	#endif
#endif