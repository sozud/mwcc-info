/*  Metrowerks Standard Library  */

/*  $Date: 1999/03/24 05:17:51 $ 
 *  $Revision: 1.3 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */

/*
 *		ThreadLocalData.c
 */
 
#include "ThreadLocalData.h"
#include <cstdlib>    /* hh 980122 added <cstdlib> */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Private function set up to be called "atexit" by _InitializeThreadDataIndex.
void _DisposeThreadDataIndex(void);

static unsigned long _gThreadDataIndex = -1;

// This function must be called before _InitializeThreadData.
// Returns false on error, true on success.
int _InitializeThreadDataIndex(void)
{
	if (_gThreadDataIndex != -1) {
		// _gThreadDataIndex is already initialized
		return 1;
	}
	
	_gThreadDataIndex = TlsAlloc();

	if (_gThreadDataIndex == -1) {
		// The allocation failed.
		return 0;
	}
	
	if (atexit(_DisposeThreadDataIndex)) {
		return 0;
	} else {
		return 1;
	}
}

void _DisposeThreadDataIndex(void)
{
	if (_gThreadDataIndex == -1) {
		// _gThreadDataIndex has an invalid value.
		// Maybe it was never allocated?
		return;
	}
	
	TlsFree(_gThreadDataIndex);
	_gThreadDataIndex = -1;
}

// This function sets up a structure of thread local data needed by various MSL functions.
// When the thread ends it should call _DisposeThreadData.
// Returns false on error, true on success.
int _InitializeThreadData(HANDLE threadHandle)
{
	_ThreadLocalData * theThreadLocalData = 
			(_ThreadLocalData *)malloc(sizeof(_ThreadLocalData));
	
	if (!theThreadLocalData) {
		return 0;
	}
	
	// All initialization of items in the _ThreadLocalData structure
	// goes here...
	
	theThreadLocalData->errno = 0;	
	theThreadLocalData->random_next = 1;
	theThreadLocalData->strtok_n = (unsigned char *) "";
	theThreadLocalData->strtok_s = (unsigned char *) "";
	theThreadLocalData->thread_handle = threadHandle;
	
	return TlsSetValue(_gThreadDataIndex, theThreadLocalData);
}


int _InitializeMainThreadData(void)
{
    // First, get the thread data index by using tlsAlloc
    
    if (_InitializeThreadDataIndex())
    {
        // now, get the data for the main thread
        
        if (_InitializeThreadData(NULL))
        {
            return 1;
        }
    }
    
    return 0;
}


void _DisposeThreadData(void)
{
	_ThreadLocalData * theThreadLocalData = _GetThreadLocalData();
	
	if (!theThreadLocalData) {
		return;
	}
	
	// All destruction of items in the _ThreadLocalData structure
	// goes here...
	
	free (theThreadLocalData);
	
	TlsSetValue(_gThreadDataIndex, 0);
}

_ThreadLocalData * _GetThreadLocalData(void)
{
    _ThreadLocalData *tld;
    
	tld = (_ThreadLocalData *)TlsGetValue(_gThreadDataIndex);
	
	if (tld == 0)
	{
		_InitializeThreadData(NULL);
		tld = (_ThreadLocalData *)TlsGetValue(_gThreadDataIndex);
	}
	
	if (tld == 0)
	{
	    MessageBox(NULL, TEXT("Could not get thread local data"), TEXT("MW Win32 Runtime"), MB_OK);
	    exit(0);
	}
	return tld;
}

/*
hh  980122 added <cstdlib>
hh  980122 replaced <wtypes.h> with <WINDEF.H> 
rjk 980314 fixed _GetThreadLocalData to allocate it if it was not already
blc 990317 modified Windows.h include strategy, added threadHandle support
*/
