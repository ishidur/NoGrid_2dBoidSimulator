#pragma once
#include "Eigen/Core"

class Direction
{
public:
	double angle;
	double x;
	double y;
	Eigen::Vector2d vector;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	/**
	* \brief
	* \param angle: y-axis is 0, left cycle
	*/
	Direction(double angle);
	/**
	* \brief
	* \param x: right is plus
	* \param y: top is plus
	*/
	Direction(double x, double y);
	/**
	* \brief
	* \param vector: Eigen vector
	*/
	Direction(Eigen::Vector2d& vector);
};
