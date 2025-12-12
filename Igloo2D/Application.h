#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <winrt/base.h>
#include <wincodec.h>
#include "ResourceGuard.h"

using winrt::com_ptr;

class Application
{
public:
	Application(HINSTANCE hInstance);
	~Application();
	void Start();
	void Setup();
	void Update();
	void Draw();
	bool IsKeyDown(int keyCode);
	HRESULT CreateBitmapFromFile(const wchar_t* filePath, com_ptr<ID2D1Bitmap>& bitmap);
private:
	ResourceGuard rg;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND hwnd;
	bool windowRunning;
	com_ptr<ID2D1Factory> d2dFactory;
	com_ptr<ID2D1HwndRenderTarget> renderTarget;

	com_ptr<IWICImagingFactory> wicFactory;

	// Resources
	com_ptr<ID2D1SolidColorBrush> brush;
	com_ptr<ID2D1Bitmap> img;


};

