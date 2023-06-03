#include "camerabase.h"

void CameraBase::SetCoordinate(int x, int y)
{
	m_x = x;
	m_y = y;
}

int CameraBase::x()
{
	return m_x;
}

int CameraBase::y()
{
	return m_y;
}
