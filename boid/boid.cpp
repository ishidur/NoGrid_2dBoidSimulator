// boid.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>

#define BIRD_SIZE 0.5 //size of bird
#define BIRD_SPEED 2.0 //initial bird speed
#define BIRDS_NO 30 //number of birds
#define FLAME_RATE 100 //rerender after this FLAME_RATE milliseconds
#define WINDOW_SIZE 600 //window size
#define BOUNDARY 10.0 //area boundary
#define VIEW_ANGLE 30.0 //bird view angle
#define OPTIMUM_DISTANCE 2.0 //
#define GRAVITY_WEIGHT 0.5 //gravity point weight
#define DISTANT_WEIGHT 1.0 //nearest bird weight
#define ALIGNMENT_WEIGHT 2.0 //alignment weight

int time = 0; //time

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

double radianToDegree(double rad)
{
	return rad * 180.0 / M_PI;
}

double degreeToRadian(double deg)
{
	return deg * M_PI / 180.0;
}

double calcDist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
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

	void drawBird() //TODO:鳥らしく
	{
		glPushMatrix();
		glTranslated(x, y, 0.0);
		glRotated(radianToDegree(angle), 0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
		glVertex2d(0.0, BIRD_SIZE);
		glVertex2d(-0.4 * BIRD_SIZE * sqrt(3.0) / 2.0, -BIRD_SIZE / 2.0);
		glVertex2d(0.4 * BIRD_SIZE * sqrt(3.0) / 2.0, -BIRD_SIZE / 2.0);
		glEnd();
		glPopMatrix();
	}

	void updatePosition()
	{
		x += dx * FLAME_RATE / 1000.0;
		y += dy * FLAME_RATE / 1000.0;
		x = checkBoundary(x);
		y = checkBoundary(y);
		double speed = calcDist(dx, dy, 0.0, 0.0);
		angle = acos(dy / speed);
		if (dx > 0.0)
		{
			angle = -angle;
		}
	}

	Bird findNearestBird(double viewAngle, Bird birds[BIRDS_NO])
	{
		Bird nearestBird = birds[0];
		double minDist = calcDist(x, y, nearestBird.x, nearestBird.y);
		if (minDist == 0.0)
		{
			nearestBird = birds[1];
			minDist = calcDist(x, y, nearestBird.x, nearestBird.y);
		}
		for (int i = 1; i < BIRDS_NO; i++)
		{
			double dist = calcDist(x, y, birds[i].x, birds[i].y);
			if (minDist >= dist && dist != 0.0)
			{
				nearestBird = birds[i];
				minDist = dist;
			}
		}
		return nearestBird;
	}
};

Bird birds[BIRDS_NO];

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0);
	for (int i = 0; i < BIRDS_NO; i++)
	{
		birds[i].drawBird();
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
	double gx = 0.0, gy = 0.0;
	for (int i = 0; i < BIRDS_NO; i++)
	{
		birds[i].updatePosition(); //TODO:次の時間ステップにおける速度ベクトルの計算
		gx += birds[i].x;
		gy += birds[i].y;
	}
	gx /= double(BIRDS_NO);
	gy /= double(BIRDS_NO);
	double viewAngle = degreeToRadian(VIEW_ANGLE);
	for (int i = 0; i < BIRDS_NO; ++i)
	{
		double dist = calcDist(gx, gy, birds[i].x, birds[i].y);
		double gvx = 0.0;
		double gvy = 0.0;
		gvx = (gx - birds[i].x) / dist;
		gvy = (gy - birds[i].y) / dist;
		Bird nearestBird = birds[i].findNearestBird(viewAngle, birds);
		double nvx = 0.0;
		double nvy = 0.0;
		double birdDist = calcDist(nearestBird.x, nearestBird.y, birds[i].x, birds[i].y);
		nvx = (nearestBird.x - birds[i].x) / birdDist;
		nvy = (nearestBird.y - birds[i].y) / birdDist;
		if (birdDist < OPTIMUM_DISTANCE)
		{
			nvx = -nvx;
			nvy = -nvy;
		}
		double dv = calcDist(birds[i].dx, birds[i].dy, nearestBird.dx, nearestBird.dy);
		double avx = (nearestBird.dx - birds[i].dx)/dv;
		double avy = (nearestBird.dy - birds[i].dy)/dv;

		birds[i].dx += GRAVITY_WEIGHT * gvx + DISTANT_WEIGHT * nvx + ALIGNMENT_WEIGHT * avx;
		birds[i].dy += GRAVITY_WEIGHT * gvy + DISTANT_WEIGHT * nvy + ALIGNMENT_WEIGHT * avy;
	}
	//boid速度ベクトルの計算部分	}
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
		birds[i] = Bird((double(rand()) - RAND_MAX / 2.0) * BOUNDARY * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * BOUNDARY * 2.0 / RAND_MAX, double(rand()) / RAND_MAX * 2.0 * M_PI);
	}
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(FLAME_RATE, timer, time);
	glutMainLoop();
	return 0;
}
