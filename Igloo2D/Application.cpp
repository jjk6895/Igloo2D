#include "Application.h"

Application::Application(HINSTANCE hInstance)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"IglooWindow";
	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(0, wc.lpszClassName, L"Igloo 2D", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, nullptr);
	ShowWindow(hwnd, SW_SHOWNORMAL);

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactory.put());
	
	RECT rc;
	GetClientRect(hwnd, &rc);
	hr = d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), 
		D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), 
		renderTarget.put());

	hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(255.0f, 255.0f, 0.0f), brush.put());
}

Application::~Application()
{
}

void Application::Start()
{
	MSG msg = { 0 };
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			// Do game shit
			renderTarget->BeginDraw();

			renderTarget->FillRectangle(D2D1::RectF(100.0f, 100.0f, 200.0f, 200.0f), brush.get());

			renderTarget->EndDraw();
		}

	}
}

bool Application::IsKeyDown(int keyCode)
{
	return GetAsyncKeyState(keyCode) & 0x8000;
}

LRESULT Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
