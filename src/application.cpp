#include "application.hpp"


Application::Application() {

	LPCWSTR mutexName = L"win32ogl";
	this->mutex = CreateMutex(NULL, true, mutexName);
	DWORD lastError = GetLastError();
	if (lastError == ERROR_INVALID_HANDLE || lastError == ERROR_ALREADY_EXISTS) {
		PostQuitMessage(1);
	}

	this->window = std::unique_ptr<OpenGLWindow>(new OpenGLWindow());

}

Application::~Application() {
	if (this->mutex != NULL) {
		ReleaseMutex(this->mutex);
		CloseHandle(this->mutex);
	}
}

WPARAM Application::enterMessageQueue() const {

	MSG message;
	ZeroMemory(&message, sizeof(MSG));

	do {

		switch (MsgWaitForMultipleObjectsEx(0, NULL, 12, QS_ALLINPUT, 0)) {
		case WAIT_OBJECT_0:
			while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		case WAIT_TIMEOUT:
			this->window->render();
		}

	} while (message.message != WM_QUIT);

	return message.wParam;

}

void Application::showWindow(int nCmdShow) const {
	ShowWindow(this->window->getHandle(), nCmdShow);
	UpdateWindow(this->window->getHandle());
}
