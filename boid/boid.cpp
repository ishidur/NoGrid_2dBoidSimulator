// boid.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>

#define BIRD_SIZE 0.5 //size of bird
#define BIRD_SPEED 2.0 //initial bird speed
#define BIRDS_NO 50 //number of birds
#define FLAME_RATE 100 //rerender after this FLAME_RATE milliseconds
#define WINDOW_SIZE 600 //window size
#define BOUNDARY 10.0 //area boundary

int time = 0;
//周期的境界条件
double checkBoundary(double pos)
{
	if (pos > BOUNDARY)
	{
		pos -= BOUNDARY * 2.0;
	}
	else if (pos < -BOUNDARY)
	{
		pos += BOUNDARY * 2.0;
	}
	return pos;
}

// TODO:Boidモデルの速度ベクトルの計算を実装
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
		x = checkBoundary(x);
		y = checkBoundary(y);
		double speed = sqrt(dx * dx + dy * dy);
		angle = acos(dy / speed);
		if (dx > 0.0)
		{
			angle = -angle;
		}
	}
};

Bird birds[BIRDS_NO];

double radianToDegree(double rad)
{
	return rad * 180.0 / M_PI;
}

double degreeToRadian(double deg)
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

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-w / WINDOW_SIZE * BOUNDARY, w / WINDOW_SIZE * BOUNDARY, -h / WINDOW_SIZE * BOUNDARY, h / WINDOW_SIZE * BOUNDARY, -1.0, 1.0);
}

void timer(int value)
{
	for (int i = 0; i < BIRDS_NO; i++)
	{
		birds[i].updatePosition(); //TODO:boidの次の時間ステップにおける速度ベクトルの計算
		if (time % 50 == 49)
		{
			birds[i].dx = (double(rand() % 3) - 1.0) * BIRD_SPEED;
			birds[i].dy = (double(rand() % 3) - 1.0) * BIRD_SPEED;
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
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	init();
	for (int i = 0; i < BIRDS_NO; i++)
	{
		birds[i] = Bird((double(rand()) - RAND_MAX / 2.0) * BOUNDARY / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * BOUNDARY / RAND_MAX, double(rand()) / RAND_MAX * 2.0 * M_PI);
	}
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(FLAME_RATE, timer, time);
	glutMainLoop();
	return 0;
}
