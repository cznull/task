#include "map.h"
#include "resource.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

const int pointcount = 32768;
HDC hdc1, hdcc, hdc2;
double ang1, ang2, len;
int mx, my;
int cx, cy;
int startt = 0;
GLuint vbo;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void draw(void) {
	glClear(0x00004100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(len * cos(ang1)*cos(ang2), len * sin(ang2), len * sin(ang1)*cos(ang2), 0, 0, 0, 0, cos(ang2), 0);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.0, 0.0, 0.0);
	glDrawArrays(GL_POINTS, 0, pointcount);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -0.0f);
	glVertex3f(-1.0f, 0.0f, -0.0f);
	glVertex3f(0.0f, 1.0f, -0.0f);
	glVertex3f(0.0f, -1.0f, -0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glEnd();
	SwapBuffers(wglGetCurrentDC());
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	MSG msg;
	HWND hWnd;
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDS_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow, &hWnd))
	{
		return FALSE;
	}
st:
	if (startt) {
		for (;;) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (startt) {
				if (cudacalc()) {
					MessageBoxA(hWnd, "evo failed", "message", MB_OK);
					break;
				}
				draw();
			}
			else {
				goto st;
			}
		}
	}
	else {
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); if (startt) {
				goto st;
			}
		}
	}
	return cudafin();
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND* hWnd)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	*hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, 1060, 1040, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(*hWnd, nCmdShow);
	UpdateWindow(*hWnd);

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId) {
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		draw();
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_CREATE: {
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
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0);
		((bool(_stdcall*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(0);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, pointcount * 12, 0, GL_DYNAMIC_DRAW);
		if (cudainit(hWnd, vbo)) {
			PostQuitMessage(0);
		}
		break;
	}
	case WM_SIZE: {
		cx = lParam & 0xffff;
		cy = (lParam & 0xffff0000) >> 16;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-(float)cx / (cx + cy) / 5, (float)cx / (cx + cy) / 5, -(float)cy / (cx + cy) / 5, (float)cy / (cx + cy) / 5, 0.08, 40.0);
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
