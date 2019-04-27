#include "GridGraph.h"
#include "Connection.h"
#include "Node.h"
#include "Grid.h"
#include "Game.h"
#include <vector>

GridGraph::GridGraph( Grid* pGrid )
:Graph()
,mpGrid(pGrid)
{
}

GridGraph::~GridGraph()
{
}

void GridGraph::init()
{
	if( mpGrid != NULL )
	{
		int numGridIndices = mpGrid->getGridWidth() * mpGrid->getGridHeight();

		//size the node vector
		mNodes.resize( numGridIndices, NULL );
		
		//go through all elements of the grid and make Nodes for them - also add them to Node vector
		for( int i=0; i<numGridIndices; i++ )
		{
			//create a Node for this location
			Node* pNode = new Node( i );
			//add to NodeVector
			mNodes[i] = pNode;
		}

		//go through all elements of the grid - get adjacencies and create connections for each
		for( int i=0; i<numGridIndices; i++ )
		{
			//get the Node for this location
			Node* pFromNode = mNodes[i];

			//get all the adjacencies
			std::vector<int> adjacencies = mpGrid->getAdjacentIndices( i );

			//create a vector of Connections
			std::vector<Connection*> connections;

			//go through all adjacent indices, get the nodes for them, create connections, and add to connections map
			for( unsigned int adjIndex = 0; adjIndex < adjacencies.size(); adjIndex++ )
			{
				//check for  terrain
				switch (mpGrid->getValueAtIndex(adjacencies[adjIndex]))
				{
				case(CLEAR_VALUE):
				{

				Node* pToNode = mNodes[adjacencies[adjIndex]];//find to node

				Connection* pConnection = new Connection(pFromNode, pToNode, adjIndex % 2 == 0 ? 1.0f : 2.0f);//create a connection
				mConnections.push_back(pConnection);
				connections.push_back(pConnection);
				}
				case(TERRAIN_SAND):
				{
				Node* pToNode = mNodes[adjacencies[adjIndex]];//find to node

				Connection* pConnection = new Connection(pFromNode, pToNode, adjIndex % 2 == 0 ? 4.0f : 5.0f);//create a connection
																						
				mConnections.push_back(pConnection);
				connections.push_back(pConnection);
				}
				case(TERRAIN_WATER):
				{
				Node* pToNode = mNodes[adjacencies[adjIndex]];//find to node

				Connection* pConnection = new Connection(pFromNode, pToNode, adjIndex % 2 == 0 ? 8.0f : 9.0f);//create a connection
																							
				mConnections.push_back(pConnection);
				connections.push_back(pConnection);
				}
				}
			}

			//put temp connection vector into map
			mConnectionMap[i] = connections;
		}
	}
}
