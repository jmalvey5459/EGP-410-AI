/*main.cpp
	*
	*	Dean Lawson
	*	Champlain College
	*	2011
	*
	*/
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>

#include <SDL.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Defines.h"
#include <Timer.h>
#include <MemoryTracker.h>
#include <PerformanceTracker.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

PerformanceTracker* gpPerformanceTracker = NULL;

int main(int argc, char **argv)
{

	srand((unsigned int)time(NULL));
	gpPerformanceTracker = new PerformanceTracker();

	gpGame = new Game();

	gpGame->init();

	bool shouldExit = false;

	while( !shouldExit )
	{
		gpGame->beginLoop();
		gpGame->processLoop();
		shouldExit = gpGame->endLoop();
	}

	//cleanup
	gpGame->cleanup();
	delete gpGame;
	delete gpPerformanceTracker;

	gMemoryTracker.reportAllocations( std::cout );
//	_CrtDumpMemoryLeaks();
	system("pause");

	return 0;
}

