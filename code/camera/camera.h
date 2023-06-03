#pragma once
#include "camerabase.h"
#include "obj/obj.h"

class Camera : public CameraBase
{
public:
	Camera() = delete;
	Camera(int index);
	virtual ~Camera() = default;
	void ResetCoordinate();

private:
	int m_index = 0;
};
