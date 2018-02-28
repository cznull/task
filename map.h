#pragma once
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <gl/glew.h>
#include <math.h>
#include <stdio.h>

int cudainit(HWND, GLuint);
int cudacalc(void);
int cudafin(void);
