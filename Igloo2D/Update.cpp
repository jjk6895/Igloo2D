#include "Application.h"

void Application::Setup()
{
	CreateBitmapFromFile(L"C:/Users/jkjt1/Downloads/art.jpg", img);
	CreateBitmapFromFile(L"C:/Users/jkjt1/Downloads/gran.jpg", img2);

	imgSize = { 100.0f, 100.0f };
	imgRect = { 100.0f, 200.0f, 100.0f + imgSize.x, 200.0f + imgSize.y };
	imgVel = { 10.0f, 2.5f };
	boxSize = { 200.0f, 200.0f };
	boxRect = { 300.0f, 200.0f, 300.0f + boxSize.x, 300.0f + boxSize.y };
	boxVel = { 0.0f, 0.0f };
}

bool RectsOverlap(const D2D1_RECT_F& rect1, const D2D1_RECT_F& rect2)
{
	return	rect1.left < rect2.right && rect1.right > rect2.left && 
			rect1.top < rect2.bottom && rect1.bottom > rect2.top;
}

void Application::Update()
{
	// Update positions
	imgRect.left += imgVel.x;
	imgRect.top += imgVel.y;
	imgRect.right = imgRect.left + imgSize.x;
	imgRect.bottom = imgRect.top + imgSize.y;
	boxRect.left += boxVel.x;
	boxRect.top += boxVel.y;
	boxRect.right = boxRect.left + boxSize.x;
	boxRect.bottom = boxRect.top + boxSize.y;

	if (imgRect.left < 0 || imgRect.right > clientWidth)
	{
		imgVel.x = -imgVel.x;
	}
	if (imgRect.top < 0 || imgRect.bottom > clientHeight)
	{
		imgVel.y = -imgVel.y;
	}
	if (boxRect.left < 0 || boxRect.right > clientWidth)
	{
		boxVel.x = -boxVel.x;
	}
	if (boxRect.top < 0 || boxRect.bottom > clientHeight)
	{
		boxVel.y = -boxVel.y;
	}

	imgVel.x *= 1.001f;
	imgVel.y *= 1.001f;

	if (RectsOverlap(imgRect, boxRect))
	{
		float m1 = imgSize.x * imgSize.y;
		float m2 = boxSize.x * boxSize.y;
		float massSum = m1 + m2;

		float dx = min(abs(imgRect.left - boxRect.right), abs(imgRect.right - boxRect.left));
		float dy = min(abs(imgRect.top - boxRect.bottom), abs(imgRect.bottom - boxRect.top));
		if (dx < dy)
		{
			// 1D elastic collision along X axis
			float u1 = imgVel.x;
			float u2 = boxVel.x;
			float v1 = (u1 * (m1 - m2) + 2.0f * m2 * u2) / massSum;
			float v2 = (u2 * (m2 - m1) + 2.0f * m1 * u1) / massSum;
			imgVel.x = v1;
			boxVel.x = v2;
		}
		else
		{
			// 1D elastic collision along Y axis
			float u1 = imgVel.y;
			float u2 = boxVel.y;
			float v1 = (u1 * (m1 - m2) + 2.0f * m2 * u2) / massSum;
			float v2 = (u2 * (m2 - m1) + 2.0f * m1 * u1) / massSum;
			imgVel.y = v1;
			boxVel.y = v2;
		}
	}
}

void Application::Draw()
{
	renderTarget->BeginDraw();

	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	renderTarget->DrawBitmap(img.get(), imgRect);
	renderTarget->DrawBitmap(img2.get(), boxRect);

	renderTarget->EndDraw();
}