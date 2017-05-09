#pragma once
#include "Eigen/Core"
#include "Eigen/StdVector"


class BaseBoid
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
	Eigen::Vector2d vctr; //
	double speed; // speed
	int grid_y = -1; //grid address y
	int grid_x = -1; //grid address x
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		/**
		* \brief
		* \param x
		* \param y
		* \param angle
		* \param speed
		* \param id
		*/
		BaseBoid(double x = 0.0, double y = 0.0, double angle = 0.0, double speed = 0.0, int id = -1);

	/**
	* \brief
	* \param red
	* \param green
	* \param blue
	*/
	void setColor(double red, double green, double blue);

	void drawBaseBoid();

	void updatePosition();

	/**
	* \brief
	* \param x
	* \param y
	* \return
	*/
	bool isVisible(double x, double y);
};
