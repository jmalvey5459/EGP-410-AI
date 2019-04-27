#pragma once
#include "Node.h"
#include "GridPathfinder.h"
#include <vector>
#include "Vector2D.h"
#include "Game.h"
#include "GameApp.h"
#include "Grid.h"

class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;
class GameApp;

//This structure is used to keep track of the
//information we need for each node

//changed name to avoid redefinition
struct NodeRecordA
{
	Node* node;
	Connection* connection;
	float costSoFar, estimatedTotalCost;
};

struct Heuristic
{

	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	Grid* pGrid = pGame->getGrid();
	float mEstimate;
	float getEstimate(Node* nodeToEstimate, Node* goalNode)
	{
		Vector2D startPoint  =	pGrid->getULCornerOfSquare(nodeToEstimate->getId());
		Vector2D endPoint = pGrid->getULCornerOfSquare(goalNode->getId());
		Vector2D estimate2D = endPoint - startPoint;
		mEstimate = estimate2D.getLength();
		return mEstimate;
	};
};

class ASTAR :public GridPathfinder
{
public:
	ASTAR(Graph* pGraph);
	~ASTAR();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

private:
	bool containedIn(Node* check, std::vector<NodeRecordA*> list); //im not gonna be a moron for this one
	NodeRecordA* findInList(Node* node, std::vector<NodeRecordA*> list);

};

#pragma once
