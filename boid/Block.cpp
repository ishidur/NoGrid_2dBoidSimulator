#include "stdafx.h"
#include "Block.h"
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
	GLfloat vtxs[CIRCLE_SLICE * 2 + 4] = {0.0};
	vtxs[0] = GLfloat(x);
	vtxs[1] = GLfloat(y);
	for (int i = 1; i <= CIRCLE_SLICE + 1; ++i)
	{
		int j = i * 2;
		vtxs[j] = GLfloat(r * cos(double(i) * angl) + x);
		vtxs[j + 1] = GLfloat(r * sin(double(i) * angl) + y);
	}
	glVertexPointer(2, GL_FLOAT, 0, vtxs);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SLICE + 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Block::setDisabled()
{
	disabled = true;
}
