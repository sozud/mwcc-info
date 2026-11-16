/*/  Metrowerks x86 Runtime Support Library  Version 2.1  Mar 14 1998 /*/

/*
 *	dllmaincrt.c
 *	
 *		Copyright © 1995-1998 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */
 
#include <ansi_parms.h>
#include <stdarg.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "crtl.h"
#include "ExceptionX86.h"
#include "ThreadLocalData.h"
#include <abort_exit.h>     /* hh 971207 Added <abort_exit.h> header */
//#include "critical_regions.h"

// C++ Exception Tables
static ExceptionTableHeader ExceptionTables =
{
    &_PcToActionStart,
    &_PcToActionEnd,
    0
};

// DLL init/term function for the runtime / MSL DLL.
// On init and term we do nothing more than register our exception
// tables.  The main process or first DLL to use this guy will cause
// the actual initilization.
//
// On thread start we allocate the thread local storage required by
// MSL and on thread termination we deallocat it.

BOOL __stdcall _DllMainCRTStartup(HINSTANCE inst, DWORD fdwReason, LPVOID lpvReserved);     /* hh 971207 Added prototype */
BOOL __stdcall _DllMainCRTStartup(HINSTANCE inst, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			// Register our exception handling tables
			_CRTStartup();
			_RegisterExceptionTables(&ExceptionTables);
			break; 

		case DLL_THREAD_ATTACH:
		
			// Get MSL thread local data
			if (!_InitializeThreadData()) {
	        	return 0;		
			}
			break;

		case DLL_PROCESS_DETACH:
			// Unregister the exception tables
			_UnRegisterExceptionTables(&ExceptionTables);
			break;

		case DLL_THREAD_DETACH:
			// Free thread local data
			_DisposeThreadData();
			break;

	}

	// Success.
	return 1;
}

