/*  Metrowerks Standard Library  */

/*  $Date: 1999/01/22 23:44:12 $ 
 *  $Revision: 1.2 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*
 *	io.h
 *
 *		This file was built to correspond with the Visual C++ io.h
 */

#ifndef __IO__
#define __IO__

#include <ansi_parms.h>          /* hh 971206 */
#include <extras.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <unix.h>
#ifdef __cplusplus
   extern "C" {
#endif
   
#ifndef _FSIZE_T_DEFINED
typedef unsigned long _fsize_t; 
#define _FSIZE_T_DEFINED
#endif
#ifndef _FINDDATA_T_DEFINED

struct _finddata_t {
    unsigned    attrib;
    time_t      time_create;    /* -1 for FAT file systems */
    time_t      time_access;    /* -1 for FAT file systems */
    time_t      time_write;
    _fsize_t    size;
    char        name[260];
};

#endif

#define _A_NORMAL       0x00    /* Normal file - No read/write restrictions */
#define _A_RDONLY       0x01    /* Read only file */
#define _A_HIDDEN       0x02    /* Hidden file */
#define _A_SYSTEM       0x04    /* System file */
#define _A_SUBDIR       0x10    /* Subdirectory */
#define _A_ARCH         0x20    /* Archive file */
 long __cdecl _findfirst(const char *, struct _finddata_t *);
 int __cdecl _findnext(long, struct _finddata_t *);
 int __cdecl _findclose(long);
 #ifdef __cplusplus
  }
#endif
#define _O_TEXT O_TEXT
#define _O_RDWR O_RDWR
#define _O_RDONLY O_RDONLY
#define _O_WRONLY O_WRONLY
#define _O_APPEND O_APPEND
#define _O_CREAT O_CREAT
#define _O_EXCL O_EXCL
#define _O_TRUNC O_TRUNC
#define _O_NRESOLVE O_NRESOLVE
#define _O_ALIAS O_ALIAS
#define _O_RSRC O_RSRC
#define _O_BINARY O_BINARY
#endif /* __IO__ */

/*
	Change Record
 * hh 971206  Added namespace support
 * mf 980612  added prototypes/macros for _findxxx(single character version) 
 */
