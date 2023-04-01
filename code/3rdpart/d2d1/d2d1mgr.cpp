#include "d2d1mgr.h"

D2D1Mgr& D2D1Mgr::Instance()
{
	static D2D1Mgr mgr;
	return mgr;
}

int D2D1Mgr::InitD2D1Mgr()
{
    HRESULT h_result;
    h_result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (S_OK != h_result)
    {
        return h_result;
    }

    h_result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
    if (S_OK != h_result)
    {
        return h_result;
    }

    h_result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_write_factory), reinterpret_cast<IUnknown**>(&m_write_factory));
    if (S_OK != h_result)
    {
        return h_result;
    }
    h_result = m_write_factory->CreateTextFormat(L"ËÎÌו", NULL, DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16, L"", &m_text_format);
    if (S_OK != h_result)
    {
        return h_result;
    }

    h_result = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(m_image_factory), (LPVOID*)&m_image_factory);
    if (S_OK != h_result)
    {
        return h_result;
    }

    return 0;
}

int D2D1Mgr::InitD2D1MgrFromWindows(HWND hWnd)
{
    HRESULT h_result;

    RECT rect;
    GetClientRect(hWnd, &rect);

    h_result = m_factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)),
        &m_render_target
    );
    if (S_OK != h_result)
    {
        return h_result;
    }

    h_result = m_render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &m_brush);
    if (S_OK != h_result)
    {
        return h_result;
    }
    return 0;
}

int D2D1Mgr::LoadAllImage()
{
    LoadImageFromFile(L"background/ndmz.bmp");
    LoadImageFromFile(L"picture/white.bmp");
    LoadImageFromFile(L"picture/white1.bmp");
    LoadImageFromFile(L"picture/jiantou0.bmp");
    LoadImageFromFile(L"picture/jiantou1.bmp");
    LoadImageFromFile(L"picture/xiaojiantou0.bmp");
    LoadImageFromFile(L"picture/xiaojiantou1.bmp");
    LoadImageFromFile(L"picture/xiaojiantou2.bmp");
    LoadImageFromFile(L"picture/xiaojiantou3.bmp");
    LoadImageFromFile(L"picture/xiaojiantou4.bmp");
    LoadImageFromFile(L"picture/xiaojiantou5.bmp");
    LoadImageFromFile(L"picture/xiaojiantou6.bmp");
    return 0;
}

void D2D1Mgr::DestroyD2D1Mgr()
{
	for (auto& pair : m_image_map)
	{
		if (nullptr == pair.second)
		{
			continue;
		}
		pair.second->Release();
	}
    m_image_factory->Release();

    m_text_format->Release();
    m_write_factory->Release();
	m_brush->Release();
	m_render_target->Release();
    m_factory->Release();
}

int D2D1Mgr::LoadImageFromFile(const std::wstring& file_name)
{
    HRESULT h_result;

    IWICBitmapDecoder* bitmapdecoder = NULL;
    std::wstring full_file_name = IMAGE_PATH_HEAD + file_name;
    h_result = m_image_factory->CreateDecoderFromFilename(full_file_name.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapdecoder);
    if (S_OK != h_result)
    {
        return h_result;
    }

    IWICBitmapFrameDecode* pframe = NULL;
    h_result = bitmapdecoder->GetFrame(0, &pframe);
    if (S_OK != h_result)
    {
        return h_result;
    }

    IWICFormatConverter* fmtcovter = NULL;
    h_result = m_image_factory->CreateFormatConverter(&fmtcovter);
    if (S_OK != h_result)
    {
        return h_result;
    }
    h_result = fmtcovter->Initialize(pframe, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
    if (S_OK != h_result)
    {
        return h_result;
    }

    ID2D1Bitmap* bitmap = NULL;
    h_result = m_render_target->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bitmap);
    if (S_OK != h_result)
    {
        return h_result;
    }
    m_image_map.emplace(full_file_name, bitmap);

    fmtcovter->Release();
    pframe->Release();
    bitmapdecoder->Release();

    return 0;
}

ID2D1Factory* D2D1Mgr::GetID2D1Factory()
{
    return m_factory;
}

ID2D1HwndRenderTarget* D2D1Mgr::GetID2D1HwndRenderTarget()
{
    return m_render_target;
}

ID2D1SolidColorBrush* D2D1Mgr::GetID2D1SolidColorBrush()
{
    return m_brush;
}

IDWriteFactory* D2D1Mgr::GetIDWriteFactory()
{
    return m_write_factory;
}

IDWriteTextFormat* D2D1Mgr::GetIDWriteTextFormat()
{
    return m_text_format;
}

IWICImagingFactory* D2D1Mgr::GetIWICImagingFactory()
{
    return m_image_factory;
}

ID2D1Bitmap* D2D1Mgr::GetID2D1Bitmap(const std::wstring& file_name)
{
    std::wstring full_file_name = IMAGE_PATH_HEAD + file_name;
    auto it = m_image_map.find(full_file_name);
    if (it == m_image_map.end())
    {
        return nullptr;
    }
    return it->second;
}

