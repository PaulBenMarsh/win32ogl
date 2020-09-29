
#pragma comment(lib, "opengl32.lib")
#define WIN32_LEAN_AND_MEAN

#include "application.hpp"

#include <exception>


int __stdcall WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdLine, _In_ int nCmdShow) {

	WPARAM exit = 1;

	try {
		Application application;
		application.showWindow(nCmdShow);
		exit = application.enterMessageQueue();
	}
	catch (std::exception& exception) {
		MessageBoxA(NULL, exception.what(), "Error!", MB_OK | MB_ICONEXCLAMATION);
	}

	return exit;
}