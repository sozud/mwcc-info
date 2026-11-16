/*  Metrowerks Standard Library  */

/*  $Date: 1999/01/22 23:44:12 $ 
 *  $Revision: 1.2 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*
 *	File:		utime.h
 *
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 */

#ifndef _UTIME
#define _UTIME

#include <ansi_parms.h>          /* hh 971206 */

#ifndef _TIME
#include <time.h>
#endif

#include <WINDEF.H>
#include <WINBASE.H> 
#define _INC_WINDOWS /* this prevents windows.h from being included */


/*
 * 960815: As this file was I got a timeval redeclared error when this file
 * was included before winsock.h. Including winsock.h first eliminates this
 * problem.
 */

/*
hh 980122 As this file was ( ;-) ) I got a lots of redeclared errors when
          <winsock.h> was included.*/
#include <winsock.h>           /* mm 980514 */

#pragma options align=native
/* #pragma direct_destruction off */  /*  980807 vss */

/* struct for utime */
struct utimbuf {
	time_t actime;					/* access time  */
	time_t modtime;					/* modification time */
};

/* struct for utimes */
#ifndef	_WINSOCKAPI_
struct timeval {
	long tv_sec;						/* seconds */
	long tv_usec;					/* microseconds  */
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	Set the file time stamps.
 */
int utime(const char *path, struct utimbuf *buf);

/*
 *	Set the file time stamps.
 */
int utimes(const char *path, struct timeval buf[2]);

#ifdef __cplusplus
}
#endif

#pragma options align=reset
/* #pragma direct_destruction reset */  /*  980807 vss */

#endif

/*
	Change Record
 * hh 971206  Added #include <ansi_parms.h>
 * hh 980122 <winsock.h> commented out.
 * mm 980514 Reinserted #include <winsock.h> as it is needed despite result warning messages
 *           hh concurs with this action.
 * mf 06/25/98  prevent header from including windows.h-- avoids
 *              annoying warnings due to inclusion of winsock.h
 * vss 980807 remove pragma  - no longer supported by compiler
*/
