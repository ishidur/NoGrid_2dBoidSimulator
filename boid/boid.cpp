// boid.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

//TODO: 群れを複数にできるようにしたい
//TODO: 捕食者
//TODO: ファイルを分けたい
#include "stdafx.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

#define GRID_NO 15 //number of grid; boid can see around 1 grid
#define BOID_SIZE 0.5 //size of boid
#define BLOCK_SIZE 0.3 //size of block
#define BOID_SPEED 3.0 //initial boid speed
#define BOIDS_NO 50 //number of boids
#define FLAME_RATE 100 //rerender after this FLAME_RATE milliseconds
#define WINDOW_SIZE 600 //window size
#define BOUNDARY 20.0 //area boundary
#define VIEW_ANGLE 360.0 //boid view angle: degree
#define REPEL_WEIGHT 0.5 //repel force weight

#define OPTIMUM_DISTANCE 10.0 //optimum distance
#define CENTRIPETAL_WEIGHT 0.1 //centripetal  weight
#define ALIGNMENT_WEIGHT 0.2 //alignment weight
#define ACCEL 1.3 //accelaration
#define MAXSPEED 8.0 //boid: max speed
#define MINSPEED 2.0//boid: minimum speed

int time = 0; //time
//For debug
//double posX = 1.0;
//double posY = 1.0;
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

class Direction
{
public:
	double angle;
	double x;
	double y;

	Direction(double _angle)
	{
		angle = _angle;
		x = -sin(_angle);
		y = cos(_angle);
	}

	Direction(double _x, double _y)
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

class Grid
{
public:
	double left;
	double right;
	double top;
	double bottom;
	vector<int> boidIndexes;

	Grid(double _top = 0.0, double _bottom = 0.0, double _left = 0.0, double _right = 0.0)
	{
		left = _left;
		right = _right;
		top = _top;
		bottom = _bottom;
	}

	void addBoidByIndex(int index)
	{
		boidIndexes.push_back(index);
		sort(boidIndexes.begin(), boidIndexes.end());
	}

	void deleteBoidByIndex(int index)
	{
		if (find(boidIndexes.begin(), boidIndexes.end(), index) != boidIndexes.end())
		{
			remove(boidIndexes.begin(), boidIndexes.end(), index);
			boidIndexes.pop_back();
		}
	}

	bool findBoidByIndex(int index)
	{
		if (find(boidIndexes.begin(), boidIndexes.end(), index) != boidIndexes.end())
		{
			return true;
		}
		return false;
	}
};

Grid grids[GRID_NO][GRID_NO];

//this function needs grids
vector<int> getAroundGridBoids(int id, int grid_x, int grid_y)
{
	vector<int> indexes = grids[grid_y][grid_x].boidIndexes;
	if (grid_y > 0)
	{
		indexes.insert(indexes.end(), grids[grid_y - 1][grid_x].boidIndexes.begin(), grids[grid_y - 1][grid_x].boidIndexes.end());
		if (grid_x > 0)
		{
			indexes.insert(indexes.end(), grids[grid_y - 1][grid_x - 1].boidIndexes.begin(), grids[grid_y - 1][grid_x - 1].boidIndexes.end());
		}
		if (grid_x < GRID_NO - 1)
		{
			indexes.insert(indexes.end(), grids[grid_y - 1][grid_x + 1].boidIndexes.begin(), grids[grid_y - 1][grid_x + 1].boidIndexes.end());
		}
	}
	if (grid_y < GRID_NO - 1)
	{
		indexes.insert(indexes.end(), grids[grid_y + 1][grid_x].boidIndexes.begin(), grids[grid_y + 1][grid_x].boidIndexes.end());
		if (grid_x > 0)
		{
			indexes.insert(indexes.end(), grids[grid_y + 1][grid_x - 1].boidIndexes.begin(), grids[grid_y + 1][grid_x - 1].boidIndexes.end());
		}
		if (grid_x < GRID_NO - 1)
		{
			indexes.insert(indexes.end(), grids[grid_y + 1][grid_x + 1].boidIndexes.begin(), grids[grid_y + 1][grid_x + 1].boidIndexes.end());
		}
	}
	if (grid_x > 0)
	{
		indexes.insert(indexes.end(), grids[grid_y][grid_x - 1].boidIndexes.begin(), grids[grid_y][grid_x - 1].boidIndexes.end());
	}
	if (grid_x < GRID_NO - 1)
	{
		indexes.insert(indexes.end(), grids[grid_y][grid_x + 1].boidIndexes.begin(), grids[grid_y][grid_x + 1].boidIndexes.end());
	}

	auto result = remove(indexes.begin(), indexes.end(), id);
	auto result2 = unique(indexes.begin(), result);
	indexes.erase(result2, indexes.end());
	return indexes;
}

//this needs for Biod::isVisible
double viewAngle = degreeToRadian(VIEW_ANGLE) / 2.0;

class Boid
{
private:
	double r = 1.0; //color; red
	double g = 1.0; //color; green
	double b = 1.0; //color; blue
public:
	int id = -1; //id
	double x; //_x-position
	double y; //y-position
	double angle; //radian angle: 0 vector is (0, 1)
	double speed; // speed
	int grid_y = -1; //grid address y
	int grid_x = -1; //grid address x

