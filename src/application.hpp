#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include "window.hpp"

#include <memory>


class Application {
public:
	Application();
	~Application();

	WPARAM enterMessageQueue() const;

	void showWindow(int nCmdShow) const;

private:
	HANDLE mutex;

	std::unique_ptr<OpenGLWindow> window;
};

#endif