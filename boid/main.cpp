// main.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
// Created by Ryota Ishidu, Morishita Lab.

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
int mouseState = 0; //0 is not pressed, 1 is distractor, 2 is attractor
double mouseX = 0.0;
double mouseY = 0.0;

double calcDist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

Grid grids[GRID_NO + 2][GRID_NO + 2];

std::vector<Block> blocks;
std::vector<std::pair<int, int>> boidConnections;

void addConnections(std::pair<int, int> newConnection)
{
	boidConnections.push_back(newConnection);
	std::sort(boidConnections.begin(), boidConnections.end());
	auto result = std::unique(boidConnections.begin(), boidConnections.end());
	boidConnections.erase(result, boidConnections.end());
}

void removeAllConnections()
{
	boidConnections.clear();
}

//this function needs grids
std::vector<int> getAroundGridBoids(int id, int grid_x, int grid_y)
{
	std::vector<int> indexes;
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			indexes.insert(indexes.end(), grids[grid_y + j][grid_x + i].boidIndexes.begin(), grids[grid_y + j][grid_x + i].boidIndexes.end());
		}
	}
	auto result = remove(indexes.begin(), indexes.end(), id);
	auto result2 = unique(indexes.begin(), result);
	indexes.erase(result2, indexes.end());
	return indexes;
}

std::vector<BaseBoid> boids;

double degreeToRadian(double deg)
{
	return deg * M_PI / 180.0;
}

//this needs for Boid::isVisible
double _viewAngle = degreeToRadian(THETA_1) / 2.0;

BaseBoid updateSpeedAndAngle(BaseBoid& boid)
{
	Eigen::Vector2d q1 = Eigen::Vector2d::Zero();
	Eigen::Vector2d q2 = Eigen::Vector2d::Zero();
	Eigen::Vector2d q3 = Eigen::Vector2d::Zero();
	Eigen::Vector2d q4 = Eigen::Vector2d::Zero();
	int n1 = 0;
	int n2 = 0;
	int n3 = 0;
	int n4 = 0;
	auto indexes = getAroundGridBoids(boid.id, boid.grid_x, boid.grid_y);
	/*loop starts here*/
	for (auto i : indexes)
	{
		double dist = calcDist(boid.x, boid.y, boids[i].x, boids[i].y);
		if (boid.isVisible(boids[i].x, boids[i].y, _viewAngle))
		{
			/*boidが見える範囲内にいる*/
			if (dist - (boid.size + boids[i].size) < R_1)
			{
				int first;
				int second;
				if (boid.id < boids[i].id)
				{
					first = boid.id;
					second = boids[i].id;
				}
				else
				{
					first = boids[i].id;
					second = boid.id;
				}
				std::pair<int, int> connection = std::make_pair(first, second);
				addConnections(connection);
				/*rule1: Alignment*/
				n1++;
				q1 += boids[i].vctr.normalized();
				if (boid.id == 0)
				{
					/*in boid(id:0)'s rule 1 area*/
					//					boids[i].setColor(0.6, 1.0, 0.0);
				}
			}
			if (dist - (boid.size + boids[i].size) < R_2)
			{
				/*rule2: Cohesion*/
				n2++;
				q2 += Eigen::Vector2d(boids[i].x - boid.x, boids[i].y - boid.y) / dist / dist * R_2;
				if (boid.id == 0)
				{
					/*in boid(id:0)'s rule 2 area*/
					//					boids[i].setColor(0.6, 1.0, 0.0);
				}
			}
			if (dist - (boid.size + boids[i].size) < R_3)
			{
				/*rule3: Separation-boid*/
				n3++;
				q3 += Eigen::Vector2d(boids[i].x - boid.x, boids[i].y - boid.y) / dist / dist * R_3;
				if (boid.id == 0)
				{
					/*in boid(id:0)'s rule 3 area*/
					//					boids[i].setColor(1.0, 1.0, 0.0);
				}
			}
		}
	}
	/*loop ends here*/

	Eigen::Vector2d wallRepel = Eigen::Vector2d::Zero();
	for (auto n : grids[boid.grid_y][boid.grid_x].blockIndexes)
	{
		double dist = calcDist(boid.x, boid.y, blocks[n].x, blocks[n].y);
		if (dist - BLOCK_SIZE - boid.size <= R_4 && !blocks[n].disabled)
		{
			/*rule4: Separation-block*/
			n4++;
			q4 += Eigen::Vector2d(blocks[n].x - boid.x, blocks[n].y - boid.y) / dist / dist * R_4;
		}
	}
	/*mouse*/
	if (mouseState != 0)
	{
		double dist = calcDist(boid.x, boid.y, mouseX, mouseY);
		if (mouseState == 1)
		{
			if (dist - MOUSE_SIZE - boid.size <= R_4)
			{
				/*rule4*/
				n4++;
				q4 += MOUSE_DISTRACTION_FORCE * Eigen::Vector2d(mouseX - boid.x, mouseY - boid.y) / dist / dist * R_4;
			}
		}
		if (mouseState == 2)
		{
			if (dist - MOUSE_SIZE - boid.size < R_2)
			{
				/*rule2*/
				n2++;
				q2 += MOUSE_ATTRACTION_FORCE * Eigen::Vector2d(mouseX - boid.x, mouseY - boid.y) / dist / dist * R_2;
			}
			if (dist - MOUSE_SIZE - boid.size < R_3)
			{
				/*rule3*/
				n3++;
				q3 += Eigen::Vector2d(mouseX - boid.x, mouseY - boid.y) / dist / dist * R_3;
			}
		}
	}
	if (n1 != 0)
	{
		q1 /= double(n1);
	}
	if (n2 != 0)
	{
		q2 /= double(n2);
	}
	if (n3 != 0)
	{
		q3 /= double(n3);
	}
	if (n4 != 0)
	{
		q4 /= double(n4);
	}

	/*wall repel*/
	double wall = BOUNDARY - WALL_SIZE;
	double bound = wall - boid.size - R_4;
	if (boid.x >= bound)
	{
		wallRepel.x() = 1.0 / (wall - boid.x);
	}
	else if (boid.x <= -bound)
	{
		wallRepel.x() = -1.0 / (wall + boid.x);
	}
	if (boid.y >= bound)
	{
		wallRepel.y() = 1.0 / (wall - boid.y);
	}
	else if (boid.y <= -bound)
	{
		wallRepel.y() = -1.0 / (wall + boid.y);
	};
	Eigen::Vector2d V = ALPHA_1 * q1.normalized() + ALPHA_2 * q2 - ALPHA_3 * q3 - ALPHA_4 * q4 + ALPHA_5 * boid.vctr.normalized() - REPEL_WALL_WEIGHT * wallRepel;
	boid.angle = Direction(V).angle;
	boid.speed = BETA * log(V.norm() + 1.0);
	boid.vctr = Eigen::Vector2d(-sin(boid.angle) * boid.speed, cos(boid.angle) * boid.speed);
	return boid;
}

