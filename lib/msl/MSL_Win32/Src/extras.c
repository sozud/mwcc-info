/*  Metrowerks Standard Library  */

/*  $Date: 1999/03/31 16:43:29 $ 
 *  $Revision: 1.6 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

#include <crtl.h>
#include <ctype.h>
#include <cstddef>
#include <climits>          /* mm 990329 */
#include <errno.h>
#include <extras.h>
#include <direct.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <unistd.h>
#include <cstdarg>  /* hh 980122 needed for va_start */

// change drive
#if (__dest_os	== __win32_os)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int _chdrive(int drive)
{
    char path[3];
    
    if (drive < 1 || drive > 31) {
    	return -1;
    }
    path[0] = (char) ('A' + drive -1);
    path[1] = ':';
    path[2] = 0x00;
    
    if (SetCurrentDirectory(path))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int _getpid(void)
{
	return GetCurrentProcessId();
}

#endif

// return unused heap memory

int _heapmin(void)
{
    errno = ENOSYS;
    return -1;
}

// Compare lexigraphically two strings

int _stricmp(const char *s1, const char *s2)
{
    char c1, c2;
    while (1)
    {
    	c1 = tolower(*s1++);
    	c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0) return 0;
    }
}

// Compare lexigraphically two strings up to a max length

int _strnicmp(const char *s1, const char *s2, int n)
{
    int i;
    char c1, c2;
    for (i=0; i<n; i++)
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (!c1) return 0;
    }
    return 0;
}


// reverse a string in place

char * _strrev(char * str)
{
	int SmallIndex = 0;
	int BigIndex = strlen(str) - 1;
	
	while (SmallIndex < BigIndex) {
		char Temp = str[SmallIndex];
		
		str[SmallIndex] = str[BigIndex];
		str[BigIndex] = Temp;
		
		SmallIndex++;
		BigIndex--;
	}
	
	return str;
}

wchar_t * _wstrrev(wchar_t * str)
{
	int SmallIndex = 0;
	int BigIndex = wcslen(str) - 1;
	
	while (SmallIndex < BigIndex) {
		wchar_t Temp = str[SmallIndex];
		
		str[SmallIndex] = str[BigIndex];
		str[BigIndex] = Temp;
		
		SmallIndex++;
		BigIndex--;
	}
	
	return str;
}
// Duplicate a string in malloc'ed memory

char *_strdup(const char *str)
{
	char * rval = (char *)malloc(strlen(str)+1);
	
	if (rval) {
		strcpy(rval, str);
	}
	
	return rval;
}


// Uppercase a string

char *_strupr(char *str)
{
    while (*str)
    {
        *str = toupper(*str);
        str++;
    }
    return str;
}


// Get the date in a string

char *_strdate(char *str)
{
    time_t timer;
    
    time(&timer);
    strftime(str, 10, "%m/%d/%y", localtime(&timer));
    return str;
}

// Set characters of string to character

char * _strset(char *str, int c)
{
	char *save = str;

	while (*str)
	{
		*str++ = c;
	}
	
	return save;
}

// Set first n characters of string to character

char * _strnset(char *str, int c, size_t n)
{
	char *save = str;

	while (n && *str)
	{
		*(str++) = c;
		n--;
	}

	return save;
}

// return pointer to first character in s1 that isn't in s2

char *_strspnp(char *s1, const char *s2)
{
	while (*s1)
	{
		const char *set = s2;
		while (*set)
		{
			if (*s1 != *(set++))
			{
				return s1;
			}
		}

		s1++;
	}
	
	return NULL;
}

// Convert integer to string

