/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *		maincrt.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */

#include <ansi_parms.h> 
#include <WINDEF.H>
#include <WINBASE.H>
#include <crtl.h>
#include "ExceptionX86.h"
#include <stdlib.h>
#include "ThreadLocalData.h"

int main(int argc, char **argv);     /* hh 971207 Added prototype */

extern int _MWCHandler(
    PEXCEPTION_RECORD exc, void *exc_stack, LPCONTEXT context, void *mystery);
// used to be LPEXCEPTION_RECORD
// C++ exception tables

static ExceptionTableHeader ExceptionTables =
{
    &_PcToActionStart,
    &_PcToActionEnd,
    0
};

// The main entry point for Win32 console apps.

void mainCRTStartup();     /* hh 971207 Added prototype */
void mainCRTStartup()
{
    // Set up exception handler
    
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
        
    // Perform library initialization
    
    _CRTStartup();
    
	// Initialize the thread local data structure.
    if (!_InitializeMainThreadData()) {
    	// Initialization failed.
    	exit(-1);
    }

    // Set up the command line arguments
    #if (__dest_os==__win32_os)
    _SetupArgs(); //there is no commmand line in WINCE
    #endif

    // Run the init code (which runs static C++ constructors)
    
    _RunInit();
    
    // Call the main program and exit with the status it returns
    
    exit(main(__argc, __argv));
}

/*
	Change Record
 * hh 971207 Added main prototype
 * hh 971207 Added mainCRTStartup prototype
*/
