#pragma once

#include <Windows.h>

class Application
{
public:
	Application(HINSTANCE hInstance);
	~Application();
	void Start();
private:
	HWND hwnd;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

