#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#define resX 1080
#define resY 1920

typedef struct VEC2{int x;int y;}VEC2;

typedef struct PLANET{
	char * text;
	int x;
	int y;
	int size;
}PLANET;

int planetCount;
PLANET * planets;

unsigned char texture[resX][resY][3];
PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1,
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,
	24,0, 0, 0, 0, 0, 0,0,0,0,
	0,0,0,0,32,0,0,PFD_MAIN_PLANE,
	0,0,0,0	};
HWND hwnd;
MSG Msg;
HDC wdcontext;

inline int notLessThanZero(int val){
	if(val > 0){
		return val;
	}
	return 0;
}

void drawPlanet(PLANET *p){
	for(int i = p->x;i < p->x + p->size;i++){
		for(int i2 = p->y;i2 < p->y + p->size;i2++){
			texture[i][i2][0] = p->text[0];
			texture[i][i2][1] = p->text[1];
			texture[i][i2][2] = p->text[2];
			p->text += 3;
		}
	}
	p->text -= p->size * p->size * 3;
}

void initPlanet(PLANET *p){
	p->text = malloc(p->size * p->size * 3);
	for(int i = 0;i < p->size;i++){
		for(int i2 = 0;i2 < p->size;i2++){
			VEC2 norm = {i - p->size / 2,i2 - p->size / 2};
			float dist = sqrtf(norm.x * norm.x + norm.y * norm.y);
			p->text[0] = notLessThanZero(dist - p->size/2.5) * rand() % 20 + 20;
			p->text[1] = notLessThanZero(dist - p->size/2.5) * rand() % 20 + 20;
			p->text[2] = notLessThanZero(dist - p->size/2.5) * rand() % 20 + 20;
			p->text += 3;
		}
	}
	p->text -= p->size * p->size * 3;
}

void WINAPI Quarter1(){
	SetPixelFormat(wdcontext, ChoosePixelFormat(wdcontext, &pfd), &pfd);
	wglMakeCurrent(wdcontext, wglCreateContext(wdcontext));
	glPixelZoom(1920 / resY,1080 / resX);
	for(;;){
		glDrawPixels(resY,resX,GL_RGB,GL_UNSIGNED_BYTE,&texture);
		SwapBuffers(wdcontext);  
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
	case WM_KEYDOWN:
		if(GetAsyncKeyState(VK_ESCAPE) < 0){
			exit(0);
		}
		break;
	case WM_CLOSE:
		exit(0);
	case WM_DESTROY:
		exit(0);
	default:	
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX),0,WndProc,0,0,hInstance,0,0,0,0,"class",0 };
	RegisterClassEx(&wc);
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "class", "raycasting", WS_VISIBLE | WS_POPUP | WS_EX_TOPMOST,
		0, 0, 1920, 1080, NULL, NULL, hInstance, NULL);
	wdcontext = GetDC(hwnd);
	ShowCursor(0);
	CreateThread(0,0,Quarter1,0,0,0);
	for(;;){
		while(PeekMessage(&Msg,hwnd,0,0,0)){
			GetMessage(&Msg, hwnd, 0, 0);
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
		}
		Sleep(3);
	}
	return Msg.wParam;
}
