#include <windows.h>
#include <glew.h>
#include <GL/gl.h>

unsigned int shaderProgram;

unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int VBO;

char *VERTsource;
char *FRAGsource;

int *heap;
const char name[] = "window";
HWND window;
HDC dc;
MSG Msg;

long _stdcall proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

WNDCLASS wndclass = {0,proc,0,0,0,0,0,0,name,name};

PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1,
PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,
24,0, 0, 0, 0, 0, 0,0,0,0,
0,0,0,0,32,0,0,PFD_MAIN_PLANE,
0,0,0,0	};

float quad[6] = {-0.5,-0.5,0.5,-0.5,-0.5,0.5};

void main(){
	heap = HeapAlloc(GetProcessHeap(),0,30000);
	wndclass.hInstance = GetModuleHandle(0);
	RegisterClass(&wndclass);
	window = CreateWindowEx(0,name,name,0x10080000,0,0,500,500,0,0,wndclass.hInstance,0);
	dc = GetDC(window);

	HANDLE h = CreateFile("vertex.vs",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	int fsize = GetFileSize(h,0);
	VERTsource = HeapAlloc(GetProcessHeap(),8,fsize+1);
	ReadFile(h,VERTsource,fsize+1,0,0);
	CloseHandle(h);

	h = CreateFile("fragment.fs",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	fsize = GetFileSize(h,0);
	FRAGsource = HeapAlloc(GetProcessHeap(),8,fsize+1);
	ReadFile(h,FRAGsource,fsize+1,0,0);
	CloseHandle(h);

	SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd);
	wglMakeCurrent(dc, wglCreateContext(dc));

	glewInit();


	shaderProgram = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader,1,(const char**)&VERTsource,0);
	glShaderSource(fragmentShader,1,(const char**)&FRAGsource,0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	glCreateBuffers(1,&VBO);	
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,6 * sizeof(float),quad,GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,0,4 * sizeof(float),(void*)0);

	for(;;){
		glDrawArrays(GL_TRIANGLES,0,6);
		SwapBuffers(dc);
		GetMessage(&Msg,window,0,0);
		TranslateMessage(&Msg);
		DispatchMessageW(&Msg);
	}
}
