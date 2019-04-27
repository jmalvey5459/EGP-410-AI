#include <stdio.h>
#include <assert.h>

#include <sstream>
#include <SDL.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Font.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Timer.h"
#include "PlayerMoveToMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "ItemManager.h"
#include "ArriveSteering.h"
#include "CreateItemMessage.h"

#include <Vector2D.h>

#include <fstream>

#include <fstream>
#include <vector>
#include <ctime>

#include "SpawnSystem.h"
#include "InputSystem.h"

Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;
const Uint32 MAX_ITEMS = 10;

const int GRID_SQUARE_SIZE = 32;
const std::string gFileName = "gamedata.txt";

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpFont(NULL)
	,mShouldExit(false)
	,mBackgroundBufferID("")
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpItemManager(NULL)
{
	readData();
}

Game::~Game()
{
	cleanup();
}

void Game::readData()
{

	std::string junk;
	std::ifstream varReader;
	varReader.open(gFileName);
	if (varReader)
	{
		while (!varReader.eof())
		{
			varReader >> junk;
			varReader >> mEnemyHealth;
			varReader >> junk;
			varReader >> mEnemyDamage;
			varReader >> junk;
			varReader >> mEnemySpawnRate;
			varReader >> junk;
			varReader >> mEnemyDamageRadius;
			varReader >> junk;
			varReader >> mEnemySeekRadius;
			varReader >> junk;
			varReader >> mPlayerHealth;
			varReader >> junk;
			varReader >> mPlayerSpeedMult;
			varReader >> junk;
			varReader >> mPlayerDamage;
			varReader >> junk;
			varReader >> mPlayerDamageRadius;
			varReader >> junk;
			varReader >> mHealthUpSpawnRate;
			varReader >> junk;
			varReader >> mHealthUpValue;
			varReader >> junk;
			varReader >> mSpeedUpSpawnRate;
			varReader >> junk;
			varReader >> mSpeedUpValue;
			varReader >> junk;
			varReader >> mSpeedUpDuration;
			varReader >> junk;
			varReader >> mDamageUpSpawnRate;
			varReader >> junk;
			varReader >> mDamageUpValue;
			varReader >> junk;
			varReader >> mDamageUpDuration;
		}

	}
	else
	{
		varReader.close();
		exitGame();
	}
	varReader.close();
	
}


