/*  Metrowerks Standard Library  */

/*  $Date: 1999/03/24 05:16:30 $ 
 *  $Revision: 1.4 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *		ThreadLocalData.h
 */

#ifndef __THREADLOCALDATA_H__
#define __THREADLOCALDATA_H__

#include <ansi_parms.h>          /* hh 971206 */

#include <cstdio>	/* For L_tmpnam declaration */
#include <ctime>

#pragma options align=native

#ifdef __cplusplus          /* hh 971206 */
	extern "C" {
#endif

typedef struct {
	int errno;
	
	unsigned long int random_next;
	unsigned char *	strtok_n;
	unsigned char *	strtok_s;
	/* really HANDLE */ void * thread_handle;
	struct __std(tm) gmtime_tm; /* hh 990124 fixed __std */
	char asctime_result[26];
	char tmpnam_temp_name[L_tmpnam];
} _ThreadLocalData;

int _InitializeThreadDataIndex();
int _InitializeThreadData();
void _DisposeThreadData();
int _InitializeMainThreadData();
_ThreadLocalData * _GetThreadLocalData();

#ifdef __cplusplus          /* hh 971206 */
	}
#endif

#pragma options align=reset

#endif

/*
	Change Record
 * hh 971206  Added namespace support
 * hh 990124 fixed __std
 * blc 990317 Added thread_handle to allow for beginthread/endthread support
 */
