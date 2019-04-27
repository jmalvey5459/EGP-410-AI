/*
	Author: Gabe Troyan
	I certify this is all my original work, except where otherwise specified. 
*/

#include "DijkstrasPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <vector>
#include <algorithm>

using namespace std;

DijkstrasPathfinder::DijkstrasPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}

DijkstrasPathfinder::~DijkstrasPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

bool DijkstrasPathfinder::containsNode(Nodelist list, Node* node)
{
	for (auto iter = list.begin(); iter != list.end(); ++iter)
	{
		if (node->getId() == (*iter)->mpNode->getId())
		{
			return true;
		}
	}
	return false;
}

DijkstrasPathfinder::NodeRecord* DijkstrasPathfinder::findNode(Nodelist list, Node* node)
{
	for (auto iter = list.begin(); iter != list.end(); ++iter)
	{
		if (node->getId() == (*iter)->mpNode->getId())
		{
			return (*iter);
		}
	}
	return NULL;
}

Path* DijkstrasPathfinder::findPath(Node* pFrom, Node* pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	Nodelist open;
	Nodelist closed;
	NodeRecord *startRecord = new NodeRecord();
	startRecord->mpNode = pFrom;
	
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
		if(pCurrent->mpNode == pTo) 
			break;

		// otherwise get connections
		auto connections = mpGraph->getConnections(*pCurrent->mpNode);
		NodeRecord* endNodeRecord;

		//loop through connections
		for (const auto connection : connections)
		{
			auto endNode = connection->getToNode();
			float endNodeCost = pCurrent->costSoFar + connection->getCost();
			//endNodeCost = endNodeCost > 0 ? endNodeCost : 0;
			// skips if node is in closed list
			if (containsNode(closed, endNode))
			{
				continue;
			}
			//or if it is open and we've found a worse route
			else if (containsNode(open, endNode))
			{
				//here we find record in open list
				
				endNodeRecord = findNode(open, endNode);
				if (endNodeRecord->costSoFar <= endNodeCost)
				{
					continue;
				}
			}
			//	otherwise we know we got an unvisited node
			else
			{
				endNodeRecord = new NodeRecord();
				endNodeRecord->mpNode = endNode;
			}
			//	we're here if we need to update the node
			
			endNodeRecord->costSoFar = endNodeCost;
			endNodeRecord->mpConnection = connection;

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
		
		//Connections connections;
		while (pCurrent->mpNode != pFrom)
		{
			nodesToAdd.push_back(pCurrent);
			//connections.push_back(pCurrent->mpConnection);
			pCurrent = findNode(closed, pCurrent->mpConnection->getFromNode());
			//pCurrent = pCurrent->mpConnection->getFromNode();
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
		return pPath;
	}
	
}

DijkstrasPathfinder::NodeRecord * DijkstrasPathfinder::smallestNode(Nodelist list)
{
	NodeRecord* smallest = list.front();
	for (NodeRecord* temp : list)
	{
		smallest = temp->costSoFar < smallest->costSoFar ? temp : smallest;
	}
	return smallest;
}

