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
#include <gl\glew.h>
#include <math.h>
#include <COmmdlg.h>
#include <stdio.h>
#include <immintrin.h>

#define SIZE 9

struct double3 {
	double x, y, z;
};
struct double4 {
	double x, y, z, w;
};
struct float3 {
	float x, y, z;
};

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
// TODO: reference additional headers your program requires here
