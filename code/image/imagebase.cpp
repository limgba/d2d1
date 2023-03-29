#include "imagebase.h"

void ImageBase::SetIntervalMs(long interval_ms)
{
	m_interval_ms = interval_ms;
}

long ImageBase::GetIntervalMs()
{
	return m_interval_ms;
}

void ImageBase::SetNextIndex(int next_index)
{
	m_next_index = next_index;
}

int ImageBase::GetNextIndex()
{
	return m_next_index;
}

void ImageBase::SetID2D1Bitmap(ID2D1Bitmap* bitmap_image)
{
	m_bitmap_image = bitmap_image;
}

ID2D1Bitmap* ImageBase::GetID2D1Bitmap()
{
	return m_bitmap_image;
}

