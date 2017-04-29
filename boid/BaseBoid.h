#pragma once
#include <vector>

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
	double speed; // speed
	int grid_y = -1; //grid address y
	int grid_x = -1; //grid address x

	/**
	 * \brief 
	 * \param _x 
	 * \param _y 
	 * \param _angle 
	 * \param _speed 
	 * \param _id 
	 */
	BaseBoid(double _x = 0.0, double _y = 0.0, double _angle = 0.0, double _speed = 0.0, int _id = -1);

	void setColor(double red, double green, double blue);

	void drawBaseBoid();

	void updatePosition();

	bool isVisible(BaseBoid boid);
	
};
