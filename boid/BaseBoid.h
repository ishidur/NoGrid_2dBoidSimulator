#pragma once
#include "Eigen/Core"


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
	Eigen::Vector2d vctr; //
	Eigen::Vector2d q1; //
	Eigen::Vector2d q2; //
	Eigen::Vector2d q3; //
	Eigen::Vector2d q4; //
	Eigen::Vector2d q5; //
	double speed; // speed
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
	BaseBoid(double x = 0.0, double y = 0.0, double angle = 0.0, double speed = 0.0, int id = -1);

	/**
	* \brief
	* \param red
	* \param green
	* \param blue
	*/
	void setColor(double red, double green, double blue);

	void drawBaseBoid();

	void visualizeBoidVector();

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
