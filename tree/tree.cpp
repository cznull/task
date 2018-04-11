// tree.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "tree.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


HDC hdc1;
double ang1, ang2, len;
int mx, my;
int cx, cy;
float3 *point, *color, color1 = { 0.5,0.25,0.0 }, color2 = { 0.0,1.0,0.0 }, color3 = { 1.0,0.0,0.25 };
int end;
const float u[16] = { 0.0,1.0,1.0,0.0,0.0,1.0,1.0,0.0, 0.0,1.0,1.0,0.0, 0.0,1.0,1.0,0.0 };
const float r[16] = { 0.04,0.02,-0.02,-0.04,0.04,0.02,0.02,0.04,0.04,0.02,-0.02,-0.04,-0.04,-0.02,-0.02,-0.04 };
const float f[16] = { 0.04,0.02,0.02,0.04,-0.04,-0.02,0.02,0.04,-0.04,-0.02,-0.02,-0.04,0.04,0.02,-0.02,-0.04 };
const float u1[24] = { 1.0,1.5,1.5,1.0, 1.0,1.5,1.5,1.0, 1.0,1.5,1.5,1.0,  1.0,1.5,1.5,1.0 ,1.0,1.0,1.0,1.0,1.5,1.5,1.5,1.5 };
const float r1[24] = { 0.2,0.2,-0.2,-0.2,0.2,0.2,0.2,0.2,0.2,0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,0.2,0.2,-0.2,-0.2 ,0.2,0.2,-0.2,-0.2 };
const float f1[24] = { 0.2,0.2,0.2,0.2,-0.2,-0.2,0.2,0.2,-0.2,-0.2,-0.2,-0.2,0.2,0.2,-0.2,-0.2,-0.2,0.2,0.2,-0.2,0.2,-0.2,-0.2,0.2 };
const int levelcount = 7;

float3 operator+(float3 a, float3 b) {
	float3 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}

float3 operator*(float3 a, float b) {
	float3 c;
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
	return c;
}

inline void mul(float3 a, float3 b, float3 *c) {
	c->x = a.y*b.z - a.z*b.y;
	c->y = a.z*b.x - a.x*b.z;
	c->z = a.x*b.y - a.y*b.x;
}

int getsize3(int a) {
	int size, i, sinl;
	sinl = 1;
	size = 0;
	for (i = 0; i <= a; i++) {
		size += sinl * 16;
		sinl *= 4;
	}
	size += sinl * 24;
	return size;
}

