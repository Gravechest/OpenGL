#include <windows.h>
#include <thread>
#include "glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "global.h"
LRESULT CALLBACK WndProc(HWND app, UINT msg, WPARAM wparam, LPARAM lparam);
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE previnstance,
	LPTSTR cmdline,
	int cmdshow
	
) {	
	PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR), 1,                     
	PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL |PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,        
	24,0, 0, 0, 0, 0, 0,0,0,0,           
	0,0,0,0,32,0,0,PFD_MAIN_PLANE,        
	0,0,0,0               
	};
	MSG msg;
	WNDCLASS window = {};
	window.lpfnWndProc = WndProc;
	window.lpszClassName = L"yeetboi2";
	window.hInstance = hInstance;
	RegisterClass(&window);
	HWND app = CreateWindowEx(0, L"yeetboi2", L"yeetboi", WS_OVERLAPPEDWINDOW, 0, 0, 1000, 1000, NULL, NULL, hInstance, NULL);
	ShowWindow(app, cmdshow);
	HDC pixel = GetDC(app);
	SetPixelFormat(pixel, ChoosePixelFormat(pixel,&pfd), &pfd);
	wglMakeCurrent(pixel, wglCreateContext(pixel));
	glewInit();
	unsigned int shaderprogram = init_game(app);
	while (true) {
		if (PeekMessage(&msg, app, 0, 0, 0) != 0) {
			GetMessage(&msg, app, 0, 0);
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		the_game(pixel,shaderprogram,app);
		SwapBuffers(pixel);
		if (PeekMessage(&msg, app, 0, 0, 0) != 0) {
			GetMessage(&msg, app, 0, 0);
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}
LRESULT CALLBACK WndProc(HWND app, UINT msg, WPARAM wparam, LPARAM lparam) {
	return DefWindowProc(app, msg, wparam, lparam);
}
