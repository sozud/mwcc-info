/*  Metrowerks Standard Library  */

/*  $Date: 1999/02/03 21:36:58 $ 
 *  $Revision: 1.5 $ 
 *  $NoKeywords: $ 
 *
 *		Copyright © 1995-1999 Metrowerks, Inc.
 *		All rights reserved.
 */
 
/*
 *	sysenv.win32.c
 *	
 *	Routines
 *	--------
 *		getenv
 *		system
 *
 */

#include <crtl.h>
#include <stdlib.h>
#include <string.h>
#include <windef.h>
#include <winbase.h>

/* This routine takes the name of an environment variable and returns
 * its corresponding value.	*/
#if ( __dest_os==__wince_os)
char * getenv(const char * /*inVarName*/)
{
return 0;
}
#else
char * getenv(const char * inVarName)
{
	/* begin mm 990115 */
	char * CurEnvString;
	int theVarNameLength = strlen(inVarName);
	int StrInx = 0;
	 
	while((CurEnvString = _Environ[StrInx++]) != NULL)		/* mm 990125 */
	{
		if ((CurEnvString[theVarNameLength] == '=') &&
		    (_strnicmp(CurEnvString, inVarName, theVarNameLength) == 0)) 
			/* The string matches. Return a pointer to the value
			 * after the '=' sign.	*/
			return (CurEnvString + theVarNameLength + 1);
	}
	/* end mm 990115 */
	
	return 0;
}
#endif 


int system(const char * cmdLine)
{
	int result = 0;
	unsigned short bResult = 0;
	
	if(cmdLine)
	{
		PROCESS_INFORMATION piProcInfo; 
		STARTUPINFO			siStartInfo;

		/* Create the child process. */ 
        memset(&siStartInfo,0,sizeof(siStartInfo));
        siStartInfo.cb = sizeof(siStartInfo);

		bResult = CreateProcess(	NULL, 				/* application name */
									(char *) cmdLine,	/* command line */
									NULL,				/* process security attributes */
									NULL,				/* primary thread security attributes */
									TRUE,				/* handles are inherited */
									0,					/* creation flags */
									NULL,				/* use parent's environment */
									NULL,				/* working directory path */
									&siStartInfo,		/* STARTUPINFO pointer */
									&piProcInfo);		/* receives PROCESS_INFORMATION */

	    if( bResult ) 
	    {
	    	WaitForSingleObject(piProcInfo.hProcess, (DWORD)(-1L)); /* wait until the process has completed */
	    	CloseHandle(piProcInfo.hProcess);
			CloseHandle(piProcInfo.hThread);
			result = 1;
		} 
		else 
		{
			result = 0;
		}

	}

	return result;	
}

/*  Change History
 * 961211 KO	Wrote this file.
 * mm 990115    Rewrote getenv() to make use of _Environ
 * vss 990121   Moved system function to sysenv.c
 * mm 990125  Change to allow compilation without warning message
 */
