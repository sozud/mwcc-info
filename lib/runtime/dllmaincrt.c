/*/  Metrowerks x86 Runtime Support Library  Version 1.0  20-Dec-96  /*/

/*
 *	dllmaincrt.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */
 
#include <ansi_parms.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "crtl.h"
#include "ExceptionX86.h"
#include "ThreadLocalData.h"
#include <abort_exit.h>     /* hh 971207 Added <abort_exit.h> header */

extern BOOL __stdcall DllMain(HINSTANCE, DWORD, LPVOID);
static int __proc_attached = 0;
// C++ Exception Tables
static ExceptionTableHeader ExceptionTables =
{
    &_PcToActionStart,
    &_PcToActionEnd,
    0
};

BOOL (WINAPI* _pRawDllMain)(HINSTANCE, DWORD, LPVOID);

// This procedure does C library initialization and cleanup for DLLs. This is called
// automatically when the default DLL entry point is used. Only DLL's using custom
// entry points should call this directly.
// Even though this is named _CRT_INIT, the whole library is initialized; not just the
// run time.
BOOL __stdcall _CRT_INIT(HINSTANCE, DWORD, LPVOID);
BOOL __stdcall _CRT_INIT(
	HINSTANCE inst, 
	DWORD fdwReason, 
	LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			// Register our exception handling tables
			_RegisterExceptionTables(&ExceptionTables);

			// Start up the runtime system in case it isn't already
			_CRTStartup();

			// Set up the thread local data structure.
			if (!_InitializeMainThreadData()) {
				return 0;
			}

			// Run the constructors for static and global C++ objects.    
			_RunInit();

            /*
             * increment flag to indicate process attach notification
             * has been received
             */
            __proc_attached++;

			break;
		case DLL_THREAD_ATTACH:
			if (!_InitializeThreadData())
			{
	        	return 0;		
			}
			break;
			
		case DLL_PROCESS_DETACH:
			if (__proc_attached > 0)
			{
				__proc_attached--;
				
				// only get rid of these items when the DLL shuts down completely
				if (__proc_attached == 0)
				{
					// Destroy static objects
					__destroy_global_chain();
					
					// Make sure all of the thread data is deleted to avoid
					// memory leak
					_DisposeThreadData();

					// Unregister the exception tables
					_UnRegisterExceptionTables(&ExceptionTables);
					
					// This procedure does all the clean up that exit would do but it
					// doesn't call ExitProcess. Calling ExitProcess would prevent
					// the rest of the process from being able to clean up.
					_CleanUpMSL();
				}
			}
			break;
			
		case DLL_THREAD_DETACH:
			_DisposeThreadData();
			break;
	}

	// Success.
	return 1;
}

// The default entry point for DLLs.
BOOL __stdcall _DllMainCRTStartup(HINSTANCE, DWORD, LPVOID);
BOOL __stdcall _DllMainCRTStartup(
	HINSTANCE hDllHandle,
	DWORD dwReason,
	LPVOID lpreserved)
{
        BOOL retcode = TRUE;

        /*
         * If this is a process detach notification, check that there has
         * been a prior process attach notification.
         */
        if ((dwReason == DLL_PROCESS_DETACH) && (__proc_attached == 0))
            return FALSE;

        if (dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH)
        {
            if (_pRawDllMain)
                retcode = (*_pRawDllMain)(hDllHandle, dwReason, lpreserved);

            if (retcode)
                retcode = _CRT_INIT(hDllHandle, dwReason, lpreserved);

            if (!retcode)
                return FALSE;
        }

        retcode = DllMain(hDllHandle, dwReason, lpreserved);

        if ((dwReason == DLL_PROCESS_ATTACH) && !retcode)
        {
			// The user's DllMain routine returned failure, the C runtime
			// needs to be cleaned up. Do this by calling _CRT_INIT again,
			// this time imitating DLL_PROCESS_DETACH. Note this will also
			// clear the __proc_attached flag so the cleanup will not be
			// repeated upon receiving the real process detach notification.
			_CRT_INIT(hDllHandle, DLL_PROCESS_DETACH, lpreserved);
		}

        if ((dwReason == DLL_PROCESS_DETACH) ||
			(dwReason == DLL_THREAD_DETACH))
        {
            if (_CRT_INIT(hDllHandle, dwReason, lpreserved) == FALSE)
                retcode = FALSE;

            if (retcode && _pRawDllMain)
				retcode = (*_pRawDllMain)(hDllHandle, dwReason, lpreserved);
        }

        return retcode;
}
/* Change Record
961220 KO  Mostly rewrote this file.
hh  971207  Added <abort_exit.h> header
hh  971207  Added namespace support
hh  971207  Added _CRT_INIT prototype
hh  971207  Added _DllMainCRTStartup prototype
blc 990304  Incorporated MFC DLL fixes for multiple MFC users
blc 990315  Fixed problems with MFC initialization
*/
