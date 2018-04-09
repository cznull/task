// galaxy.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "galaxy.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
double source[256];
double4 r1[SIZE], r2[SIZE], v1[SIZE], v2[SIZE];
double m[SIZE];
float3 *re;
unsigned int *in;
int reav, reus, vbous;
HDC hdc1, hdcc, hdc2;
double ang1, ang2, len;
int mx, my;
int cx, cy;
int startt;
GLuint vbo[128], ibo[SIZE];
const double dt = 3.83495197e-4;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void draw(void) {
	int i, j, k;
	glClear(0x00004100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(len * cos(ang1)*cos(ang2), len * sin(ang2), len * sin(ang1)*cos(ang2), 0, 0, 0, 0, cos(ang2), 0);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -0.0f);
	glVertex3f(-1.0f, 0.0f, -0.0f);
	glVertex3f(0.0f, 1.0f, -0.0f);
	glVertex3f(0.0f, -1.0f, -0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();
	for (j = 0; j < vbous; j++) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[j]);
		for (i = 0; i < SIZE; i++) {
			glColor3f(sin(i * 23)*0.5 + 0.5, sin(i * 13)*0.5 + 0.5, sin(i * 34)*0.5 + 0.5);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glDrawElements(GL_LINE_STRIP, 16385, GL_UNSIGNED_INT, NULL);
		}
	}
	if (((reus - 1) / 9) % 16384) {
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[((reus - 1) / 9) / 16384]);
		for (i = 0; i < SIZE; i++) {
			glColor3f(sin(i * 23)*0.5 + 0.5, sin(i * 13)*0.5 + 0.5, sin(i * 34)*0.5 + 0.5);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glDrawElements(GL_LINE_STRIP, (reus / 9 - 1) % 16384 + 1, GL_UNSIGNED_INT, NULL);
		}
	}
	SwapBuffers(wglGetCurrentDC());
}

inline int evo(double4 *rf, double4 *rt, double4 *vf, double4 *vt, double *m, double t) {
	int i, j;
	double l;
	__m256d a[9], r, r2, f1, f2;
	for (i = 0; i < SIZE; i++) {
		a[i] = _mm256_set1_pd(0);
	}
	for (i = 0; i < SIZE-1; i++) {
		for (j = i + 1; j < SIZE; j++) {
			r = _mm256_sub_pd(_mm256_load_pd(&rf[j].x), _mm256_load_pd(&rf[i].x));
			r2 = _mm256_mul_pd(r, r);
			l = r2.m256d_f64[0] + r2.m256d_f64[1] + r2.m256d_f64[2];
			l = 1.0 / sqrt(l);
			l = l*l*l;
			f1 = _mm256_mul_pd(r, _mm256_set1_pd(m[j] * l));
			f2 = _mm256_mul_pd(r, _mm256_set1_pd(m[i] * l));
			a[i] = _mm256_add_pd(a[i], f1);
			a[j] = _mm256_sub_pd(a[j], f2);
		}
	}
	for (i = 0; i < SIZE; i++) {
		r = _mm256_add_pd(_mm256_load_pd(&vf[i].x), _mm256_mul_pd(a[i], _mm256_set1_pd(t)));
		_mm256_store_pd(&vt[i].x, r);
		_mm256_store_pd(&rt[i].x, _mm256_add_pd(_mm256_load_pd(&rf[i].x), _mm256_mul_pd(r, _mm256_set1_pd(t))));
	}
	return 0;
}

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
    LoadStringW(hInstance, IDC_GALAXY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GALAXY));
	unsigned int t, t1, count, f;
	char s[32];
	t = GetTickCount();
	count = 0;
	f = 0;
    MSG msg;
