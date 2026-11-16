/*  Metrowerks Standard Library  */

/*  $Date: 1999/01/22 23:43:44 $ 
 *  $Revision: 1.2 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	File:		unix.c
 *					
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 *
 */

#include <unix.h>
#include <signal.h>
#include "ansi_files.h"
/*
#include <errno.h>
#include <string.h>
#include "file_io.h"
*/  /* do not need these header files yet --m.f. */
/* function prototypes (exported) */
long __myraise(long signal);

/*
 *	long __myraise(long signal);
 *
 *	raise a signal (note that it is not int size dependant)
 */
long __myraise(long signal)
{
	return (raise(signal));
}	


/*
 *	int fileno(FILE *stream)
 *
 *		Returns the file number associated with a stream.
 */
int _fileno(FILE *stream)
{
	return (stream->handle);
}

/*
 *	FILE *fdopen(int handle, char *mode)
 *
 *		Converts a file handle into an ANSI C file stream.
 */
FILE *_fdopen(int handle, char *mode)
{
	// The Mac version of this routine makes sure that the requested
	// mode can be granted given the permissions of the handle. Currently
	// this routine doesn't do that check.
	
	FILE * theFileP = __find_unopened_file();
	
	if (!theFileP)
		return(0);
	
	return(__handle_reopen(handle, mode, theFileP));
}

/*
 *	long tell(int fildes)
 *	
 *		Return the current on disk position.
 */
long _tell(int fildes)
{
	return (lseek(fildes, NULL, SEEK_CUR));
}

/*	Change History
	
961031 KO	Wrote fdopen. Previously it was an empty definition.
980622 mf   got rid of un-needed header files
*/