char * _itoa(int val, char *str, int radix) {
	char IsNegative = 0;
	int theNum = val;
	int StrIndex = 0;

	if (val == INT_MIN)					/* mm 990329 */
		strcpy(str, "-2147483648");		/* mm 990329 */
	else								/* mm 990329 */
	{	
		if (theNum < 0) 
		{
			theNum = -theNum;
			IsNegative = 1;
		}
		
		do 
		{
			int CurDigit = theNum % radix;
			if (CurDigit > 9)
				str[StrIndex++] = CurDigit + 'A' - 10;
			else
				str[StrIndex++] = CurDigit + '0';

			theNum /= radix;
		} while (theNum);

		if (IsNegative) 
			str[StrIndex++] = '-';
		str[StrIndex++] = 0;

		// Now reverse the string.
		_strrev(str);
	}									/* mm 990329 */

	return str;
}

wchar_t* _itow(int val, wchar_t *str, int radix) {
	char IsNegative = 0;
	int theNum = val;
	int StrIndex = 0;

	if (val == INT_MIN)					/* mm 990329 */
		wcscpy(str, L"-2147483648");	/* mm 990329 */
	else								/* mm 990329 */
	{	
		if (theNum < 0) {
			theNum = -theNum;
			IsNegative = 1;
		}

		do {
			int CurDigit = theNum % radix;
			if (CurDigit > 9)
				str[StrIndex++] = CurDigit + L'A' - 10;
			else
				str[StrIndex++] = CurDigit + L'0';

			theNum /= radix;
		} while (theNum);

		if (IsNegative) {
			str[StrIndex++] = '-';
		}
		str[StrIndex++] = 0;

		// Now reverse the string.
		_wstrrev(str);
	}									/* mm 990329 */

	return str;
}
int _doserrno = 0;

// Convert unsigned integer to a string

char * _ultoa(unsigned long val, char *str, int radix) {
	unsigned long theNum = val;
	int StrIndex = 0;

	do {
		int CurDigit = theNum % radix;
		if (CurDigit > 9)
			str[StrIndex++] = CurDigit + 'A' - 10;
		else
			str[StrIndex++] = CurDigit + '0';
		
		theNum /= radix;
	} while (theNum);
		
	str[StrIndex++] = 0;
	
	// Now reverse the string.
	_strrev(str);
		
	return str;
}


// Convert a value as a g format

char *_gcvt(double value, int digits, char *buffer)
{
    sprintf(buffer, "%.*g", digits, value);
    return buffer;
}

// Get file handle of a stream

