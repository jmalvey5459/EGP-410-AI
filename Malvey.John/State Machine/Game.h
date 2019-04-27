#pragma once

#include "Trackable.h"
#include "PerformanceTracker.h"
#include "Defines.h"
#include <string>

class GraphicsSystem;
class GraphicsBuffer;
class Font;
class GraphicsBufferManager;
class SpriteManager;
class KinematicUnit;
class GameMessageManager;
class Timer;
class ComponentManager;
class UnitManager;
class ItemManager;




/*
class GridPathfinder;
class Grid;
class GridGraph;
class GridVisualizer;
class DebugDisplay;
*/
const IDType BACKGROUND_SPRITE_ID = 0;
const IDType PLAYER_ICON_SPRITE_ID = 1;
const IDType AI_ICON_SPRITE_ID = 2;
const IDType TARGET_SPRITE_ID = 3;
const IDType HUNTER_ID = 4;
const IDType BOOSTER_ID = 5;
const IDType HEALER_ID = 6;
const IDType HEAL_ID = 7;
const IDType BOOST_ID = 8;
const IDType STRENGTH_ID = 9;

const float LOOP_TARGET_TIME = 33.3f;//how long should each frame of execution take? 30fps = 33.3ms/frame

class Game:public Trackable
{
public:
	Game();
	~Game();

	bool init();//returns true if no errors, false otherwise
	void cleanup();

	//game loop
	void beginLoop();
	void processLoop();
	bool endLoop();
	void exitGame();
	void readData();

	//void reloadGrid();

	inline GraphicsSystem* getGraphicsSystem() const { return mpGraphicsSystem; };
	inline GraphicsBufferManager* getGraphicsBufferManager() const { return mpGraphicsBufferManager; };
	inline SpriteManager* getSpriteManager() const { return mpSpriteManager; };
	inline GameMessageManager* getMessageManager() { return mpMessageManager; };
	inline ComponentManager* getComponentManager() { return mpComponentManager; };
	inline UnitManager* getUnitManager() { return mpUnitManager; };
	inline ItemManager* getItemManager() { return mpItemManager; };
	inline Timer* getMasterTimer() const { return mpMasterTimer; };
	inline double getCurrentTime() const { return mpMasterTimer->getElapsedTime(); };
	inline Font* getFont() const { return mpFont; };
	/*
	inline Grid* getGrid() const { return mpGrid; };
	inline GridPathfinder* getPathfinder() const { return mpPathfinder; };
	inline GridGraph* getGridGraph() const { return mpGridGraph; };
	inline GridVisualizer* getGridVisualizer() const { return mpGridVisualizer; };
	*/
	int getScore() { return mScore; };


	//DATA DRIVEN VARIABLES:
	float mEnemyHealth;
	float	mEnemyDamage;
	int	mEnemySpawnRate;
	float	mEnemyDamageRadius;
	float	mEnemySeekRadius;
	float	mPlayerHealth;
	float	mPlayerSpeedMult;
	float	mPlayerDamage;
	float	mPlayerDamageRadius;
	float	mHealthUpSpawnRate;
	float	mHealthUpValue;
	float	mSpeedUpSpawnRate;
	float	mSpeedUpValue;
	float	mSpeedUpDuration;
	float	mDamageUpSpawnRate;
	float	mDamageUpValue;
	float	mDamageUpDuration;

	void genItems();

private:
	GraphicsSystem* mpGraphicsSystem;
	GraphicsBufferManager* mpGraphicsBufferManager;
	SpriteManager* mpSpriteManager;
	GameMessageManager* mpMessageManager;
	ComponentManager* mpComponentManager;
	UnitManager* mpUnitManager;
	ItemManager* mpItemManager;
	Font* mpFont;
	Timer* mpLoopTimer;
	Timer* mpMasterTimer;
	Timer* mpScoreTimer;
	/*
	Grid * mpGrid;
	GridPathfinder* mpPathfinder;
	DebugDisplay* mpDebugDisplay;
	GridVisualizer* mpGridVisualizer;
	GridGraph* mpGridGraph;
	*/


	bool mSpawnAgain = true;

	bool mShouldExit;

	//should be somewhere else
	GraphicsBufferID mBackgroundBufferID = "woods";
	GraphicsBufferID mPlayerIconBufferID = "player";
	//GraphicsBufferID mPlayerDamagedID = "playerDamaged";
	GraphicsBufferID mEnemyIconBufferID = "enemyBase";
	GraphicsBufferID mEnemyIconBufferHunterID = "enemyHunter";
	GraphicsBufferID mEnemyIconBufferBoosterID = "enemyBooster";
	GraphicsBufferID mEnemyIconBufferHealerID = "enemyHealer";
	//GraphicsBufferID mEnemyDamagedID = "enemeyDamaged";
	GraphicsBufferID mTargetBufferID = "target";
	GraphicsBufferID mBoostID = "boost";
	GraphicsBufferID mHealID = "heal";
	GraphicsBufferID mStrengthID = "strength";


	int mScore = 0;

};

float genRandomBinomial();//range -1:1 from "Artificial Intelligence for Games", Millington and Funge
float genRandomFloat();//range 0:1 from "Artificial Intelligence for Games", Millington and Funge

extern Game* gpGame;
extern PerformanceTracker* gpPerformanceTracker;

