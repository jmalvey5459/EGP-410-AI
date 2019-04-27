/*
Author: Gabe Troyan
I certify this is all my original work, except where otherwise specified.
*/

#pragma once

#include "GridPathfinder.h"
#include <vector>
#include <list>

#define Nodelist std::vector<NodeRecord*> 
#define Connections std::vector<Connection*> 


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;



class DijkstrasPathfinder : public GridPathfinder
{
	struct NodeRecord
	{
		static NodeRecord smallestNode;
		Node* mpNode = NULL;
		Connection* mpConnection = NULL;
		float costSoFar = 0;
		bool operator==(NodeRecord &rhs)
		{
			return mpNode == rhs.mpNode;
		}
		bool operator==(const Node &rhs)
		{
			return mpNode == &rhs;
		}
	};
	NodeRecord * smallestNode(Nodelist  list);
	bool containsNode(Nodelist  list, Node* node);
	NodeRecord* findNode(Nodelist  list, Node* node);
public:

	
	DijkstrasPathfinder(Graph* pGraph);
	~DijkstrasPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);
};

