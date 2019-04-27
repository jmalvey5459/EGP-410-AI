/*
Author: Gabe Troyan
I certify this is all my original work, except where otherwise specified.
*/

#include "aStarPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <vector>
#include <algorithm>

using namespace std;


aStarPathfinder::aStarPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}

aStarPathfinder::~aStarPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

bool aStarPathfinder::containsNode(Nodelist list, Node* node)
{
	for (const auto tempNode : list)
	{
		if (node->getId() == tempNode->mpNode->getId())
		{
			return true;
		}
	}
	return false;
}

aStarPathfinder::NodeRecord* aStarPathfinder::findNode(Nodelist list, Node* node)
{
	for (const auto tempNode : list)
	{
		if (node->getId() == tempNode->mpNode->getId())
		{
			return tempNode;
		}
	}
	return NULL;
}

Path* aStarPathfinder::findPath(Node* pFrom, Node* pTo)
{

	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	Nodelist open;
	Nodelist closed;
	NodeRecord *startRecord = new NodeRecord();
	startRecord->mpNode = pFrom;
	Heuristic heuristic = Heuristic(pTo);
	startRecord->estimatedTotalCost = heuristic.estimate(pFrom);

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif

	NodeRecord* pCurrent = NULL;

	//	initialise open and closed
	open.push_back(startRecord);

	while (open.size() > 0)
	{
		// Find the smallest element in the open list
		pCurrent = smallestNode(open);

		// If it is the goal, break.
		if (pCurrent->mpNode == pTo)
			break;

		// otherwise get connections
		auto connections = mpGraph->getConnections(*pCurrent->mpNode);

		//loop through connections
		for (const auto connection : connections)
		{
			auto endNode = connection->getToNode();
			float endNodeCost = pCurrent->costSoFar + connection->getCost();
			NodeRecord* endNodeRecord;
			float endNodeHeuristic = 0.0f;
			// skips if node is in closed list
			if (containsNode(closed, endNode))
			{
				//	here we find record in the closed list
				endNodeRecord = findNode(closed, endNode);

				//	if we didn't find a shorter route, skip
				if (endNodeRecord->costSoFar <= endNodeCost)
				{
					continue;

				}

				/* We can use the node’s old cost values
					 to calculate its heuristic without calling
					 the possibly expensive heuristic function*/

				endNodeHeuristic = endNodeCost - endNodeRecord->costSoFar;
			}
			//	skip if the node is open and we've no better route
			else if (containsNode(open, endNode))
			{
				//here we find record in open list
				endNodeRecord = findNode(open, endNode);

				//	if our route is no better, skip
				if (endNodeRecord->costSoFar <= endNodeCost)
				{
					continue;
				}

				/* We can use the node’s old cost values
				to calculate its heuristic without calling
				the possibly expensive heuristic function*/

				endNodeHeuristic = endNodeCost - endNodeRecord->costSoFar;
			}
			//	otherwise we know we got an unvisited node
			else
			{
				endNodeRecord = new NodeRecord();
				endNodeRecord->mpNode = endNode;

				/*	We’ll need to calculate the heuristic value
					using the function, since we don’t have an
					existing record to use	*/
				endNodeHeuristic = heuristic.estimate(endNode);
			}
			//	we're here if we need to update the node
			endNodeRecord->costSoFar = endNodeCost;
			endNodeRecord->mpConnection = connection;
			endNodeRecord->estimatedTotalCost = endNodeCost + endNodeHeuristic;

			//  add it to the open list
			if (!containsNode(open, endNode))
			{
				open.push_back(endNodeRecord);
			}
		}
		//	we've finished looking at the connections for the current node
		//	so add it to the closed list
		auto newEnd = remove(open.begin(), open.end(), pCurrent);
		open.erase(newEnd, open.end());
		closed.push_back(pCurrent);
#ifdef VISUALIZE_PATH
		mVisitedNodes.push_back(pCurrent->mpNode);
#endif
	}

	//	At this point we've either found the goal or we've no more nodes
	if (pCurrent->mpNode != pTo)
	{
		//	we've run out of nodes; oh noes
		return NULL;
	}
	else
	{
		//	Compile list of connections in the path
		Path* pPath = new Path();
		Nodelist nodesToAdd;

		while (pCurrent->mpNode != pFrom)
		{
			nodesToAdd.push_back(pCurrent);
			pCurrent = findNode(closed, pCurrent->mpConnection->getFromNode());
		}
		pPath->addNode(pFrom);
		reverse(nodesToAdd.begin(), nodesToAdd.end());
		for (const auto node : nodesToAdd)
		{
			pPath->addNode(node->mpNode);

		}

		gpPerformanceTracker->stopTracking("path");
		mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");
#ifdef VISUALIZE_PATH
		mpPath = pPath;
#endif
		//cout << "Path created!\n";
		return pPath;
	}

}

aStarPathfinder::NodeRecord * aStarPathfinder::smallestNode(Nodelist list)
{
	NodeRecord* smallest = list.front();
	for (NodeRecord* temp : list)
	{
		smallest = temp->costSoFar < smallest->costSoFar ? temp : smallest;
	}
	return smallest;
}

