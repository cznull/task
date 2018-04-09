#pragma once
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <gl/glew.h>
#include <math.h>
#include <stdio.h>

int cudainit(HWND);
int cudacalc(float, float, float*);
int cudafin(void);
void draw(void);

const int arraysize=64;
extern unsigned char *data;
