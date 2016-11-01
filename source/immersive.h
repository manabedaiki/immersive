#pragma once

#ifndef INCLUDED_IMMERSIVE_H
#define INCLUDED_IMMERSIVE_H

#include <windows.h>

#define INVALID_HWND ((HWND)-1)

HWND GetWindowHandleFromProcessId(DWORD processId);
DWORD Immersive(HWND hwnd);

#endif
