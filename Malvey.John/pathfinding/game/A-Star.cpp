#include "A-Star.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include "Connection.h"
#include "GameApp.h"
using namespace std;


ASTAR::ASTAR(Graph* pGraph) : GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}
ASTAR::~ASTAR()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path* ASTAR::findPath(Node* pFrom, Node* pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	Path* pPath = new Path();
	//Initialize the record for the start node
	NodeRecordA* pStartRecord = new NodeRecordA();
	Heuristic heuristic;
	pStartRecord->node = pFrom;
	pStartRecord->connection = NULL;
	pStartRecord->costSoFar = 0;
	pStartRecord->estimatedTotalCost = heuristic.getEstimate(pFrom, pTo);

	// Initialize the open and closed lists
	vector<NodeRecordA*> open, closed;
	open.push_back(pStartRecord);

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif

	NodeRecordA *pCurrent;
	//Iterate through processing each node
	while (open.size() > 0)
	{

		//Find the smallest element in the open list
		pCurrent = open.front();

		//If it is the goal node, then terminate
		if (pCurrent->node == pTo)
			break;

		//Otherwise get its outgoing connections
		vector<Connection*> pConnections;
		pConnections = mpGraph->getConnections(pCurrent->node->getId());

		NodeRecordA* endNodeRecord;
		// Loop through each connection in turn
		for (unsigned int i = 0; i < pConnections.size(); i++)
		{
			// Get the cost estimate for the end node
			Node* endNode = pConnections[i]->getToNode();
			float endNodeCost = pCurrent->costSoFar + pConnections[i]->getCost();
			float endNodeHeuristic = 0.0f;

			//If the node is closed we may have to
			// skip, or remove it from the closed list.
			if (containedIn(endNode, closed))
			{
				//Here we find the record in the closed list
				// corresponding to the endNode.
				endNodeRecord = findInList(endNode, closed);

				//If we didn’t find a shorter route, skip
				if (endNodeRecord->costSoFar <= endNodeCost)
					continue;

				// Otherwise remove it from the closed list
			//	auto deleteMe = remove(closed.begin(), closed.end(), endNodeRecord);
			//	closed.erase(deleteMe);

				// We can use the node’s old cost values  to calculate its heuristic without calling the possibly expensive heuristic function
				endNodeHeuristic = (endNodeCost - endNodeRecord->costSoFar);
			}
			else if (containedIn(endNode, open))
			{
				//Here we find the record in the open list
				// corresponding to the endNode.
				endNodeRecord = findInList(endNode, open);

				//If we didn’t find a shorter route, skip
				if (endNodeRecord->costSoFar <= endNodeCost)
					continue;
				endNodeHeuristic = (endNodeCost - endNodeRecord->costSoFar);

			}
			else
			{
				endNodeRecord = new NodeRecordA();
				endNodeRecord->node = endNode;

				// We’ll need to calculate the heuristic value using the function, since we don’t have an existing record to use
				endNodeHeuristic = heuristic.getEstimate(pCurrent->node, endNode);
			}
			// We’re here if we need to update the node Update the cost, estimate and connection
			endNodeRecord->costSoFar = endNodeCost;
			endNodeRecord->connection = pConnections[i];
			endNodeRecord->estimatedTotalCost = endNodeCost + endNodeHeuristic;

			// And add it to the open list
			if (!containedIn(endNode, open))
				open.push_back(endNodeRecord);
		}

		// We’ve finished looking at the connections for
		// the current node, so add it to the closed list
		// and remove it from the open list
		auto deleteMe = remove(open.begin(), open.end(), pCurrent);
		open.erase(deleteMe);

		closed.push_back(pCurrent);
	}

	//We’re here if we’ve either found the goal, or
	//if we’ve no more nodes to search, find which.
	if (pCurrent->node != pTo)
	{
		// We’ve run out of nodes without finding the
	   // goal, so there’s no solution
		return (NULL);
	}
	else
	{
		// Compile the list of connections in the path

		Path* pFrontPath = new Path();
		//Work back along the path, accumulating connections
		pFrontPath->addNode(pTo);
		while (pCurrent->node != pFrom)
		{
			for (unsigned int o = 0; o < closed.size(); o++)
			{
				if (pCurrent->connection->getFromNode() == closed[o]->node)
				{
					pFrontPath->addNode(mpGraph->getNode(closed[o]->node->getId()));
					pCurrent = closed[o];
					break;
				}
			}
		}
		//Reverse the path, and return it
		
		while (pFrontPath->getNumNodes() > 0)
		{
			Node* tempNode = pFrontPath->getAndRemoveNextNode();
			pPath->addNode(tempNode);
		}

		gpPerformanceTracker->stopTracking("path");
		mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
		mpPath = pPath;
#endif
		delete pFrontPath;
		return pPath;
	}
}

bool ASTAR::containedIn(Node* node, std::vector<NodeRecordA*> list)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (node == list[i]->node)
		{
			return true;
		}
	}
	return false;
}

NodeRecordA* ASTAR::findInList(Node* node, std::vector<NodeRecordA*> list)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (node == list[i]->node)
			return list[i];
	}
	//shouldn't happen, but might
	return NULL;
}
