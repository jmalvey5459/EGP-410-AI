#pragma once

#include "Graph.h"

class Grid;

class GridGraph:public Graph
{
public:
	GridGraph( Grid* pGrid );
	virtual ~GridGraph();

	void setGrid( Grid* pGrid ) { mpGrid = pGrid; };
	void init();
	int getSize();
private:
	Grid* mpGrid;
	int mNumGridIndices;
};