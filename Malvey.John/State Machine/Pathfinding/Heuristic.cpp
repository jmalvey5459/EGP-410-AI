/*
Author: Gabe Troyan
I certify this is all my original work, except where otherwise specified.
*/

#include "Heuristic.h"
#include "GridGraph.h"
#include "Node.h"
#include "Vector2D.h"


const static int WIDTH = 31;

Heuristic::Heuristic(Node * goal, GridGraph* grid)
{
	mpGridGraph = grid;
	mpGoalNode = goal;

	goalX = goal->getId() % WIDTH;
	goalY = goal->getId() / WIDTH;
}

Heuristic::Heuristic(Node * goal)
{
	mpGoalNode = goal;

	goalX = goal->getId() % WIDTH;
	goalY = goal->getId() / WIDTH;
}

float Heuristic::estimate(Node* node)
{
	int x, y;
	x = node->getId() % WIDTH;
	y = node->getId() / WIDTH;
	
	float length = sqrt(powf((float)x - goalX, 2.0f) + powf((float)y - goalY, 2.0f));
	node->getId();
	
	if (mpGridGraph)
	{
		
	}
	
	return length;
}