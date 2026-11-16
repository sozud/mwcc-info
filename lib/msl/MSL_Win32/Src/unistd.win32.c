/*  Metrowerks Standard Library  */

/*  $Date: 1999/08/06 01:05:29 $ 
 *  $Revision: 1.4 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *	File:		unistd.winsioux.c
 *
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 *
 */


/* #include <x86_prefix.h>   mf  980518  */
#include <stdlib.h>
#include <errno.h>
/*
hh 980122 Replaced <windows.h> with the following TWO includes because it is seriously
          broken.  The following 2 includes must be carefully ordered as shown, because
          they are broken too.
#include <windows.h>
*/
#include <WINDEF.H>
#include <WINBASE.H>

#include "crtl.h"

#include <unistd.h>

/*
Already defined in extra.c
int _doserrno;
hh 971130
*/

#ifdef _WINSIOUX                    /* mm 980119 */
#include <WinSIOUX.h>				/* mm 990423 */
#endif /* _WINSIOUX  */             /* mm 980119 */
 
FileStruct *_HandleTable[NUM_HANDLES];
int _HandPtr;

#if (__dest_os==__win32_os)  /*  mf  980518 */

/*
 *	int chdir(const char *path)
 *
 *		Changes the current working directory 
 */
int chdir(const char *dirname)								/* mm 980518 */
{
    char path[_MAX_PATH];
    char drive[4];
    
    if (SetCurrentDirectory(dirname))
    {
        GetCurrentDirectory(_MAX_PATH, path);
        if (path[1] == ':')
        {
            drive[0] = '=';
            drive[1] = path[0];
            drive[2] = ':';
            drive[3] = 0;
            SetEnvironmentVariable(drive, path);
        }
        return 0;
    }
    else
        return -1;
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
int _chdir(const char *dirname)								/* mm 980518 */
{															/* mm 980518 */
	return (chdir(dirname));								/* mm 980518 */
}

/*
 *	char *getcwd(char *path, int size)
 *
 *		Returns the path to the current directory.
 */
char *getcwd(char *path, int maxlen)						/* mm 980518 */
{
    int len;
    
    len = GetCurrentDirectory(0, NULL);
    if (path == NULL)
    {
        if (maxlen > len)
        {
            len = maxlen;
        }
        path = (char*)malloc(len);            /* hh 971206 added (char*) cast */
        if (!path)
        {
            errno = ENOMEM;
            return NULL;
        }
    }
    else if (len > maxlen)
    {
        errno = ERANGE;
        return NULL;
    }
    GetCurrentDirectory(len, path);
    return path;
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
char* _getcwd(char *path, int maxlen)						/* mm 980518 */
{															/* mm 980518 */
	return (getcwd(path, maxlen));							/* mm 980518 */
}															/* mm 980518 */


/*
 *	int isatty(int fildes)
 *
 *		Determines is a filestream is going to the console window.
 */
int isatty(int fildes)									/* mm 980518 */
{
	extern long _get_osfhandle(int filehandle);

    return GetFileType((void *)_get_osfhandle(fildes)) == FILE_TYPE_CHAR;
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
int _isatty(int fildes)										/* mm 980518 */
{															/* mm 980518 */
	return (isatty(fildes));								/* mm 980518 */
}															/* mm 980518 */

#endif  /*  mf  980518 */															
															/* mm 980518 */
/*
 *	int close(int fildes)
 *	
 *		Closes a file stream.
 */
int close(int fildes)										/* mm 980518 */
{
	HANDLE h;

    if (fildes >= NUM_HANDLES || !_HandleTable[fildes])
    {
        return -1;
    }
    
	h = _HandleTable[fildes]->handle;
	
    if (CloseHandle(h))
    {
        free(_HandleTable[fildes]);
        _HandleTable[fildes] = 0;
        
        return 0;
    }
    else
    {
		// get error code	
		_doserrno = GetLastError();
				
        return -1;
    }
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
int _close(int fildes)										/* mm 980518 */
{															/* mm 980518 */
	return (close(fildes));									/* mm 980518 */
}															/* mm 980518 */

/*
 *	char *cuserid(char *string)
 *
 *		Returns the user's name associated with the current process (owner name from
 *		Sharing Setup CP).
 */
char *cuserid(char *string)
{
	char			*name = getlogin();

	if (name != NULL && string != NULL)
		strcpy(string, name);
	return (name == NULL ? NULL : string == NULL ? name : string);
}

/*
 *	int exec(const char *path, ...)
 *
 *		Launchs an application and then quits the current app.
 */
int exec(const char */*path*/, ...)  /* hh 980122 commented out unused arg */
{
	return(-1);						/* mm 981230 */
}


/*
 *	char *getlogin(void)
 *
 *		Returns the current user's login name (owner name from Sharing Setup CP).
 */
char *getlogin(void)
{
	return(getenv("USERNAME"));               /* mm 981230 */
}

/*
 *	long lseek(int fildes, long offset,int whence)
 *	
 *		Seek in a file (fildes is the MacOS refnum).
 */
long lseek(int fildes, long offset,int whence)				/* mm 980518 */
{
	HANDLE h;
	DWORD method;

    if (fildes >= NUM_HANDLES || !_HandleTable[fildes])
    {
		errno = EBADF;
        return -1;
    }
    
	h = _HandleTable[fildes]->handle;
	
	switch(whence)
	{
	case SEEK_SET:
	    method = FILE_BEGIN;
		break;

	case SEEK_CUR:
	    method = FILE_CURRENT;
		break;

	case SEEK_END:
		method = FILE_END;
		break;
	}

    return SetFilePointer(h, offset, NULL, method);
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
long _lseek(int fildes, long offset,int whence)				/* mm 980518 */
{															/* mm 980518 */
	return (lseek(fildes, offset, whence));					/* mm 980518 */
}															/* mm 980518 */

/*
 *	int read(int fildes, char *buf, int count)
 *	
 *		Read from a file (fildes is the file's MacOS refnum).
 */
int read(int fildes, char *buf, int count)					/* mm 980518 */
{
    DWORD read;
	HANDLE h;
	int t;
	int ReadResult;

    if (fildes >= NUM_HANDLES || !_HandleTable[fildes])
    {
		errno = EBADF;
        return -1;
    }
	h = _HandleTable[fildes]->handle;
	t = _HandleTable[fildes]->translate;

#ifdef _WINSIOUX                    /* mm 980119 */
	if (fildes == 0)
		ReadResult = ReadCharsFromConsole((void *)buf, (DWORD)count, &read);
	else
#endif  /* if _WINSIOUX */			/* mm 980119 */
		ReadResult = ReadFile(h, (void *)buf, (DWORD)count, &read, 0);
    if (ReadResult)
	{
	    if (read && t)
	    {
	        char *cp, *op;
	        int i, n;
	       
	        cp = op = buf;
	        n = 0;
	        for(i=0; i<read; i++)
	        {
	            // trun CRLF into just LF
	            
	            if (cp[0] == '\r' && cp[1] == '\n')
	            {
	                cp++;
	                n++;
	            }
	            else
	            {
	                *op++ = *cp++;
	            }
	        }
	        
	        // If buffer ended in a CR and the read did not come up short due to hitting the 
	        // end of file, pretend we did not see that CR and back up the file pointer.
	        
	        if (read == count && read > 1 && buf[read-1] == '\r')
	        {
	            lseek(fildes, -1, SEEK_CUR); 
	            n++;
	        }
	        read -= n;
	    }
	    return read;
	}
	else
	{
		// get error code	
		_doserrno = GetLastError();
		
	    return 0;
	}
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
int _read(int fildes, char *buf, int count)					/* mm 980518 */
{															/* mm 980518 */
	return (read(fildes, buf, count));						/* mm 980518 */
}															/* mm 980518 */

/*
 *	int rmdir(const char *path)
 *
 *		Removes a directory (must be empty).
 */
int rmdir(const char *path)									/* mm 980518 */
{
	return RemoveDirectory((LPTSTR)path) ? 0 : 1;
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
int _rmdir(const char *path)								/* mm 980518 */
{															/* mm 980518 */
	return (rmdir(path));									/* mm 980518 */
}															/* mm 980518 */

/*
 *	unsigned int sleep(unsigned int seconds)
 *
 *		Pauses program execution for seconds seconds.
 */
unsigned int sleep(unsigned int seconds)
{
	Sleep(seconds * 1000);
	return 0;
}

/*
 *	char *ttyname(int fildes)
 *
 *		Returns the name of the associated terminal or NULL if none.
 */
char *ttyname(int /*fildes*/)  /* hh 980122 commented out unused arg */
{
/*
	if (fildes >=0 && fildes <= 2)
		return __ttyname((long)fildes);
	else
*/
		return NULL;
}

/*
 *	int unlink(const char *path)
 *	
 *		Unlink (i.e. delete) a file.
 */
int unlink(const char *path)								/* mm 980518 */
{
    if (DeleteFile((LPCTSTR)path))
    {
        return 0;
    }
    else
    {
		// get error code	
		_doserrno = GetLastError();
		
        return -1;
    }
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
int _unlink(const char *path)								/* mm 980518 */
{															/* mm 980518 */
	return (unlink(path));									/* mm 980518 */
}															/* mm 980518 */

/*
 *	int write(int fildes, const char *buf, int count)
 *	
 *		Write to a file (fildes is the file's MacOS refnum).
 */
int write(int fildes, const char *buf, int count)			/* mm 980518 */
{
    DWORD wrote;
	HANDLE h;
	int t;
	char *bp = 0;
	int status;
	
    if (fildes >= NUM_HANDLES || !_HandleTable[fildes])
    {
		errno = EBADF;
        return -1;
    }

	h = _HandleTable[fildes]->handle;
	t = _HandleTable[fildes]->translate;
	
    if (t)
    {
         int i, n;
         char *cp;
         
         n = 0;
         for (i=0; i<count; i++)
         {
             if (buf[i] == '\n')
             {
                 n++;
             }
         }
         bp = (char *)malloc(count + n);
         cp = bp;
         for (i=0; i<count; i++)
         {
             if (buf[i] == '\n')
             {
                 *cp++ = '\r';
             }
             *cp++ = buf[i];
         }
         count += n;
         buf = bp;
    }
    
    // Do the write operation
    
    if (_HandleTable[fildes]->append)
    {
        lseek(fildes, 0, SEEK_END);
    }
#ifdef _WINSIOUX                    /* mm 980119 */

    if ((fildes == 1) || (fildes == 2))
    {
    	WriteCharsToConsole(buf, count);
    	status = 1;
    	wrote  = count;
    }
    else
#endif /* _WINSIOUX  */             /* mm 980119 */

    status = WriteFile(h, (void *)buf, (DWORD)count, &wrote, 0);
    
    // Free the local buffer if we allocated one
    
    if (bp)
    {
        free(bp);
    }
    
    // And return status
    
    if (status)
	{
	    return wrote;
	}
	else
	{
		// get error code	
		_doserrno = GetLastError();
		
	    return 0;
	}
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
int _write(int fildes, const char *buf, int count)			/* mm 980518 */
{															/* mm 980518 */
	return (write(fildes, buf, count));						/* mm 980518 */
}															/* mm 980518 */

// check accessibility of a file

int access(const char *path, int mode)						/* mm 980518 */
{
    UINT access;
    HANDLE handle;
    
    access = 0;
    if (mode & 2)
    {
        access |= GENERIC_READ;
    }
    if (mode & 4)
    {
        access |= GENERIC_WRITE;
    }
    
    handle = CreateFile((LPTSTR)path, access, 0, NULL, OPEN_EXISTING, 0, 0);
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        return 0;
    }
    else
    {
        if (GetLastError() == ERROR_INVALID_ACCESS)
        {
            errno = EACCES;
        }
        else
        {
            errno = ENOENT;
        }
        return -1;
    }
}
/* The following stub is a hack for the benefit of legacy code mm 980518 */
int _access(const char *path, int mode)						/* mm 980518 */
{															/* mm 980518 */
	return (access(path, mode));							/* mm 980518 */
}															/* mm 980518 */


/*
	Change Record
 * hh 971206  Commented out _doserrno because it is already defined in extra.c
 * hh 971206  Added (char*) cast
 * hh 980122  Replaced <windows.h> with the following TWO includes because it is seriously
              broken.  The following 2 includes must be carefully ordered as shown, because
              they are broken too.
 * hh 980122  commented out unused arg 2 places
 * mf 980303  merged in support for Michael's winsioux
 * mm 980518  Changes to revert _lseek, _read, _write, _access, _close, _unlink, 
 *				_rmdir, _chdir, _getcwd to lseek, read, write, access, close etc.
 *				while accommodating legacy code.  Complement of changes in unistd.win32.h 
 * mf 980518  wince changes
 * mm 981230  Gave some body to getlogin() and exec() to prevent crash MW08465 
 * mm 990423  Made proper use of WinSIOUX.h instead of having builtin WinSIOUX prototypes   
*/
