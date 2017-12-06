#pragma once

#include "resource.h"
#include "windowsx.h"
#include <vector>
#include <string>
#include <cmath>
//using StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// listview & treeview
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
using namespace std;

#define MAX_LOADSTRING 100


BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hWnd);
void OnDestroy(HWND hWnd);
void OnClose(HWND hwnd);
