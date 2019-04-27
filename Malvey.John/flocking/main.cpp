#include <stdio.h>

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Unit.h"
#include "Timer.h"
#include "PerformanceTracker.h"
#include "MemoryTracker.h"
#include "NotificationQueue.h"
#include "CircularQueue.h"
#include "ComponentManager.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"

using namespace std;

//test callback
/*void testCallback(Notification* pMessage)
{
	cout << "message callback: message id=" << pMessage->getID() << endl;
}*/

PerformanceTracker* gpPerformanceTracker = NULL;

int main(int argc, char **argv)
{
	gpPerformanceTracker = new PerformanceTracker;
	gpPerformanceTracker->startTracking("init");

	//create the global game object
	gpGame = new Game;
	//init the game
	bool goodGame = gpGame->init();
	if (!goodGame)
	{
		fprintf(stderr, "failed to initialize Game object!\n");
		return -1;
	}

	gpPerformanceTracker->stopTracking("init");
	cout << "initialization took:" << gpPerformanceTracker->getElapsedTime("init") << "ms\n";
	bool shouldExit = false;

	//game loop
	while (!shouldExit)
	{
		gpPerformanceTracker->clearTracker("loop");
		gpPerformanceTracker->startTracking("loop");

		gpGame->beginLoop();

		gpPerformanceTracker->clearTracker("draw");
		gpPerformanceTracker->startTracking("draw");

		gpGame->processLoop();

		gpPerformanceTracker->stopTracking("draw");

		shouldExit = gpGame->endLoop();

		gpPerformanceTracker->stopTracking("loop");
		
	}
	gpGame->saveWeights();
	gpGame->cleanup();
	delete gpGame;
	gpGame = NULL;

	delete gpPerformanceTracker;
	gMemoryTracker.reportAllocations( cout );
	system("pause");

	return 0;
}