	Boid(double _x = 0.0, double _y = 0.0, double _angle = 0.0, double _speed = BOID_SPEED, int _id = -1)
	{
		id = _id;
		x = _x;
		y = _y;
		angle = _angle;
		speed = _speed;
	}

	void setColor(double red, double green, double blue)
	{
		r = red;
		g = green;
		b = blue;
	}

	void drawBoid() //TODO:鳥らしく
	{
		glColor3d(r, g, b);
		glPushMatrix();
		glTranslated(x, y, 0.0);
		glRotated(radianToDegree(angle), 0.0, 0.0, 1.0);
		glBegin(GL_POLYGON);
		glVertex2d(0.0, BOID_SIZE);
		glVertex2d(-0.4 * BOID_SIZE * sqrt(3.0) / 2.0, -BOID_SIZE / 2.0);
		glVertex2d(0.4 * BOID_SIZE * sqrt(3.0) / 2.0, -BOID_SIZE / 2.0);
		glEnd();
		glPopMatrix();
	}

	void updatePosition()
	{
		Direction vector = Direction(angle);
		double dx = speed * vector.x;
		double dy = speed * vector.y;
		x += dx * FLAME_RATE / 1000.0;
		y += dy * FLAME_RATE / 1000.0;
		x = checkBoundary(x);
		y = checkBoundary(y);
		Direction nextDirection = Direction(dx, dy);
		angle = nextDirection.angle;
	}

	bool isVisible(Boid boid)
	{
		double dx = boid.x - x;
		double dy = boid.y - y;
		Direction bDirection = Direction(dx, dy);
		double maxAngle = angle + viewAngle;
		double minAngle = angle - viewAngle;
		bool max = maxAngle > 2.0 * M_PI ? bDirection.angle > maxAngle - 2.0 * M_PI : bDirection.angle > maxAngle;
		bool min = minAngle < 0.0 ? bDirection.angle < minAngle + 2.0 * M_PI : bDirection.angle < minAngle;
		if (max && min)
		{
			return false;
		}
		return true;
	}

	Boid findNearestBoid(vector<Boid> boids)
	{
		Boid nearestBoid;
		double minDist = 0.0;
		auto indexes = getAroundGridBoids(id, grid_x, grid_y);
		if (id == 0)
		{
			setColor(1.0, indexes.size() / double(BOIDS_NO / 2), 0.0);
		}
		for (auto i: indexes)
		{
			double dist = calcDist(x, y, boids[i].x, boids[i].y);
			if (isVisible(boids[i]) && (minDist == 0.0 || minDist >= dist) && dist != 0.0)
			{
				nearestBoid = boids[i];
				minDist = dist;
			}
		}
		return nearestBoid;
	}
	