/*int _fileno(FILE *stream)
{
    return stream->handle;
}
*/
#if (__dest_os	== __win32_os)
char * _fullpath(char * absPath, const char * relPath, size_t maxLength) {
	// Take paths such as:
	// A:\DIR\file
	// DIR\file
	// .\file
	// file
	// \DIR\DIRB\..\file
	// ..\DIR\file
	//
	// And convert them to:
	// A:\DIR\file

	// There are several ways a pathname can start:
	// [Drive Letter]:
	// \[Root Directory or File]
	// [Directory or File in Current Path]

	char * TempBuffer = 0;
	const char * SlashDotDot = "\\..";
	const char * SlashDot = "\\.";
	
	if (strlen(relPath) == 0)
		return 0;

	// First add the drive letter and current directory if necessary
	// to the path.

	if ((strlen(relPath) > 1) && (relPath[1] == ':')) {
		// The given path has a drive in the name..
		// Just move it to TempBuffer.
		TempBuffer = _strdup(relPath);
		
		if (!TempBuffer) {
			// _strdup error.
			return 0;
		}
	} else if (relPath[0] == '\\') {
		char * CurrentPath;
		
		// I took this technique from the _getcwd function.
		int CurrentDirLength = GetCurrentDirectory(0, NULL);

		CurrentPath = (char *)malloc(CurrentDirLength+1);
		
		if (!CurrentPath) {
			// Memory allocation error
			return 0;
		}
		
		if (!_getcwd(CurrentPath, CurrentDirLength+1)) {
			// _getcwd failed.
			free(CurrentPath);
			return 0;
		}

		if (CurrentPath[1] != ':') {
			// I got a bogus path from _getcwd
			free(CurrentPath);
			return 0;
		}

		CurrentPath[2] = 0;	// Make the string just the letter and a colon

		TempBuffer = (char *)malloc(strlen(CurrentPath)+strlen(relPath)+1);
		
		if (!TempBuffer) {
			// Memory allocation error.
			free(CurrentPath);
			return 0;
		}

		TempBuffer[0] = 0;
		strcat(TempBuffer, CurrentPath);
		strcat(TempBuffer, relPath);

		free(CurrentPath);
	} else {
		// Set TempBuffer to the current drive and path plus a \ if not one isn't
		// already there plus relPath.
		
		// I took this technique from the _getcwd function.
		int CurrentDirLength = GetCurrentDirectory(0, NULL);
		int Temp;

		// Allocate space for the current path, the relPath, a slash, and null
		// character.
		TempBuffer = (char *)malloc(CurrentDirLength+strlen(relPath)+2);

		if (!TempBuffer) {
			// Memory allocation error.
			return 0;
		}
		
		if (!_getcwd(TempBuffer, CurrentDirLength+1)) {
			// _getcwd failed.
			free(TempBuffer);
			return 0;	
		}
		
		if ((strlen(TempBuffer) < 3) || (TempBuffer[1] != ':')) {
			// I got a bogus path from _getcwd
			free(TempBuffer);
			return 0;
		}
		
		// Add a '\' character to the end of the current path if there isn't already
		// one there.
		Temp = strlen(TempBuffer);
		if (TempBuffer[Temp-1] != '\\') {
			TempBuffer[Temp] = '\\';
			TempBuffer[Temp+1] = 0;
		}
				
		strcat(TempBuffer, relPath);
	}

	// Now I have a string in TempBuffer that contains the full 
	// path including the drive letter. Now I have to clean all
	// the "." and ".." directories out.

//	printf("At Checkpoint 1: TempBuffer = %s\n", TempBuffer);

	// Clean out the "\.."'s.
	while (strstr(TempBuffer, SlashDotDot)) {
		char * theSecondSlash = strstr(TempBuffer, SlashDotDot);
		char * BufferPtr = theSecondSlash;
		char * MoveFrom;

		// Read the string from theSecondSlash backwards until
		// the previous slash is found. If there is no previous slash
		// the path is invalid.

		BufferPtr--;	// Move past the slash in the "\.."

		// I added the second part of the while statement so that 
		// a case like: "C:\temp\.\.\.." is properly resolved to "C:\"
		while ((*BufferPtr != '\\') || (*(BufferPtr+1) == '.')) {
			if (BufferPtr == TempBuffer) {
				// There is no previous slash. This path is invalid.
				free(TempBuffer);
				return 0;
			}

			BufferPtr--;
		}

		// Copy the rest of the string that is after the "\.." directly
		// after the space in the string pointed to by BufferPtr.
		
		MoveFrom = theSecondSlash + strlen(SlashDotDot);
		if (*MoveFrom != 0) {
			// There is more text after the "\..". If it isn't a '\' the
			// path name is invalid.
			if (*MoveFrom != '\\') {
				free(TempBuffer);
				return 0;
			}

			// I don't want to copy this slash directly after another
			// slash.
			MoveFrom++;
		}

		memmove(BufferPtr+1, MoveFrom,
			   strlen(TempBuffer) + 1 - (MoveFrom - TempBuffer));

//		printf("At Checkpoint 2: TempBuffer = %s\n", TempBuffer);
	}

	// Clean out the "\."'s.
	while (strstr(TempBuffer, SlashDot)) {
		char * theSlashDotP = strstr(TempBuffer, SlashDot);
		char * MoveFrom = theSlashDotP + strlen(SlashDot);
		
		memmove(theSlashDotP, MoveFrom,
			   strlen(TempBuffer) + 1 - (MoveFrom - TempBuffer));

//		printf("At Checkpoint 3: TempBuffer = %s\n", TempBuffer);
	}


	// TempBuffer is the final string.

	if ((strlen(TempBuffer) + 1) > maxLength) {
		free(TempBuffer);
		
		return 0;
	}
	
	if (absPath == 0) {
		absPath = (char *)malloc(maxLength);
		
		if (absPath == 0) {
			free(TempBuffer);

			return 0;
		}
	}
		
	strcpy(absPath, TempBuffer);
	
	free(TempBuffer);

	return absPath;
}

