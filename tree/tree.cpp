// ways.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ways.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
map m1;
int r;

HDC hdc1;
int mx, my, imx, imy;
int cx, cy;
GLuint texbuffer;
unsigned char *img = NULL;

void draw(void) {
	glClear(0x00004100);
	//	glBindTexture(GL_TEXTURE_2D, texbuffer);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	if (cx*m1.h > cy*m1.w) {
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-float(cy)*m1.w / cx / m1.h, -1.0f, 0.0f);
		glTexCoord2f(float(m1.w) / imx, 0.0f);
		glVertex3f(float(cy)*m1.w / cx / m1.h, -1.0f, 0.0f);
		glTexCoord2f(float(m1.w) / imx, float(m1.h) / imy);
		glVertex3f(float(cy)*m1.w / cx / m1.h, 1.0f, 0.0f);
		glTexCoord2f(0.0f, float(m1.h) / imy);
		glVertex3f(-float(cy)*m1.w / cx / m1.h, 1.0f, 0.0f);
		glEnd();
	}
	else {
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -float(cx)*m1.h / cy / m1.w, 0.0f);
		glTexCoord2f(float(m1.w) / imx, 0.0f);
		glVertex3f(1.0f, -float(cx)*m1.h / cy / m1.w, 0.0f);
		glTexCoord2f(float(m1.w) / imx, float(m1.h) / imy);
		glVertex3f(1.0f, float(cx)*m1.h / cy / m1.w, 0.0f);
		glTexCoord2f(0.0f, float(m1.h) / imy);
		glVertex3f(-1.0f, float(cx)*m1.h / cy / m1.w, 0.0f);
		glEnd();
	}
	SwapBuffers(hdc1);
}

inline int frand(void) {
	r = (r * 23433 + 23) & 65535;
	return r;
}

int g1(map* m, int d, int count) {
	int i, j, k, l, xc, yc, xc1, yc1, c;
	int mx[8] = { 1,0,-1,0 ,1,-1,-1,1 }, my[8] = { 0,1,0,-1 ,1,1,-1,-1 };
	for (i = 0; i < m->h; i++) {
		for (j = 0; j < m->w; j++) {
			if (m->mapd[j + i*m->w] > d) {
				for (k = 0; k < 4; k++) {
					xc = j + mx[k];
					yc = i + my[k];
					if (xc > 0 && yc > 0 && xc < m->w - 1 && yc < m->h - 1 && m->mapd[xc + yc*m->w] == -1) {
						c = 0;
						for (l = 0; l < 8; l++) {
							xc1 = xc + mx[l];
							yc1 = yc + my[l];
							if (m->mapd[xc1 + yc1*m->w] == -1)
								c++;
						}
						if (c > 5)
							return 1;
					}
				}
			}
		}
	}
	return 0;
}

void g(map* m, int d, int count) {
	int i, j, k, l, xc, yc, xc1, yc1, c;
	int mx[8] = { 1,0,-1,0 ,1,-1,-1,1 }, my[8] = { 0,1,0,-1 ,1,1,-1,-1 };
	for (i = 0; i < m->h; i++) {
		for (j = 0; j < m->w; j++) {
			if (m->mapd[j + i*m->w] > d) {
				for (k = 0; k < 4; k++) {
					xc = j + mx[k];
					yc = i + my[k];
					if (xc > 0 && yc > 0 && xc < m->w - 1 && yc < m->h - 1 && frand() < 10000 && m->mapd[xc + yc*m->w] == -1) {
						c = 0;
						for (l = 0; l < 8; l++) {
							xc1 = xc + mx[l];
							yc1 = yc + my[l];
							if (m->mapd[xc1 + yc1*m->w] == -1)
								c++;
						}
						if (c > 5)
							m->mapd[xc + yc*m->w] = 0;
					}
				}
			}
			if (m->mapd[j + i*m->w] > 0) {
				m->mapd[j + i*m->w] = 0;
			}
		}
	}
}

int ways1(map* m) {
	int to, i, pc, pe,n;
	int *seq,top;
	const int shi[4] = { 1,m->w,-1,-m->w };
	pc = m->xb + m->w*m->yb;
	seq = (int*)malloc(m->w*m->h * sizeof(int));
	top = 0;
	to = 0;
	n = -1;
	pe = m->xe + m->w*m->ye;
	for (;;) {
		for (i = n + 1; i < 4; i++) {
			if (!m->mapd[pc + shi[i]]) {
				pc += shi[i];
				m->mapd[pc] = 1;
				seq[top] = i;
				top++;
				if (pc == pe)
					goto next;
				i = -1;
			}
		}
		if (top) {
			top--;
			n = seq[top];
			pc -= shi[n];
		}
		else {
			free(seq);
			return 1;
		}
	}
	next:
	for (i = top - 1; i > -1; i--) {
		m->mapd[pc] = 8;
		pc -= shi[seq[i]];
	}
	m->mapd[pc] = 8;
	free(seq);
	return 0;
}

