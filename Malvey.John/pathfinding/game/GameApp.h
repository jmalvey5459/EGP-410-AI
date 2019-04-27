#pragma once

/*Game - class to hold all game related info.

Dean Lawson
Champlain College
2011
*/

#include "Game.h"
//forward declarations
class GraphicsBuffer;
class Sprite;
class KinematicUnit;
class Grid;
class GridVisualizer;
class GridGraph;
class GridPathfinder;
class DebugDisplay;
class InputSystem;
class Game;
#include "ChangeModeMessage.h"
const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame



class GameApp: public Game
{
public:
	GameApp();
	~GameApp();

	virtual bool init();
	virtual void cleanup();

	//game loop
	virtual void beginLoop();
	virtual void processLoop();
	virtual bool endLoop();

	//accessors
	inline GridVisualizer* getGridVisualizer() { return mpGridVisualizer; };
	inline GridPathfinder* getPathfinder() { return mpPathfinder; };
	inline Grid* getGrid() { return mpGrid; };
	inline GridGraph* getGridGraph() { return mpGridGraph; };

	void setPathfinder(PathType type);
private:
	Grid* mpGrid;
	GridVisualizer* mpGridVisualizer;
	GridGraph* mpGridGraph;
	DebugDisplay* mpDebugDisplay;

	//active pathfinder
	GridPathfinder* mpPathfinder;

	//pathfinding "modes"
	GridPathfinder* mpAstarPathfinder;
	GridPathfinder* mpDijkstraPathfinder;
	GridPathfinder* mpDFSPathFinder;

	InputSystem* mpInputManager;
};

