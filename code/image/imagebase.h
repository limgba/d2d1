#pragma once
#include <d2d1.h>
#include <string>

class ImageBase
{
public:
	void SetIntervalMs(long interval_ms);
	long GetIntervalMs();
	void SetNextIndex(int next_index);
	int GetNextIndex();
	void SetID2D1Bitmap(ID2D1Bitmap* bitmap_image);
	ID2D1Bitmap* GetID2D1Bitmap();
	void SetPath(const std::wstring& path) { m_path = path; }
	const std::wstring& GetPath() { return m_path; }

private:
	long m_interval_ms = 0;
	int m_next_index = 0;
	std::wstring m_path;
	ID2D1Bitmap* m_bitmap_image = nullptr;
};
