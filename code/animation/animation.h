#pragma once
#include <vector>
#include <time.h>
#include <d2d1.h>

class ImageBase;
class Obj;

class Animation
{
public:
	~Animation();
	void SetObj(Obj* obj);
	Obj* GetObj();
	void PushImageBase(ImageBase* image_base);
	void InitClock();
	void SetLoop(bool is_loop);
	void Play(ID2D1HwndRenderTarget* render_target, clock_t now_clock);
	void SetLayer(double layer);
	double GetLayer() const;
	const std::vector<ImageBase*>& GetImageVec();
	ImageBase* GetImageBase();
	ImageBase* GetFirstImageBase();
	
private:
	size_t m_index = 0;
	clock_t m_clock = 0;
	bool m_is_loop = true;
	double m_layer = 0.0;
	std::vector<ImageBase*> m_image_list;
	Obj* m_obj = nullptr;
};
