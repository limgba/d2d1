#pragma once


class CameraBase
{
public:
	CameraBase() = default;
	virtual ~CameraBase() = default;

	void SetCoordinate(int x, int y);
	int x();
	int y();

private:
	int m_x = 0;
	int m_y = 0;
};
