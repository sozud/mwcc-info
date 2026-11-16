/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *	exchand.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *	Routines
 *	--------
 *		_MWCHandler
 *	
 *	Modification History
 *	--------------------
 *
 */
#include <stdio.h>	// 960711: Wouldn't compile without it.
#include <WINDEF.H>
#include <WINBASE.H>
#include <winuser.h>
#include <exception>

// Any of these bits means handler is called in unwinding mode

#define UNWINDING_MASK			0x66

// Exception code for a C++ exception

#define CPP_EXCEPTION_CODE		0xE06D7363


// Default exception handler for the Metrowerks C/C++ runtime library 

extern "C" int _MWCHandler(
    PEXCEPTION_RECORD exc, void */*exc_stack*/, LPCONTEXT context, void *mystery);       /* hh 971207 Added prototype */

extern "C" int _MWCHandler(
    PEXCEPTION_RECORD exc, void */*exc_stack*/, LPCONTEXT context, void */*mystery*/)
{
    TCHAR buffer[1024];


	// Don't do anything if we are unwinding
	
	if (exc->ExceptionFlags & UNWINDING_MASK)
	{
	    return 1;
	}

	// Check for a C++ exception
	
	if (exc->ExceptionCode == CPP_EXCEPTION_CODE)
	{
		// Call the terminate function, which the user
		// might replace
		
		terminate();
		
		// should not return, but it it does, we will
		// put out the message below.
	}



    // Make a message
#ifdef UNICODE   

swprintf(buffer, sizeof(buffer)+1,L"Unhandled exception: %08x\n" 
                                  L"At address:          %08x\n",
			    			    exc->ExceptionCode ,
    						    context->Eip);
    
    			    
#else
#if(__dest_os == __wince_os)
 #error "the macro UNICODE must be defined when compiling the runtime for Windows CE"
#endif     
sprintf(buffer, "Unhandled exception: %08x\n"
   			    "At address:          %08x\n",
   			    exc->ExceptionCode,
   			    context->Eip);
#endif /* UNICODE */

    // And display it in a message box
#if(__dest_os == __wince_os)
   MessageBox(NULL, buffer, TEXT("Unhandled Exception"), MB_OK );
#else    			    
    MessageBox(NULL, buffer, TEXT("Unhandled Exception"), MB_OK | MB_TASKMODAL);
#endif    
    // return success
    
    return 1;
}


/*
	Change Record
 * hh 971207 Added _MWCHandler prototype
 * mf 980827 fix for CE
 * mf 980903 another fix for CE, missing comma
*/
