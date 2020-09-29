#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <windows.h>
#include "util.hpp"

class OpenGLWindow {
public:
	OpenGLWindow();
	~OpenGLWindow();

	void render() const;

	HWND getHandle() const;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LPCWSTR title;
	DWORD style;
	WNDCLASS windowClass;
	HDC deviceContext;
	HGLRC renderingContext;
	HWND handle;

	Vector2<int> position;
	Vector2<int> dimensions;

};

#endif