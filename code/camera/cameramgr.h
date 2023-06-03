#pragma once
#include "camera.h"
#include <vector>

class CameraMgr : public CameraBase
{
public:
	CameraMgr() = default;
	virtual ~CameraMgr() = default;
	static CameraMgr& Instance();
	void PushCamera(Camera* camera);
	void DeleteCamera(size_t index);
	Camera* GetCamera(size_t index);

private:
	std::vector<Camera*> m_camera_vec;
};