	//it is too long
	void updateAngleAndSpeed(double gx, double gy, vector<Boid> boids)
	{
		double dist = calcDist(gx, gy, x, y);
		double gvx = (gx - x) / dist;
		double gvy = (gy - y) / dist;
		Direction gDirection = Direction(gvx, gvy);
		Boid nearestBoid = findNearestBoid(boids);
		Direction bSpeedDirection = Direction(nearestBoid.angle);
		Direction thisBoidDirection = Direction(angle);
		double repel;
		double bx = bSpeedDirection.x;
		double by = bSpeedDirection.y;
		if (nearestBoid.id == -1)
		{
			bx = 0.0;
			by = 0.0;
		}
		double vx = thisBoidDirection.x + CENTRIPETAL_WEIGHT * gDirection.x + ALIGNMENT_WEIGHT * bx;
		double vy = thisBoidDirection.y + CENTRIPETAL_WEIGHT * gDirection.y + ALIGNMENT_WEIGHT * by;
		//		double vx = thisBoidDirection.x;
		//		double vy = thisBoidDirection.y;
		if (x >= BOUNDARY - OPTIMUM_DISTANCE)
		{
			repel = -1.0 / (BOUNDARY - BLOCK_SIZE - x);
			vx += REPEL_WEIGHT * repel;
		}
		else if (x <= -BOUNDARY + OPTIMUM_DISTANCE)
		{
			repel = 1.0 / (BOUNDARY - BLOCK_SIZE + x);
			vx += REPEL_WEIGHT * repel;
		}
		if (y >= BOUNDARY - OPTIMUM_DISTANCE)
		{
			repel = -1.0 / (BOUNDARY - BLOCK_SIZE - y);
			vy += REPEL_WEIGHT * repel;
		}
		else if (y <= -BOUNDARY + OPTIMUM_DISTANCE)
		{
			repel = 1.0 / (BOUNDARY - BLOCK_SIZE + y);
			vy += REPEL_WEIGHT * repel;
		}
		if (nearestBoid.x != BOUNDARY * 2.0)
		{
			double boidDist = calcDist(nearestBoid.x, nearestBoid.y, x, y);
			double nbx = (nearestBoid.x - x) / boidDist;
			double nby = (nearestBoid.y - y) / boidDist;

			Direction bDirection = Direction(nbx, nby);
			double innerPrdct = thisBoidDirection.x * bDirection.x + thisBoidDirection.y * bDirection.y;
			if (boidDist < OPTIMUM_DISTANCE)
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
			else if (boidDist > OPTIMUM_DISTANCE)
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
		angle = Direction(vx, vy).angle;
	}
};

vector<Boid> boids;

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

//this function needs grids
void createGrids()
{
	double width = 2.0 * BOUNDARY / GRID_NO;
	double left;
	double top = BOUNDARY;
	for (int i = 0; i < GRID_NO; ++i)
	{
		left = -BOUNDARY;
		for (int j = 0; j < GRID_NO; ++j)
		{
			grids[i][j] = Grid(top, top - width, left, left + width);
			left += width;
		}
		top -= width;
	}
}

//this function needs grids, boids
void updateGrid()
{
	for (int i = 0; i < GRID_NO; i++)
	{
		for (int j = 0; j < GRID_NO; j++)
		{
			vector<int> indexes = grids[i][j].boidIndexes;
			for (auto n : indexes)
			{
				if (boids[n].grid_y != i || boids[n].grid_x != j)
				{
					grids[i][j].deleteBoidByIndex(n);
				}
			}
		}
	}
}

void coloringGrid()
{
	for (int i = 0; i < GRID_NO; ++i)
	{
		for (int j = 0; j < GRID_NO; ++j)
		{
			glColor3d(double(i) / GRID_NO, double(j) / GRID_NO, 0.0);
			if (find(grids[i][j].boidIndexes.begin(), grids[i][j].boidIndexes.end(), 0) != grids[i][j].boidIndexes.end())
			{
				glColor3d(0.0, 0.0, 1.0);
			}
			glBegin(GL_POLYGON);
			glVertex2d(grids[i][j].left, grids[i][j].top);
			glVertex2d(grids[i][j].left, grids[i][j].bottom);
			glVertex2d(grids[i][j].right, grids[i][j].bottom);
			glVertex2d(grids[i][j].right, grids[i][j].top);
			glEnd();
		}
	}
}

//this function needs grids, boids
void findGrid(int index, double x, double y)
{
	double width = 2.0 * BOUNDARY / GRID_NO;
	int gridx = int(ceil((BOUNDARY + x) / width)) - 1;
	int gridy = int(ceil((BOUNDARY - y) / width)) - 1;
	grids[gridy][gridx].addBoidByIndex(index);
	boids[index].grid_x = gridx;
	boids[index].grid_y = gridy;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	coloringGrid();
	drawWall();
	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids[i].drawBoid();
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
	//	printf("%d\n", time);
	double gx = 0.0, gy = 0.0;
	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids[i].updatePosition(); //TODO:次の時間ステップにおける速度ベクトルの計算
		gx += boids[i].x;
		gy += boids[i].y;
		findGrid(i, boids[i].x, boids[i].y);
	}
	updateGrid();
	gx /= double(BOIDS_NO);
	gy /= double(BOIDS_NO);
	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids[i].updateAngleAndSpeed(gx, gy, boids);
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
	createGrids();

	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids.push_back(Boid((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - 0.5) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - 0.5) * 2.0 / RAND_MAX, (double(rand()) / RAND_MAX) * 2.0 * M_PI,BOID_SPEED, i));
		//		boids[i] = Boid(posX, posY + i, initAngle / 180.0 * M_PI,BOID_SPEED, i);
		findGrid(i, boids[i].x, boids[i].y);
		if (i == 0)
		{
			boids[i].setColor(1.0, 0.0, 0.0);
		}
	}
	updateGrid();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(FLAME_RATE, timer, time);
	glutMainLoop();
	return 0;
}
