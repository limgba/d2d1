#include "shape.h"
#include <cmath>

bool lmb::Impact(const lmb::Rectangle& rect, const lmb::Circle& circle)
{
	int xl1 = rect.left;
	int yl1 = rect.top;
	int xr1 = rect.right;
	int yr1 = rect.bottom;
	double xc1 = (xl1 + xr1) / 2.0;
	double yc1 = (yl1 + yr1) / 2.0;
	double xa1 = xr1 - xc1;
	double ya1 = yr1 - yc1;
	int xc2 = circle.x;
	int yc2 = circle.y;
	double xa2 = circle.radius;
	double ya2 = circle.radius;

	bool less_than_x = std::abs(xc2 - xc1) <= xa1 + xa2;
	bool less_than_y = std::abs(yc2 - yc1) <= ya1 + ya2;
	return less_than_x && less_than_y;
}

bool lmb::Impact(const lmb::Circle& circle, const lmb::Rectangle& rect)
{
	return lmb::Impact(rect, circle);
}

bool lmb::Impact(const lmb::Rectangle& rect1, const lmb::Rectangle& rect2)
{
	int xl1 = rect1.left;
	int yl1 = rect1.top;
	int xr1 = rect1.right;
	int yr1 = rect1.bottom;
	double xc1 = (xl1 + xr1) / 2.0;
	double yc1 = (yl1 + yr1) / 2.0;
	double xa1 = xr1 - xc1;
	double ya1 = yr1 - yc1;

	int xl2 = rect2.left;
	int yl2 = rect2.top;
	int xr2 = rect2.right;
	int yr2 = rect2.bottom;
	double xc2 = (xl2 + xr2) / 2.0;
	double yc2 = (yl2 + yr2) / 2.0;
	double xa2 = xr2 - xc2;
	double ya2 = yr2 - yc2;

	bool less_than_x = std::abs(xc2 - xc1) <= xa1 + xa2;
	bool less_than_y = std::abs(yc2 - yc1) <= ya1 + ya2;
	return less_than_x && less_than_y;
}

bool lmb::Impact(const lmb::Circle& circle1, const lmb::Circle& circle2)
{
	int x = circle1.x - circle2.x;
	int y = circle1.y - circle2.y;
	int distance = (int)std::sqrt(x * x + y * y);
	return distance < circle1.radius + circle2.radius;
}

