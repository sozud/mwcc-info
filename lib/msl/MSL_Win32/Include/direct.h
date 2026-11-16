/*  Metrowerks Standard Library  */

/*  $Date: 1999/01/22 23:44:12 $ 
 *  $Revision: 1.2 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*
 *	direct.h
 *
 *		This file was built to correspond with the Visual C++ direct.h
 */

#ifndef __DIRECT__
#define __DIRECT__

#include <ansi_parms.h>          /* hh 971206 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif

#ifndef _DISKFREE_T_DEFINED
struct _diskfree_t {
	unsigned long total_clusters;
	unsigned long avail_clusters;
	unsigned long sectors_per_cluster;
	unsigned long bytes_per_sector;
	};

#define _DISKFREE_T_DEFINED
#endif


int				_chdir(const char *);
char *			_getcwd(char *, int);
int				_mkdir(const char *);
int				_rmdir(const char *);
int				_chdrive(int);
char *			_getdcwd(int, char *, int);
int				_getdrive(void);
unsigned long	_getdrives(void);
unsigned		_getdiskfree(unsigned, struct _diskfree_t *);

#define chdir	_chdir
#define getcwd	_getcwd
#define mkdir	_mkdir
#define rmdir	_rmdir

#define diskfree_t  _diskfree_t

#ifdef __cplusplus
}
#endif

#endif /* __DIRECT__ */

/*
	Change Record
 * hh 971206  Added #include <ansi_parms.h>
 */
