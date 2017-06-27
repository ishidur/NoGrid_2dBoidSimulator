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
	* \brief top > bottom, right > left
	* \param top: y-axis
	* \param bottom: y-axis
	* \param left: x-axis
	* \param right: x-axis
	*/
	Grid(double top = 0.0, double bottom = 0.0, double left = 0.0, double right = 0.0);
	/**
	* \brief
	* \param index: boid-id
	*/
	void addBoidByIndex(int newIndex);
	/**
	* \brief
	* \param index: boid-id
	*/
	void deleteBoidByIndex(int index);
	/**
	* \brief
	* \param index: boid-id
	* \return
	*/
	bool findBoidByIndex(int index);
	/**
	* \brief
	*/
	void deleteAllBoids();
	/**
	* \brief
	* \param index: block-id
	*/
	void addBlockByIndex(int newIndex);
	/**
	* \brief
	* \param index: block-id
	*/
	void deleteBlockByIndex(int index);
	/**
	* \brief
	*/
	void deleteAllBlocks();
};
