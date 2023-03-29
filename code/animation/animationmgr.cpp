#include "animationmgr.h"
#include "animation.h"
#include <algorithm>
#include <Windows.h>

AnimationMgr& AnimationMgr::Instance()
{
	static AnimationMgr mgr;
	return mgr;
}

void AnimationMgr::DeleteAllAnimation()
{
	for (size_t i = 0; i < m_animation_list.size(); ++i)
	{
		delete m_animation_list[i];
		m_animation_list[i] = nullptr;
	}
}

void AnimationMgr::Play()
{
	if (m_dirty_layer)
	{
		this->SortAnimation();
		m_dirty_layer = false;
	}
	clock_t now_clock = clock();
	for (auto* animation : m_animation_list)
	{
		animation->Play(now_clock);
	}
	++m_play_count;
	if (m_play_count >= FRAME_PER_SECOND)
	{
		m_play_count = 0;
	}
	clock_t play_clock = clock();
	clock_t diff_clock = play_clock - now_clock;
	if (diff_clock < FRAME_MILLISECOND)
	{
		Sleep(FRAME_MILLISECOND - diff_clock);
		if (0 == m_play_count)
		{
			Sleep(FILL_THE_FRAME);
		}
	}
}

void AnimationMgr::PushAnimation(Animation* animation)
{
	if (nullptr == animation)
	{
		return;
	}
	m_animation_list.push_back(animation);
}

void AnimationMgr::DeleteAnimation(Animation* delete_animation)
{
	if (nullptr == delete_animation)
	{
		return;
	}
	auto delete_it = std::remove(m_animation_list.begin(), m_animation_list.end(), delete_animation);
	if (m_animation_list.end() == delete_it)
	{
		return;
	}
	for (auto it = delete_it; it != m_animation_list.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
	m_animation_list.erase(delete_it, m_animation_list.end());
}

size_t AnimationMgr::AnimationSize()
{
	return m_animation_list.size();
}

void AnimationMgr::SortAnimation()
{
	std::sort(m_animation_list.begin(), m_animation_list.end(), [](const Animation* l, const Animation* r)
		{
			return l->GetLayer() < r->GetLayer();
		});
}

void AnimationMgr::DirtyLayer()
{
	m_dirty_layer = true;
}

