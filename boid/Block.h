#pragma once

class Block
{
public:
	bool disabled = false;
	double x; //center x
	double y; //center y
	double r; //radius
	Block(double x, double y, double r);

	void drawBlock();
	void setDisabled();
};
