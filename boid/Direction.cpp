#include "stdafx.h"
#include "Direction.h"

Direction::Direction(double _angle)
{
	angle = _angle;
	x = -sin(_angle);
	y = cos(_angle);
	vector = Eigen::Vector2d(x, y);
}

Direction::Direction(double _x, double _y)
{
	double dist = sqrt(_x * _x + _y * _y);
	x = _x / dist;
	y = _y / dist;
	vector = Eigen::Vector2d(x, y);
	if (_x < 0.0)
	{
		angle = acos(_y / dist);
	}
	else
	{
		angle = -acos(_y / dist);
	}
}

Direction::Direction(Eigen::Vector2d& p)
{
	double dist = p.norm();
	x = p.x() / dist;
	y = p.y() / dist;
	vector = Eigen::Vector2d(x, y);
	if (p.x() < 0.0)
	{
		angle = acos(p.y() / dist);
	}
	else
	{
		angle = -acos(p.y() / dist);
	}
}
