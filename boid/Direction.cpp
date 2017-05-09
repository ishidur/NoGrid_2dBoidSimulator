#include "stdafx.h"
#include "Direction.h"
#include <cmath>
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
	if (_x < 0.0)
	{
		angle = acos(_y / dist);
	}
	else
	{
		angle = -acos(_y / dist);
	}
}
Direction::Direction(Point p)
{
	double dist = sqrt(p.x * p.x + p.y * p.y);
	x = p.x / dist;
	y = p.y / dist;
	if (p.x < 0.0)
	{
		angle = acos(p.y / dist);
	}
	else
	{
		angle = -acos(p.y / dist);
	}
}
