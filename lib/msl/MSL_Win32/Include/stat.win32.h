/*  Metrowerks Standard Library  */

/*  $Date: 1999/04/16 23:23:27 $ 
 *  $Revision: 1.4 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*
 *	File:		stat.win32.h
 *
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 */

#ifndef	_STAT
#define	_STAT

#include <ansi_parms.h>          /* hh 971206 */

#ifndef _TIME
#include <time.h>
#endif

/*
 *	Local typedefs for stat struct
 */
typedef unsigned short	mode_t;


/*
 * 960827: Microsoft has ino_t defined as an unsigned short and
 * dev_t defined as an unsigned int.
 */
 /* 980113 mf-- removed #if __dest_os==win32 */                
#if __dest_os==__win32_os 
#include <types.h>  /* includes microsofts definitions of _ino_t,_dev_t */
#define ino_t _ino_t
#define dev_t _dev_t
#else
typedef unsigned short ino_t;
typedef unsigned int   dev_t;
#endif

typedef short			nlink_t;  /* 980113 mf--changed types from long to short */
typedef unsigned short	uid_t;   
typedef unsigned short	gid_t;
typedef long			off_t;

#pragma options align=native
/* #pragma direct_destruction off */  /* 980807 vss */

/*
 *	(stat) st_mode bit values
 */
 
#define S_IFMT         0xF000         /* file type mask */
#define S_IFDIR        0x4000         /* directory */
#define S_IFCHR        0x2000         /* character special */
#define S_IFIFO        0x1000         /* pipe */
#define S_IFBLK		   0x6000		  /* blocking stream   */
#define S_IFREG        0x8000         /* regular */
#define S_IREAD        S_IRUSR         /* read permission, owner */
#define S_IWRITE       S_IWUSR         /* write permission, owner */
#define S_IEXEC        S_IXUSR         /* execute/search permission, owner */


/*
 *	File type macros
 */
#define S_ISFIFO(m)	(((m)&(S_IFMT)) == (S_IFIFO))
#define S_ISDIR(m)	(((m)&(S_IFMT)) == (S_IFDIR))
#define S_ISCHR(m)	(((m)&(S_IFMT)) == (S_IFCHR))
/*#define S_ISBLK(m)	(((m)&(S_IFMT)) == (S_IFBLK))*/
#define S_ISREG(m)	(((m)&(S_IFMT)) == (S_IFREG))


/*
 * The following file mode macros are defined to allow compilation of programs that
 * reference them; however, they are not relevant for Windows
 */
#define	S_IRWXU	0x0E00		/* Permissions for file owner class */	/* mm 990107 */
#define 	S_IRUSR 0x0800	/* Read permission file owner class */	/* mm 990107 */
#define 	S_IWUSR 0x0400	/* writ permission file owner class */	/* mm 990107 */
#define 	S_IXUSR 0x0200	/* exec permission file owner class */	/* mm 990107 */
#define S_IRWXG 0x01C0		/* permissions for file group class */	/* mm 990107 */
#define 	S_IRGRP 0x0100	/* Read permission file group class  */	/* mm 990107 */
#define 	S_IWGRP 0x0800	/* writ permission file group class  */	/* mm 990107 */
#define 	S_IXGRP 0x0400	/* exec permission file group class  */	/* mm 990107 */
#define S_IRWXO 0x0038		/* permissions for file other class */	/* mm 990107 */
#define 	S_IROTH 0x0200	/* Read permission file other class */	/* mm 990107 */
#define 	S_IWOTH 0x0100	/* writ permission file other class */	/* mm 990107 */
#define 	S_IXOTH 0x0800	/* exec permission file other class */	/* mm 990107 */
#define S_ISUID 0x0004		/* set user ID on execution         */	/* mm 990107 */
#define S_ISGID 0x0002		/* set group ID on execution        */	/* mm 990107 */

/*
 * The following file mode macros are defined to allow compilation of programs that
 * reference them; however, they are not relevant for the Mac
 */
#define	S_IRWXU	0x0E00		/* Permissions for file owner class */	/* mm 990107 */
#define 	S_IRUSR 0x0800	/* Read permission file owner class */	/* mm 990107 */
#define 	S_IWUSR 0x0400	/* writ permission file owner class */	/* mm 990107 */
#define 	S_IXUSR 0x0200	/* exec permission file owner class */	/* mm 990107 */
#define S_IRWXG 0x01C0		/* permissions for file group class */	/* mm 990107 */
#define 	S_IRGRP 0x0100	/* Read permission file group class  */	/* mm 990107 */
#define 	S_IWGRP 0x0800	/* writ permission file group class  */	/* mm 990107 */
#define 	S_IXGRP 0x0400	/* exec permission file group class  */	/* mm 990107 */
#define S_IRWXO 0x0038		/* permissions for file other class */	/* mm 990107 */
#define 	S_IROTH 0x0200	/* Read permission file other class */	/* mm 990107 */
#define 	S_IWOTH 0x0100	/* writ permission file other class */	/* mm 990107 */
#define 	S_IXOTH 0x0800	/* exec permission file other class */	/* mm 990107 */
#define S_ISUID 0x0004		/* set user ID on execution         */	/* mm 990107 */
#define S_ISGID 0x0002		/* set group ID on execution        */	/* mm 990107 */

struct _stat
{
	mode_t		st_mode;		/* File mode; see #define's above 			*/
	ino_t		st_ino;			/* File serial number 						*/
	dev_t		st_dev;			/* ID of device containing this file		*/
	nlink_t		st_nlink;		/* Number of links 							*/
	uid_t		st_uid;			/* User ID of the file's owner 				*/
	gid_t		st_gid;			/* Group ID of the file's group 			*/
  /*dev_t		st_rdev; */		/* Device type , not in posix				*/
	off_t		st_size;		/* File size in bytes 						*/
	time_t		st_atime;		/* Time of last access 						*/
	time_t		st_mtime;		/* Time of last data modification 			*/
	time_t		st_ctime;		/* Time of last file status change 			*/
  /*long		st_blksize;*/	/*Optimal blocksize , not in POSIX 			*/
  /*long		st_blocks; */	/*blocks allocated for file, not in POSIX	*/
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	Get state information on a file.
 */
int _stat(const char *path, struct _stat *buf);

/*
 *	Get state information on an open file.
 */
int _fstat(int fildes, struct _stat *buf);
int _mkdir(const char *path);

#define stat	_stat
#define	fstat	_fstat
#define	mkdir	_mkdir

#ifdef __cplusplus
}
#endif

#pragma options align=reset
/* #pragma direct_destruction reset */  /* 980807 vss */

#endif
/*  Change History:

    011398---changed stat structure types to match up
             with Microsoft--namely mode_t,uid_t,guid_t
             are short's instead of long's
             Also made macro names match those of Microsoft.
   050698--mf integrated changes for wince.
   062098--mf bad #ifdef
   980807--vss remove pragma  - no longer supported by compiler
 * mm 990107 Completed definition of file mode macros in accord with POSIX standard. MW07297
*/             
             
