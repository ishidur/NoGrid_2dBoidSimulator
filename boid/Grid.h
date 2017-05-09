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
	/**
	 * \brief 
	 * \param index 
	 */
	void addBoidByIndex(int index);
	/**
	 * \brief 
	 * \param index 
	 */
	void deleteBoidByIndex(int index);
	/**
	 * \brief 
	 * \param index 
	 * \return 
	 */
	bool findBoidByIndex(int index);
	/**
	 * \brief 
	 * \param index 
	 */
	void addBlockByIndex(int index);
	/**
	 * \brief 
	 * \param index 
	 */
	void deleteBlockByIndex(int index);
	/**
	 * \brief 
	 */
	void deleteAllBlocks();
};