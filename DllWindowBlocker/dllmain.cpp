// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"

//#pragma comment( lib, "user32.lib")
//#pragma comment( lib, "gdi32.lib")

#define SHMEMSIZE 8

static LPVOID lpvMem = NULL;
static HANDLE hMapObject = NULL;

HWND gh_hwndMain;

/****************************************************************
  WH_CBT hook procedure
 ****************************************************************/

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)  // do not process message 
        return CallNextHookEx(NULL, nCode, wParam,
            lParam);

    switch (nCode)
    {
    case HCBT_ACTIVATE:
        if ((HWND)wParam == gh_hwndMain)
        {
            return TRUE;
        }

    case HCBT_MINMAX:
        if ((HWND)wParam == gh_hwndMain)
        {
            return TRUE;
        }

        break;
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void hide_window(HWND window)
{
    ShowWindow(window, SW_MINIMIZE);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    BOOL fInit, fIgnore;

    switch (ul_reason_for_call)
    {
        // DLL load due to process initialization or LoadLibrary

    case DLL_PROCESS_ATTACH:

        // Create a named file mapping object

        hMapObject = CreateFileMapping(
            INVALID_HANDLE_VALUE,   // use paging file
            NULL,                   // default security attributes
            PAGE_READWRITE,         // read/write access
            0,                      // size: high 32-bits
            SHMEMSIZE,              // size: low 32-bits
            TEXT("dllmemfilemap")); // name of map object

        if (hMapObject == NULL)
            return FALSE;

        // The first process to attach initializes memory

        fInit = (GetLastError() != ERROR_ALREADY_EXISTS);

        // Get a pointer to the file-mapped shared memory

        lpvMem = MapViewOfFile(
            hMapObject,     // object to map view of
            FILE_MAP_WRITE, // read/write access
            0,              // high offset:  map from
            0,              // low offset:   beginning
            0);             // default: map entire file

        if (lpvMem == NULL)
            return FALSE;

        // Initialize memory if this is the first process
        if (fInit)
        {
            memset(lpvMem, '\0', SHMEMSIZE);

            *(HWND*)lpvMem = GetTarget();

            hide_window(*(HWND*)lpvMem);
        }
        
        gh_hwndMain = *(HWND*)lpvMem;

        break;

        // The attached process creates a new thread

    case DLL_THREAD_ATTACH:
        break;

        // The thread of the attached process terminates

    case DLL_THREAD_DETACH:
        break;

        // DLL unload due to process termination or FreeLibrary

    case DLL_PROCESS_DETACH:

        // Unmap shared memory from the process's address space

        fIgnore = UnmapViewOfFile(lpvMem);

        // Close the process's handle to the file-mapping object

        fIgnore = CloseHandle(hMapObject);

        break;

    default:
        break;
    }

    return TRUE;
}
