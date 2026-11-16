/*  Metrowerks Standard Library  */

/*  $Date: 1999/01/22 23:44:12 $ 
 *  $Revision: 1.2 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	File:		fcntl.h
 *
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 */

#ifndef	_FCNTL
#define	_FCNTL

#include <ansi_parms.h>          /* hh 971206 */

#pragma options align=native
/* #pragma direct_destruction off */  /* vss 980807 */

/* local typedefs (used by creat) */
#ifndef	_STAT
#include <stat.win32.h> /* mm 971129 for mode_t*/
#endif

/*
 *	Mode values accessible to open()
 */

#define O_TEXT			0x0			/* 960827: Added this for Visual C++ compatibility. */
#define O_RDWR			0x1			/* open the file in read/write mode */	/* mm 980420 */
#define O_RDONLY		0x2			/* open the file in read only mode */	/* mm 980420 */
#define O_WRONLY		0x4			/* open the file in write only mode */	/* mm 980420 */
#define O_APPEND		0x0100		/* open the file in append mode */
#define O_CREAT			0x0200		/* create the file if it doesn't exist */
#define O_EXCL			0x0400		/* if the file already exists don't create it again */
#define O_TRUNC			0x0800		/* truncate the file after opening it */
#define O_NRESOLVE		0x1000		/* Don't resolve any aliases */
#define O_ALIAS			0x2000		/* Open alias file (if the file is an alias) */
#define O_RSRC 			0x4000		/* Open the resource fork */
#define O_BINARY		0x8000		/* open the file in binary mode (default is text mode) */

/*
 *	Commands available to fcntl()
 */
#define F_DUPFD			0x0			/* return a duplicate file descriptor */

#ifdef __cplusplus
extern "C" {
#endif

int open(const char *path, int oflag, ...);				/* mm 980507 */  /* vss 980729 */
int creat(const char *path, mode_t mode);				/* mm 980507 */
int fcntl(int fildes, int cmd, ...);					/* mm 980507 */
/* The following prototypes are left to handle legacy code mm 980507 */
int _open(const char *path, int oflag);
int _creat(const char *path, mode_t mode);
int _fcntl(int fildes, int cmd, ...);

/*#define	open	_open
#define	creat	_creat
#define	fcntl	_fcntl*/

#ifdef __cplusplus
}
#endif

#pragma options align=reset
/* #pragma direct_destruction reset */  /* 980807 vss */

#endif

/*
	Change Record
 * mm 971129 added include of stat.win32.h to get definition of mode_t MW01300
 * hh 971206  Added namespace support
 * mm 980420  Change to make O_RDWR have non-zero value MW00042
 * mm 980507 Changes to revert from _open, _creat, and _fcntl to open, creat, fcntl while 
             accommodating legacy code.  MW01597
 * vss 980729  Match open to POSIX Standard
 * vss 980807 remove pragma  - no longer supported by compiler
*/
