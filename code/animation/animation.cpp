#include "animation.h"
#include "image/imagebase.h"
#include "obj/rigidbodymgr.h"
#include "obj/obj.h"
#include "3rdpart/d2d1/d2d1mgr.h"

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

void Animation::Play(clock_t now_clock)
{
	if (m_index >= m_image_list.size())
	{
		return;
	}
	ImageBase* image_base = m_image_list[m_index];
	if (nullptr == image_base)
	{
		return;
	}

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
	ID2D1Bitmap* bitmap = image_base->GetID2D1Bitmap();
	if (nullptr == bitmap)
	{
		return;
	}
	ID2D1HwndRenderTarget* render_target = D2D1Mgr::Instance().GetID2D1HwndRenderTarget();
	if (nullptr == render_target)
	{
		return;
	}
	D2D1_SIZE_U size = bitmap->GetPixelSize();
	int x = m_obj->x();
	int y = m_obj->y();
	D2D1_RECT_F imgr = { x, y, x + size.width, y + size.height };
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

