/*  Metrowerks Standard Library  */

/*  $Date: 1999/05/07 18:44:51 $ 
 *  $Revision: 1.3 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
/*
 *	crtl.h
 *
 *		This header provides various runtime declarations.
 */
 
 #ifndef _CTRL_H
 #define _CTRL_H

#include <ansi_parms.h>          /* hh 971206 */

#ifdef __cplusplus          /* hh 971206 */
	extern "C" {
#endif

#if !(__dest_os == __win32_os || __dest_os == __wince_os)
	#error This is a Win32 header and should only be used when targeting Win32.
#endif

/* This flag is set when a DLL is running terminate code */
extern int _DllTerminate;	// 961218 KO


/* Envioronment pointers */
#if (__dest_os==__win32_os)
  extern char *(*environ);
  extern char **_Environ;	/* vss 981210 */ /* mm 990115 */
#endif


/* Argument list variables */

extern char **__argv;
extern int __argc;


/* Structure alocated for each ed file handle */

typedef struct
{
    void *handle;
    char translate;
    char append;
} FileStruct;

/* Pointers to file structure indexed by the handle */

#define NUM_HANDLES 256

/* The table of handles */
extern FileStruct *_HandleTable[NUM_HANDLES];
extern int _HandPtr;


/* functions */
extern void _RunInit();
extern void _SetupArgs();
extern void _CRTStartup();

#ifdef __cplusplus          /* hh 971206 */
	}
#endif

#endif /*  _CTRL_H  */

/* Change History
961218 KO	Moved the definition of _DllTerminate to dllmaincrt.c and moved the
			declaration here.
 * hh 971206 Added #include <ansi_parms.h>
 * hh 971206 Added namespace support
*/
