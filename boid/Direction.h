﻿#pragma once
#include "Eigen/Core"

class Direction
{
public:
	double angle;
	double x;
	double y;

	/**
	 * \brief 
	 * \param _angle 
	 */
	Direction(double _angle);
	/**
	 * \brief 
	 * \param _x 
	 * \param _y 
	 */
	Direction(double _x, double _y);
	Direction(Eigen::Vector2d& point);
};
