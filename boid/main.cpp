// boid.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

//TODO: 群れを複数にできるようにしたい
//TODO: 捕食者
//TODO: ブロックを消せるようにしたい 
//TODO: 引き寄せるブロックも作りたい 
//TODO: ブロックの中に入れないようにしたい
#include "stdafx.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include "Grid.h"
#include "BaseBoid.h"
#include "Direction.h"
#include "Block.h"
#include "parameters.h" //import common parameters
#include "Eigen/Core"

int time = 0; //time
bool isPress = false;
double mouseX = 0.0;
double mouseY = 0.0;

double calcDist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

Grid grids[GRID_NO][GRID_NO];

std::vector<Block> blocks;

//this function needs grids
std::vector<int> getAroundGridBoids(int id, int grid_x, int grid_y)
{
	std::vector<int> indexes = grids[grid_y][grid_x].boidIndexes;
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

std::vector<BaseBoid> boids;

std::tuple<BaseBoid, Eigen::Vector2d> findNearestBoid(BaseBoid& boid)
{
	BaseBoid nearestBaseBoid;
	double minDist = 0.0;
	Eigen::Vector2d g = Eigen::Vector2d::Zero();
	auto indexes = getAroundGridBoids(boid.id, boid.grid_x, boid.grid_y);
	for (auto i : indexes)
	{
		double dist = calcDist(boid.x, boid.y, boids[i].x, boids[i].y);
		g.x() += boids[i].x;
		g.y() += boids[i].y;
		if (boid.id == 0 && boid.isVisible(boids[i].x, boids[i].y))
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

		if (boid.isVisible(boids[i].x, boids[i].y) && (minDist == 0.0 || minDist >= dist) && dist != 0.0)
		{
			nearestBaseBoid = boids[i];
			minDist = dist;
		}
	}
	if (indexes.size() != 0)
	{
		g.x() /= indexes.size();
		g.y() /= indexes.size();
	}
	return std::forward_as_tuple(nearestBaseBoid, g);
}

Eigen::Vector2d repelWall(Eigen::Vector2d& p, BaseBoid& boid)
{
	Eigen::Vector2d repel = Eigen::Vector2d::Zero();
	double dist = BOUNDARY - WALL_SIZE;
	double bound = dist - BOID_SIZE - OPTIMUM_DISTANCE;
	if (boid.x >= bound)
	{
		repel.x() = -1.0 / (dist - boid.x);
	}
	else if (boid.x <= -bound)
	{
		repel.x() = 1.0 / (dist + boid.x);
	}
	if (boid.y >= bound)
	{
		repel.y() = -1.0 / (dist - boid.y);
	}
	else if (boid.y <= -bound)
	{
		repel.y() = 1.0 / (dist + boid.y);
	}
	p = p + REPEL_WALL_WEIGHT * repel;
	return p;
}

Eigen::Vector2d repelBlock(Eigen::Vector2d& p, BaseBoid& boid)
{
	for (auto n : grids[boid.grid_y][boid.grid_x].blockIndexes)
	{
		double dist = calcDist(boid.x, boid.y, blocks[n].x, blocks[n].y);
		if (dist - BLOCK_SIZE - BOID_SIZE <= OPTIMUM_DISTANCE && !blocks[n].disabled)
		{
			// repel
			p.x() += -REPEL_WEIGHT * (blocks[n].x - boid.x) / dist / dist * OPTIMUM_DISTANCE;
			p.y() += -REPEL_WEIGHT * (blocks[n].y - boid.y) / dist / dist * OPTIMUM_DISTANCE;
		}
	}
	if (isPress)
	{
		double dist = calcDist(boid.x, boid.y, mouseX, mouseY);
		if (dist <= OPTIMUM_DISTANCE)
		{
			// repel
			p.x() += -REPEL_WEIGHT * (mouseX - boid.x) / dist / dist * OPTIMUM_DISTANCE;
			p.y() += -REPEL_WEIGHT * (mouseY - boid.y) / dist / dist * OPTIMUM_DISTANCE;
		}
	}
	return p;
}

//TODO: it is too long
BaseBoid updateAngleAndSpeed(BaseBoid& boid)
{
	BaseBoid nearestBaseBoid;
	Eigen::Vector2d g;
	std::tie(nearestBaseBoid, g) = findNearestBoid(boid);
	double dist = calcDist(g.x(), g.y(), boid.x, boid.y);
	Eigen::Vector2d gv = Eigen::Vector2d(g.x() - boid.x, g.y() - boid.y);
	gv /= dist;
	Eigen::Vector2d gd = Eigen::Vector2d::Zero();
	if (g != Eigen::Vector2d::Zero())
	{
		gd = gv;
	}
	Direction bSpeedDirection = Direction(nearestBaseBoid.angle);
	Direction thisBaseBoidDirection = Direction(boid.angle);
	Eigen::Vector2d nbv = Eigen::Vector2d::Zero();
	if (nearestBaseBoid.id != -1)
	{
		nbv = Eigen::Vector2d(bSpeedDirection.x, bSpeedDirection.y);
		double boidDist = calcDist(nearestBaseBoid.x, nearestBaseBoid.y, boid.x, boid.y);
		Eigen::Vector2d nb = Eigen::Vector2d((nearestBaseBoid.x - boid.x), (nearestBaseBoid.y - boid.y));
		Direction bDirection = Direction(nb);
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
	Eigen::Vector2d v = thisBaseBoidDirection.vector + CENTRIPETAL_WEIGHT * gd + ALIGNMENT_WEIGHT * nbv;
	v = repelWall(v, boid);
	v = repelBlock(v, boid);

	boid.angle = Direction(v).angle;
	boid.vctr = Eigen::Vector2d(-sin(boid.angle) * boid.speed, cos(boid.angle) * boid.speed);
	return boid;
}

void drawWall()
{
	glColor3d(0.5, 0.5, 0.5);
	double boundary = BOUNDARY;
	glBegin(GL_POLYGON);
	glVertex2d(boundary, boundary);
	glVertex2d(boundary - WALL_SIZE, boundary);
	glVertex2d(boundary - WALL_SIZE, -boundary);
	glVertex2d(boundary, -boundary);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(boundary, boundary);
	glVertex2d(boundary, boundary - WALL_SIZE);
	glVertex2d(-boundary, boundary - WALL_SIZE);
	glVertex2d(-boundary, boundary);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-boundary, -boundary);
	glVertex2d(-boundary, -boundary + WALL_SIZE);
	glVertex2d(boundary, -boundary + WALL_SIZE);
	glVertex2d(boundary, -boundary);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex2d(-boundary, -boundary);
	glVertex2d(-boundary + WALL_SIZE, -boundary);
	glVertex2d(-boundary + WALL_SIZE, boundary);
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
			std::vector<int> indexes = grids[i][j].boidIndexes;
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
			//			if (find(grids[i][j].blockIndexes.begin(), grids[i][j].blockIndexes.end(), 0) != grids[i][j].blockIndexes.end())
			//			{
			//				glColor3d(0.3, 0.3, 0.3);
			//			}
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

void whereBlock(int index, double x, double y)
{
	double width = 2.0 * BOUNDARY / GRID_NO;
	int gridx = int(ceil((BOUNDARY + x) / width)) - 1;
	int gridy = int(ceil((BOUNDARY - y) / width)) - 1;
	grids[gridy][gridx].addBlockByIndex(index);
	if (gridx > 0)
	{
		grids[gridy][gridx - 1].addBlockByIndex(index);
		if (gridy > 0)
		{
			grids[gridy - 1][gridx - 1].addBlockByIndex(index);
		}
		if (gridy < GRID_NO - 1)
		{
			grids[gridy + 1][gridx - 1].addBlockByIndex(index);
		}
	}
	if (gridx < GRID_NO - 1)
	{
		grids[gridy][gridx + 1].addBlockByIndex(index);
		if (gridy > 0)
		{
			grids[gridy - 1][gridx + 1].addBlockByIndex(index);
		}
		if (gridy < GRID_NO - 1)
		{
			grids[gridy + 1][gridx + 1].addBlockByIndex(index);
		}
	}
	if (gridy > 0)
	{
		grids[gridy - 1][gridx].addBlockByIndex(index);
	}
	if (gridy < GRID_NO - 1)
	{
		grids[gridy + 1][gridx].addBlockByIndex(index);
	}
}

void removeBlock(int index, double x, double y)
{
	double width = 2.0 * BOUNDARY / GRID_NO;
	int gridx = int(ceil((BOUNDARY + x) / width)) - 1;
	int gridy = int(ceil((BOUNDARY - y) / width)) - 1;
	grids[gridy][gridx].deleteBlockByIndex(index);
	if (gridx > 0)
	{
		grids[gridy][gridx - 1].deleteBlockByIndex(index);
		if (gridy > 0)
		{
			grids[gridy - 1][gridx - 1].deleteBlockByIndex(index);
		}
		if (gridy < GRID_NO - 1)
		{
			grids[gridy + 1][gridx - 1].deleteBlockByIndex(index);
		}
	}
	if (gridx < GRID_NO - 1)
	{
		grids[gridy][gridx + 1].deleteBlockByIndex(index);
		if (gridy > 0)
		{
			grids[gridy - 1][gridx + 1].deleteBlockByIndex(index);
		}
		if (gridy < GRID_NO - 1)
		{
			grids[gridy + 1][gridx + 1].deleteBlockByIndex(index);
		}
	}
	if (gridy > 0)
	{
		grids[gridy - 1][gridx].deleteBlockByIndex(index);
	}
	if (gridy < GRID_NO - 1)
	{
		grids[gridy + 1][gridx].deleteBlockByIndex(index);
	}
	blocks[index].setDisabled();
}

void removeAllBlocks()
{
	for (int i = 0; i < GRID_NO; ++i)
	{
		for (int j = 0; j < GRID_NO; ++j)
		{
			grids[i][j].deleteAllBlocks();
		}
	}
	blocks.clear();
}

int findDuplicateBlock(double x, double y)
{
	double width = 2.0 * BOUNDARY / GRID_NO;
	int gridx = int(ceil((BOUNDARY + x) / width)) - 1;
	int gridy = int(ceil((BOUNDARY - y) / width)) - 1;
	for (auto i : grids[gridy][gridx].blockIndexes)
	{
		double dist = calcDist(x, y, blocks[i].x, blocks[i].y);
		if (dist <= 2.0 * BLOCK_SIZE && !blocks[i].disabled)
		{
			return i;
		}
	}
	return -1;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	coloringGrids();
	drawWall();
	for (auto boid : boids)
	{
		boid.drawBaseBoid();
	}
	for (auto block : blocks)
	{
		if (!block.disabled)
		{
			block.drawBlock();
		}
	}
	glFlush();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glLoadIdentity();
	glOrtho(-w / WINDOW_SIZE * BOUNDARY, w / WINDOW_SIZE * BOUNDARY, -h / WINDOW_SIZE * BOUNDARY, h / WINDOW_SIZE * BOUNDARY, -1.0, 1.0);
}

void mouse(int button, int state, int x, int y)
{
	double pos_x = BOUNDARY * (double(x) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
	double pos_y = -BOUNDARY * (double(y) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		int index = findDuplicateBlock(pos_x, pos_y);
		if (index != -1)
		{
			std::cout << "exist" << index << std::endl;
			removeBlock(index, pos_x, pos_y);
		}
		else
		{
			blocks.push_back(Block(pos_x, pos_y, BLOCK_SIZE));
			whereBlock(blocks.size() - 1, blocks[blocks.size() - 1].x, blocks[blocks.size() - 1].y);
		}
	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			std::cout << "pressing [" << pos_x << ", " << pos_y << "]" << std::endl;
			mouseX = pos_x;
			mouseY = pos_y;
			isPress = true;
		}
		else
		{
			isPress = false;
		}
	}
}

void key(unsigned char key, int x, int y)
{
	if (key == 'r')
	{
		std::cout << "refresh" << std::endl;
		removeAllBlocks();
	}
}

void timer(int value)
{
	//	if (time % 10 == 0)
	//	{
	//		cout << time / 10 << endl;
	//	}
	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids[i].updatePosition();
		if (i != 0)
		{
			boids[i].setColor(1.0, 1.0, 1.0);
		}
		findGrid(i, boids[i].x, boids[i].y);
	}
	updateGrids();
	for (int i = 0; i < BOIDS_NO; i++)
	{
		//boid速度ベクトルの計算部分
		boids[i] = updateAngleAndSpeed(boids[i]);
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
	glutMouseFunc(mouse);
	glutKeyboardFunc(key);
	init();
	createGrids();

	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids.push_back(BaseBoid((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) / RAND_MAX) * 2.0 * M_PI, BOID_SPEED, i));
		findGrid(i, boids[i].x, boids[i].y);
		if (i == 0)
		{
			boids[i].setColor(1.0, 0.0, 0.0);
		}
	}
	for (int i = 0; i < BLOCK_NO; ++i)
	{
		blocks.push_back(Block((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) / RAND_MAX, BLOCK_SIZE));
		whereBlock(i, blocks[i].x, blocks[i].y);
	}
	updateGrids();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(FLAME_RATE, timer, time);
	glutMainLoop();
	return 0;
}