int ways(map* m) {
	int xc, yc, to;
	xc = m->xb;
	yc = m->yb;
	to = 0;
	while (xc != m->xe || yc != m->ye) {
		switch (to) {
		case 0:
			if (!m->mapd[xc + 1 + yc*m->w]) {
				xc++;
				m->mapd[xc + yc*m->w] = 1;
				to = 0;
				continue;
			}
		case 1:
			if (!m->mapd[xc + (yc + 1)*m->w]) {
				yc++;
				m->mapd[xc + yc*m->w] = 2;
				to = 0;
				continue;
			}
		case 2:
			if (!m->mapd[xc - 1 + yc*m->w]) {
				xc--;
				m->mapd[xc + yc*m->w] = 3;
				to = 0;
				continue;
			}
		case 3:
			if (!m->mapd[xc + (yc - 1)*m->w]) {
				yc--;
				m->mapd[xc + yc*m->w] = 4;
				to = 0;
				continue;
			}
		}
		to = m->mapd[xc + yc*m->w];
		switch (to) {
		case 1:
			xc--;
			break;
		case 2:
			yc--;
			break;
		case 3:
			xc++;
			break;
		case 4:
			yc++;
			break;
		case -1:
			return 1;
		}
	}
	int pc = m->xe + m->w*m->ye;
	const int shi[4] = { 1,m->w,-1,-m->w };
	while (m->mapd[pc] > 0) {
		int i = pc;
		pc -= shi[m->mapd[pc] - 1];
		m->mapd[i] = 8;
	}
	m->mapd[pc] = 8;
	return 0;
}


