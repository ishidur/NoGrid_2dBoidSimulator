#pragma once

class Block
{
public:
	double x; //center x
	double y; //center y
	double r; //radius
	Block(double _x, double _y, double _r);

	void drawBlock();
};
