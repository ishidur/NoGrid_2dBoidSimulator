#include "stdafx.h"
#include "Block.h"
#include <GL/glut.h>
#include <corecrt_math_defines.h>
#include <cmath>

Block::Block(double _x, double _y, double _r)
{
	x = _x;
	y = _y;
	r = _r;
}

void Block::drawBlock()
{
	int n = 20;
	double angl = 2.0 * M_PI / n;
	glColor3d(0.5, 0.5, 0.5);
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i < n; ++i)
	{
		glVertex2d(r * cos(double(i) * angl), r * sin(double(i) * angl));
	}
	glEnd();
	glPopMatrix();
}
