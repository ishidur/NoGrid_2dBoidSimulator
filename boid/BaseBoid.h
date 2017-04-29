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
	 * \param x 
	 * \param y 
	 * \param angle 
	 * \param speed 
	 * \param id 
	 */
	BaseBoid(double x = 0.0, double y = 0.0, double angle = 0.0, double speed = 0.0, int id = -1);

	void setColor(double red, double green, double blue);

	void drawBaseBoid();

	void updatePosition();

	bool isVisible(double x, double y);
};
