#include "Application.h"

Application::Application(HINSTANCE hInstance)
{
	WNDCLASSEX wc = { 0 };
	static bool classRegistered = false;
	if (!classRegistered)
	{
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = L"IglooWindow";
		RegisterClassEx(&wc);
		classRegistered = true;

	}

	const DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	hwnd = CreateWindowEx(0, wc.lpszClassName, L"Igloo 2D", style, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, nullptr);
	ShowWindow(hwnd, SW_SHOWNORMAL);
	
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactory.put());
	rg.AddResource(d2dFactory);
	
	RECT rc;
	GetClientRect(hwnd, &rc);
	hr = d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), 
		D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), 
		renderTarget.put());
	rg.AddResource(renderTarget);

	hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(255.0f, 255.0f, 0.0f), brush.put());
	rg.AddResource(brush);
	
	windowRunning = true;

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(wicFactory.put()));
	rg.AddResource(wicFactory);
}

Application::~Application()
{
}

void Application::Start()
{
	Setup();

	MSG msg = { 0 };
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				windowRunning = false;
				break;
			}
		}
		else if (windowRunning)
		{
			Update();
			Draw();
		}

	}
}

bool Application::IsKeyDown(int keyCode)
{
	return GetAsyncKeyState(keyCode) & 0x8000;
}

HRESULT Application::CreateBitmapFromFile(const wchar_t* filePath, com_ptr<ID2D1Bitmap>& bitmap)
{
	com_ptr<IWICBitmapDecoder> decoder;
	com_ptr<IWICBitmapFrameDecode> source;
	com_ptr<IWICStream> stream;
	com_ptr<IWICFormatConverter> converter;
	com_ptr<IWICBitmapScaler> scaler;

	HRESULT hr = wicFactory->CreateDecoderFromFilename(filePath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.put());
	hr = decoder->GetFrame(0, source.put());
	hr = wicFactory->CreateFormatConverter(converter.put());
	hr = converter->Initialize(source.get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);

	hr = renderTarget->CreateBitmapFromWicBitmap(converter.get(), bitmap.put());
	rg.AddResource(bitmap);

	return hr;
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