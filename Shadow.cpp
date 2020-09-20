#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Shadows"

int questionnumber = 1;
boolean sphere = true;
boolean pyramid = false;
double rotateY = 0.2;
double radius = 1;
int slices = 20;
int stacks = 20;
float posX = 0.0, posY = 1.0, posZ = 5.0;
double baseRadius = 0.01;
double topRadius = 0.3;
double height = 0.8;

float lightX = 0, lightX2 = 2;
float lightY = 5, lightY2 = 5;
float lightZ = 0, lightZ2 = 0;

GLUquadricObj* myQuadricObject = gluNewQuadric();


LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			PostQuitMessage(0);
			break;
		}
		case 'K':
		{
			posX -= 0.1;
			break;
		}
		case 'L':
		{
			posX += 0.1;
			break;
		}
		case 'I':
		{
			posZ -= 0.1;
			break;
		}
		case 'O':
		{
			posZ += 0.1;
			break;
		}
		case 'W':
		{
			lightY += 0.1;
			break;
		}
		case 'S':
		{
			lightY -= 0.1;
			break;
		}
		case 'A':
		{
			lightX -= 0.1;
			break;
		}
		case 'D':
		{
			lightX += 0.1;
			break;
		}
		case 'T':
		{
			lightY2 += 0.1;
			break;
		}
		case 'G':
		{
			lightY2 -= 0.1;
			break;
		}
		case 'F':
		{
			lightX2 -= 0.1;
			break;
		}
		case 'H':
		{
			lightX2 += 0.1;
			break;
		}
		case 'Y':
		{
			lightZ2 += 0.1;
			break;
		}
		case 'R':
		{
			lightZ2 -= 0.1;
			break;
		}
		case 'E':
		{
			lightZ += 0.1;
			break;
		}
		case 'Q':
		{
			lightZ -= 0.1;
			break;
		}
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	glMatrixMode(GL_MODELVIEW);
	GLfloat lightdiffusecolour[] = { 1,1,1,1 };
	GLfloat secondlightdiffusecolour[] = { 1,1,1,1 };
	GLfloat lightposition[] = { lightX, lightY, lightZ, 0 };
	GLfloat secondlightposition[] = { lightX2, lightY2, lightZ2, 0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiffusecolour);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, secondlightdiffusecolour);
	glLightfv(GL_LIGHT0, GL_POSITION, secondlightposition);

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	glTranslatef(0, 1, 0);
	GLfloat spherediffusecolour[] = { 1,1,1,1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, spherediffusecolour);
	gluSphere(myQuadricObject, 0.2, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1, 1, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, spherediffusecolour);
	gluSphere(myQuadricObject, 0.2, slices, stacks);
	glPopMatrix();

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-2, 0, 2);
	glVertex3f(2, 0, 2);
	glVertex3f(2, 0, -2);
	glVertex3f(-2, 0, -2);
	glEnd();

	glPushMatrix();
	glTranslatef(0.0f, 0.01f, 0.0f);
	GLfloat matrix[16] = { lightY,0,0,0,-lightX,0,-lightZ,-1,0,0,lightY,0,0,0,0,lightY };
	glMultMatrixf(matrix);
	glTranslatef(0.0f, 1.0f, 0.0f);
	GLfloat shadowdiffusecolour[] = { 0,0,0,1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shadowdiffusecolour);
	gluSphere(myQuadricObject, 0.2, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.01f, 0.0f);
	//GLfloat matrix[16] = { lightY,0,0,0,-lightX,0,-lightZ,-1,0,0,lightY,0,0,0,0,lightY };
	glMultMatrixf(matrix);
	glTranslatef(1.0f, 1.0f, 0.0f);
	//GLfloat shadowdiffusecolour[] = { 0,0,0,1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, shadowdiffusecolour);
	gluSphere(myQuadricObject, 0.2, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.01f, 0.0f);
	GLfloat secondmatrix[16] = { lightY2,0,0,0,-lightX2,0,-lightZ2,-1,0,0,lightY2,0,0,0,0,lightY2 };
	glMultMatrixf(secondmatrix);
	glTranslatef(0.0f, 1.0f, 0.0f);
	GLfloat secondshadowdiffusecolour[] = { 0,0,0,1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, secondshadowdiffusecolour);
	gluSphere(myQuadricObject, 0.2, slices, stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.01f, 0.0f);
	//GLfloat secondmatrix[16] = { lightY2,0,0,0,-lightX2,0,-lightZ2,-1,0,0,lightY2,0,0,0,0,lightY2 };
	glMultMatrixf(secondmatrix);
	glTranslatef(1.0f, 1.0f, 0.0f);
	//GLfloat shadowdiffusecolour[] = { 0,0,0,1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, secondshadowdiffusecolour);
	gluSphere(myQuadricObject, 0.2, slices, stacks);
	glPopMatrix();
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR commandstring, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1, 1, -1, 1, 0, 1);
	gluPerspective(45, 1, 0.1, 10);
	//glFrustum(-0.2, 0.2, -0.2, 0.2, 0.1, 10);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------