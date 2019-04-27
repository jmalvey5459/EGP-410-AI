#include "PathToMessage.h"
#include "../Game.h"
#include "GridPathfinder.h"
#include "Grid.h"
#include "GridGraph.h"

PathToMessage::PathToMessage( const Vector2D& from, const Vector2D& to )
:GameMessage(PATH_TO_MESSAGE)
,mFrom(from)
,mTo(to)
{
}

PathToMessage::~PathToMessage()
{
}

void PathToMessage::process()
{
	
	
	if( gpGame != NULL ) 
	{
		/*
		GridPathfinder* pPathfinder = gpGame->getPathfinder();
		GridGraph* pGridGraph = gpGame->getGridGraph();
		Grid* pGrid = gpGame->getGrid();
		int fromIndex = pGrid->getSquareIndexFromPixelXY( (int)mFrom.getX(), (int)mFrom.getY() );
		int toIndex = pGrid->getSquareIndexFromPixelXY( (int)mTo.getX(), (int)mTo.getY() );
		Node* pFromNode = pGridGraph->getNode( fromIndex );
		Node* pToNode = pGridGraph->getNode( toIndex );
		pPathfinder->findPath( pFromNode, pToNode );
		*/
	}
}
