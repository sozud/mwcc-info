@ECHO off

rem Edit this for your installation of Codewarrior for Windows, Version 9
set CWFolder=c:\Program Files\Metrowerks\Codewarrior

if "%1" == "-help" goto help
if "%1" == "-?" goto help
if "%1" == "/?" goto help

rem This should match CWDS9...
if EXIST "%CWFolder%\bin\PluginLib6.dll" goto gotdir

echo The directory %CWFolder% 
echo does not exist or does not contain the expected files.  
echo Please edit the definition of "CWFolder"
echo in this file to point to your installation of 
echo Codewarrior for Windows, Version 9.
echo.
pause

:help

echo This batch file sets up environment variables to enable use of
echo the shared MSL runtime and MFC DLLs from within the IDE, and also
echo to enable command-line development. This script is only necessary
echo if you elected to have the installer not install environment 
echo variables for command-line development.
echo.
echo Usage:  cwenv.bat [-help] [-quiet] [-cross] [-ms] (order counts!)
echo.
echo View this file for more information, or see the directory 
echo Release Notes/CLT Notes for information on setting up the
echo command-line tools.
echo.
echo Note, as of CWDS 8, the command-line tools bring in default libraries
echo before MWLibraryFiles, so the "-k6" and "-dll" options are not 
echo supported anymore.  Use "-inst 3dnow" or "-runtime dllmulti" instead.

goto end

:gotdir

rem Define this to a nonblank value to link against the DLL versions
rem of MSL and MFC instead of the static versions by default.
rem Note, default libraries take precedence.
set DLL=

rem Define this to a nonblank value to set up variables for cross-compiling
rem by default.  Under Win9x, be sure to crank up your environment size!
set CROSS=

set QUIET=
if NOT "%1" == "-quiet" goto loud
set QUIET=1
shift
:loud

if NOT "%1" == "-cross" goto host
set CROSS=1
shift
:host

rem Define this variable to a nonblank value to use VC++ emulation
rem options (from "cl" and "link").
set MS=
if "%1" == "-ms" goto mscompat
if NOT "%1" == "-vc" goto last
:mscompat
set MS=1
shift

:last
if NOT "%1" == "" goto help


rem Windows defaults
set MWCWinx86Options=
set MWLinkWinx86Options=
set MWCWinx86Includes=%CWFOLDER%\MSL\MSL_C\MSL_Common\Include;%CWFOLDER%\MSL\MSL_C\MSL_Win32\Include;%CWFOLDER%\MSL\MSL_C\MSL_X86;%CWFOLDER%\MSL\MSL_C++\MSL_Common\Include;%CWFOLDER%\MSL\MSL_Extras\MSL_Common\Include;%CWFOLDER%\MSL\MSL_Extras\MSL_Win32\Include;%CWFOLDER%\Win32-x86 Support\Headers\Win32 SDK;%CWFOLDER%\Win32-x86 Support\Headers\Precompiled Headers;%CWFOLDER%\Win32-x86 Support\Headers\Prefix Files;%CWFOLDER%\Win32-x86 Support\Headers\VCPP Headers;%CWFOLDER%\Win32-x86 Support\Headers\Alloca;%CWFOLDER%\Win32-x86 Support\Headers\Intrinsics;%CWFOLDER%\Win32-x86 Support\Headers\MFC;%CWFOLDER%\Win32-x86 Support\Headers\ATL;%CWFOLDER%\MSL\MSL_C++\MSL_Precompiled_Headers
set MWWinx86Libraries=%CWFOLDER%\Win32-x86 Support\Libraries\Runtime\Libs;%CWFOLDER%\Win32-x86 Support\Libraries\Win32 SDK;%CWFOLDER%\MSL\MSL_C\MSL_Win32\Lib\x86;%CWFOLDER%\MSL\MSL_C++\MSL_Win32\Lib\x86;%CWFOLDER%\MSL\MSL_Extras\MSL_Win32\Lib\x86;%CWFOLDER%\Win32-x86 Support\Libraries\MFC\Libs;%CWFOLDER%\Win32-x86 Support\Libraries\ATL\Libs

