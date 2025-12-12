#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <winrt/base.h>

using winrt::com_ptr;

class Application
{
public:
	Application(HINSTANCE hInstance);
	~Application();
	void Start();
	bool IsKeyDown(int keyCode);
private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND hwnd;
	com_ptr<ID2D1Factory> d2dFactory;
	com_ptr<ID2D1HwndRenderTarget> renderTarget;
	com_ptr<ID2D1SolidColorBrush> brush;
};

