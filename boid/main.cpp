// main.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
// Created by Ryota Ishidu, Morishita Lab.

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include "ppl.h"
#include "Grid.h"
#include "BaseBoid.h"
#include "Direction.h"
#include "Block.h"
#include "Eigen/Core"
//
//int seconds = 0; //seconds
//int mouseState = 0; //0 is not pressed, 1 is distractor, 2 is attractor
//double mouseX = 0.0;
//double mouseY = 0.0;
//
//double calcDist(double x1, double y1, double x2, double y2)
//{
//	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
//}
//
//Grid grids[GRID_NO + NEAR_GRID_NO * 2][GRID_NO + NEAR_GRID_NO * 2];
//
//std::vector<Block> blocks;
//std::vector<std::tuple<int, int>> boidConnections;

//void addConnections(std::tuple<int, int> newConnection)
//{
//	boidConnections.push_back(newConnection);
//	sort(boidConnections.begin(), boidConnections.end());
//	auto result = unique(boidConnections.begin(), boidConnections.end());
//	boidConnections.erase(result, boidConnections.end());
//}
//
//void removeAllConnections()
//{
//	boidConnections.clear();
//}
//
////this function needs grids
//std::vector<int> getAroundGridBoids(int id, int grid_x, int grid_y)
//{
//	std::vector<int> indexes;
//	for (int i = -NEAR_GRID_NO; i <= NEAR_GRID_NO; ++i)
//	{
//		for (int j = -NEAR_GRID_NO + abs(i); j <= NEAR_GRID_NO - abs(i); ++j)
//		{
//			indexes.insert(indexes.end(), grids[grid_y + j][grid_x + i].boidIndexes.begin(), grids[grid_y + j][grid_x + i].boidIndexes.end());
//		}
//	}
//	auto result = remove(indexes.begin(), indexes.end(), id);
//	auto result2 = unique(indexes.begin(), result);
//	indexes.erase(result2, indexes.end());
//	return indexes;
//}
//
//std::vector<BaseBoid> boids;
//
//double degreeToRadian(double deg)
//{
//	return deg * M_PI / 180.0;
//}
//
////this needs for Boid::isVisible
//double _viewAngle = degreeToRadian(THETA_1) / 2.0;
//
//BaseBoid updateSpeedAndAngle(BaseBoid& boid)
//{
//	Eigen::Vector2d q1 = Eigen::Vector2d::Zero();
//	Eigen::Vector2d q2 = Eigen::Vector2d::Zero();
//	Eigen::Vector2d q3 = Eigen::Vector2d::Zero();
//	Eigen::Vector2d q4 = Eigen::Vector2d::Zero();
//	int n1 = 0;
//	int n2 = 0;
//	int n3 = 0;
//	int n4 = 0;
//	auto indexes = getAroundGridBoids(boid.id, boid.grid_x, boid.grid_y);
//	/*loop starts here*/
//	for (auto i : indexes)
//	{
//		//		if (boid.id == 0)
//		//		{
//		//			boids[i].setColor(0.0, 0.0, 1.0);
//		//		}
//		double dist = calcDist(boid.x, boid.y, boids[i].x, boids[i].y);
//		if (boid.isVisible(boids[i].x, boids[i].y, _viewAngle))
//		{
//			/*boidが見える範囲内にいる*/
//			if (dist - (boid.size + boids[i].size) < R_1)
//			{
//				int first;
//				int second;
//				if (boid.id < boids[i].id)
//				{
//					first = boid.id;
//					second = boids[i].id;
//				}
//				else
//				{
//					first = boids[i].id;
//					second = boid.id;
//				}
//				std::tuple<int, int> connection = std::make_pair(first, second);
//				addConnections(connection);
//				/*rule1: Alignment*/
//				n1++;
//				q1 += boids[i].vctr.normalized();
//				//				if (boid.id == 0)
//				//				{
//				//					/*in boid(id:0)'s rule 1 area*/
//				//					boids[i].setColor(0.6, 1.0, 0.0);
//				//				}
//			}
//			if (dist - (boid.size + boids[i].size) < R_2)
//			{
//				/*rule2: Cohesion*/
//				n2++;
//				q2 += Eigen::Vector2d(boids[i].x - boid.x, boids[i].y - boid.y) / dist / dist * R_2;
//				//				if (boid.id == 0)
//				//				{
//				//					/*in boid(id:0)'s rule 2 area*/
//				//					boids[i].setColor(0.6, 1.0, 0.0);
//				//				}
//			}
//			if (dist - (boid.size + boids[i].size) < R_3)
//			{
//				/*rule3: Separation-boid*/
//				n3++;
//				q3 += Eigen::Vector2d(boids[i].x - boid.x, boids[i].y - boid.y) / dist / dist * R_3;
//				//				if (boid.id == 0)
//				//				{
//				//					/*in boid(id:0)'s rule 3 area*/
//				//					boids[i].setColor(1.0, 1.0, 0.0);
//				//				}
//			}
//		}
//	}
//	/*loop ends here*/
//
//	Eigen::Vector2d wallRepel = Eigen::Vector2d::Zero();
//	for (auto n : grids[boid.grid_y][boid.grid_x].blockIndexes)
//	{
//		double dist = calcDist(boid.x, boid.y, blocks[n].x, blocks[n].y);
//		if (dist - BLOCK_SIZE - boid.size <= R_4 && !blocks[n].disabled)
//		{
//			/*rule4: Separation-block*/
//			n4++;
//			q4 += Eigen::Vector2d(blocks[n].x - boid.x, blocks[n].y - boid.y) / dist / dist * R_4;
//		}
//	}
//	/*mouse*/
//	if (mouseState != 0)
//	{
//		double dist = calcDist(boid.x, boid.y, mouseX, mouseY);
//		if (mouseState == 1)
//		{
//			if (dist - MOUSE_SIZE - boid.size <= R_4)
//			{
//				/*rule4*/
//				n4++;
//				q4 += MOUSE_DISTRACTION_FORCE * Eigen::Vector2d(mouseX - boid.x, mouseY - boid.y) / dist / dist * R_4;
//			}
//		}
//		if (mouseState == 2)
//		{
//			if (dist - MOUSE_SIZE - boid.size < R_2)
//			{
//				/*rule2*/
//				n2++;
//				q2 += MOUSE_ATTRACTION_FORCE * Eigen::Vector2d(mouseX - boid.x, mouseY - boid.y) / dist / dist * R_2;
//			}
//			if (dist - MOUSE_SIZE - boid.size < R_3)
//			{
//				/*rule3*/
//				n3++;
//				q3 += Eigen::Vector2d(mouseX - boid.x, mouseY - boid.y) / dist / dist * R_3;
//			}
//		}
//	}
//	if (n1 != 0)
//	{
//		q1 /= double(n1);
//	}
//	if (n2 != 0)
//	{
//		q2 /= double(n2);
//	}
//	if (n3 != 0)
//	{
//		q3 /= double(n3);
//	}
//	if (n4 != 0)
//	{
//		q4 /= double(n4);
//	}
//
//	/*wall repel*/
//	double wall = BOUNDARY - WALL_SIZE;
//	double bound = wall - boid.size - R_4;
//	if (boid.x >= bound)
//	{
//		wallRepel.x() = 1.0 / (wall - boid.x);
//	}
//	else if (boid.x <= -bound)
//	{
//		wallRepel.x() = -1.0 / (wall + boid.x);
//	}
//	if (boid.y >= bound)
//	{
//		wallRepel.y() = 1.0 / (wall - boid.y);
//	}
//	else if (boid.y <= -bound)
//	{
//		wallRepel.y() = -1.0 / (wall + boid.y);
//	}
//	Eigen::Vector2d V = ALPHA_1 * q1.normalized() + ALPHA_2 * q2 - ALPHA_3 * q3 - ALPHA_4 * q4 + ALPHA_5 * boid.vctr.normalized() - REPEL_WALL_WEIGHT * wallRepel;
//	boid.angle = Direction(V).angle;
//	boid.speed = BETA * log(V.norm() + 1.0);
//	boid.vctr = Eigen::Vector2d(-sin(boid.angle) * boid.speed, cos(boid.angle) * boid.speed);
//	return boid;
//}
//
//void drawConnections()
//{
//	glColor3d(0.6, 0.6, 0.6);
//	for (auto tuple: boidConnections)
//	{
//		glBegin(GL_LINES);
//		glVertex2d(boids[std::get<0>(tuple)].x, boids[std::get<0>(tuple)].y);
//		glVertex2d(boids[std::get<1>(tuple)].x, boids[std::get<1>(tuple)].y);
//		glEnd();
//	}
//}
//
//void drawWall()
//{
//	glColor3d(0.5, 0.5, 0.5);
//	double boundary = BOUNDARY;
//	glBegin(GL_POLYGON);
//	glVertex2d(boundary, boundary);
//	glVertex2d(boundary - WALL_SIZE, boundary);
//	glVertex2d(boundary - WALL_SIZE, -boundary);
//	glVertex2d(boundary, -boundary);
//	glEnd();
//	glBegin(GL_POLYGON);
//	glVertex2d(boundary, boundary);
//	glVertex2d(boundary, boundary - WALL_SIZE);
//	glVertex2d(-boundary, boundary - WALL_SIZE);
//	glVertex2d(-boundary, boundary);
//	glEnd();
//	glBegin(GL_POLYGON);
//	glVertex2d(-boundary, -boundary);
//	glVertex2d(-boundary, -boundary + WALL_SIZE);
//	glVertex2d(boundary, -boundary + WALL_SIZE);
//	glVertex2d(boundary, -boundary);
//	glEnd();
//	glBegin(GL_POLYGON);
//	glVertex2d(-boundary, -boundary);
//	glVertex2d(-boundary + WALL_SIZE, -boundary);
//	glVertex2d(-boundary + WALL_SIZE, boundary);
//	glVertex2d(-boundary, boundary);
//	glEnd();
//}
//
////this function needs grids
//void createGrids()
//{
//	double width = 2.0 * BOUNDARY / GRID_NO;
//	double left;
//	double top = -BOUNDARY - double(NEAR_GRID_NO) * width;
//	for (int i = 0; i < GRID_NO + NEAR_GRID_NO * 2; ++i)
//	{
//		left = -BOUNDARY - double(NEAR_GRID_NO) * width;
//		for (int j = 0; j < GRID_NO + NEAR_GRID_NO * 2; ++j)
//		{
//			grids[i][j] = Grid(top, top + width, left, left + width);
//			left += width;
//		}
//		top += width;
//	}
//}
//
////this function needs grids, boids
//void updateGrids()
//{
//	for (int i = NEAR_GRID_NO; i < GRID_NO + NEAR_GRID_NO; i++)
//	{
//		for (int j = NEAR_GRID_NO; j < GRID_NO + NEAR_GRID_NO; j++)
//		{
//			std::vector<int> indexes = grids[i][j].boidIndexes;
//			for (auto n : indexes)
//			{
//				if (boids[n].grid_y != i || boids[n].grid_x != j)
//				{
//					grids[i][j].deleteBoidByIndex(n);
//				}
//			}
//		}
//	}
//}
//
//void coloringGrids()
//{
//	for (int i = NEAR_GRID_NO; i < GRID_NO + NEAR_GRID_NO; ++i)
//	{
//		for (int j = NEAR_GRID_NO; j < GRID_NO + NEAR_GRID_NO; ++j)
//		{
//			glColor3d(double(i) / (GRID_NO + NEAR_GRID_NO), double(j) / (GRID_NO + NEAR_GRID_NO), 1.0 - double(i) / (GRID_NO + NEAR_GRID_NO));
//			if (find(grids[i][j].boidIndexes.begin(), grids[i][j].boidIndexes.end(), 0) != grids[i][j].boidIndexes.end())
//			{
//				glColor3d(0.3, 0.3, 0.3);
//			}
//			glBegin(GL_POLYGON);
//			glVertex2d(grids[i][j].left, grids[i][j].top);
//			glVertex2d(grids[i][j].left, grids[i][j].bottom);
//			glVertex2d(grids[i][j].right, grids[i][j].bottom);
//			glVertex2d(grids[i][j].right, grids[i][j].top);
//			glEnd();
//		}
//	}
//}
//
////this function needs grids, boids
//void findGrid(int index, double x, double y)
//{
//	double width = 2.0 * BOUNDARY / GRID_NO;
//	int gridx = int(ceil((BOUNDARY + x) / width)) - 1 + NEAR_GRID_NO;
//	int gridy = int(ceil((BOUNDARY + y) / width)) - 1 + NEAR_GRID_NO;
//	grids[gridy][gridx].addBoidByIndex(index);
//	boids[index].grid_x = gridx;
//	boids[index].grid_y = gridy;
//}
//
//void whereBlock(int index, double x, double y)
//{
//	double width = 2.0 * BOUNDARY / GRID_NO;
//	int gridx = int(ceil((BOUNDARY + x) / width)) - 1 + NEAR_GRID_NO;
//	int gridy = int(ceil((BOUNDARY + y) / width)) - 1 + NEAR_GRID_NO;
//	for (int i = -NEAR_GRID_NO; i <= NEAR_GRID_NO; ++i)
//	{
//		for (int j = -NEAR_GRID_NO + abs(i); j <= NEAR_GRID_NO - abs(i); ++j)
//		{
//			grids[gridy + i][gridx + j].addBlockByIndex(index);
//		}
//	}
//}
//
//void removeBlock(int index, double x, double y)
//{
//	double width = 2.0 * BOUNDARY / GRID_NO;
//	int gridx = int(ceil((BOUNDARY + x) / width)) - 1 + NEAR_GRID_NO;
//	int gridy = int(ceil((BOUNDARY - y) / width)) - 1 + NEAR_GRID_NO;
//	for (int i = -NEAR_GRID_NO; i <= NEAR_GRID_NO; ++i)
//	{
//		for (int j = -NEAR_GRID_NO + abs(i); j <= NEAR_GRID_NO - abs(i); ++j)
//		{
//			grids[gridy + i][gridx + j].deleteBlockByIndex(index);
//		}
//	}
//	blocks[index].setDisabled();
//}
//
//void removeAllBlocks()
//{
//	for (int i = 0; i < GRID_NO + NEAR_GRID_NO * 2; ++i)
//	{
//		for (int j = 0; j < GRID_NO + NEAR_GRID_NO * 2; ++j)
//		{
//			grids[i][j].deleteAllBlocks();
//		}
//	}
//	blocks.clear();
//}
//
//void removeAllBoids()
//{
//	for (int i = 0; i < GRID_NO + NEAR_GRID_NO * 2; ++i)
//	{
//		for (int j = 0; j < GRID_NO + NEAR_GRID_NO * 2; ++j)
//		{
//			grids[i][j].deleteAllBoids();
//		}
//	}
//	boids.clear();
//}
//
//int findDuplicateBlock(double x, double y)
//{
//	double width = 2.0 * BOUNDARY / GRID_NO;
//	int gridx = int(ceil((BOUNDARY + x) / width)) - 1 + NEAR_GRID_NO;
//	int gridy = int(ceil((BOUNDARY + y) / width)) - 1 + NEAR_GRID_NO;
//	for (auto i : grids[gridy][gridx].blockIndexes)
//	{
//		double dist = calcDist(x, y, blocks[i].x, blocks[i].y);
//		if (dist <= 2.0 * BLOCK_SIZE && !blocks[i].disabled)
//		{
//			return i;
//		}
//	}
//	return -1;
//}
//
//void display(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	//	coloringGrids();
//	drawWall();
//	drawConnections();
//	for (auto boid : boids)
//	{
//		boid.drawBaseBoid();
//	}
//	for (auto block : blocks)
//	{
//		if (!block.disabled)
//		{
//			block.drawBlock();
//		}
//	}
//	if (mouseState != 0)
//	{
//		GLdouble r = 0.0;
//		GLdouble g = 0.0;
//		GLdouble b = 0.0;
//		switch (mouseState)
//		{
//		case 1:
//			r = 0.8;
//			g = 0.0;
//			b = 0.3;
//			break;
//		case 2:
//			r = 0.0;
//			g = 0.8;
//			b = 0.3;
//			break;
//		default:
//			break;
//		}
//
//		double angl = 2.0 * M_PI / CIRCLE_SLICE;
//		glColor3d(r, g, b);
//		glPushMatrix();
//		glTranslated(mouseX, mouseY, 0.0);
//		glBegin(GL_POLYGON);
//		for (int i = 0; i < CIRCLE_SLICE; ++i)
//		{
//			glVertex2d(BLOCK_SIZE * cos(double(i) * angl), BLOCK_SIZE * sin(double(i) * angl));
//		}
//		glEnd();
//		glPopMatrix();
//	}
//	glFlush();
//}
//
//void resize(int w, int h)
//{
//	glViewport(0, 0, w, h);
//	glLoadIdentity();
//	glOrtho(-w / WINDOW_SIZE * BOUNDARY, w / WINDOW_SIZE * BOUNDARY, -h / WINDOW_SIZE * BOUNDARY, h / WINDOW_SIZE * BOUNDARY, -1.0, 1.0);
//}
//
//void mouse(int button, int state, int x, int y)
//{
//	double pos_x = BOUNDARY * (double(x) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
//	double pos_y = -BOUNDARY * (double(y) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
//	if (state == GLUT_DOWN)
//	{
//		if (button == GLUT_LEFT_BUTTON)
//		{
//			std::cout << "distractor" << std::endl;
//			mouseX = pos_x;
//			mouseY = pos_y;
//			mouseState = 1;
//		}
//		if (button == GLUT_RIGHT_BUTTON)
//		{
//			std::cout << "attractor" << std::endl;
//			mouseX = pos_x;
//			mouseY = pos_y;
//			mouseState = 2;
//		}
//	}
//	else
//	{
//		mouseState = 0;
//	}
//}
//
//void motion(int x, int y)
//{
//	if (mouseState != 0)
//	{
//		double pos_x = BOUNDARY * (double(x) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
//		double pos_y = -BOUNDARY * (double(y) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
//		mouseX = pos_x;
//		mouseY = pos_y;
//	}
//}
//
//void key(unsigned char key, int x, int y)
//{
//	double pos_x = BOUNDARY * (double(x) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
//	double pos_y = -BOUNDARY * (double(y) - WINDOW_SIZE / 2.0) / double(WINDOW_SIZE / 2.0);
//	if (key == 'r')
//	{
//		/*remove all blocks*/
//		removeAllBlocks();
//		std::cout << "refresh" << std::endl;
//	}
//	if (key == 'i')
//	{
//		/*initialize blocks*/
//		removeAllBlocks();
//		for (int i = 0; i < BLOCK_NO; ++i)
//		{
//			blocks.push_back(Block((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, BLOCK_SIZE));
//			whereBlock(i, blocks[i].x, blocks[i].y);
//		}
//		/*initialize boids*/
//		removeAllBoids();
//	}
//	if (key == 's')
//	{
//		/*seed boids*/
//		removeAllBoids();
//		for (int i = 0; i < BOIDS_NO; i++)
//		{
//			boids.push_back(BaseBoid((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) / RAND_MAX) * 2.0 * M_PI, BOID_SIZE, BOID_SPEED, i));
//			findGrid(i, boids[i].x, boids[i].y);
//			if (i == 0)
//			{
//				boids[i].setColor(1.0, 0.0, 0.0);
//			}
//		}
//	}
//	if (key == 'b')
//	{
//		/*add block*/
//		int index = findDuplicateBlock(pos_x, pos_y);
//		if (index != -1)
//		{
//			std::cout << "exist" << index << std::endl;
//			removeBlock(index, pos_x, pos_y);
//		}
//		else
//		{
//			blocks.push_back(Block(pos_x, pos_y, BLOCK_SIZE));
//			whereBlock(blocks.size() - 1, blocks[blocks.size() - 1].x, blocks[blocks.size() - 1].y);
//		}
//	}
//	if (key == 'a')
//	{
//		/*add boid*/
//		double bound = BOUNDARY - BOID_SIZE - WALL_SIZE;
//		if (pos_x > bound || pos_x < -bound || pos_y > bound || pos_y < -bound)
//		{
//			std::cout << "out of range" << std::endl;
//		}
//		else
//		{
//			int index = boids.size();
//			boids.push_back(BaseBoid(pos_x, pos_y, (double(rand()) / RAND_MAX) * 2.0 * M_PI, BOID_SIZE, BOID_SPEED, index));
//			if (index == 0)
//			{
//				boids[index].setColor(1.0, 0.0, 0.0);
//			}
//			findGrid(index, boids[index].x, boids[index].y);
//		}
//	}
//}
//
//void timer(int value)
//{
//	//	if (seconds % 10 == 0)
//	//	{
//	//		std::cout << "seconds: " << seconds / 10 << std::endl;
//	//		std::for_each(boidConnections.begin(), boidConnections.end(), [](std::tuple<int, int> x) { std::cout << "[" << x.first << ", " << x.second << "]" << "; "; });
//	//		std::cout << "" << std::endl;
//	//	}
//	removeAllConnections();
//	for (int i = 0; i < boids.size(); i++)
//	{
//		boids[i].updatePosition();
//		//		if (i != 0)
//		//		{
//		//			boids[i].setColor(1.0, 1.0, 1.0);
//		//		}
//		findGrid(i, boids[i].x, boids[i].y);
//	}
//	updateGrids();
//	for (int i = 0; i < boids.size(); i++)
//	{
//		//boid速度ベクトルの計算部分
//		boids[i] = updateSpeedAndAngle(boids[i]);
//	}
//	glutPostRedisplay();
//	seconds++;
//	glutTimerFunc(FLAME_RATE, timer, seconds);
//}
//
//void init(void)
//{
//	glClearColor(0.0, 0.0, 0.0, 1.0);
//}
//
//int main(int argc, char* argv[])
//{
//	glutInitWindowPosition(0, 0);
//	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGBA);
//	glutCreateWindow(argv[0]);
//	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
//	printf("Vecdor: %s\n", glGetString(GL_VENDOR));
//	printf("GPU: %s\n", glGetString(GL_RENDERER));
//	glutMouseFunc(mouse);
//	glutMotionFunc(motion);
//	glutKeyboardFunc(key);
//	init();
//	createGrids();
//
//	for (int i = 0; i < BOIDS_NO; i++)
//	{
//		boids.push_back(BaseBoid((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - WALL_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) / RAND_MAX) * 2.0 * M_PI, BOID_SIZE, BOID_SPEED, i));
//		findGrid(i, boids[i].x, boids[i].y);
//		//		if (i == 0)
//		//		{
//		//			boids[i].setColor(1.0, 0.0, 0.0);
//		//		}
//	}
//	for (int i = 0; i < BLOCK_NO; ++i)
//	{
//		blocks.push_back(Block((double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, (double(rand()) - RAND_MAX / 2.0) * (BOUNDARY - BLOCK_SIZE - BOID_SIZE) * 2.0 / RAND_MAX, BLOCK_SIZE));
//		whereBlock(i, blocks[i].x, blocks[i].y);
//	}
//	updateGrids();
//	glutDisplayFunc(display);
//	glutReshapeFunc(resize);
//	glutTimerFunc(FLAME_RATE, timer, seconds);
//	glutMainLoop();
//	return 0;
//}
//-------- 各種外部変数 ----------//
float points[] =
{
	0,1,0,
	-0.5, 0,0,
	0.5, 0,0
};

