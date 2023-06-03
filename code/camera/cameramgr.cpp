#include "cameramgr.h"


CameraMgr& CameraMgr::Instance()
{
	static CameraMgr mgr;
	return mgr;
}

void CameraMgr::PushCamera(Camera* camera)
{
	m_camera_vec.push_back(camera);
}

void CameraMgr::DeleteCamera(size_t index)
{
	if (index >= m_camera_vec.size())
	{
		return;
	}
	if (nullptr == m_camera_vec[index])
	{
		return;
	}
	delete m_camera_vec[index];
	m_camera_vec[index] = nullptr;
}

Camera* CameraMgr::GetCamera(size_t index)
{
	if (index >= m_camera_vec.size())
	{
		return nullptr;
	}
	return m_camera_vec[index];
}
