#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>

#include "ivec2.h"
#include "vec2.h"

#define winName "windows"

#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_ARRAY_BUFFER 0x8892
#define GL_DYNAMIC_DRAW 0x88E8

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"opengl32.lib")

unsigned int VBO;

unsigned int shaderprogram;
unsigned int vertexshader;
unsigned int fragmentshader;

unsigned int (*glCreateProgram)();
unsigned int (*glCreateShader)(unsigned int shader);
unsigned int (*wglSwapIntervalEXT)(unsigned int satus);

void (*glShaderSource)(unsigned int shader,int count,const char **string,int *length);
void (*glCompileShader)(unsigned int shader);
void (*glAttachShader)(unsigned int program,unsigned int shader);
void (*glLinkProgram)(unsigned int program);
void (*glUseProgram)(unsigned int program);
void (*glEnableVertexAttribArray)(unsigned int index);
void (*glVertexAttribPointer)(unsigned int index,int size,unsigned int type,unsigned char normalized,unsigned int stride,const void *pointer);
void (*glBufferData)(unsigned int target,unsigned int size,const void *data,unsigned int usage);
void (*glCreateBuffers)(unsigned int n,unsigned int *buffers);
void (*glBindBuffer)(unsigned int target,unsigned int buffer);
void (*glGetShaderInfoLog)(unsigned int shader,unsigned int maxlength,unsigned int *length,unsigned char *infolog);

PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1,
PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
24,0, 0, 0, 0, 0, 0,0,0,0,
0,0,0,0,32,0,0,PFD_MAIN_PLANE,
0,0,0,0	};

char *VERTsource;
char *FRAGsource;

typedef struct{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}RGB;

typedef struct{
	VEC2 p1;
	VEC2 tc1;
	VEC2 p2;
	VEC2 tc2;
	VEC2 p3;
	VEC2 tc3;
	VEC2 p4;
	VEC2 tc4;
	VEC2 p5;
	VEC2 tc5;
	VEC2 p6;
	VEC2 tc6;
}QUAD;

unsigned int quadC;
QUAD *quad;

const IVEC2 reso  = {700,700};
const IVEC2 vreso = {700,700};

HWND window;
HDC dc;
MSG Msg;

BITMAPINFO bmi = {sizeof(BITMAPINFOHEADER),0,0,1,24,BI_RGB};

long _stdcall proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {
	return DefWindowProcA(hwnd,msg,wParam,lParam);
}	

WNDCLASS wndclass = {0,proc,0,0,0,0,0,0,winName,winName};

char *loadFile(const char *name){
	char *r;
	HANDLE h = CreateFileA(name,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	int fsize = GetFileSize(h,0);
	r = HeapAlloc(GetProcessHeap(),8,fsize+1);
	ReadFile(h,r,fsize+1,0,0);
	CloseHandle(h);
	return r;
}

void render(){
	FRAGsource = loadFile("fragment.frag");
	VERTsource = loadFile("vertex.vert");

	SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
	wglMakeCurrent(dc, wglCreateContext(dc));

	glCreateProgram		  = wglGetProcAddress("glCreateProgram");
	glCreateShader		  = wglGetProcAddress("glCreateShader");
	glShaderSource		  = wglGetProcAddress("glShaderSource");
	glCompileShader		  = wglGetProcAddress("glCompileShader");
	glAttachShader		  = wglGetProcAddress("glAttachShader");
	glLinkProgram		  = wglGetProcAddress("glLinkProgram");
	glUseProgram		  = wglGetProcAddress("glUseProgram");
	glEnableVertexAttribArray = wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer     = wglGetProcAddress("glVertexAttribPointer");
	glBufferData           	  = wglGetProcAddress("glBufferData");
	glCreateBuffers           = wglGetProcAddress("glCreateBuffers");
	glBindBuffer              = wglGetProcAddress("glBindBuffer");
	glGetShaderInfoLog        = wglGetProcAddress("glGetShaderInfoLog");
	wglSwapIntervalEXT        = wglGetProcAddress("wglSwapIntervalEXT");

	wglSwapIntervalEXT(1);

	shaderprogram   = glCreateProgram();
	vertexshader    = glCreateShader(GL_VERTEX_SHADER);
	fragmentshader  = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexshader,1,(const char**)&VERTsource,0);
	glShaderSource(fragmentshader,1,(const char**)&FRAGsource,0);
	glCompileShader(vertexshader);
	glCompileShader(fragmentshader);
	glAttachShader(shaderprogram,vertexshader);
	glAttachShader(shaderprogram,fragmentshader);
	glLinkProgram(shaderprogram);
	glUseProgram(shaderprogram);

	glCreateBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,0,4 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,0,4 * sizeof(float),(void*)(2 * sizeof(float)));

	for(;;){
		glClear(GL_COLOR_BUFFER_BIT);
		glBufferData(GL_ARRAY_BUFFER,24 * sizeof(float)*quadC,quad,GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES,0,24*quadC);
		SwapBuffers(dc);
		Sleep(15);
	}
}

void main(){
	quad = HeapAlloc(GetProcessHeap(),8,sizeof(QUAD)*256);
	ball = HeapAlloc(GetProcessHeap(),8,sizeof(BALL)*256);
	for(int i = 0;i < 256;i++){
		quad[i].tc1 = (VEC2){0.0f,0.0f};
		quad[i].tc2 = (VEC2){0.0f,1.0f};
		quad[i].tc3 = (VEC2){1.0f,0.0f};
		quad[i].tc4 = (VEC2){1.0f,1.0f};
		quad[i].tc5 = (VEC2){0.0f,1.0f};
		quad[i].tc6 = (VEC2){1.0f,0.0f};
	}
	timeBeginPeriod(1);
	bmi.bmiHeader.biHeight = vreso.x;
	bmi.bmiHeader.biWidth  = vreso.y;
	wndclass.hInstance = GetModuleHandleA(0);
	RegisterClassA(&wndclass);
	window = CreateWindowExA(0,winName,winName,0x10080000,0,0,reso.y+16,reso.x+39,0,0,wndclass.hInstance,0);
	dc = GetDC(window);
	CreateThread(0,0,render,0,0,0);
	for(;;){
		while(PeekMessageA(&Msg,window,0,0,0)){
			GetMessageA(&Msg,window,0,0);
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
		}
		Sleep(15);
	}
}