void draw(void) {
	int i, j, k;
	glClear(0x00004100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(len * cos(ang1)*cos(ang2), len * sin(ang2), len * sin(ang1)*cos(ang2), 0, 0, 0, 0, cos(ang2), 0);
/*	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -0.0f);
	glVertex3f(-1.0f, 0.0f, -0.0f);
	glVertex3f(0.0f, 1.0f, -0.0f);
	glVertex3f(0.0f, -1.0f, -0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();*/
	glVertexPointer(3, GL_FLOAT, 0, point);
	glColorPointer(3, GL_FLOAT, 0, color);
	glDrawArrays(GL_QUADS, 0, getsize3(levelcount));
	SwapBuffers(wglGetCurrentDC());
}

void addp3(int level, float size, float3 st, float3 up, float3 right) {
	float3 front;
	int i;
	mul(right, up, &front);
	for (i = 0; i < 16; i++) {
		point[end + i] = st + (up*u[i]+right*r[i]+front*f[i])*size;
	}
	for (i = 0; i < 4; i++) {
		color[end + i * 4 + 0] = color1*((level + 1.0) / (levelcount + 1.0)) + color2*((levelcount - level) / 7.0);
		color[end + i * 4 + 1] = color1*((level + 0.0) / (levelcount + 1.0)) + color2*((levelcount + 1.0 - level) / 7.0);
		color[end + i * 4 + 2] = color1*((level + 0.0) / (levelcount + 1.0)) + color2*((levelcount + 1.0 - level) / 7.0);
		color[end + i * 4 + 3] = color1*((level + 1.0) / (levelcount + 1.0)) + color2*((levelcount - level) / 7.0);
	}
	end += 16;
	if (level) {
		float3 st1, up1, right1;
		float a1, a2, size1;
		st1 = st + up*size;
		for (int i = 0; i < 4; i++) {
			a1 = rand() / 32768.0f - 0.5f;
			a2 = rand() / 32768.0f - 0.5f;
			size1 = size*((rand() / 32768.0f - 0.5f)*0.15f + 0.5f);
			up1 = up*cos(a1) + front*sin(a1);
			right1 = right*cos(a2) + up1*sin(-a2);
			up1 = up1*cos(a2) + right*sin(a2);
			addp3(level - 1, size1, st1, up1, right1);
		}
	}
	else {
		for (i = 0; i < 24; i++) {
			point[end + i] = st + (up*u1[i] + right*r1[i] + front*f1[i])*size;
			color[end + i] = color3;
		}
		end += 24;
	}
}

void addp(int level, float size, float3 st, float3 up, float3 right) {
	point[end + 0] = st + right*0.04*size;
	point[end + 1] = st + (right*0.02 + up)*size;
	point[end + 2] = st + (right*-0.02 + up)*size;
	point[end + 3] = st + right*-0.04*size;
	color[end + 0] = color1*((level + 1.0) / 7.0) + color2*((6.0 - level) / 7.0);
	color[end + 1] = color1*((level + 0.0) / 7.0) + color2*((7.0 - level) / 7.0);
	color[end + 2] = color1*((level + 0.0) / 7.0) + color2*((7.0 - level) / 7.0);
	color[end + 3] = color1*((level + 1.0) / 7.0) + color2*((6.0 - level) / 7.0);
	end += 4;
	if (level) {
		float3 st1, up1, right1;
		float a,size1;
		st1 = st + up*size;
		for (int i = 0; i < 4; i++) {
			a = rand() / 32768.0f - 0.5f;
			size1 = size*((rand() / 32768.0f - 0.5f)*0.15f + 0.5f);
			up1 = up*cos(a) + right*sin(a);
			right1 = right*cos(a) + up*sin(-a);
			addp(level - 1, size1, st1, up1, right1);
		}
	}
	else {
		point[end + 0] = st + (right*0.25 + up)*size;
		point[end + 1] = st + (right*0.25 + up*1.5f)*size;
		point[end + 2] = st + (right*-0.25 + up*1.5f)*size;
		point[end + 3] = st + (right*-0.25 + up)*size;
		color[end + 0] = color3;
		color[end + 1] = color3;
		color[end + 2] = color3;
		color[end + 3] = color3;
		end += 4;
	}
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TREE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TREE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TREE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TREE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			draw();
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CREATE: {
		int i, j;
		SetTimer(hWnd, 1, 10, NULL);
		hdc1 = GetDC(hWnd);
		len = 2.0;
		ang1 = 1.57;
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,0,0,
			0,
			0,0,0,0,
			32,
			0,0,
			PFD_MAIN_PLANE,
			0,0,0,0
		};
		int uds = ::ChoosePixelFormat(hdc1, &pfd);
		::SetPixelFormat(hdc1, uds, &pfd);
		HGLRC m_hrc = ::wglCreateContext(hdc1);
		::wglMakeCurrent(hdc1, m_hrc);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		point = (float3*)malloc(getsize3(levelcount) * sizeof(float3));
		color = (float3*)malloc(getsize3(levelcount) * sizeof(float3));
		end = 0;
		srand(GetTickCount());
		addp3(levelcount, 1.0f, { 0.0f,-1.0f,0.0f }, { 0.0f,1.0f,0.0f }, { 1.0f,0.0f,0.0f });
		break;
	}
	case WM_SIZE: {
		cx = lParam & 0xffff;
		cy = (lParam & 0xffff0000) >> 16;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-(float)cx / (cx + cy) / 40, (float)cx / (cx + cy) / 40, -(float)cy / (cx + cy) / 40, (float)cy / (cx + cy) / 40, 0.01, 400.0);
		glViewport(0, 0, cx, cy);
		break;
	}
	case WM_MOUSEMOVE: {
		int x, y;
		x = (lParam & 0xffff);
		y = ((lParam & 0xffff0000) >> 16);
		if (MK_LBUTTON&wParam) {
			ang1 += (x - mx)*0.002;
			ang2 += (y - my)*0.002;
			draw();
		}
		mx = x;
		my = y;
		break;
	}
	case WM_KEYDOWN: {
		switch (wParam) {
		case ' ': {
			end = 0;
			srand(GetTickCount());
			addp3(levelcount, 1.0f, { 0.0f,-1.0f,0.0f }, { 0.0f,1.0f,0.0f }, { 1.0f,0.0f,0.0f });
			draw();
			break;
		}
		case 'W': {
			len /= 1.01;
			draw();
			break;
		}
		case 'S': {
			len *= 1.01;
			draw();
			break;
		}
		}
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
