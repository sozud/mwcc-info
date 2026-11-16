/*/  Metrowerks x86 Runtime Support Library  19-Sep-99  /*/

/*
 *		setupargs.c
 *	
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */
 
// Set up the argument count and argument vector that are passed to main

#include <ansi_parms.h>
#include <windef.h>
#include <winbase.h>
#include <winnt.h>
#include <winuser.h>
#include "crtl.h"
#include <stdlib.h>

// The argument pointer and count variables
char **__argv;
int __argc;

// Copy of command line, modified to fill argv[]
static char *__command_line = 0;

static void _FreeCommandLine(void)
{
	if (__command_line)		free(__command_line);
	
	// __argc == 0 => we ran out of memory and 
	// __argv is just a ptr to static memory
	if (__argv && __argc)	free(__argv);		
}

static void _MemFailure(void)
{
	static char *fakeargs[] = { NULL };
	__argv = fakeargs;
	__argc = 0;
}

void _SetupArgs()
{
#if __dest_os== __win32_os

	#define WHITESPACE " \t\r\n\v"
	
	char	*inptr;		// pointer into ASCII version of cmdline string 
	char 	*outptr;	// current outptr into copy of cmdline string and argv[x]
	int 	maxargc;	// # slots (k*16) allocated for argv[] array (including final NULL entry)

	/*	This code forms the argv[] array from a copy of the GetCommandLine()
		string returned by Windows, and sets up argv[] with pointers to the
		data therein.  The string is modified by terminating each entry of
		argv[] with a NUL and shortening strings if quotes or escaped quotes 
		are encountered. */

	maxargc = 1;
	__argc = 0;
	__argv = (char **) malloc(sizeof(char *) * maxargc);
	
	//  utter failure?!
	if (__argv == NULL) {
		_MemFailure();
		return;
	}
	
	//	explicitly get ASCII version of command line string to
	//	satisfy translation into required C strings under Unicode builds.
	inptr = GetCommandLineA();

	// 	make duplicate of command line to generate argv[]
	outptr = __command_line = (char *)malloc(strlen(inptr)+1);
	if (!outptr) {
		_MemFailure();
		return;
	}
	
	//	skip unthinkable leading spaces
	while (strchr(WHITESPACE, *inptr) != NULL)
		inptr++;
	
	while (*inptr)
	{
		int		quoting;	// inside quoted part?

		//  allocate more space for argv[]
		if (__argc + 1 >= maxargc)
		{
			char **newargs;
			
			maxargc += 16;
			newargs = (char **) realloc(__argv, maxargc * sizeof(char *));
			if (newargs == NULL)
			{
				// 	failed!  keep old args;
				//	fall through and zero last entry.
				break;
			}
			__argv = newargs;
		}
	
		__argv[__argc] = outptr;
		__argc++;
	
		//  Parse one argument from inptr and copy into outptr.
		//  Rules:  a series of chars inside double quotes are not broken;
		//	and such double quotes are removed.
		//	Escaped quotes (\") are kept as one quote char;
		//	other escapes are completely ignored.
	
		// 	EJS 990917:  remove newlines and such (encountered in Cygwin32
		//	tools from the bash shell with the `...` construct)
		
		quoting = 0;
		while (*inptr)
		{
			// argument breaker?
			if (!quoting && strchr(WHITESPACE, *inptr) != NULL)
			{
				do	inptr++;	
				while (*inptr && strchr(WHITESPACE, *inptr) != NULL);

				*outptr++ = 0;	// break current argument
				break;			// on to next argument
			}
			else
			// quote?
			if (*inptr == '\"')
			{
				// eat quote
				inptr++;

				// change state				
				quoting = !quoting;
			}
			else
			// escaped quote?
			if (*inptr == '\\' && *(inptr+1) == '\"')
			{
				*outptr++ = '\"';
				inptr += 2;
			}
			else
			// argument char	
			{
				*outptr++ = *inptr++;
			}
		}

	}
	
	//	end last argument string
	*outptr = 0;
	
	//	terminate argv[] list
	__argv[__argc] = NULL;
	
	//	free memory at exit
	atexit(_FreeCommandLine);
	
#else	// win32

	__argc = 0;
	__argv = (char **)malloc(sizeof(char *) * (__argc + 1));
	
	//  utter failure?
	if (__argv == NULL)
	{
		static char *fakeargs[] = { NULL };
		__argv = fakeargs;
	}
	else
		__argv[__argc] = NULL;
	
#endif	// win32
}

/* Change record
 * mm  980730  Made sure that argv[argc] = NULL, C Std 5.1.2.2.1  MW08173
 * blc 980908  Fixed problem with no memory getting allocated for __argv on WinCE
 * ejs 990304  Rewrote to allow escaped quotes and to be more efficient
 * blc 990524  Fix for code stomping on global return value of GetCommandLine()
 * ejs 990917  Added more characters to whitespace stripping and clarified algorithm
 *             Also, support Unicode build.
 */
