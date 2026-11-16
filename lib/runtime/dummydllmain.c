/*/  Metrowerks x86 Runtime Support Library  Version 1.0  06-May-96  /*/

/*
 *	dummydllmain.c
 *	
 *		Copyright © 1995-1996 Metrowerks, Inc.
 *		All rights reserved.
 *	
 *
 */

#include <ansi_parms.h>

#include <wchar.h>
#include <stdarg.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// This is a dummy dll main which is included if the user doesn't make one

BOOL __stdcall DllMain(HINSTANCE inst, DWORD fdwReason, LPVOID lpvReserved);     /* hh 971207 Added prototype */
BOOL __stdcall DllMain(HINSTANCE inst, DWORD fdwReason, LPVOID lpvReserved)
{
    return 1;
}

/*
	Change Record
 * hh 971207 Added DllMain prototype
*/
