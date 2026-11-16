#include <windows.h>

// abort for CPPEXP.DLL

void abort()
{
    MessageBox(NULL, "Unhandled CPP Exception", "CPP RTl", MB_OK);
    ExitProcess(0);
}