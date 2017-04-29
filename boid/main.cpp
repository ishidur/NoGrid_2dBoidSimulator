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
#include "Grid.h"
#include "BaseBoid.h"
#include "Direction.h"
#include "parameters.h" //import common parameters
using namespace std;


int time = 0; //time
//For debug
//double posX = 1.0;
//double posY = 1.0;
//double initAngle = 90.0;

double calcDist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

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

vector<BaseBoid> boids;

BaseBoid findNearestBoid(BaseBoid boid)
{
	BaseBoid nearestBaseBoid;
	double minDist = 0.0;
	auto indexes = getAroundGridBoids(boid.id, boid.grid_x, boid.grid_y);
	for (auto i : indexes)
	{
		double dist = calcDist(boid.x, boid.y, boids[i].x, boids[i].y);
		if (boid.id == 0 && boid.isVisible(boids[i]))
		{
			if (dist > OPTIMUM_DISTANCE)
			{
				boids[i].setColor(0.1, 0.1, 1.0);
			}
			else
			{
				boids[i].setColor(0.0, 1.0, 1.0);
			}
		}

		if (boid.isVisible(boids[i]) && (minDist == 0.0 || minDist >= dist) && dist != 0.0)
		{
			nearestBaseBoid = boids[i];
			minDist = dist;
		}
	}
	return nearestBaseBoid;
}

//TODO: it is too long
BaseBoid updateAngleAndSpeed(double gx, double gy, BaseBoid boid)
{
	
	double dist = calcDist(gx, gy, boid.x, boid.y);
	double gvx = (gx - boid.x) / dist;
	double gvy = (gy - boid.y) / dist;
	Direction gDirection = Direction(gvx, gvy);
	BaseBoid nearestBaseBoid = findNearestBoid(boid);
	Direction bSpeedDirection = Direction(nearestBaseBoid.angle);
	Direction thisBaseBoidDirection = Direction(boid.angle);
	double repel;
	double bx = nearestBaseBoid.x == BOUNDARY * 2.0 ? 0.0 : bSpeedDirection.x;
	double by = nearestBaseBoid.y == BOUNDARY * 2.0 ? 0.0 : bSpeedDirection.y;
	double vx = thisBaseBoidDirection.x + CENTRIPETAL_WEIGHT * gDirection.x + ALIGNMENT_WEIGHT * bx;
	double vy = thisBaseBoidDirection.y + CENTRIPETAL_WEIGHT * gDirection.y + ALIGNMENT_WEIGHT * by;
	//		double vx = thisBaseBoidDirection.x;
	//		double vy = thisBaseBoidDirection.y;
	if (boid.x >= BOUNDARY - OPTIMUM_DISTANCE)
	{
		repel = -1.0 / (BOUNDARY - BLOCK_SIZE - boid.x);
		vx += REPEL_WEIGHT * repel;
	}
	else if (boid.x <= -BOUNDARY + OPTIMUM_DISTANCE)
	{
		repel = 1.0 / (BOUNDARY - BLOCK_SIZE + boid.x);
		vx += REPEL_WEIGHT * repel;
	}
	if (boid.y >= BOUNDARY - OPTIMUM_DISTANCE)
	{
		repel = -1.0 / (BOUNDARY - BLOCK_SIZE - boid.y);
		vy += REPEL_WEIGHT * repel;
	}
	else if (boid.y <= -BOUNDARY + OPTIMUM_DISTANCE)
	{
		repel = 1.0 / (BOUNDARY - BLOCK_SIZE + boid.y);
		vy += REPEL_WEIGHT * repel;
	}

	if (nearestBaseBoid.x != BOUNDARY * 2.0)
	{
		double boidDist = calcDist(nearestBaseBoid.x, nearestBaseBoid.y, boid.x, boid.y);
		double nbx = (nearestBaseBoid.x - boid.x) / boidDist;
		double nby = (nearestBaseBoid.y - boid.y) / boidDist;
		//TODO: 加減速正しい？
		Direction bDirection = Direction(nbx, nby);
		double innerPrdct = thisBaseBoidDirection.x * bDirection.x + thisBaseBoidDirection.y * bDirection.y;
		if (boidDist < OPTIMUM_DISTANCE)
		{
			//close
			if (innerPrdct >= 0.0)
			{
				//front
				boid.speed /= ACCEL;
			}
			else
			{
				//back
				boid.speed *= ACCEL;
			}
		}
		else if (boidDist > OPTIMUM_DISTANCE)
		{
			//far
			if (innerPrdct >= 0.0)
			{
				//front
				boid.speed *= ACCEL;
			}
			else
			{
				//back
				boid.speed /= ACCEL;
			}
		}
		if (boid.speed < MINSPEED)
		{
			boid.speed = MINSPEED;
		}
		else if (boid.speed > MAXSPEED)
		{
			boid.speed = MAXSPEED;
		}
	}
	boid.angle = Direction(vx, vy).angle;
	return boid;
}

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

//this function needs grids, boids: 非効率かも
void updateGrids()
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

void coloringGrids()
{
	for (int i = 0; i < GRID_NO; ++i)
	{
		for (int j = 0; j < GRID_NO; ++j)
		{
			glColor3d(double(i) / GRID_NO, double(j) / GRID_NO, 0.0);
			if (find(grids[i][j].boidIndexes.begin(), grids[i][j].boidIndexes.end(), 0) != grids[i][j].boidIndexes.end())
			{
				glColor3d(0.3, 0.3, 0.3);
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
	coloringGrids();
	drawWall();
	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids[i].drawBaseBoid();
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
	//	if (time % 10 == 0)
	//	{
	//		cout << time / 10 << endl;
	//	}
	double gx = 0.0, gy = 0.0;
	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids[i].updatePosition();
		gx += boids[i].x;
		gy += boids[i].y;
		if (i != 0)
		{
			boids[i].setColor(1.0, 1.0, 1.0);
		}
		findGrid(i, boids[i].x, boids[i].y);
	}
	updateGrids();
	gx /= double(BOIDS_NO);
	gy /= double(BOIDS_NO);
	for (int i = 0; i < BOIDS_NO; i++)
	{
		//boid速度ベクトルの計算部分
		boids[i] = updateAngleAndSpeed(gx, gy, boids[i]);
		//		boids[i].updateAngleAndSpeed(gx, gy, boids);
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
	createGrids();

	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids.push_back(BaseBoid((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - 0.5) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - 0.5) * 2.0 / RAND_MAX, (double(rand()) / RAND_MAX) * 2.0 * M_PI,BOID_SPEED, i));
		//		boids[i] = BaseBoid(posX, posY + i, initAngle / 180.0 * M_PI,BOID_SPEED, i);
		findGrid(i, boids[i].x, boids[i].y);
		if (i == 0)
		{
			boids[i].setColor(1.0, 0.0, 0.0);
		}
	}
	updateGrids();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(FLAME_RATE, timer, time);
	glutMainLoop();
	return 0;
}