rem Note: this is a FALLBACK SETTING.  The default libraries should already be correct.
set MWWinx86LibraryFiles=MSL_All_x86.lib;gdi32.lib;kernel32.lib;user32.lib

rem MacOS defaults
if "%CROSS%" == "" goto nomac

set MWCMacPPCIncludes=+%CWFolder%\MSL;+%CWFolder%\MacOS Support
set MWMacPPCLibraries=+%CWFolder%\MSL;+%CWFolder%\MacOS Support
set MWMacMPWPPCLibraries=+%CWFolder%\MSL;+%CWFolder%\MacOS Support
set MWMacMPWPPCLibraryFiles=MSL_C_PPC_MPW.Lib;MSL_MPWCRuntime_PPC.Lib;PPCToolLibs.o;InterfaceLib;MathLib
set MWMacPPCLibraryFiles=MSL_C_PPC.Lib;MSL_C++_PPC.Lib;MSL_SIOUX_PPC.Lib;MSL_Runtime_PPC.Lib;InterfaceLib;MathLib

:nomac

rem If this variable gets too long, lmgr326b.dll will crash
if NOT "%ORIG_LM_LICENSE_FILE%" == "" goto gotlic
set ORIG_LM_LICENSE_FILE=%LM_LICENSE_FILE%
:gotlic
set LM_LICENSE_FILE=%CWFolder%\license.dat;%ORIG_LM_LICENSE_FILE%

rem PATH is not really a variable, it's a command.  
rem Since Win9x and WinNT/Win2000 interpret quotes differently, 
rem we have to check the OS here to avoid trashing the PATH.
if NOT "%OS%" == "" goto noquotes
set PATH="%CWFolder%\Bin";"%CWFolder%\Other Metrowerks Tools\Command Line Tools";"%CWFolder%\Win32-X86 Support\Libraries\Runtime\Libs\MSL_All-DLLs";"%CWFolder%\Win32-x86 Support\Libraries\MFC\Libs";"%CWFolder%\Win32-x86 Support\Libraries\ATL\Libs";"%PATH%"
goto gotpath
:noquotes
set PATH=%CWFolder%\Bin;%CWFolder%\Other Metrowerks Tools\Command Line Tools;%CWFolder%\Win32-X86 Support\Libraries\Runtime\Libs\MSL_All-DLLs;%CWFolder%\Win32-x86 Support\Libraries\MFC\Libs;%CWFolder%\Win32-x86 Support\Libraries\ATL\Libs;%PATH%
:gotpath

if NOT "%QUIET%" == "" goto noinfo
echo Metrowerks Codewarrior Dev Studio v9 command-line variables set up.
echo Run "mwcc -c" to compile and "mwld" to link targeting Win32.
if "%CROSS%" == "" goto nomacinfo
echo Run "MWCPPC" / "MWLinkPPC" for Mac/PPC classic,
echo and "MWC68K" / "MWLink68K" for Mac/68K classic.
:nomacinfo
echo.
:noinfo

if NOT "%MS%" == "" goto ms

set MWCL_MS_COMPAT=

if NOT "%QUIET%" == "" goto final

echo Using Metrowerks options by default in "mwcc" and "mwld".

goto final

:ms

set MWCL_MS_COMPAT=on

if NOT "%QUIET%" == "" goto final
echo Using VC++ emulation options in "mwcc" and "mwld".
echo.
:final

if NOT "%QUIET%" == "" goto end
echo See %CWFolder%\Release Notes\CLT Notes\CLT_Usage_Notes.txt 
echo for more information on the command-line tools.
echo.
echo Run this batch file with "-quiet" as the first parameter
echo to turn off these helpful messages.
:end

rem Clear out temporary variables
set SPACE=
set QUIET=
set CROSS=
set MS=

rem Not sure why we sometimes have errors...
set ERRORLEVEL=0

rem Uncomment one of these lines if you're using the Cygwin tools
rem to invoke a shell with these variables available
rem cygnus.bat
rem bash