void drawConnections()
{
	glColor3d(0.6, 0.6, 0.6);
	for (auto pair: boidConnections)
	{
		glBegin(GL_LINES);
		glVertex2d(boids[pair.first].x, boids[pair.first].y);
		glVertex2d(boids[pair.second].x, boids[pair.second].y);
		glEnd();
	}
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
	double top = -BOUNDARY - width;
	for (int i = 0; i < GRID_NO + 2; ++i)
	{
		left = -BOUNDARY - width;
		for (int j = 0; j < GRID_NO + 2; ++j)
		{
			grids[i][j] = Grid(top, top + width, left, left + width);
			left += width;
		}
		top += width;
	}
}

//this function needs grids, boids
void updateGrids()
{
	for (int i = 1; i < GRID_NO + 1; i++)
	{
		for (int j = 1; j < GRID_NO + 1; j++)
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
	for (int i = 1; i < GRID_NO + 1; ++i)
	{
		for (int j = 1; j < GRID_NO + 1; ++j)
		{
			glColor3d(double(i) / (GRID_NO + 1), double(j) / (GRID_NO + 1), 1.0 - double(i) / (GRID_NO + 1));
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
	int gridx = int(ceil((BOUNDARY + x) / width));
	int gridy = int(ceil((BOUNDARY + y) / width));
	grids[gridy][gridx].addBoidByIndex(index);
	boids[index].grid_x = gridx;
	boids[index].grid_y = gridy;
}

void whereBlock(int index, double x, double y)
{
	double width = 2.0 * BOUNDARY / GRID_NO;
	int gridx = int(ceil((BOUNDARY + x) / width));
	int gridy = int(ceil((BOUNDARY + y) / width));
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			grids[gridy + i][gridx + j].addBlockByIndex(index);
		}
	}
}

void removeBlock(int index, double x, double y)
{
	double width = 2.0 * BOUNDARY / GRID_NO;
	int gridx = int(ceil((BOUNDARY + x) / width));
	int gridy = int(ceil((BOUNDARY - y) / width));
	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			grids[gridy + i][gridx + j].deleteBlockByIndex(index);
		}
	}
	blocks[index].setDisabled();
}

void removeAllBlocks()
{
	for (int i = 0; i < GRID_NO + 2; ++i)
	{
		for (int j = 0; j < GRID_NO + 2; ++j)
		{
			grids[i][j].deleteAllBlocks();
		}
	}
	blocks.clear();
}

void removeAllBoids()
{
	for (int i = 0; i < GRID_NO + 2; ++i)
	{
		for (int j = 0; j < GRID_NO + 2; ++j)
		{
			grids[i][j].deleteAllBoids();
		}
	}
	boids.clear();
}

