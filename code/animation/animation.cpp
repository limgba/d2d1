#include "animation.h"
#include "image/imagebase.h"
#include "obj/rigidbodymgr.h"
#include "obj/obj.h"
#include "3rdpart/d2d1/d2d1mgr.h"
#include "obj/controlobjmgr.h"
#include "camera/cameramgr.h"

Animation::~Animation()
{
	for (ImageBase* image_base : m_image_list)
	{
		if (nullptr == image_base)
		{
			continue;
		}
		delete image_base;
		image_base = nullptr;
	}
	m_image_list.clear();
}

void Animation::SetObj(Obj* obj)
{
	if (nullptr == obj)
	{
		return;
	}
	m_obj = obj;
}

Obj* Animation::GetObj()
{
	return m_obj;
}

void Animation::PushImageBase(ImageBase* image_base)
{
	if (nullptr == image_base)
	{
		return;
	}
	m_image_list.push_back(image_base);
}

void Animation::InitClock()
{
	m_clock = clock();
}

void Animation::SetLoop(bool is_loop)
{
	m_is_loop = is_loop;
}

void Animation::Play(ID2D1HwndRenderTarget* render_target, int index, clock_t now_clock)
{
	if (nullptr == render_target)
	{
		return;
	}
	if (m_index >= m_image_list.size())
	{
		return;
	}
	ImageBase* image_base = m_image_list[m_index];
	if (nullptr == image_base)
	{
		return;
	}
	ID2D1Bitmap* bitmap = image_base->GetID2D1Bitmap();
	if (nullptr == bitmap)
	{
		return;
	}
	Camera* camera = CameraMgr::Instance().GetCamera(index);
	if (nullptr == camera)
	{
		return;
	}

	//do
	//{
	//	HWND hWnd = render_target->GetHwnd();
	//	const std::wstring file_name = image_base->GetPath();
	//	ID2D1Bitmap* check_bitmap = D2D1Mgr::Instance().GetID2D1Bitmap(hWnd, file_name);
	//	if (bitmap != check_bitmap)
	//	{
	//		return;
	//	}
	//} while (false);

	if (image_base->GetIntervalMs() > 0 && now_clock - m_clock >= image_base->GetIntervalMs())
	{
		m_index = image_base->GetNextIndex();
		if (m_index >= m_image_list.size())
		{
			if (!m_is_loop)
			{
				return;
			}
			m_index = 0;
			m_clock = now_clock;
		}
		image_base = m_image_list[m_index];
		if (nullptr == image_base)
		{
			return;
		}
	}

	camera->ResetCoordinate();
	D2D1_SIZE_U render_size = render_target->GetPixelSize();
	D2D1_SIZE_U size = bitmap->GetPixelSize();
	int camera_x = camera->x();
	int camera_y = camera->y();
	int origin_x = camera_x - render_size.width / 2;
	int origin_y = camera_y - render_size.height / 2;
	int x = m_obj->x();
	int y = m_obj->y();
	x -= origin_x;
	y -= origin_y;
	int width = x + size.width;
	int height = y + size.height;
	if (width < 0 || height < 0)
	{
		return;
	}
	int obj_type = m_obj->GetObjType();
	int render_width = origin_x + render_size.width;
	int render_height = origin_y + render_size.height;
	if (x > render_width || y > render_height)
	{
		return;
	}
	D2D1_RECT_F imgr = { x, y, width, height };
	render_target->DrawBitmap(bitmap, imgr);
}

void Animation::SetLayer(double layer)
{
	m_layer = layer;
}

double Animation::GetLayer() const
{
	return m_layer;
}

const std::vector<ImageBase*>& Animation::GetImageVec()
{
	return m_image_list;
}

ImageBase* Animation::GetImageBase()
{
	if (m_index >= m_image_list.size())
	{
		return nullptr;
	}
	return m_image_list[m_index];
}

ImageBase* Animation::GetFirstImageBase()
{
	if (m_image_list.empty())
	{
		return nullptr;
	}
	return m_image_list.front();
}

