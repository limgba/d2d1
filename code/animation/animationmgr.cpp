#include "animationmgr.h"
#include "animation.h"
#include "3rdpart/d2d1/d2d1mgr.h"
#include <algorithm>
#include <Windows.h>

AnimationMgr& AnimationMgr::Instance()
{
	static AnimationMgr mgr;
	return mgr;
}

void AnimationMgr::InitAnimationMgr(size_t index)
{
	if (index >= m_animation_data_list.size())
	{
		m_animation_data_list.resize(index + 1);
	}
}

void AnimationMgr::DestroyAnimationMgr()
{
	for (auto& animation_data : m_animation_data_list)
	{
		auto& animation_list = animation_data.animation_list;
		for (size_t i = 0; i < animation_list.size(); ++i)
		{
			if (nullptr == animation_list[i])
			{
				continue;
			}
			delete animation_list[i];
			animation_list[i] = nullptr;
		}
		animation_list.clear();
	}
}

void AnimationMgr::DestroyOneAnimationMgr(size_t index)
{
	if (index >= m_animation_data_list.size())
	{
		return;
	}
	auto& animation_data = m_animation_data_list[index];
	auto& animation_list = animation_data.animation_list;
	if (animation_list.empty())
	{
		return;
	}
	for (size_t i = 0; i < animation_list.size(); ++i)
	{
		if (nullptr == animation_list[i])
		{
			continue;
		}
		delete animation_list[i];
		animation_list[i] = nullptr;
	}
	animation_list.clear();
}

void AnimationMgr::Play(ID2D1HwndRenderTarget* render_target, size_t index, clock_t now_clock)
{
	//WindowsDependent* windows_dependent = D2D1Mgr::Instance().GetWindowsDependent(index);
	//if (nullptr == windows_dependent)
	//{
	//	return;
	//}
	auto& animation_data = m_animation_data_list[index];
	if (animation_data.dirty_layer)
	{
		this->SortAnimation(index);
		animation_data.dirty_layer = false;
	}

	auto& animation_list = animation_data.animation_list;
	for (auto* animation : animation_list)
	{
		animation->Play(render_target, now_clock);
	}
}

void AnimationMgr::PushAnimation(size_t index, Animation* animation)
{
	if (index >= m_animation_data_list.size())
	{
		return;
	}
	if (nullptr == animation)
	{
		return;
	}
	auto& animation_data = m_animation_data_list[index];
	animation_data.animation_list.push_back(animation);
}

void AnimationMgr::DeleteAnimation(size_t index, Animation* delete_animation)
{
	if (index >= m_animation_data_list.size())
	{
		return;
	}
	if (nullptr == delete_animation)
	{
		return;
	}
	auto& animation_data = m_animation_data_list[index];
	auto& animation_list = animation_data.animation_list;
	auto delete_it = std::remove(animation_list.begin(), animation_list.end(), delete_animation);
	if (animation_list.end() == delete_it)
	{
		return;
	}
	for (auto it = delete_it; it != animation_list.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
	animation_list.erase(delete_it, animation_list.end());
}

size_t AnimationMgr::AnimationSize(size_t index)
{
	if (index >= m_animation_data_list.size())
	{
		return 0;
	}
	auto& animation_data = m_animation_data_list[index];
	return animation_data.animation_list.size();
}

void AnimationMgr::SortAnimation(size_t index)
{
	if (index >= m_animation_data_list.size())
	{
		return;
	}
	auto& animation_data = m_animation_data_list[index];
	auto& animation_list = animation_data.animation_list;
	std::sort(animation_list.begin(), animation_list.end(), [](const Animation* l, const Animation* r)
		{
			return l->GetLayer() < r->GetLayer();
		});
}

void AnimationMgr::DirtyLayer(size_t index)
{
	if (index >= m_animation_data_list.size())
	{
		return;
	}
	auto& animation_data = m_animation_data_list[index];
	animation_data.dirty_layer = true;
}

