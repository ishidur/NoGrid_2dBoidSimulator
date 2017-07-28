#pragma once
#include "Eigen/Core"
#include <vector>


class BaseBoid
{
private:
	double r = 1.0; //color: red
	double g = 1.0; //color: green
	double b = 1.0; //color: blue
public:
	int id = -1; //id
	double x; //_x-position
	double y; //y-position
	double angle; //radian angle: 0 is top
	double size = BOID_SIZE; //boid size
	Eigen::Vector2d vctr; //
	int grid_y = -1; //grid address y
	int grid_x = -1; //grid address x
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	/**
	* \brief
	* \param x: x-pos
	* \param y: y-pos
	* \param angle: radian; boid move angle
	* \param speed: boid move speed
	* \param id: identifier
	*/
	BaseBoid(double x = 0.0, double y = 0.0, double angle = 0.0, double size = 0.0, double speed = 0.0, int id = -1);

	/**
	* \brief
	* \param red
	* \param green
	* \param blue
	*/
	void setColor(double red, double green, double blue);

	void setSize(double size);

	std::vector<GLfloat> drawBaseBoid();

	void updatePosition();

	/**
	* \brief
	* \param x: object's x-pos
	* \param y: object's y-pos
	* \param viewAngle: this view angle
	* \return
	*/
	bool isVisible(double x, double y, double viewAngle);
};
