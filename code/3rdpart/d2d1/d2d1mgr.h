#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <unordered_map>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

static const std::wstring IMAGE_PATH_HEAD = L"../wndtest/d2d1/resource/image/";

class D2D1Mgr
{
public:
	static D2D1Mgr& Instance();

	int InitD2D1Mgr();
	int InitD2D1MgrFromWindows(HWND hWnd);
	int LoadAllImage();
	void DestroyD2D1Mgr();
	int LoadImageFromFile(const std::wstring& file_name);

	ID2D1Factory* GetID2D1Factory();
	ID2D1HwndRenderTarget* GetID2D1HwndRenderTarget();

	ID2D1SolidColorBrush* GetID2D1SolidColorBrush();
	IDWriteFactory* GetIDWriteFactory();
	IDWriteTextFormat* GetIDWriteTextFormat();

	IWICImagingFactory* GetIWICImagingFactory();
	ID2D1Bitmap* GetID2D1Bitmap(const std::wstring& file_name);
private:
	ID2D1Factory* m_factory = nullptr;
	ID2D1HwndRenderTarget* m_render_target = nullptr;
	ID2D1SolidColorBrush* m_brush = nullptr;
	IDWriteFactory* m_write_factory = nullptr;
	IDWriteTextFormat* m_text_format = nullptr;

	IWICImagingFactory* m_image_factory = nullptr;
	std::unordered_map<std::wstring, ID2D1Bitmap*> m_image_map;
};