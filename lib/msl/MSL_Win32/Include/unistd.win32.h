/*  Metrowerks Standard Library  */

/*  $Date: 1999/03/17 20:07:58 $ 
 *  $Revision: 1.3 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	File:		unistd.win32.h
 *
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 */

#ifndef	_UNISTD
#define	_UNISTD

#include <ansi_parms.h>          /* hh 971206 */

#pragma options align=native
/* #pragma direct_destruction off */  /* vss 980807 */

#ifndef __stdio__
	/* macros for whence parameter of lseek() (taken from <stdio.h> */
	#define SEEK_SET	0
	#define SEEK_CUR	1
	#define SEEK_END	2
#endif

#ifdef __cplusplus
extern "C" {
#endif


int  isatty(int fildes);									/* mm 980518 */
long lseek(int fildes, long offset, int whence);			/* mm 980518 */

int  read(int fildes, char *buf, int count);				/* mm 980518 */
int  write(int fildes, const char *buf, int count);			/* mm 980518 */
int  access(const char *path, int mode);					/* mm 980518 */
int  close(int fildes);										/* mm 980518 */
int  unlink(const char *path);								/* mm 980518 */

int  rmdir(const char *path);								/* mm 980518 */
int  chdir(const char *path);								/* mm 980518 */
char *getcwd(char *buf, int size);                          /* mm 980518 */
/* the following prototypes are left to handle legacy code     mm 980518 */
int  _isatty(int fildes);
long _lseek(int fildes, long offset, int whence);

int  _read(int fildes, char *buf, int count);
int  _write(int fildes, const char *buf, int count);
int  _access(const char *path, int mode);
int  _close(int fildes);
int  _unlink(const char *path);

int  _rmdir(const char *path);
int  _chdir(const char *path);
char *_getcwd(char *buf, int size);

char *ttyname(int fildes);
char *getlogin(void);
char *cuserid(char *string);
int exec(const char *path, ...);
unsigned int sleep(unsigned int sleep);

int _getpid(void);

#ifdef __cplusplus								/* mm 990106 */
}												/* mm 990106 */
inline int getpid()		{return _getpid();}		/* bc 990317 */
inline int getppid()	{return((int) 8000);}	/* mm 990106 */
inline int getuid()		{return((int) 200);}	/* mm 990106 */
inline int geteuid()	{return((int) 200);}	/* mm 990106 */
inline int getgid()		{return((int) 100);}	/* mm 990106 */
inline int getegid()	{return((int) 100);}	/* mm 990106 */
inline int getpgrp()	{return((int) 9000);}	/* mm 990106 */
extern "C" {									/* mm 990106 */
#else											/* mm 990106 */
#define getpid			_getpid					/* bc 990317 */
#define getppid()		((int) 8000)
#define getuid()		((int) 200)
#define geteuid()		((int) 200)
#define getgid()		((int) 100)
#define getegid()		((int) 100)
#define getpgrp()		((int) 9000)
#endif											/* mm 990106 */
/*
#define	lseek			_lseek
#define read			_read
#define	write			_write
#define	access  		_access
#define close			_close
#define	unlink			_unlink

#define rmdir			_rmdir
#define chdir			_chdir
#define	getcwd			_getcwd
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
 * mm 980518  Changes to revert _lseek, _read, _write, _access, _close, _unlink, 
 *				_rmdir, _chdir, _getcwd to lseek, read, write, access, close etc.
*				while accommodating legacy code.  Complement of changes in unistd.win32.c   
 * vss 980807 remove pragma  - no longer supported by compiler
 * mm 990106  Fixes to avoid problem reported in MW07575     
 * blc 990317 Provided implementation for _getpid()
 */
