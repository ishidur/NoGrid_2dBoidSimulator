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
	 * \param _angle 
	 */
	Direction(double _angle);
	/**
	 * \brief 
	 * \param _x 
	 * \param _y 
	 */
	Direction(double _x, double _y);
	/**
	 * \brief 
	 * \param point 
	 */
	Direction(Eigen::Vector2d& point);
};
