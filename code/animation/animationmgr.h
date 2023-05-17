#pragma once
#include <vector>
#include <time.h>
#include <d2d1.h>
#include <unordered_map>
class Animation;

// 每秒帧数
static const int FRAME_PER_SECOND = 60;
// 每秒需要补帧
static const int FILL_THE_FRAME = 4;
// 每帧消耗毫秒数
static const int FRAME_MILLISECOND = static_cast<int>(1.0 / FRAME_PER_SECOND * 1000);


class AnimationMgr
{
public:
	static AnimationMgr& Instance();
	void InitAnimationMgr(size_t index);
	void DestroyAnimationMgr();
	void DestroyOneAnimationMgr(size_t index);

	void Play(ID2D1HwndRenderTarget* render_target, size_t index, clock_t now_clock);
	void PushAnimation(size_t index, Animation* animation);
	void DeleteAnimation(size_t index, Animation* delete_animation);
	size_t AnimationSize(size_t index);
	void SortAnimation(size_t index);
	void DirtyLayer(size_t index);			
	
private:
	struct AnimationData
	{
		bool dirty_layer = false;
		std::vector<Animation*> animation_list;
	};
	std::vector<AnimationData> m_animation_data_list;
};
