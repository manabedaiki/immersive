#include "immersive.h"

#include <windows.h>

struct EnumChildWindowsParams
{
    DWORD processId;
    HWND hwnd;
};

static BOOL EnumChildWindowsCallback(HWND hwndChild, LPARAM lParam)
{
    struct EnumChildWindowsParams* const params = (struct EnumChildWindowsParams*)lParam;

    DWORD processId;
    GetWindowThreadProcessId(hwndChild, &processId);
    if (processId != params->processId)
    {
        return TRUE;
    }

    params->hwnd = hwndChild;
    return FALSE;
}

HWND GetWindowHandleFromProcessId(DWORD processId)
{
    HWND const clientHwnd = GetDesktopWindow();
    if (clientHwnd == INVALID_HWND)
    {
        return INVALID_HWND;
    }

    struct EnumChildWindowsParams params;
    params.processId = processId;
    params.hwnd = INVALID_HWND;
    EnumChildWindows(clientHwnd, EnumChildWindowsCallback, (LPARAM)&params);

    return params.hwnd;
}

DWORD Immersive(HWND hwnd)
{
    RECT workingArea;
    if (SystemParametersInfo(SPI_GETWORKAREA, 0, &workingArea, 0) == 0)
    {
        return GetLastError();
    }

    SetWindowLongPtr(hwnd, GWL_STYLE, 0);
    if (SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE) == 0)
    {
        DWORD const error = GetLastError();
        if (error != 0)
        {
            return error;
        }
    }

    if (SetWindowPos(
            hwnd,
            HWND_TOP,
            workingArea.left,
            workingArea.top,
            workingArea.right  - workingArea.left,
            workingArea.bottom - workingArea.top,
            SWP_NOZORDER | SWP_FRAMECHANGED) == 0)
    {
        return GetLastError();
    }

    return 0;
}