#endif

// Allocate memory on the stack

__declspec(naked) char * _alloca(size_t /*size*/)  /* hh 980122 removed unused arg size */
{
    asm
    {
     	pop		ecx				// pop return address
     	pop		eax				// get amount to alloc
     	sub		esp, eax		// subtract from stack pointer
     	mov		eax, esp		// and return the new stack pointer
     	push	eax				// leave something for the caller to pop
     	jmp		ecx
    }    
}

// put together a path

void _makepath(char *path, const char *drive, const char *dir, const char *fname, const char *ext)
{
    char ch;
    
    path[0] = 0;
    if (drive && drive[0])
    {
        strncat(path, drive, _MAX_PATH);
        if (path[strlen(path)-1] != ':')
        {
            strncat(path, ":", _MAX_PATH-strlen(path));
        }
    }
    if (dir && dir[0])
    {
        strncat(path, dir, _MAX_PATH-strlen(path));
        ch = path[strlen(path)-1];
        if (ch != '\\' && ch != '/')
        {
            strncat(path, "\\", _MAX_PATH-strlen(path));
        }
    }
    if (fname && fname[0])
    {
        strncat(path, fname, _MAX_PATH-strlen(path));
    }
    if (ext && ext[0])
    {
        if (ext[0] != '.')
        {
            strncat(path, ".", _MAX_PATH-strlen(path));
        }
        strncat(path, ext, _MAX_PATH-strlen(path));
    }
}

// Search environment path for a symbol
#if (__dest_os	== __win32_os)
void _searchenv(const char *filename, const char *varname, char *pathname)
{
    char buffer[1024], *cp;
    
    GetEnvironmentVariable(varname, buffer, sizeof(buffer));
    SearchPath(buffer, filename, "", _MAX_PATH, pathname, &cp);
}
#endif
// Return operating system file handle (as opposed to C file handle)

long _get_osfhandle(int filehandle)
{
    if (filehandle < 0 || filehandle > NUM_HANDLES || _HandleTable[filehandle] == NULL)
    {
        return -1;
    }
    return (long)_HandleTable[filehandle]->handle;
}


// Open an operating system handle as a C file handle

int _open_osfhandle(long ofshandle, int flags)
{
    int h;
    
    h = GetHandle();
    if (h == -1)
    {
        return -1;
    }
	_HandleTable[h] = (FileStruct *)malloc(sizeof(FileStruct));
	if (!_HandleTable[h])
	{
	    return -1;
	}
	
    
    _HandleTable[h]->handle = (HANDLE)ofshandle;
    _HandleTable[h]->translate = (flags & O_BINARY) == 0;
    _HandleTable[h]->append = (flags & O_APPEND) != 0;
    
    return h;
}

int GetHandle()
{
    int i;
    
    for (i=0; i<256; i++)
    {
        if (_HandPtr == NUM_HANDLES)
        {
            _HandPtr = 3;
        }
        if (_HandleTable[_HandPtr] == 0)
        {
            return _HandPtr++;
        }
        _HandPtr++;
    }
    return -1;
}
#if (__dest_os	== __win32_os)
unsigned _getdiskfree(unsigned drive, struct _diskfree_t *dfs)
{
	char path[4];
	
	if (drive > 26) 
		return -1;
	else {
		path[0] = (char) (drive + 'A' -1);
		path[1] = ':';
		path[2] = '\\';
		path[3] = 0x00;
	}
	
	if (!GetDiskFreeSpace(drive ? path : NULL,
			&(dfs->sectors_per_cluster), &(dfs->bytes_per_sector),&(dfs->avail_clusters),&(dfs->total_clusters)))
		return -1;
	return 0;
}

