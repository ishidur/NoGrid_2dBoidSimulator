#include "stdafx.h"
#include "BaseBoid.h"
#include <corecrt_math_defines.h>
#include "Direction.h"

double checkBoundary(double pos)
{
	if (pos > BOUNDARY - WALL_SIZE)
	{
		pos = BOUNDARY - WALL_SIZE - 0.1;
	}
	else if (pos < -BOUNDARY + WALL_SIZE)
	{
		pos = -BOUNDARY + WALL_SIZE + 0.1;
	}
	return pos;
}

double radianToDegree(double rad)
{
	return rad * 180.0 / M_PI;
}

BaseBoid::BaseBoid(double _x, double _y, double _angle, double _size, double _speed, int _id)
{
	id = _id;
	x = _x;
	y = _y;
	angle = _angle;
	size = _size;
	vctr = Eigen::Vector2d(-sin(_angle) * _speed, cos(_angle) * _speed);
}

void BaseBoid::setColor(double red, double green, double blue)
{
	r = red;
	g = green;
	b = blue;
}

std::vector<GLfloat> BaseBoid::drawBaseBoid()
{
	glColor3d(r, g, b);
	std::vector<GLfloat> vtx3 = {
		GLfloat(0.0 * cos(angle) + size * (-sin(angle)) + x), GLfloat(0.0 * sin(angle) + size * cos(angle) + y),
		GLfloat(-0.4 * size * sqrt(3.0) / 2.0 * cos(angle) + (-size / 2.0) * (-sin(angle)) + x), GLfloat(-0.4 * size * sqrt(3.0) / 2.0 * sin(angle) + (-size / 2.0) * cos(angle) + y),
		GLfloat(0.4 * size * sqrt(3.0) / 2.0 * cos(angle) + (-size / 2.0) * (-sin(angle)) + x), GLfloat(0.4 * size * sqrt(3.0) / 2.0 * sin(angle) + (-size / 2.0) * cos(angle) + y),
	};
	return vtx3;
	//	glVertexPointer(2, GL_FLOAT, 0, vtx3.data());
	//	glEnableClientState(GL_VERTEX_ARRAY);
	//	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void BaseBoid::updatePosition()
{
	x += vctr.x() * FLAME_RATE / 1000.0;
	y += vctr.y() * FLAME_RATE / 1000.0;
	x = checkBoundary(x);
	y = checkBoundary(y);
}

bool BaseBoid::isVisible(double _x, double _y, double _viewAngle)
{
	double dx = _x - x;
	double dy = _y - y;
	Direction bDirection = Direction(dx, dy);
	double maxAngle = angle + _viewAngle;
	double minAngle = angle - _viewAngle;

	bool max = maxAngle > M_PI ? bDirection.angle > maxAngle - 2.0 * M_PI : bDirection.angle > maxAngle;
	bool min = minAngle < -M_PI ? bDirection.angle < minAngle + 2.0 * M_PI : bDirection.angle < minAngle;
	if (maxAngle <= M_PI && minAngle > -M_PI)
	{
		if (max || min)
		{
			return false;
		}
	}
	else
	{
		if (max && min)
		{
			return false;
		}
	}
	return true;
}
