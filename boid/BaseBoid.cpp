#include "stdafx.h"
#include "BaseBoid.h"
#include <GL/glut.h>
#include <corecrt_math_defines.h>
#include "Direction.h"
#include "parameters.h" //import common parameters

//境界条件: 壁
double checkBoundary(double pos)
{
	if (pos > BOUNDARY - WALL_SIZE)
	{
		pos = BOUNDARY - WALL_SIZE;
	}
	else if (pos < -BOUNDARY + WALL_SIZE)
	{
		pos = -BOUNDARY + WALL_SIZE;
	}
	return pos;
}

double radianToDegree(double rad)
{
	return rad * 180.0 / M_PI;
}

double degreeToRadian(double deg)
{
	return deg * M_PI / 180.0;
}

//this needs for Biod::isVisible
double _viewAngle = degreeToRadian(VIEW_ANGLE) / 2.0;

BaseBoid::BaseBoid(double _x, double _y, double _angle, double _speed, int _id)
{
	id = _id;
	x = _x;
	y = _y;
	angle = _angle;
	speed = _speed;
	vctr = Eigen::Vector2d(-sin(_angle) * _speed, cos(_angle) * _speed);
}

void BaseBoid::setColor(double red, double green, double blue)
{
	r = red;
	g = green;
	b = blue;
}

void BaseBoid::drawBaseBoid() //TODO:鳥らしく
{
	glColor3d(r, g, b);
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glRotated(radianToDegree(angle), 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2d(0.0, BOID_SIZE);
	glVertex2d(-0.4 * BOID_SIZE * sqrt(3.0) / 2.0, -BOID_SIZE / 2.0);
	glVertex2d(0.4 * BOID_SIZE * sqrt(3.0) / 2.0, -BOID_SIZE / 2.0);
	glEnd();
	glPopMatrix();
}

void BaseBoid::updatePosition()
{
	x += vctr.x() * FLAME_RATE / 1000.0;
	y += vctr.y() * FLAME_RATE / 1000.0;
	x = checkBoundary(x);
	y = checkBoundary(y);
}

bool BaseBoid::isVisible(double _x, double _y)
{
	double dx = _x - x;
	double dy = _y - y;
	Direction bDirection = Direction(dx, dy);
	double maxAngle = angle + _viewAngle;
	double minAngle = angle - _viewAngle;

	bool max = maxAngle > M_PI ? bDirection.angle > maxAngle - 2.0 * M_PI : bDirection.angle > maxAngle;
	bool min = minAngle < -M_PI ? bDirection.angle < minAngle + 2.0 * M_PI : bDirection.angle < minAngle;
	if (max && min)
	{
		return false;
	}
	return true;
}
