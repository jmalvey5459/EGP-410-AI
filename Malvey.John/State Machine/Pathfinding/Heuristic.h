/*
Author: Gabe Troyan
I certify this is all my original work, except where otherwise specified.
*/

#pragma once
#include "GridGraph.h"



class Heuristic
{
	Node* mpGoalNode;
	GridGraph* mpGridGraph;
	int goalX, goalY;
public:
	Heuristic(Node* goal, GridGraph* grid);
	Heuristic(Node * goal);
	float estimate(Node* node);
};