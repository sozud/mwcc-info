/*  Metrowerks Standard Library  */

/*  $Date: 1999/01/22 23:44:12 $ 
 *  $Revision: 1.2 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	extras.h
 */

#ifndef EXTRAS_H
#define EXTRAS_H

#include <ansi_parms.h>          /* hh 971206 */

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus          /* hh 971206 */
	extern "C" {
#endif

int 	_heapmin(void);

int 	GetHandle();
int		_fileno(FILE *stream);
int 	_isatty(int fileno);

int _chdir(const char *dirname);
char * _getcwd(char *path, int maxlen);
void _searchenv(const char *filename, const char *varname, char *pathname);

int 	_chdrive(int drive);
void 	_makepath(char *path, const char *drive, const char *dir, const char *fname, const char *ext);

long 	_get_osfhandle(int filehandle);
int 	_open_osfhandle(long ofshandle, int flags);
int _getdrive();  /* hh 980205 added */

/*

These function declarations have all been relocated to the header where they appear in
Visual C++

char * _fullpath(char * absPath, const char * relPath, size_t maxLength);

__declspec(naked) char * _alloca(size_t size);
FILE *fdopen(int handle, const char *mode);

char * _itoa(int val, char *str, int radix);
#define _ltoa(x, y, z) _itoa(x, y, z)
char * _ultoa(unsigned long val, char *str, int radix);

int 	_stricmp(const char *s1, const char *s2);
int 	_strnicmp(const char *s1, const char *s2, int n);
char 	* _strrev(char *str);
char 	*_strdup(const char *str);
char 	*_strupr(char *str);
char 	*_strdate(char *str);

char *_gcvt(double value, int digits, char *buffer);

*/
#ifdef __cplusplus          /* hh 971206 */
	}
#endif

#endif

/*
	Change Record
 * hh 971206  Added namespace support
 */
