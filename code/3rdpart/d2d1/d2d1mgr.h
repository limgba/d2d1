#pragma once
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <unordered_map>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

static const std::wstring IMAGE_PATH_HEAD = L"../wndtest/d2d1/resource/image/";
struct WindowsDependent
{
	~WindowsDependent();
	ID2D1HwndRenderTarget* render_target = nullptr;
	ID2D1SolidColorBrush* brush = nullptr;
	std::unordered_map<std::wstring, ID2D1Bitmap*> image_map;
};

class D2D1Mgr
{
public:
	static D2D1Mgr& Instance();

	int InitD2D1Mgr();
	int InitD2D1MgrFromWindows(HWND hWnd, size_t index);
	int LoadAllImage(size_t index);
	void DestroyD2D1Mgr();
	void DestroyWindowsDependent(size_t index);
	int LoadImageFromFile(WindowsDependent* windows_dependent, const std::wstring& file_name);

	ID2D1Factory* GetID2D1Factory();
	WindowsDependent* GetWindowsDependent(size_t index);

	IDWriteFactory* GetIDWriteFactory();
	IDWriteTextFormat* GetIDWriteTextFormat();

	IWICImagingFactory* GetIWICImagingFactory();
	ID2D1Bitmap* GetID2D1Bitmap(size_t index, const std::wstring& file_name);
private:
	ID2D1Factory* m_factory = nullptr;
	IDWriteFactory* m_write_factory = nullptr;
	IDWriteTextFormat* m_text_format = nullptr;
	IWICImagingFactory* m_image_factory = nullptr;
	std::vector<WindowsDependent*> m_windows_dependent_list;
};