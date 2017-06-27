#include "stdafx.h"
#include "Block.h"
#include <corecrt_math_defines.h>
#include <cmath>

Block::Block(double _x, double _y, double _r)
{
	x = _x;
	y = _y;
	r = _r;
}

void Block::setColor(double _red, double _green, double _blue)
{
	red = _red;
	green = _green;
	blue = _blue;
}

void Block::drawBlock()
{
	double angl = 2.0 * M_PI / CIRCLE_SLICE;
	glColor3d(red, green, blue);
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < CIRCLE_SLICE; ++i)
	{
		glVertex2d(r * cos(double(i) * angl), r * sin(double(i) * angl));
	}
	glEnd();
	glPopMatrix();
}

void Block::setDisabled()
{
	disabled = true;
}
