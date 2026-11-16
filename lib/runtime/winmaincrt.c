/*/  Metrowerks x86 Runtime Support Library  30-Oct-96  /*/

/*
 *	winmaincrt.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */

#include <ansi_parms.h>
#include <windef.h>
#include <winbase.h>
#include <winnt.h>
#include <winuser.h>

#include "crtl.h"
#include "ExceptionX86.h"
#include "ThreadLocalData.h"
#include <abort_exit.h>
#include <stdlib.h>
extern int _MWCHandler(
    PEXCEPTION_RECORD exc, void *exc_stack, LPCONTEXT context, void *mystery);

// C++ exception tables

static ExceptionTableHeader ExceptionTables =
{
    &_PcToActionStart,
    &_PcToActionEnd,
    0
};



// This is the real main entry point for Win32 GUI applications

void WinMainCRTStartup();        /* hh 971207 Added prototype */
void WinMainCRTStartup()
{
    TCHAR *CmdPtr;
    STARTUPINFO si;
    int show;
    
    // Set up excpetion handler
    
    asm
    {
    		
    		push	ebp
    		mov		eax, -1
    		push	eax
    		push	eax
    		push	OFFSET _MWCHandler
    		push	fs:[0]
    		mov		fs:[0], esp
    		
    }
    
    // Register the exception C++ handling tables
    
    _RegisterExceptionTables(&ExceptionTables);
        
    // Do C runtime library startup
    
    _CRTStartup();
    
	// Initialize the thread local data structure.
    if (!_InitializeMainThreadData()) {
    	// Initialization failed.
    	exit(-1);
    }
    
    // Setup the globals __argc and __argv.
    _SetupArgs();

    // Run the init code
    _RunInit();
    
    // Get the command line  
#if(__dest_os==__win32_os)  
    CmdPtr = GetCommandLine();
    
    // Skip program name (up to first space or tab)
    
    while ((*CmdPtr) && (*CmdPtr != ' ') && (*CmdPtr != '\t'))
    {
    	if (*CmdPtr == '\"') {
    		CmdPtr++;
    		
    		// Quoted block. Skip until closing quote or null.
		    while (*CmdPtr && *CmdPtr != '\"') {
		    	CmdPtr++;
		    }
    	
    		// Skip closing double quote if there is one.
    		if (*CmdPtr == '\"') {
    			CmdPtr++;
    		}
    	
    	} else {
	        CmdPtr++;
        }
    }
    
    // Skip up to first printable char
    
    while (*CmdPtr && *CmdPtr <= ' ')
    {
        CmdPtr++;
    }
    
    // Get the startup info for the show command
    
    GetStartupInfo(&si);
    if (si.dwFlags & STARTF_USESHOWWINDOW)
    {
        show = si.wShowWindow;
    }
    else
    {
        show = SW_SHOWDEFAULT;
    }
     exit(WinMain(GetModuleHandle(0), 0, CmdPtr, show));
    // Call the WinMain and exit
#else
     CmdPtr=NULL;
     
     exit(WinMain((void*)0x00400000/*(void*)GetCurrentProcessId()*/, 0, CmdPtr, SW_SHOW));
     /* why wince wants GetCurrentProcessId() I don't know,
        MS says this will change, but this works for now */
#endif
   
}

/*
	Change Record
 * hh 971207 Added WinMainCRTStartup prototype
*/
