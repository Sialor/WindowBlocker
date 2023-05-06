#include <windows.h>
#include <strsafe.h>
#include <memory.h>
#include <tchar.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HOOKPROC hkprcSysMsg;
    HINSTANCE hinstDLL;
    HHOOK hhookSysMsg;
    MSG msg;
    BOOL bRet;

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"",                            // Window text
        WS_SYSMENU,                     // Window style

        // Size and position
        200, 200, 0, 0,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // ShowWindow(hwnd, nShowCmd);

#ifdef UNICODE
    hinstDLL = LoadLibraryW(TEXT("DLL.dll"));
#elif
    hinstDLL = LoadLibraryA(TEXT("DLL.dll"));
#endif

    if (hinstDLL == NULL)
    {
        int err = GetLastError();

        return 1;
    }

    hkprcSysMsg = (HOOKPROC)GetProcAddress(hinstDLL, "CBTProc");

    if (hkprcSysMsg == NULL)
    {
        int err = GetLastError();

        return 1;
    }

    // set procedure
    hhookSysMsg = SetWindowsHookEx(WH_CBT, hkprcSysMsg, hinstDLL, NULL); // may WH_SYSMSGFILTER
    
    if (hhookSysMsg == NULL)
    {
        int err = GetLastError();

        return 1;
    }

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            // handle the error and possibly exit
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UnhookWindowsHookEx(hhookSysMsg);

    {
        int err = GetLastError();

        return err;
    }
}