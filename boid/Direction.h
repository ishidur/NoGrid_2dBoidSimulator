#pragma once
#include "Point.h"

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
	Direction(Point point);
};
