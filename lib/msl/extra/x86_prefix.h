#ifndef __X86_PREFIX_H
#define __X86_PREFIX_H

/* Defines needed to compile windows headers */

#ifndef _X86_
	#define _X86_ 1
#endif

/* Pro4 compilers should automatically define this to appropriate value */
#ifndef _M_IX86
	#define _M_IX86	500
#endif

#ifndef WIN32
	/* same as definition in OLE2.h where 100 implies WinNT version 1.0 */
	#define WIN32 100
#endif

#ifndef _WIN32
	#define _WIN32 1
#endif

#ifndef _MSC_VER
	/* we implement the feature set of CL 9.0 (MSVC++ 2) */
	#define _MSC_VER 900
#endif

#ifndef _CRTAPI1
	#define _CRTAPI1 __cdecl
#endif

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif

#ifndef _WIN32_IE
	#define _WIN32_IE 0x0400
#endif


/* These keywords are built in to MSVC++ */
#ifndef _stdcall
	#define _stdcall __stdcall
#endif
#ifndef _cdecl
	#define _cdecl
#endif
#ifndef _asm
	#define _asm asm
#endif
#ifndef _inline
	#define _inline inline
#endif

/* these types are defined in MSVC++ */
#ifndef __int8
	#define __int8 char
#endif
#ifndef __int16
	#define __int16 short
#endif
#ifndef __int32
	#define __int32 long
#endif
#ifndef __int64
	#define __int64  long long
#endif

/* compiler settings visible in MSVC++ */

#if __option(unsigned_char)
	#define _CHAR_UNSIGNED
#endif

#if __option(RTTI)
	#define _CPPRTTI
#endif

#if __option(exceptions)
	#define _CPPUNWIND
#endif

#ifndef _STDCALL_SUPPORTED
	#define _STDCALL_SUPPORTED
#endif

#endif /* __X86_PREFIX_H */