int findDuplicateBlock(double x, double y)
{
	double width = 2.0 * BOUNDARY / GRID_NO;
	int gridx = int(ceil((BOUNDARY + x) / width));
	int gridy = int(ceil((BOUNDARY + y) / width));
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
	//	coloringGrids();
	drawWall();
	drawConnections();
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
	if (mouseState != 0)
	{
		GLdouble r = 0.0;
		GLdouble g = 0.0;
		GLdouble b = 0.0;
		switch (mouseState)
		{
		case 1:
			r = 0.8;
			g = 0.0;
			b = 0.3;
			break;
		case 2:
			r = 0.0;
			g = 0.8;
			b = 0.3;
			break;
		default:
			break;
		}

		double angl = 2.0 * M_PI / CIRCLE_SLICE;
		glColor3d(r, g, b);
		glPushMatrix();
		glTranslated(mouseX, mouseY, 0.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < CIRCLE_SLICE; ++i)
		{
			glVertex2d(BLOCK_SIZE * cos(double(i) * angl), BLOCK_SIZE * sin(double(i) * angl));
		}
		glEnd();
		glPopMatrix();
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
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			std::cout << "distractor" << std::endl;
			mouseX = pos_x;
			mouseY = pos_y;
			mouseState = 1;
		}
		if (button == GLUT_RIGHT_BUTTON)
		{
			std::cout << "attractor" << std::endl;
			mouseX = pos_x;
			mouseY = pos_y;
			mouseState = 2;
		}
	}
	else
	{
		mouseState = 0;
	}
}

void motion(int x, int y)
{
	if (mouseState != 0)
	{
		double pos_x = BOUNDARY * (double(x) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
		double pos_y = -BOUNDARY * (double(y) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
		mouseX = pos_x;
		mouseY = pos_y;
	}
}

void key(unsigned char key, int x, int y)
{
	double pos_x = BOUNDARY * (double(x) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
	double pos_y = -BOUNDARY * (double(y) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
	if (key == 'r')
	{
		/*remove all blocks*/
		removeAllBlocks();
		std::cout << "refresh" << std::endl;
	}
	if (key == 'i')
	{
		/*initialize blocks*/
		removeAllBlocks();
		for (int i = 0; i < BLOCK_NO; ++i)
		{
			blocks.push_back(Block((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, BLOCK_SIZE));
			whereBlock(i, blocks[i].x, blocks[i].y);
		}
		/*initialize boids*/
		removeAllBoids();
	}
	if (key == 's')
	{
		/*seed boids*/
		removeAllBoids();
		for (int i = 0; i < BOIDS_NO; i++)
		{
			boids.push_back(BaseBoid((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) / RAND_MAX) * 2.0 * M_PI, BOID_SIZE, BOID_SPEED, i));
			findGrid(i, boids[i].x, boids[i].y);
			if (i == 0)
			{
				//			boids[i].setColor(1.0, 0.0, 0.0);
			}
		}
	}
	if (key == 'b')
	{
		/*add block*/
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
	if (key == 'a')
	{
		/*add boid*/
		double bound = BOUNDARY - BOID_SIZE - WALL_SIZE;
		if (pos_x > bound || pos_x < -bound || pos_y > bound || pos_y < -bound)
		{
			std::cout << "out of range" << std::endl;
		}
		else
		{
			int index = boids.size();
			boids.push_back(BaseBoid(pos_x, pos_y, (double(rand()) / RAND_MAX) * 2.0 * M_PI, BOID_SIZE, BOID_SPEED, index));
			if (index == 0)
			{
				//				boids[index].setColor(1.0, 0.0, 0.0);
			}
			findGrid(index, boids[index].x, boids[index].y);
		}
	}
}

void timer(int value)
{
	if (time % 10 == 0)
	{
		//		std::cout << "time: " << time / 10 << std::endl;
		//		std::for_each(boidConnections.begin(), boidConnections.end(), [](std::pair<int, int> x) { std::cout << "[" << x.first << ", " << x.second << "]" << "; "; });
		//		std::cout << "" << std::endl;
	}
	removeAllConnections();
	for (int i = 0; i < boids.size(); i++)
	{
		boids[i].updatePosition();
		if (i != 0)
		{
			//			boids[i].setColor(1.0, 1.0, 1.0);
		}
		findGrid(i, boids[i].x, boids[i].y);
	}
	updateGrids();
	for (int i = 0; i < boids.size(); i++)
	{
		//boid速度ベクトルの計算部分
		boids[i] = updateSpeedAndAngle(boids[i]);
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
	glutMotionFunc(motion);
	glutKeyboardFunc(key);
	init();
	createGrids();

	for (int i = 0; i < BOIDS_NO; i++)
	{
		boids.push_back(BaseBoid((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) / RAND_MAX) * 2.0 * M_PI, BOID_SIZE, BOID_SPEED, i));
		findGrid(i, boids[i].x, boids[i].y);
		if (i == 0)
		{
			//			boids[i].setColor(1.0, 0.0, 0.0);
		}
	}
	for (int i = 0; i < BLOCK_NO; ++i)
	{
		blocks.push_back(Block((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, BLOCK_SIZE));
		whereBlock(i, blocks[i].x, blocks[i].y);
	}
	updateGrids();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutTimerFunc(FLAME_RATE, timer, time);
	glutMainLoop();
	return 0;
}