char *_getdcwd(int drive, char *buffer, int maxlen)
{
	int n;
	char dir[_MAX_PATH];
	char *file;
	
	if (drive != 0) {
		char dstr[4];
		
		if (drive > 26) {
			errno = EACCES;
			return NULL;
		}
		
		dstr[0] = (char) (drive + 'A' -1);
		dstr[1] = ':';
		dstr[2] = '.';
		dstr[3] = 0x00;

		n = GetFullPathName(dstr,sizeof(dir),dir,&file);
	}
	else
		n = GetCurrentDirectory(sizeof(dir),dir);
		
	if (!n || ++n > sizeof(dir))
		return NULL;
		
	
	if (!buffer) {
		if (!(buffer = (char *) malloc(n > maxlen ? n : maxlen))) {
			errno = ENOMEM;
			return NULL;
		}
	}
	else 
		if (n > maxlen) {
			errno = ERANGE;
			return NULL;
		}
		
	return strcpy(buffer,dir);
}

int _getdrive()
{
	char	dir[_MAX_PATH];
	int		drive = 0;
	
	if (GetCurrentDirectory(sizeof(dir),dir))
		if (dir[1] == ':')
			drive = toupper(dir[0]) - 'A' - 1;
			
	return drive;
	
}

unsigned long _getdrives()
{
	return GetLogicalDrives();
}
char * _strlwr (char *string)
{
	char *s = string;
	
	while (*s)
	{
		*s = tolower (*s);
		s++;
	}

	return string;
}

#endif

/* _splitpath takes a path and returns pointers to each of the
 * components of the path. */

void _splitpath (const char *path, char *drive, char *dir, char *fname, 
	char *ext)
{
	const char *end, *dir_end_fwd, *dir_end_back;

	/* first, check for drive letter in string... we should find a colon
	 * in the second character position if its included. */

	if (path [1] == ':')
	{
		if (drive)
		{
			drive [0] = path [0];
			drive [1] = ':';
			drive [2] = '\0';
		}
		path += 2;		/* increment current position to beyond
						 * drive letter */
	}
	else if (drive != NULL)	/* if no drive, make empty */
		strcpy (drive, "");

	/* to find the directory, look for the last slash character */
  
	dir_end_back = strrchr (path, '\\');
	dir_end_fwd = strrchr (path, '/');

	/* use the one farthest to the end as the delimiter */

	if (dir_end_fwd && dir_end_back)
		end = (dir_end_fwd > dir_end_back ? dir_end_fwd : dir_end_back);
	else if (dir_end_fwd)
		end = dir_end_fwd;
	else
		end = dir_end_back;
  
	if (end == NULL)		/* if no dir, zero out path */
	{
		if (dir != NULL)
			strcpy (dir, "");
	}
	else			/* we have a directory */
	{
		if (dir != NULL)	/* do we need to copy it? */
		{
			memcpy (dir, path, end - path + 1);
			dir [end - path + 1] = '\0';
		}
		path = end + 1;		/* next search starts after dir */
	}

	/* now, we are at the filename... to isolate this, find the last
	 * period, everything up to this is filename, everything after is
	 * extension */

	end = strrchr (path, '.');

	if (end == NULL)		/* no extension */
	{
		end = path + strlen (path); /* set to end of string */
	}

	if (fname != NULL)
	{
		memcpy (fname, path, end - path);
		fname [end - path] = '\0';
	}

	if (ext != NULL)
		strcpy (ext, end);
}

int _wtoi (const wchar_t *str)
{
	return watoi (str);
}

/*
 * lowercase all characters in str
 */

