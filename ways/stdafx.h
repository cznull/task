// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <COmmdlg.h>
#include <atlimage.h>

struct int2 {
	int x, y;
};

struct map {
	int *mapd;
	int w, h, xb, yb, xe, ye;
};


// TODO: reference additional headers your program requires here
