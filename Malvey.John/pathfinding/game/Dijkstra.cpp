#include "Dijkstra.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include "Connection.h"
using namespace std;

Dijkstra::Dijkstra(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif

}

Dijkstra::~Dijkstra()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path* Dijkstra::findPath(Node* pFrom, Node* pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	//create Path
	Path* pPath = new Path();

	//Graph* pGraph = new Graph();
	//Initialize the record for the start node
	NodeRecord *pStartRecord = new NodeRecord();
	pStartRecord->node = pFrom;
	pStartRecord->connection = NULL;
	pStartRecord->costSoFar = 0;


	// Initialize the open and closed lists
	vector<NodeRecord*> open,closed;
	open.push_back(pStartRecord);

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	mVisitedNodes.push_back(pFrom);
#endif
	
	NodeRecord *pCurrent;
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
		NodeRecord* endNodeRecord;
		// Loop through each connection in turn
		 for (unsigned int i = 0; i < pConnections.size(); i++)
		{
			// Get the cost estimate for the end node
			 Node* endNode = pConnections[i]->getToNode();
			 float endNodeCost = pCurrent->costSoFar + pConnections[i]->getCost();

			 //Skip if the node is closed
			bool isClosed = false;
			bool isOpen = false;
			 
			isClosed = containedIn(endNode, closed);
			isOpen = containedIn(endNode, open);
			

			 // = new NodeRecord;

			 if (isClosed)
				 continue;
			 else if (isOpen)
			 {
				 // Here we find the record in the open list corresponding to the endNode.
				 
				 endNodeRecord = findInList(endNode, open);

				 if (endNodeRecord->costSoFar <= endNodeCost)
					 continue;	
			 }
			//Otherwise we know we’ve got an unvisited node, so make a record for it
			 else
			 {
				 endNodeRecord = new NodeRecord;
				 endNodeRecord->node = endNode;
			 }
			
			 //We’re here if we need to update the node 
			 //Update the cost and connection
			endNodeRecord->costSoFar = endNodeCost;
			endNodeRecord->connection = pConnections[i];

			// And add it to the open list
			bool dontAdd = false;
			for (unsigned int c = 0; c < open.size(); c++)
			{
				if (endNode == open[c]->node)
				{
					dontAdd = true;
				}
			}
			
			if (!dontAdd) //gabe is currently yelling at me for doing this but i stand by my horrible decision
				open.push_back(endNodeRecord);
			
		}
		 // We’ve finished looking at the connections for the current node, so add it to the closed list and remove it from the open list
		 auto deleteMe = remove(open.begin(), open.end(), pCurrent);
		 open.erase(deleteMe);
		closed.push_back(pCurrent);
	}

	//We’re here if we’ve either found the goal, or if we’ve no more nodes to search, find which.
	if (pCurrent->node != pTo)
	{
		// We’ve run out of nodes without finding the goal, so there’s no solution
		return NULL;
	}


	else
	{	
		Path* pFrontPath = new Path();
		

		//Work back along the path, accumulating connections
		 pFrontPath->addNode(pTo);
		 while(pCurrent->node!= pFrom)
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
			//pCurrent->node = pCurrent->connection->getFromNode();
			
			
		 }
		 
		 //reverse reverse (1 hop this time)
		 while (pFrontPath->getNumNodes() > 0)
		 {
			 Node* tempNode = pFrontPath->getAndRemoveNextNode();
			 pPath->addNode(tempNode);
		 }

		 

#ifdef VISUALIZE_PATH
			//	mVisitedNodes.push_back(pTempToNode);
#endif


	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mpPath = pPath;
#endif
	delete pFrontPath;
	return pPath;
	}
}


bool Dijkstra::containedIn(Node* node, std::vector<NodeRecord*> list)
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

NodeRecord* Dijkstra::findInList(Node* node, std::vector<NodeRecord*> list)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (node == list[i]->node)
			return list[i];
	}
	//shouldn't happen, but might
	return NULL;
}