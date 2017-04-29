#pragma once
#include <vector>


class Grid
{
public:
	double left;
	double right;
	double top;
	double bottom;
	std::vector<int> boidIndexes;
	std::vector<int> blockIndexes;

	/**
	 * \brief 
	 * \param _top
	 * \param _bottom 
	 * \param _left 
	 * \param _right 
	 */
	Grid(double _top = 0.0, double _bottom = 0.0, double _left = 0.0, double _right = 0.0);
	void addBoidByIndex(int index);
	void deleteBoidByIndex(int index);
	bool findBoidByIndex(int index);
	void addBlockByIndex(int index);
};
