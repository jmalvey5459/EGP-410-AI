#pragma once
#include "Node.h"
#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;

//This structure is used to keep track of the
//information we need for each node
struct NodeRecord 
{
	Node* node;
	Connection* connection;
	float costSoFar;
};


class Dijkstra :public GridPathfinder
{
public:
	Dijkstra(Graph* pGraph);
	~Dijkstra();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

private:
	bool containedIn(Node* check, std::vector<NodeRecord*> list); //im not gonna be a moron for this one
	NodeRecord* findInList(Node* node, std::vector<NodeRecord*> list);

};

