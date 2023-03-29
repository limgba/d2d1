#pragma once
#include <vector>
class Animation;

// ÿ��֡��
static const int FRAME_PER_SECOND = 60;
// ÿ����Ҫ��֡
static const int FILL_THE_FRAME = 4;
// ÿ֡���ĺ�����
static const int FRAME_MILLISECOND = static_cast<int>(1.0 / FRAME_PER_SECOND * 1000);


class AnimationMgr
{
public:
	static AnimationMgr& Instance();
	void DeleteAllAnimation();

	void Play();
	void PushAnimation(Animation* animation);
	void DeleteAnimation(Animation* delete_animation);
	size_t AnimationSize();
	void SortAnimation();
	void DirtyLayer();			
	
private:
	bool m_dirty_layer = false;
	int m_play_count = 0;
	std::vector<Animation*> m_animation_list;
};
