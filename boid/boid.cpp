// boid.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <GL/glut.h>
#include <math.h>

#define BIRD_SIZE 0.3

class Bird
{
public:
	double x;
	double y;
	double dx;
	double dy;
	Bird(double _x, double _y, double _dx, double _dy)
	{
		x = _x;
		y = _y;
		dx = _dx;
		dy = _dy;
	}
};

void drawBird(Bird bird)
{
	glBegin(GL_POLYGON);
	glVertex2d(bird.x, bird.y + BIRD_SIZE);
	glVertex2d(bird.x - BIRD_SIZE * sqrt(3.0) / 2.0, bird.y - BIRD_SIZE / 2.0);
	glVertex2d(bird.x + BIRD_SIZE * sqrt(3.0) / 2.0, bird.y - BIRD_SIZE / 2.0);
	glEnd();
}

void display(void)
{
	Bird birds[4] = { Bird(-0.5, -0.5, 0.0, 0.0), Bird(0.5, -0.5, 0.0, 0.0) , Bird(-0.5, 0.5, 0.0, 0.0) , Bird(0.5, 0.5, 0.0, 0.0) };
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1.0, 0.0, 0.0);
	drawBird(birds[0]);
	glColor3d(0.0, 1.0, 0.0);
	drawBird(birds[1]);
	glColor3d(0.0, 0.0, 1.0);
	drawBird(birds[2]);
	glColor3d(1.0, 0.0, 1.0);
	drawBird(birds[3]);
	glFlush();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}
