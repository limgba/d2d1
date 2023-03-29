#pragma once

namespace lmb
{
struct Circle
{
	int x;
	int y;
	int radius;
};

struct Rectangle
{
	int left;
	int top;
	int right;
	int bottom;
};

bool Impact(const Rectangle& rect, const Circle& circle);
bool Impact(const Circle& circle, const Rectangle& rect);
bool Impact(const Rectangle& rect1, const Rectangle& rect2);
bool Impact(const Circle& circle1, const Circle& circle2);

}
