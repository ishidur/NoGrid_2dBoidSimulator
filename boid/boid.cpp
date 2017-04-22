// boid.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>

#define BIRD_SIZE 0.3 //size of bird
#define BIRD_SPEED 0.1 //initial bird speed
#define BIRDS_NO 10 //number of birds
#define FLAME_RATE 100 //rerender after this FLAME_RATE milliseconds

int time = 0;

class Bird
{
public:
	double x; //x-position
	double y; //y-position
	double angle; //radian angle: 0 vector is (0, 1)
	double dx; // x-direction speed
	double dy; // y-direction speed

	Bird(double _x = 0.0, double _y = 0.0, double _angle = 0.0)
	{
		x = _x;
		y = _y;
		angle = _angle;
		dx = BIRD_SPEED * sin(_angle);
		dy = BIRD_SPEED * cos(_angle);
	}

	void updatePosition()
	{
		x += dx * FLAME_RATE / 1000.0;
		y += dy * FLAME_RATE / 1000.0;
		double speed = sqrt(dx * dx + dy * dy);
		angle = acos(dy / speed);
		// TODO:角度計算がおかしい
	}
};

Bird birds[BIRDS_NO];

double radianToDegree(int rad)
{
	return rad * 180.0 / M_PI;
}

double degreeToRadian(int deg)
{
	return deg * M_PI / 180.0;
}

void drawBird(Bird bird) //TODO:鳥らしく
{
	glPushMatrix();
	glTranslated(bird.x, bird.y, 0.0);
	glRotated(radianToDegree(bird.angle), 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2d(0.0, BIRD_SIZE);
	glVertex2d(-0.4 * BIRD_SIZE * sqrt(3.0) / 2.0, -BIRD_SIZE / 2.0);
	glVertex2d(0.4 * BIRD_SIZE * sqrt(3.0) / 2.0, - BIRD_SIZE / 2.0);
	glEnd();
	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0);
	for (int i = 0; i < BIRDS_NO; i++)
	{
		drawBird(birds[i]);
	}
	glFlush();
}

void timer(int value)
{
	for (int i = 0; i < BIRDS_NO; i++)
	{
		birds[i].updatePosition(); //TODO:次の時間ステップにおける速度ベクトルの計算
		if (time == 20)
		{
			birds[i].dx = 0.0;
			birds[i].dy = -1.0;
		}
	}
	if (time % 10 == 0)
	{
		printf("[%f, %f]:%f\n", birds[0].dx, birds[0].dy, radianToDegree(birds[0].angle));
	}
	glutPostRedisplay();
	time++;
	glutTimerFunc(FLAME_RATE, timer, time);
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
	init();
	for (int i = 0; i < BIRDS_NO; i++)
	{
		birds[i] = Bird((double(rand()) - RAND_MAX / 2.0) / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) / RAND_MAX, double(rand()) / RAND_MAX * 2.0 * M_PI);
	}
	glutDisplayFunc(display);
	glutTimerFunc(FLAME_RATE, timer, time);
	glutMainLoop();
	return 0;
}