st:
	if (startt) {
		for (;;) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else if (startt) {
				int i, j;
				for (j = 0; j < 128; j++) {
					if (reus < reav) {
						for (i = 0; i < 16; i++) {
							evo(r1, r2, v1, v2, m, dt);
							evo(r2, r1, v2, v1, m, dt);
						}
						for (i = 0; i < SIZE; i++) {
							re[reus + i].x = r2[i].x;
							re[reus + i].y = r2[i].y;
							re[reus + i].z = r2[i].z;
						}
						reus += 9;
						for (i = 0; i < 16; i++) {
							evo(r1, r2, v1, v2, m, dt);
							evo(r2, r1, v2, v1, m, dt);
						}
						for (i = 0; i < SIZE; i++) {
							re[reus + i].x = r2[i].x;
							re[reus + i].y = r2[i].y;
							re[reus + i].z = r2[i].z;
						}
						reus += 9;
						/*evo(r2, r1, v2, v1, m, 0.0001);
						for (i = 0; i < SIZE; i++) {
							re[reus + i].x = r1[i].x;
							re[reus + i].y = r1[i].y;
							re[reus + i].z = r1[i].z;
						}
						reus += 9;*/
					}
					else {
						draw();
						startt = 0;
						goto st;
					}
				}
				while (vbous < (reus - 9) / 9 / 16384) {
					glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[vbous]);
					glBufferDataARB(GL_ARRAY_BUFFER_ARB, 9 * 12 * 16385, re + vbous * 9 * 16384, GL_STATIC_DRAW_ARB);
					vbous++;
				}
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[(reus - 18) / 9 / 16384]);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, 9 * 12 * 16385, re + ((reus - 18) / 9 / 16384) * 9 * 16384, GL_STATIC_DRAW_ARB);
				draw();
				count += 1;
				t1 = GetTickCount();
				if (t1 - t >= 1000) {
					sprintf_s(s, "%.2ffps", count*1000.0 / (t1 - t));
					TextOutA(hdc2, 0, 0, s, strlen(s));
					t = t1;
					count = 0;
				}
			}
			else {
				goto st;
			}
		}
	}
	else {
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (startt) {
				goto st;
			}
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GALAXY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GALAXY);
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
			case ID_FILE_OPEN: {
				TCHAR szBuffer[MAX_PATH] = { 0 };
				OPENFILENAMEW ofn = { 0 };
				FILE *fi;
				int i, j, k, n, b;
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = NULL;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrFile = szBuffer;
				ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
				ofn.nFilterIndex = 0;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
				if (GetOpenFileName(&ofn)) {
					if (!_wfopen_s(&fi, szBuffer, _T("r"))) {
						for (i = 0; i < 9; i++)
							fscanf_s(fi, "%lf%lf%lf%lf%lf%lf%lf", &r1[i].x, &r1[i].y, &r1[i].z, &v1[i].x, &v1[i].y, &v1[i].z, m + i);
						fclose(fi);
					}
				}
				break;
			}
			case ID_FILE_SAVE: {
				TCHAR szBuffer[MAX_PATH] = { 0 };
				OPENFILENAMEW ofn = { 0 };
				FILE *fi;
				int i, j, k, n, b;
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFilter = NULL;
				ofn.lpstrInitialDir = NULL;
				ofn.lpstrFile = szBuffer;
				ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
				ofn.nFilterIndex = 0;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
				if (GetSaveFileName(&ofn)) {
					if (!_wfopen_s(&fi, szBuffer, _T("w"))) {
						for (i = 0; i < reus; i++) {
							fprintf(fi, "%f,%f,%f%c", re[i].x, re[i].y, re[i].z,i % 9 == 8?'\n':',');
						}
						fclose(fi);
					}
				}
				break;
			}
			case ID_FILE_RESTART: {
				reus = 0;
				vbous = 0;
				break;
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			draw();
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CREATE: {
		int i, j;
		SetTimer(hWnd, 1, 10, NULL);
		hdc1 = GetDC(hWnd);
		hdc2 = GetDC(NULL);
		len = 1.0;
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
		glewInit();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		glEnableClientState(GL_VERTEX_ARRAY);
		in = (unsigned int*)malloc(9 * 16385 * sizeof(unsigned int));
		for (i = 0; i < 16385; i++) {
			for (j = 0; j < SIZE; j++) {
				in[i + j * 16385] = i * 9 + j;
			}
		}
		glGenBuffers(128, vbo);
		glGenBuffers(SIZE, ibo);
		for (i = 0; i < SIZE; i++) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 16385 * sizeof(unsigned int), in + 16385 * i, GL_STATIC_DRAW_ARB);
		}
		((bool(_stdcall*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(0);
		reav = 18 * 1048576;
		reus = 0;
		re = (float3*)malloc((reav + 9) * sizeof(float3));
		startt = 0;
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
			startt = !startt;
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