wchar_t *_wcslwr (wchar_t *str)
{
	wchar_t *saved = str;

	while (*str)
	{
		*str = towlower (*str);
		str++;
	}
  
	return saved;
}

/*
 * uppercase all characters in str
 */

wchar_t *_wcsupr (wchar_t *str)
{
	wchar_t *saved = str;

	while (*str)
	{
		*str = towupper (*str);
		str++;
	}
  
	return saved;
}

/*
 * copy str into newly malloc'd storage, return pointer to space
 */

wchar_t *_wcsdup (const wchar_t *str)
{
	wchar_t * rval = (wchar_t *)malloc((wcslen(str)+1) * sizeof (wchar_t));
	
	if (rval) {
		wcscpy(rval, str);
	}
	
	return rval;
}

/*
 * case-insensitive compare of s1, s2
 */

int _wcsicmp(const wchar_t *s1, const wchar_t *s2)
{
    wchar_t c1, c2;
    while (1)
    {
    	c1 = towlower(*s1++);
    	c2 = towlower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0) return 0;
    }
}

/*
 * case-insensitive compare of s1, s2, stopping after n characters
 */

int _wcsnicmp(const wchar_t *s1, const wchar_t *s2, size_t n)
{
    int i;
    wchar_t c1, c2;
    for (i=0; i<n; i++)
    {
        c1 = towlower(*s1++);
        c2 = towlower(*s2++);
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (!c1) return 0;
    }
    return 0;
}

/*
 * reverse order of characters in str
 */

wchar_t * _wcsrev(wchar_t *str)
{
	int SmallIndex = 0;
	int BigIndex = wcslen(str) - 1;
	
	while (SmallIndex < BigIndex) {
		wchar_t Temp = str[SmallIndex];
		
		str[SmallIndex] = str[BigIndex];
		str[BigIndex] = Temp;
		
		SmallIndex++;
		BigIndex--;
	}
	
	return str;
}

/*
 * set all characters of str to wc
 */

wchar_t *_wcsset(wchar_t *str, wchar_t wc)
{
	wchar_t *saved = str;

	while (*str)
	{
		*str++ = wc;
	}
  
	return saved;
}

/*
 * set characters of str to wc, up to max of n characters
 */

wchar_t *_wcsnset(wchar_t *str, wchar_t wc, size_t n)
{
	wchar_t *saved = str;

	while (*str && n--)
	{
		*str = wc;
	}
  
	return saved;
}

// return pointer to first character in s1 that isn't in s2

wchar_t *_wcsspnp(const wchar_t *s1, const wchar_t *s2)
{
	const wchar_t *set;
	while (*s1)
	{
		for (set = s2; *set; set++)
		{
			if (*s1 != *(set++))
			{
				return (wchar_t *)s1;
			}
		}

		s1++;
	}
	
	return NULL;
}

/* Change History

961120 KO	Made a fix to _fullpath.
961212 KO	Made wcslen and wcscpy work better and added swprintf (which probably won't work
			too well.) 
hh 980122 Replaced <windows.h> with the following TWO includes because it is seriously
          broken.  The following 2 includes must be carefully ordered as shown, because
          they are broken too.
hh 980122 needed <cstdarg> for va_start
hh 980122 removed unused arg size
mf 980227 added splitpath and _strlwr to support MFC
mf 980228 removed wcskeb abd wcscpy as they are now in wstring.c
vss 980310 removed - defined in wprintf.c
blc 980324 added wide character functions used in WinCE
mf  980505 commented out implementation of functions that use win32 api's not available in
           ce
mf  980515 added a couple of non-standard wchar routines _itow,_wrevstr           
blc 990312 added _strset, _strnset, _strspnp, _wcsspnp
mm  990329 Make _itoa and _itow work for largest negative value WB1-5143, IL9903-2106           
blc 990331 Fixed double spacing problem with mm's last check in
*/
