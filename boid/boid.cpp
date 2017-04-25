// boid.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>

#define BIRD_SIZE 0.3 //size of bird
#define BIRD_SPEED 2.0 //initial bird speed
#define BIRDS_NO 30 //number of birds
#define FLAME_RATE 100 //rerender after this FLAME_RATE milliseconds
#define WINDOW_SIZE 600 //window size
#define BOUNDARY 10.0 //area boundary
#define VIEW_ANGLE 30.0 //bird view angle: degree
#define OPTIMUM_DISTANCE 2.0 //
#define GRAVITY_WEIGHT 0.5 //gravity point weight
#define DISTANT_WEIGHT 1.0 //nearest bird weight
#define ALIGNMENT_WEIGHT 0.8 //alignment weight
#define ACCEL 1.2 //accelaration

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

class Vector
{
public:
	double angle;
	double x;
	double y;

//	Vector(double _angle)
//	{
//		angle = _angle;
//		x = sin(_angle);
//		y = cos(_angle);
//	}

	Vector(double _x, double _y)
	{
		double dist = sqrt(_x * _x + _y * _y);
		x = _x / dist;
		y = _y / dist;
		angle = -atan(_x / _y);
		if (_y < 0.0)
		{
			angle += M_PI * 3.0 / 2.0;
		}
	}
};

// TODO:Boidモデルの速度ベクトルの計算を実装
class Bird
{
public:
	double x; //_x-position
	double y; //y-position
	double angle; //radian angle: 0 vector is (0, 1)
	Vector vector; // y-direction speed
	double speed; // y-direction speed

	Bird(double _x = 0.0, double _y = 0.0, double _angle = 0.0, double _speed = BIRD_SPEED)
	{
		x = _x;
		y = _y;
		angle = _angle;
		speed = _speed;
		vector = Vector(sin(_angle),cos(_angle));
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
		double dx = speed * vector.x;
		double dy = speed * vector.y;
		x += dx * FLAME_RATE / 1000.0;
		y += dy * FLAME_RATE / 1000.0;
		x = checkBoundary(x);
		y = checkBoundary(y);
		angle = -atan(dx / dy);
		if (dy < 0.0)
		{
			angle += M_PI * 3.0 / 2.0;
		}
	}

	bool visible(Bird bird, double viewAngle)
	{
		double minView = angle - viewAngle;
		double maxView = angle + viewAngle;
		double direction = atan((bird.x - x) / (bird.y - y));
		if ((bird.y - y) < 0.0)
		{
			direction += M_PI * 3.0 / 2.0;
		}
		bool min = minView < 0.0 ? minView + 2.0 * M_PI <= direction : minView <= direction;
		bool max = maxView > 2.0 * M_PI ? maxView - 2.0 * M_PI >= direction : maxView >= direction;
		if (min && max)
		{
			return true;
		}
		return false;
	}

	Bird findNearestBird(double viewAngle, Bird birds[BIRDS_NO]) //TODO:視野を設定
	{
		Bird nearestBird;
		double minDist = 0.0;
		for (int i = 0; i < BIRDS_NO; i++) //TODO:ループを使わずに探索できるようにしたい
		{
			double dist = calcDist(x, y, birds[i].x, birds[i].y);
			if ((minDist == 0.0 || minDist >= dist) && dist != 0.0)
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
	double viewAngle = degreeToRadian(VIEW_ANGLE) / 2.0;
	for (int i = 0; i < BIRDS_NO; ++i)
	{
		double dist = calcDist(gx, gy, birds[i].x, birds[i].y);
		double gvx = (gx - birds[i].x) / dist;
		double gvy = (gy - birds[i].y) / dist;
		Vector gVector = Vector(gvx, gvy);
		Bird nearestBird = birds[i].findNearestBird(viewAngle, birds);
		Vector bSpeedVector = nearestBird.vector;
		double vx = birds[i].vector.x + GRAVITY_WEIGHT * gVector.x + ALIGNMENT_WEIGHT * bSpeedVector.x;
		double vy = birds[i].vector.y + GRAVITY_WEIGHT * gVector.y + ALIGNMENT_WEIGHT * bSpeedVector.y;
		birds[i].vector = Vector(vx, vy);
//		double birdDist = calcDist(nearestBird.x, nearestBird.y, birds[i].x, birds[i].y);
//		double nbx = (nearestBird.x - birds[i].x) / birdDist;
//		double nby = (nearestBird.y - birds[i].y) / birdDist;
//		Vector bVector = Vector(nbx, nby);
//		if (birdDist < OPTIMUM_DISTANCE)
//		{
//			birds[i].speed /= ACCEL;
//		}
//		else if (birdDist > OPTIMUM_DISTANCE)
//		{
//			birds[i].speed *= ACCEL;
//		}
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