bool Game::init()
{
	///finding the data type of the memory leak
//	std::cout << "Vector2D size: " << sizeof(Vector2D) << std::endl;
//	std::cout << "Steering size: " << sizeof(Steering) << std::endl;
//	std::cout << "SteeringComponent size: " << sizeof(SteeringComponent) << std::endl;
//	std::cout << "GameMessageSize size: " << sizeof(GameMessage) << std::endl;
//	std::cout << "Unit size:" << sizeof(Unit) << std::endl;
//	std::cout << "UnitID size:" << sizeof(UnitID) << std::endl;
//	std::cout << "ArriveSteering size: " << sizeof(ArriveSteering) << std::endl;
//	std::cout << "InputSystem size: " << sizeof(InputSystem) << std::endl;
//	std::cout << "PositionComponent size: " << sizeof(PositionComponent) << std::endl;
	std::cout << "PositionData size: " << sizeof(PositionData) << std::endl;
//	std::cout << "PlayerMoveToMessage size: " << sizeof(PlayerMoveToMessage) << std::endl;
//	std::cout << "PhysicsComponent size: " << sizeof(PhysicsComponent) << std::endl;
//	std::cout << "Sprite size: " << sizeof(Sprite) << std::endl;
//	std::cout << "GraphicsBuffer size: " << sizeof(GraphicsBuffer) << std::endl;
	std::cout << "State Machine size: " << sizeof(StateMachine) << std::endl;
	std::cout << "State size: " << sizeof(State) << std::endl;
	std::cout << "Transition size: " << sizeof(Transition) << std::endl;
	std::cout << "Item size: " << sizeof(Item) << std::endl;
	std::cout << "ItemManager size: " << sizeof(ItemManager) << std::endl;
	


	mShouldExit = false;
	static int seed = (int)time(0);
	srand(seed);

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init( WIDTH, HEIGHT );
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpGraphicsSystem);
	mpSpriteManager = new SpriteManager();


	mpMessageManager = new GameMessageManager();
	mpComponentManager = new ComponentManager(MAX_UNITS);
	mpUnitManager = new UnitManager(MAX_UNITS);
	mpItemManager = new ItemManager(MAX_ITEMS);

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"Assets/wallpaperGrass.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"Assets/player.png");
	//mpGraphicsBufferManager->loadBuffer(mPlayerDamagedID, "Assets/playerDamaged.png");

	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"Assets/enemyBase.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferHunterID, "Assets/enemyTargetPlayer.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferBoosterID, "Assets/enemyTargetPowerUp.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferHealerID, "Assets/enemyTargetHP.png");
	//mpGraphicsBufferManager->loadBuffer(mEnemyDamagedID, "Assets/enemyDamaged.png");


	mpGraphicsBufferManager->loadBuffer(mBoostID, "Assets/speedPowerUp.png");
	mpGraphicsBufferManager->loadBuffer(mHealID, "Assets/healthPowerUp.png");
	mpGraphicsBufferManager->loadBuffer(mStrengthID, "Assets/strengthPowerUp.png");


	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"Assets/target.png");

	//load Font
	mpFont = new Font("Assets/cour.ttf", 24);
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}

	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight() );
	}
	
	/*
	GraphicsBuffer* pDamagedPlayerBuffer = mpGraphicsBufferManager->getBuffer(mPlayerDamagedID);
	Sprite* pDamagedPlayer = NULL;
	if (pDamagedPlayerBuffer != NULL)
	{
		pDamagedPlayer = mpSpriteManager->createAndManageSprite(DAMAGED_PLAYER, pDamagedPlayerBuffer, 0, 0, (float)pDamagedPlayerBuffer->getWidth(), (float)pDamagedPlayerBuffer->getHeight());
	}
	*/




	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pHunterBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferHunterID);
	Sprite* pEnemyHunt = NULL;
	if (pHunterBuffer != NULL)
	{
		pEnemyHunt = mpSpriteManager->createAndManageSprite(HUNTER_ID, pHunterBuffer, 0, 0, (float)pHunterBuffer->getWidth(), (float)pHunterBuffer->getHeight());
	}

	GraphicsBuffer* pBoosterBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferBoosterID);
	Sprite* pEnemyBoost = NULL;
	if (pBoosterBuffer != NULL)
	{
		pEnemyBoost = mpSpriteManager->createAndManageSprite(BOOSTER_ID, pBoosterBuffer, 0, 0, (float)pBoosterBuffer->getWidth(), (float)pBoosterBuffer->getHeight());
	}

	GraphicsBuffer* pHealerBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferHealerID);
	Sprite* pEnemyHeal = NULL;
	if (pHealerBuffer != NULL)
	{
		pEnemyHeal = mpSpriteManager->createAndManageSprite(HEALER_ID, pHealerBuffer, 0, 0, (float)pHealerBuffer->getWidth(), (float)pHealerBuffer->getHeight());
	}
	/*
	GraphicsBuffer* pDamagedEnemyBuffer = mpGraphicsBufferManager->getBuffer(mEnemyDamagedID);
	Sprite* pDamagedEnemy = NULL;
	if (pDamagedEnemyBuffer != NULL)
	{
		pDamagedEnemy = mpSpriteManager->createAndManageSprite(DAMAGED_ENEMY, pDamagedEnemyBuffer, 0, 0, (float)pDamagedEnemyBuffer->getWidth(), (float)pDamagedEnemyBuffer->getHeight());
	}
	*/

	GraphicsBuffer* pHealItemBuffer = mpGraphicsBufferManager->getBuffer(mHealID);
	Sprite* pHealItem = NULL;
	if (pHealerBuffer != NULL)
	{
		pHealItem = mpSpriteManager->createAndManageSprite(HEAL_ID, pHealItemBuffer, 0, 0, (float)pHealItemBuffer->getWidth(), (float)pHealItemBuffer->getHeight());
	}


	GraphicsBuffer* pBoostItemBuffer = mpGraphicsBufferManager->getBuffer(mBoostID);
	Sprite* pBoostItem = NULL;
	if (pBoostItemBuffer != NULL)
	{
		pBoostItem = mpSpriteManager->createAndManageSprite(BOOST_ID, pBoostItemBuffer, 0, 0, (float)pBoostItemBuffer->getWidth(), (float)pBoostItemBuffer->getHeight());
	}


	GraphicsBuffer* pStrengthItemBuffer = mpGraphicsBufferManager->getBuffer(mStrengthID);
	Sprite* pStrengthItem = NULL;
	if (pStrengthItemBuffer != NULL)
	{
		pStrengthItem = mpSpriteManager->createAndManageSprite(STRENGTH_ID, pStrengthItemBuffer, 0, 0, (float)pStrengthItemBuffer->getWidth(), (float)pStrengthItemBuffer->getHeight());
	}



GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
if (pTargetBuffer != NULL)
{
	mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
}

//setup units

Unit* pUnit = mpUnitManager->createPlayerUnit(pArrowSprite);
pUnit->setShowTarget(false);

pUnit->setSteering(Steering::ARRIVE, Vector2D(20.0f, 20.0f));



mpScoreTimer = new Timer;
mpScoreTimer->start();
return true;
}
Sprite *pBackgroundSprite;
GraphicsBuffer *pDest;
void Game::cleanup()
{
	///TRY TO DELETE BACKGROUND SPRITE AND DEST, BUT DO IT LATER
	///PROBABLY SOMETHING WITH NODES? THERE'S A LOT OF MEMORY LEAKS :/



	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;
	delete mpScoreTimer;
	mpScoreTimer = NULL;

	delete mpFont;
	mpFont = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;

	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;

	delete mpItemManager;
	mpItemManager = NULL;


}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;


void Game::processLoop()
{
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpItemManager->updateAll();
	mpComponentManager->update(TARGET_ELAPSED_MS);

	//draw background
	pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	pDest = mpGraphicsSystem->getBackBuffer();
	mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

	genItems();


	//draw units
	mpUnitManager->drawAll();
	mpItemManager->drawAll();

	SDL_PumpEvents();
	int x, y;
	SDL_GetMouseState(&x, &y);

	mpGraphicsSystem->swap();

	mpMessageManager->processMessagesForThisframe();

	InputSystem::getInput();

	if (((int)(mpScoreTimer->getElapsedTime() / 1000.0) > 1 && ((int)(mpScoreTimer->getElapsedTime() / 1000.0) % mEnemySpawnRate == 0)))
	{
		if (mSpawnAgain == true)
		{
			SpawnSystem::spawn();
			mSpawnAgain = false;
		}
	}
	if ((((int)(mpScoreTimer->getElapsedTime() / 1000.0) % mEnemySpawnRate) != 0))
	{
		mSpawnAgain = true;
	}
	mScore = (int)(mpScoreTimer->getElapsedTime() / 1000.0);

	//std::cout << mpUnitManager->getPlayerUnit()->getHealth() << "\n";
}

void Game::exitGame()
{
	mShouldExit = true;
	std::cout << "Game over!\nYour score: " << (int)(mpScoreTimer->getElapsedTime() / 1000.0) << "\n";
}

bool Game::endLoop()
{
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	
	return mShouldExit;
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

void Game::genItems()
{
	int totalProbabilty = (int)(mSpeedUpSpawnRate + mHealthUpSpawnRate + mDamageUpSpawnRate);

	int spawnNum =( rand() % (totalProbabilty*75 ))+ 1;

	if (spawnNum <= mSpeedUpSpawnRate)
	{
		GameMessage* pMessage = new CreateItemMessage(SPEED);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (spawnNum <= (mSpeedUpSpawnRate + mHealthUpSpawnRate))
	{
		GameMessage* pMessage = new CreateItemMessage(HEALTH);
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	else if (spawnNum <= totalProbabilty)
	{
		GameMessage* pMessage = new CreateItemMessage(STRENGTH);
		MESSAGE_MANAGER->addMessage(pMessage, 0);

	}
}