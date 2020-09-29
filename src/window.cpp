#include "window.hpp"

#include <string>
#include <stdexcept>
#include <gl/GL.h>

OpenGLWindow::OpenGLWindow() {

	this->title = L"win32ogl";
	this->style = WS_SYSMENU;

	memset(&this->windowClass, 0, sizeof(WNDCLASS));
	static std::wstring menuName = L"MenuName";
	static std::wstring className = L"ClassName";
	this->windowClass.lpszMenuName = menuName.c_str();
	this->windowClass.lpszClassName = className.c_str();
	this->windowClass.lpfnWndProc = (WNDPROC)OpenGLWindow::WndProc;
	this->windowClass.hInstance = GetModuleHandle(NULL);
	this->windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	this->windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	this->windowClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	this->windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	this->dimensions.x = 256;
	this->dimensions.y = 256;

	RECT clientRect = {
		0,
		0,
		this->dimensions.x,
		this->dimensions.y
	};

	RECT desktopRect;
	GetWindowRect(GetDesktopWindow(), &desktopRect);

	AdjustWindowRect(&clientRect, this->style, false);
	this->dimensions.x = clientRect.right;
	this->dimensions.y = clientRect.bottom;
	this->position.x = (desktopRect.right / 2) - (this->dimensions.x / 2);
	this->position.y = (desktopRect.bottom / 2) - (this->dimensions.y / 2);

	if (!RegisterClass(&this->windowClass)) {
		throw std::runtime_error("Could not register window class");
	}

	if (!(this->handle = CreateWindow(
		this->windowClass.lpszClassName,
		this->title,
		this->style,
		this->position.x,
		this->position.y,
		this->dimensions.x,
		this->dimensions.y,
		(HWND)NULL,
		(HMENU)NULL,
		this->windowClass.hInstance,
		(LPVOID)NULL))) {
		throw std::runtime_error("Could not create window");
	}

	GLuint pixelFormat;
	static PIXELFORMATDESCRIPTOR descriptor = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA,
		16,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16,
		0, 0,
		PFD_MAIN_PLANE,
		0, 0, 0, 0
	};

	if (!(this->deviceContext = GetDC(this->handle))) {
		throw std::runtime_error("Could not get device context");
	}

	if (!(pixelFormat = ChoosePixelFormat(this->deviceContext, &descriptor))) {
		throw std::runtime_error("Could not get pixel format");
	}

	if (!(SetPixelFormat(this->deviceContext, pixelFormat, &descriptor))) {
		throw std::runtime_error("Could not set pixel format");
	}

	if (!(this->renderingContext = wglCreateContext(this->deviceContext))) {
		throw std::runtime_error("Could not create rendering context");
	}

	if (!wglMakeCurrent(this->deviceContext, this->renderingContext)) {
		throw std::runtime_error("Could not update the current rendering context");
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLdouble ratio = this->dimensions.x / this->dimensions.y;
	glFrustum(-1.0 * ratio, 1.0 * ratio, -1.0, 1.0, 1.0, 8.0);

	static GLfloat cube[] = {
		-1, -1, -1, 0, 0, 1, 1,
		-1, 1, -1, 0, 0, 1, 1,
		-1, -1, 1, 0, 0, 1, 1,
		-1, -1, 1, 0, 0, 1, 1,
		-1, 1, -1, 0, 0, 1, 1,
		-1, 1, 1, 0, 0, 1, 1,

		1, -1, -1, 0, 1, 0, 1,
		1, 1, -1, 0, 1, 0, 1,
		1, -1, 1, 0, 1, 0, 1,
		1, -1, 1, 0, 1, 0, 1,
		1, 1, -1, 0, 1, 0, 1,
		1, 1, 1, 0, 1, 0, 1,

		-1, -1, -1, 1, 0, 0, 1,
		1, -1, -1, 1, 0, 0, 1,
		-1, -1, 1, 1, 0, 0, 1,
		-1, -1, 1, 1, 0, 0, 1,
		1, -1, -1, 1, 0, 0, 1,
		1, -1, 1, 1, 0, 0, 1,

		-1, 1, -1, 0, 1, 1, 1,
		1, 1, -1, 0, 1, 1, 1,
		-1, 1, 1, 0, 1, 1, 1,
		-1, 1, 1, 0, 1, 1, 1,
		1, 1, -1, 0, 1, 1, 1,
		1, 1, 1, 0, 1, 1, 1,

		-1, -1, -1, 1, 0, 1, 1,
		1, -1, -1, 1, 0, 1, 1,
		-1, 1, -1, 1, 0, 1, 1,
		-1, 1, -1, 1, 0, 1, 1,
		1, -1, -1, 1, 0, 1, 1,
		1, 1, -1, 1, 0, 1, 1,

		-1, -1, 1, 1, 1, 0, 1,
		1, -1, 1, 1, 1, 0, 1,
		-1, 1, 1, 1, 1, 0, 1,
		-1, 1, 1, 1, 1, 0, 1,
		1, -1, 1, 1, 1, 0, 1,
		1, 1, 1, 1, 1, 0, 1
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 7 * sizeof(GLfloat), cube);
	glColorPointer(4, GL_FLOAT, 7 * sizeof(GLfloat), cube + 3);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

OpenGLWindow::~OpenGLWindow() {
	ReleaseDC(this->handle, this->deviceContext);
	DestroyWindow(this->handle);
}

void OpenGLWindow::render() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static float x = 0.0f;

	glTranslatef(0.f, 0.f, -3.0);
	glRotatef(x * 50, 1.f, 0.f, 0.f);
	glRotatef(x * 30, 0.f, 1.f, 0.f);
	glRotatef(x * 90, 0.f, 0.f, 1.f);

	x += 0.01;

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glFinish();
	SwapBuffers(this->deviceContext);

}

HWND OpenGLWindow::getHandle() const {
	return this->handle;
}

LRESULT CALLBACK OpenGLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;

}