#include "Game.h"
#include "GameApp.h"
#include "PathToMessage.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsBufferManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include <Vector2D.h>
#include "Grid.h"
#include "GridGraph.h"
#include "Connection.h"
#include "Path.h"
#include "DepthFirstPathfinder.h"
#include "Pathfinder.h"
#include "GridPathfinder.h"
#include "GridVisualizer.h"
#include "DebugDisplay.h"
#include "PathfindingDebugContent.h"
#include "Dijkstra.h"
#include "A-STAR.h"
#include "InputSystem.h"
#include <SDL.h>
#include <fstream>
#include <vector>
#include "GameMessageManager.h"
const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "pathgrid.txt";
GameApp::GameApp()
:mpGrid(NULL)
,mpGridGraph(NULL)
,mpPathfinder(NULL)
,mpDebugDisplay(NULL)
{
}

GameApp::~GameApp()
{
	cleanup();
}

bool GameApp::init()
{
	bool retVal = Game::init();
	if( retVal == false )
	{

		return false;
	}


	//create and load the Grid, GridBuffer, and GridRenderer
	mpGrid = new Grid(mpGraphicsSystem->getWidth(), mpGraphicsSystem->getHeight(), GRID_SQUARE_SIZE);
	mpGridVisualizer = new GridVisualizer( mpGrid );
	std::ifstream theStream( gFileName );
	mpGrid->load( theStream );

	//create the GridGraph for pathfinding
	mpGridGraph = new GridGraph(mpGrid);
	//init the nodes and connections
	mpGridGraph->init();


	mpMessageManager = new GameMessageManager();
	mpDFSPathFinder = new DepthFirstPathfinder(mpGridGraph);
	mpDijkstraPathfinder = new Dijkstra (mpGridGraph);
	mpAstarPathfinder = new ASTAR(mpGridGraph);
	//default to DFS
	mpPathfinder = mpDFSPathFinder;


	//load input manager
	mpInputManager = new InputSystem();

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}

	//debug display
	PathfindingDebugContent* pContent = new PathfindingDebugContent( mpPathfinder );
	mpDebugDisplay = new DebugDisplay( Vector2D(0,12), pContent );

	mpMasterTimer->start();
	return true;
}

void GameApp::cleanup()
{
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpGrid;
	mpGrid = NULL;

	delete mpGridVisualizer;
	mpGridVisualizer = NULL;

	delete mpGridGraph;
	mpGridGraph = NULL;


	if (mpDFSPathFinder != NULL)
	{
		delete mpDFSPathFinder;
		mpDFSPathFinder = NULL;
	}
	if (mpDijkstraPathfinder != NULL)
	{
		delete mpDijkstraPathfinder;
		mpDijkstraPathfinder = NULL;
	}
	if (mpAstarPathfinder != NULL)
	{
		delete mpAstarPathfinder;
		mpAstarPathfinder = NULL;
	}

	delete mpDebugDisplay;
	mpDebugDisplay = NULL;

	delete mpInputManager;
	mpInputManager = NULL;
}

void GameApp::beginLoop()
{
	//should be the first thing done
	Game::beginLoop();
}

void GameApp::processLoop()
{
	//get back buffer
	GraphicsBuffer* pBackBuffer = mpGraphicsSystem->getBackBuffer();
	//copy to back buffer
	
	mpDebugDisplay->draw(pBackBuffer);
	mpGridVisualizer->draw( *pBackBuffer );
#ifdef VISUALIZE_PATH
	//show pathfinder visualizer
	mpPathfinder->drawVisualization(mpGrid, pBackBuffer);
#endif
	

	
	mpMessageManager->processMessagesForThisframe();

	//get input
	InputSystem::getInput();
	//should be last thing in processLoop
	Game::processLoop();
}

bool GameApp::endLoop()
{
	return Game::endLoop();
}
void GameApp::setPathfinder(PathType type)
{
	switch (type)
	{
	case(DFS_TYPE):
		mpPathfinder = mpDFSPathFinder;
		break;
	case(DIJKSTRA_TYPE):
		mpPathfinder = mpDijkstraPathfinder;
		break;
	case(ASTAR_TYPE):
		mpPathfinder = mpAstarPathfinder;
		break;
	}
}