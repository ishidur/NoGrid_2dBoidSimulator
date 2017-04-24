// boid.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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

int time = 0; //time

//�����I���E����
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

// TODO:Boid���f���̑��x�x�N�g���̌v�Z������
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

	void drawBird() //TODO:���炵��
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
		angle = -atan(dx / dy);
		if (dy < 0.0)
		{
			angle += M_PI;
		}
	}

	bool visible(Bird bird, double viewAngle)
	{
		double minView = angle - viewAngle;
		double maxView = angle + viewAngle;
		double direction = atan((bird.x - x) / (bird.y - y));
		if ((bird.y - y) < 0.0)
		{
			direction += M_PI;
		}
		bool min = minView < -M_PI / 2.0 ? minView + 2.0 * M_PI <= direction : minView <= direction;
		bool max = maxView > 3.0 * M_PI / 2.0 ? maxView - 2.0 * M_PI >= direction : maxView >= direction;
		if (min && max)
		{
			return true;
		}
		return false;
	}

	Bird findNearestBird(double viewAngle, Bird birds[BIRDS_NO]) //TODO:�����ݒ�
	{
		Bird nearestBird;
		double minDist = 0.0;
		for (int i = 0; i < BIRDS_NO; i++) //TODO:���[�v���g�킸�ɒT���ł���悤�ɂ�����
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
		birds[i].updatePosition(); //TODO:���̎��ԃX�e�b�v�ɂ����鑬�x�x�N�g���̌v�Z
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
		Bird nearestBird = birds[i].findNearestBird(viewAngle, birds);
		double birdDist = calcDist(nearestBird.x, nearestBird.y, birds[i].x, birds[i].y);
		double nvx = (nearestBird.x - birds[i].x) / birdDist;
		double nvy = (nearestBird.y - birds[i].y) / birdDist;
		if (birdDist < OPTIMUM_DISTANCE)
		{
			nvx = -nvx;
			nvy = -nvy;
		}
		double dv = calcDist(birds[i].dx, birds[i].dy, nearestBird.dx, nearestBird.dy);
		double avx = (nearestBird.dx - birds[i].dx) / dv;
		double avy = (nearestBird.dy - birds[i].dy) / dv;

		birds[i].dx += GRAVITY_WEIGHT * gvx + DISTANT_WEIGHT * nvx + ALIGNMENT_WEIGHT * avx;
		birds[i].dy += GRAVITY_WEIGHT * gvy + DISTANT_WEIGHT * nvy + ALIGNMENT_WEIGHT * avy;
	}
	//boid���x�x�N�g���̌v�Z����	}
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
