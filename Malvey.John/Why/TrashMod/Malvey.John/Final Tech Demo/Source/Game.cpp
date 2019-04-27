#include <stdio.h>
#include <assert.h>

#include <sstream>
#include <SDL.h>
#include <time.h>

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
#include "InputManager.h"
Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 100;

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
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mShouldExit = false;

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

	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"Assets/NightBackground.png");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"Assets/playerKnightSmallNight.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"Assets/goblinWizardSmallNight.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"Assets/target.png");
	mpGraphicsBufferManager->loadBuffer(mFireballID, "Assets/fireballSmall.png");

	//load Font
	//mpFont = new Font("cour.ttf", 24);
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{
		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pKnightSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pKnightSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight() );
	}

	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pGoblinSprite = NULL;
	if (pAIBuffer != NULL)
	{
		pGoblinSprite = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pFireballBuffer = mpGraphicsBufferManager->getBuffer(mFireballID);
	Sprite* pFireballSprite = NULL;
	if (pFireballBuffer != NULL)
	{
		pFireballSprite = mpSpriteManager->createAndManageSprite(FIREBALL_SPRITE_ID, pFireballBuffer, 0, 0, (float)pFireballBuffer->getWidth(), (float)pFireballBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	//setup units
	Unit* pUnit = mpUnitManager->createPlayerUnit(*pKnightSprite, false, PositionData(Vector2D(92.0f,608.0f),0.0f));
	pUnit->setShowTarget(false);
	pUnit->setSteering(Steering::ARRIVE, ZERO_VECTOR2D);

	//create 2 enemies
	pUnit = mpUnitManager->createUnit(*pGoblinSprite, false, PositionData(Vector2D(939.0f,183.0f), 0.0f));
	pUnit->setShowTarget(false);
	pUnit->setSteering(Steering::ARRIVE, ZERO_VECTOR2D);
	//pUnit->setSteering(Steering::SEEK, ZERO_VECTOR2D, PLAYER_UNIT_ID);

	mpSpawnTimer = new Timer;
	mpSpawnTimer->start();
	srand((unsigned int)time(NULL));
	return true;

}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;
	delete mpSpawnTimer;
	mpSpawnTimer = NULL;

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
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
	
void Game::processLoop()
{
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
	
	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	GraphicsBuffer* pDest = mpGraphicsSystem->getBackBuffer();
	mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

	//draw units
	mpUnitManager->drawAll();

	SDL_PumpEvents();
	int x, y;
	SDL_GetMouseState(&x, &y);

	//create mouse text
	std::stringstream mousePos;
	mousePos << x << ":" << y;

	
	mpGraphicsSystem->swap();

	mpMessageManager->processMessagesForThisframe();
	//std::cout << gpGame->getUnitManager()->getPlayerUnit()->getPhysicsComponent()->getVelocity().getX() << ", " << gpGame->getUnitManager()->getPlayerUnit()->getPhysicsComponent()->getVelocity().getY() << "\n";
	InputManager::getInput();

	//FIX THIS AWFUL SPAWN SYSTEM FOR THE FIREBALLS
	if ((int)mpSpawnTimer->getElapsedTime()	% 3 == 0)
	{
		
		Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(FIREBALL_SPRITE_ID));
		
		if ((int) rand() % 2  == 0)
		{			
			pUnit->getPhysicsComponent()->lob(false);
			pUnit->setSteering(Steering::PROJECTILE, Vector2D(gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX() + 64, gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY()), PLAYER_UNIT_ID);

		}
		else
		{	pUnit->getPhysicsComponent()->lob(true);
			pUnit->getPositionComponent()->modFacing(pUnit->getPositionComponent()->getFacing()*(3.14159f / 230.0f));
			pUnit->setSteering(Steering::PROJECTILE, Vector2D((gpGame->getGraphicsBufferManager()->getBuffer(mBackgroundBufferID)->getWidth() / 2.0f) + 375.0f, -85.0f), PLAYER_UNIT_ID);
		}

		if (pUnit == NULL)
		{
			mpUnitManager->deleteRandomUnit();
		}
	}

}

void Game::exitGame()
{
	mShouldExit = true;
}

bool Game::endLoop()
{
	//mpMasterTimer->start();
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


