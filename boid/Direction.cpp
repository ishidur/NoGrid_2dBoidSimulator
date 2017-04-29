#include "stdafx.h"
#include "Direction.h"
#include <cmath>
#include <corecrt_math_defines.h>
#include "Point.h"
//-90 ~ 270 deg
Direction::Direction(double _angle)
{
	angle = _angle;
	x = -sin(_angle);
	y = cos(_angle);
}
Direction::Direction(double _x, double _y)
{
	double dist = sqrt(_x * _x + _y * _y);
	x = _x / dist;
	y = _y / dist;
	angle = atan(-_x / _y);
	if (_y < 0.0)
	{
		angle += M_PI;
	}
}
Direction::Direction(Point p)
{
	double dist = sqrt(p.x * p.x + p.y * p.y);
	x = p.x / dist;
	y = p.y / dist;
	angle = atan(-p.x / p.y);
	if (p.y < 0.0)
	{
		angle += M_PI;
	}
}