//VBO用ID
GLuint VboId[5];

//----------- 関数プロトタイプ ----------------//
void display();
void reshape(int w, int h);
void timer(int value);
void BuildVBO();


//----------- OpenGLの初期設定 ---------------//
void GLUT_INIT()
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("VBO ");
}

void GLUT_CALL_FUNC()
{
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 17);
}

bool GLEW_INIT()
{
	GLenum err;
	err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << glewGetErrorString(err) << "\n";
		return false;
	}
	return true;
}

void MY_INIT()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//---------- メイン関数 ---------------//
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	GLUT_INIT();
	GLUT_CALL_FUNC();
	MY_INIT();

	if (GLEW_INIT() == false) {
		return -1;
	}

	BuildVBO();
	glutMainLoop();

	return 0;
}

//------------- ここから各種コールバック ------------------//
void display()
{
	static float angle = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0, 1);

	//VBOで描画
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VboId[0]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);  //最後の引数は「0」を指定
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glColor3f(1, 1, 1);

	glutSwapBuffers();


	//アップデート
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VboId[0]);
	float *ptr = (float*)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB);
	if (ptr)
	{
		ptr[2] = sin(angle); //1つ目の頂点、z座標を動かす
		angle += 0.1;
		if (angle > 360) angle = 0;
		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	}

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);  //ビューポートの設定

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0); //視野の設定
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //視点の設定
}

void timer(int t)
{
	glutPostRedisplay();
	glutTimerFunc(t, timer, 17); //タイマー関数
}


//--------------------------//
void BuildVBO()
{
	glGenBuffersARB(5, &VboId[0]);   //発生
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, VboId[0]); //バインド

													//データをVBOにコピー
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 3 * 3 * sizeof(float), points, GL_DYNAMIC_DRAW);

	//例外チェック(VBOと配列のサイズがあっているか？)
	int bufferSize = 0;
	glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &bufferSize);
	if (3 * 3 * sizeof(float) != bufferSize) {//一致していない場合
		glDeleteBuffersARB(1, &VboId[0]);
		VboId[0] = 0;
		std::cout << "Can't Create VBO\n";
	}

}