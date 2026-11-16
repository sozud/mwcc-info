/*  Metrowerks Standard Library  */

/*  $Date: 1999/02/02 18:18:03 $ 
 *  $Revision: 1.3 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*
 *	File:		unix.h
 *
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 */

#ifndef	_UNIX
#define	_UNIX

#include <ansi_parms.h>          /* hh 971206 */

#ifndef __stdio__
#include <stdio.h>
#endif

#ifndef _FCNTL
#include <fcntl.h>
#endif

#ifndef _STAT
#include <stat.h>
#endif

#ifndef	_UNISTD
#include <unistd.h>
#endif

#ifndef _UTIME
#include <utime.h>
#endif

// #ifndef _UTSNAME
// #include <utsname.h>
// #endif

#pragma options align=native
/* #pragma direct_destruction off */  /* vss 980807 */

#ifdef __cplusplus
extern "C" {
#endif

int 	_fileno(FILE *stream);
FILE *	_fdopen(int fildes, char *type);
long 	_tell(int fildes);

/*  get rid of these in Pro5   vss 990202
#define	fileno	_fileno
#define	fdopen	_fdopen
#define	tell	_tell
*/

#ifdef __cplusplus
}
#endif

#pragma options align=reset
/* #pragma direct_destruction reset */  /* vss 980807 */

#endif

/*
	Change Record
 * hh 971206  Added #include <ansi_parms.h>
 * vss 980807 remove pragma  - no longer supported by compiler
 * vss 990202 Remove fileno, fdopen, tell - obsolete in Pro5
 */
