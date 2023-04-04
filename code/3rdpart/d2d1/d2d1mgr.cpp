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

int D2D1Mgr::InitD2D1MgrFromWindows(HWND hWnd, size_t index)
{
    HRESULT h_result;
    if (index >= m_windows_dependent_list.size())
    {
        m_windows_dependent_list.resize(index + 1);
    }
    WindowsDependent* windows_dependent = new WindowsDependent();
    m_windows_dependent_list[index] = windows_dependent;

    RECT rect;
    GetClientRect(hWnd, &rect);

    h_result = m_factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)),
        &windows_dependent->render_target
    );
    if (S_OK != h_result)
    {
        return h_result;
    }

    h_result = windows_dependent->render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::WhiteSmoke), &windows_dependent->brush);
    if (S_OK != h_result)
    {
        return h_result;
    }
    return 0;
}

int D2D1Mgr::LoadAllImage(size_t index)
{
    WindowsDependent* windows_dependent = this->GetWindowsDependent(index);
    if (nullptr == windows_dependent)
    {
        return -1;
    }

    LoadImageFromFile(windows_dependent, L"background/ndmz.bmp");
    LoadImageFromFile(windows_dependent, L"picture/white.bmp");
    LoadImageFromFile(windows_dependent, L"picture/white1.bmp");
    LoadImageFromFile(windows_dependent, L"picture/jiantou0.bmp");
    LoadImageFromFile(windows_dependent, L"picture/jiantou1.bmp");
    LoadImageFromFile(windows_dependent, L"picture/xiaojiantou0.bmp");
    LoadImageFromFile(windows_dependent, L"picture/xiaojiantou1.bmp");
    LoadImageFromFile(windows_dependent, L"picture/xiaojiantou2.bmp");
    LoadImageFromFile(windows_dependent, L"picture/xiaojiantou3.bmp");
    LoadImageFromFile(windows_dependent, L"picture/xiaojiantou4.bmp");
    LoadImageFromFile(windows_dependent, L"picture/xiaojiantou5.bmp");
    LoadImageFromFile(windows_dependent, L"picture/xiaojiantou6.bmp");
    return 0;
}

void D2D1Mgr::DestroyD2D1Mgr()
{
    for (auto& windows_dependent : m_windows_dependent_list)
    {
        if (nullptr == windows_dependent)
        {
            continue;
        }

        delete windows_dependent;
        windows_dependent = nullptr;
    }
    m_image_factory->Release();
    m_text_format->Release();
    m_write_factory->Release();
    m_factory->Release();
}

int D2D1Mgr::LoadImageFromFile(WindowsDependent* windows_dependent, const std::wstring& file_name)
{
    if (nullptr == windows_dependent)
    {
        return -1;
    }
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
    h_result = windows_dependent->render_target->CreateBitmapFromWicBitmap(fmtcovter, NULL, &bitmap);
    if (S_OK != h_result)
    {
        return h_result;
    }
    windows_dependent->image_map.emplace(full_file_name, bitmap);

    fmtcovter->Release();
    pframe->Release();
    bitmapdecoder->Release();

    return 0;
}

ID2D1Factory* D2D1Mgr::GetID2D1Factory()
{
    return m_factory;
}

WindowsDependent* D2D1Mgr::GetWindowsDependent(size_t index)
{
    if (index >= m_windows_dependent_list.size())
    {
        return nullptr;
    }
    return m_windows_dependent_list[index];
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

ID2D1Bitmap* D2D1Mgr::GetID2D1Bitmap(size_t index, const std::wstring& file_name)
{
    auto* windows_dependent = this->GetWindowsDependent(index);
    if (nullptr == windows_dependent)
    {
        return nullptr;
    }
    std::wstring full_file_name = IMAGE_PATH_HEAD + file_name;
    auto it = windows_dependent->image_map.find(full_file_name);
    if (it == windows_dependent->image_map.end())
    {
        return nullptr;
    }
    return it->second;
}

WindowsDependent::~WindowsDependent()
{
    for (auto& pair : this->image_map)
    {
        if (nullptr == pair.second)
        {
            continue;
        }
        pair.second->Release();
    }
    this->brush->Release();
    this->render_target->Release();
}
