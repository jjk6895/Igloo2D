#include "Application.h"

void Application::Setup()
{
	CreateBitmapFromFile(L"C:/Users/jkjt1/Downloads/DSC01786.jpg", img);
}

void Application::Update()
{
}

void Application::Draw()
{
	renderTarget->BeginDraw();

	renderTarget->FillRectangle(D2D1::RectF(100.0f, 100.0f, 200.0f, 200.0f), brush.get());
	renderTarget->DrawBitmap(img.get(), D2D1::RectF(150.0f, 150.0f, 250.0f, 250.0f));

	renderTarget->EndDraw();
}