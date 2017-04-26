// boid.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

//TODO: 群れを複数にできるようにしたい
//TODO: 捕食者
#include "stdafx.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>

#define BIRD_SIZE 0.5 //size of bird
#define BLOCK_SIZE 0.3 //size of block
#define BIRD_SPEED 3.0 //initial bird speed
#define BIRDS_NO 50 //number of birds
#define FLAME_RATE 100 //rerender after this FLAME_RATE milliseconds
#define WINDOW_SIZE 600 //window size
#define BOUNDARY 20.0 //area boundary
#define VIEW_ANGLE 360.0 //bird view angle: degree
#define OPTIMUM_DISTANCE 10.0 //optimum distance
#define GRAVITY_WEIGHT 0.1 //gravity point weight
#define ALIGNMENT_WEIGHT 0.2 //alignment weight
#define REPEL_WEIGHT 0.5 //repel force weight
#define ACCEL 1.3 //accelaration
#define MAXSPEED 8.0 //accelaration
#define MINSPEED 1.0//accelaration

int time = 0; //time
//For debug
//double posX = 0.0;
//double posY = 5.0;
//double initAngle = 90.0;

//境界条件: 壁
double checkBoundary(double pos)
{
	if (pos > BOUNDARY - BLOCK_SIZE)
	{
		pos = BOUNDARY - BLOCK_SIZE;
	}
	else if (pos < -BOUNDARY + BLOCK_SIZE)
	{
		pos = -BOUNDARY + BLOCK_SIZE;
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

	Vector(double _angle)
	{
		angle = _angle;
		x = -sin(_angle);
		y = cos(_angle);
	}

	Vector(double _x, double _y)
	{
		double dist = sqrt(_x * _x + _y * _y);
		x = _x / dist;
		y = _y / dist;
		angle = atan(-_x / _y);
		if (_y < 0.0)
		{
			angle += M_PI;
		}
	}
};

class Bird
{
public:
	double x; //_x-position
	double y; //y-position
	double angle; //radian angle: 0 vector is (0, 1)
	double speed; // vector

	Bird(double _x = 0.0, double _y = 0.0, double _angle = 0.0, double _speed = BIRD_SPEED)
	{
		x = _x;
		y = _y;
		angle = _angle;
		speed = _speed;
	}

	void drawBird() //TODO:鳥らしく
	{
		if (false)
		{
			glColor3d(1.0, 0.0, 0.0);
		}
		else
		{
			glColor3d(1.0, 1.0, 1.0);
		}
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
		Vector vector = Vector(angle);
		double dx = speed * vector.x;
		double dy = speed * vector.y;
		x += dx * FLAME_RATE / 1000.0;
		y += dy * FLAME_RATE / 1000.0;
		x = checkBoundary(x);
		y = checkBoundary(y);
		Vector nextVector = Vector(dx, dy);
		angle = nextVector.angle;
	}

	bool visible(double viewAngle, Bird bird)
	{
		double r = BOUNDARY / 2.0;
		if (r < calcDist(x, y, bird.x, bird.y))
		{
			return false;
		}
		double dx = bird.x - x;
		double dy = bird.y - y;
		Vector bVector = Vector(dx, dy);
		double maxAngle = angle + viewAngle;
		double minAngle = angle - viewAngle;
		bool max = maxAngle > 2.0 * M_PI ? bVector.angle > maxAngle - 2.0 * M_PI : bVector.angle > maxAngle;
		bool min = minAngle < 0.0 ? bVector.angle < minAngle + 2.0 * M_PI : bVector.angle < minAngle;
		if (max && min)
		{
			return false;
		}
		return true;
	}

	Bird findNearestBird(double viewAngle, Bird birds[BIRDS_NO]) //TODO:見つからない時の処理
	{
		Bird nearestBird = Bird(BOUNDARY * 2.0, BOUNDARY * 2.0);
		double minDist = 0.0;
		for (int i = 0; i < BIRDS_NO; i++) //TODO:ループを使わずに探索できるようにしたい
		{
			double dist = calcDist(x, y, birds[i].x, birds[i].y);
			if (visible(viewAngle, birds[i]) && (minDist == 0.0 || minDist >= dist) && dist != 0.0)
			{
				nearestBird = birds[i];
				minDist = dist;
			}
		}
		return nearestBird;
	}

	void updateAngleAndSpeed(double gx, double gy, double viewAngle, Bird birds[BIRDS_NO])
	{
		double dist = calcDist(gx, gy, x, y);
		double gvx = (gx - x) / dist;
		double gvy = (gy - y) / dist;
		Vector gVector = Vector(gvx, gvy);
		Bird nearestBird = findNearestBird(viewAngle, birds);
		Vector bSpeedVector = Vector(nearestBird.angle);
		Vector thisBirdVector = Vector(angle);
		double x_repel;
		double y_repel;
		double bx = nearestBird.x == BOUNDARY * 2.0 ? 0.0 : bSpeedVector.x;
		double by = nearestBird.y == BOUNDARY * 2.0 ? 0.0 : bSpeedVector.y;
		double vx = thisBirdVector.x + GRAVITY_WEIGHT * gVector.x + ALIGNMENT_WEIGHT * bx;
		double vy = thisBirdVector.y + GRAVITY_WEIGHT * gVector.y + ALIGNMENT_WEIGHT * by;
		//		double vx = thisBirdVector.x;
		//		double vy = thisBirdVector.y;
		if (x >= BOUNDARY - OPTIMUM_DISTANCE)
		{
			x_repel = -1.0 / (BOUNDARY - BLOCK_SIZE - x);
			vx += REPEL_WEIGHT * x_repel;
		}
		else if (x <= -BOUNDARY + OPTIMUM_DISTANCE)
		{
			x_repel = 1.0 / (BOUNDARY - BLOCK_SIZE + x);
			vx += REPEL_WEIGHT * x_repel;
		}
		if (y >= BOUNDARY - OPTIMUM_DISTANCE)
		{
			y_repel = -1.0 / (BOUNDARY - BLOCK_SIZE - y);
			vy += REPEL_WEIGHT * y_repel;
		}
		else if (y <= -BOUNDARY + OPTIMUM_DISTANCE)
		{
			y_repel = 1.0 / (BOUNDARY - BLOCK_SIZE + y);
			vy += REPEL_WEIGHT * y_repel;
		}
		if (nearestBird.x != BOUNDARY * 2.0)
		{
			double birdDist = calcDist(nearestBird.x, nearestBird.y, x, y);
			double nbx = (nearestBird.x - x) / birdDist;
			double nby = (nearestBird.y - y) / birdDist;

			Vector bVector = Vector(nbx, nby);
			double innerPrdct = thisBirdVector.x * bVector.x + thisBirdVector.y * bVector.y;
			if (birdDist < OPTIMUM_DISTANCE)
			{
				if (innerPrdct > 0)
				{
					//front
					speed /= ACCEL;
					if (speed < MINSPEED)
					{
						speed = MINSPEED;
					}
				}
				else
				{
					//back
					speed *= ACCEL;
					if (speed > MAXSPEED)
					{
						speed = MAXSPEED;
					}
				}
			}
			else if (birdDist > OPTIMUM_DISTANCE)
			{
				if (innerPrdct > 0)
				{
					//front
					speed *= ACCEL;
					if (speed > MAXSPEED)
					{
						speed = MAXSPEED;
					}
				}
				else
				{
					//back
					speed /= ACCEL;
					if (speed < MINSPEED)
					{
						speed = MINSPEED;
					}
				}
			}
		}


		angle = Vector(vx, vy).angle;
		//		if (i == 0)
		//		{
		//			printf("%.1f\n", radianToDegree(Vector(vx, vy).angle));
		//			printf("[%.1f, %.1f]\n", vx, vy);
		//			printf("[%.1f, %.1f]\n", x, y);
		//		}
	}
};

Bird birds[BIRDS_NO];

void drawWall()
{
	glColor3d(0.5, 0.5, 0.5);
	double boundary = BOUNDARY;
	glBegin(GL_POLYGON);
	glVertex2d(boundary, boundary);
	glVertex2d(boundary - BLOCK_SIZE, boundary);
	glVertex2d(boundary - BLOCK_SIZE, -boundary);
	glVertex2d(boundary, -boundary);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(boundary, boundary);
	glVertex2d(boundary, boundary - BLOCK_SIZE);
	glVertex2d(-boundary, boundary - BLOCK_SIZE);
	glVertex2d(-boundary, boundary);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-boundary, -boundary);
	glVertex2d(-boundary, -boundary + BLOCK_SIZE);
	glVertex2d(boundary, -boundary + BLOCK_SIZE);
	glVertex2d(boundary, -boundary);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-boundary, -boundary);
	glVertex2d(-boundary + BLOCK_SIZE, -boundary);
	glVertex2d(-boundary + BLOCK_SIZE, boundary);
	glVertex2d(-boundary, boundary);
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	drawWall();
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
		birds[i].updateAngleAndSpeed(gx, gy, viewAngle, birds);
	}
	//boid速度ベクトルの計算部分
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
		birds[i] = Bird((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - 0.5) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - 0.5) * 2.0 / RAND_MAX, (double(rand()) / RAND_MAX) * 2.0 * M_PI);
		//		birds[i] = Bird(posX, posY, initAngle / 180.0 * M_PI);
	}
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(FLAME_RATE, timer, time);
	glutMainLoop();
	return 0;
}