int sways(map* m) {
	int *seq, cur, end, i, pc;
	const int shi[4] = { 1,m->w,-1,-m->w };
	seq = (int*)malloc(m->w*m->h * sizeof(int));
	cur = 0;
	end = 1;
	seq[0] = m->xb + m->yb*m->w;
	while (cur < end) {
		pc = seq[cur];
		for (i = 0; i < 4; i++) {
			if (!m->mapd[pc + shi[i]]) {
				m->mapd[pc + shi[i]] = i + 5 + ((m->mapd[pc] - 1) & 0xfffffffc);
			//	m->mapd[pc + shi[i]] = i + 1 + (end << 4);
				seq[end] = pc + shi[i];
				end++;
			}
		}
		cur++;
	}
	free(seq);
	pc = m->xe + m->w*m->ye;
	if (m->mapd[pc]<1)
		return end;
	while (m->mapd[pc] > 0) {
		i = pc;
		pc -= shi[(m->mapd[pc] - 1) & 3];
		m->mapd[i] = 8;
	}
	m->mapd[pc] = 8;
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
	LoadStringW(hInstance, IDC_WAYS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WAYS));

	MSG msg;
	map m1;
	int i;
	FILE *fi;
	/*fopen_s(&fi, "d:/in.txt", "r");
	for (i = 0; i < 49; i++) {
		fscanf_s(fi, "%d", m1.mapd + i);
	}
	fclose(fi);
	srand(2345);
	for (i = 0; i < m1.w*m1.h; i++) {
		m1.mapd[i] = rand() < 7000 ? -1 : 0;
	}
	for (i = 0; i < m1.w; i++) {
		m1.mapd[i] = -1;
		m1.mapd[i + (m1.h - 1)*(m1.w)] = -1;
	}
	for (i = 0; i < m1.h; i++) {
		m1.mapd[i*m1.w] = -1;
		m1.mapd[i*m1.w + m1.h - 1] = -1;
	}
	m1.mapd[m1.w + 1] = -1;
	m1.mapd[m1.w*(m1.h - 2) + m1.w - 2] = 0;
	ways1(&m1);
	fopen_s(&fi, "d:/out.txt", "w");
	char *s;
	s = (char*)malloc(m1.w * m1.h * 2);
	for (i = 0; i < m1.w*m1.h; i++) {
		s[i+i] = m1.mapd[i] == 8 ? '8' : '1';
		s[i + i + 1] = (i % m1.w == m1.w - 1 ? '\n' : ' ');
	}
	fwrite(s, 1, m1.w * m1.h * 2, fi);
	free(s);
	fclose(fi);*/
	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WAYS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WAYS);
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
		case ID_FILE_RAND: {
			break;
		}
		case ID_FILE_OPEN: {
			TCHAR szBuffer[MAX_PATH] = { 0 };
			OPENFILENAMEW ofn = { 0 };
			FILE *fi;
			CImage im;
			int pitch, bitcount;
			unsigned char *imb;
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
				i = im.Load(szBuffer);
				if (i) {
					break;
				}
				if (im.GetPitch() < 0) {
					imb = (unsigned char*)im.GetBits() + (im.GetPitch()*(im.GetHeight() - 1));
				}
				else {
					imb = (unsigned char*)im.GetBits();
				}
				pitch = abs(im.GetPitch());
				bitcount = im.GetBPP() / 8;
				m1.w = im.GetWidth();
				m1.h = im.GetHeight();
				free(m1.mapd);
				m1.mapd = (int*)malloc(m1.w * m1.h * sizeof(int));
				for (i = 0; i < m1.h; i++) {
					for (j = 0; j < m1.w; j++) {
						m1.mapd[i*m1.w + j] = imb[i*pitch + j*bitcount] > 0 ? 0 : -1;
					}
				}
				m1.xb = 1;
				m1.yb = 1;
				m1.xe = m1.w - 2;
				m1.ye = m1.h - 2;
				for (i = 0; i < m1.w; i++) {
					m1.mapd[i] = -1;
					m1.mapd[i + (m1.h - 1)*(m1.w)] = -1;
				}
				for (i = 0; i < m1.h; i++) {
					m1.mapd[i*m1.w] = -1;
					m1.mapd[i*m1.w + m1.w - 1] = -1;
				}
				m1.mapd[m1.yb*m1.w + m1.xb] = -1;
				r = GetTickCount();
				/*i = sways(&m1);
				j = 0;
				while (i != j || g1(&m1, i / 2 * 4, 0)) {
					j = i;
					g(&m1, i / 2 * 4, 0);
					m1.mapd[1 + m1.w] = -1;
					i = sways(&m1);
				}*/
				if (!sways(&m1)) {
					for (i = 0; i < m1.h; i++) {
						for (j = 0; j < m1.w; j++) {
							if (m1.mapd[i*m1.w + j] == 8)
								imb[i*pitch + j*bitcount + 1] = 255;
							else {
								imb[i*pitch + j*bitcount + 1] /= 2;
							}
							imb[i*pitch + j*bitcount + 2] = m1.mapd[i*m1.w + j] / 4 * 16;
							if (m1.mapd[i*m1.w + j] > 0)
								imb[i*pitch + j*bitcount] = 255;
							else {
								imb[i*pitch + j*bitcount] /= 2;
							}
						}
					}
					im.Save(szBuffer);
				}
				imx = 1;
				while (imx < 0x40000000 && imx < m1.w) {
					imx += imx;
				}
				imy = 1;
				while (imy < 0x40000000 && imy < m1.w) {
					imy += imy;
				}
				img = (unsigned char*)malloc(imx*imy * 3 * sizeof(unsigned char));
				for (i = 0; i < m1.h; i++) {
					for (j = 0; j < m1.w; j++) {
						img[(i*imx + j) * 3 + 0] = imb[i*pitch + j*bitcount + 2];
						img[(i*imx + j) * 3 + 1] = imb[i*pitch + j*bitcount + 1];
						img[(i*imx + j) * 3 + 2] = imb[i*pitch + j*bitcount + 0];
					}
				}
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imx, imy, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
				free(img);
				draw();
			}
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
		SetTimer(hWnd, 1, 10, NULL);
		hdc1 = GetDC(hWnd);
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

		m1.w = 1024;
		m1.h = 1024;
		m1.mapd = (int*)malloc(m1.w * m1.h * sizeof(int));
		m1.xb = 1;
		m1.yb = 1;
		m1.xe = m1.w - 2;
		m1.ye = m1.h - 2;
		imx = 1024;
		imx = 1024;

		int uds = ::ChoosePixelFormat(hdc1, &pfd);
		::SetPixelFormat(hdc1, uds, &pfd);
		HGLRC m_hrc = ::wglCreateContext(hdc1);
		::wglMakeCurrent(hdc1, m_hrc);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		((bool(_stdcall*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(0);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glGenTextures(1, &texbuffer);
		glBindTexture(GL_TEXTURE_2D, texbuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	}
	case WM_SIZE: {
		cx = lParam & 0xffff;
		cy = (lParam & 0xffff0000) >> 16;
		/*if (cx <= cy) {
			glViewport(0, (cy - cx) / 2, cx, cx);
		}
		else {
			glViewport((cx - cy) / 2, 0, cy, cy);
		}*/
		glViewport(0, 0, cx, cy);
		break;
	}
	case WM_MOUSEMOVE: {
		int x, y;
		x = (lParam & 0xffff);
		y = ((lParam & 0xffff0000) >> 16);
		if (MK_LBUTTON&wParam) {
			;
		}
		mx = x;
		my = y;
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
