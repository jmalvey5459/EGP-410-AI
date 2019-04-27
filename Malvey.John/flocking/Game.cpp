#include <stdio.h>
#include <assert.h>

#include <sstream>


#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Font.h"
#include "GraphicsBufferManager.h"
#include "GameMessageManager.h"
#include "Sprite.h"
#include "Timer.h"
#include "PlayerMoveToMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "InputManager.h"
#include "FlockingSteering.h"
class FlockingSteering;
Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;
const Uint32 MAX_UNITS = 50000;


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
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"arrow.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID,"enemy-arrow.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");

	//load Font
	mpFont = new Font("cour.ttf", 24);
	
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
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}


	mpInputManager = NULL;
	//std::cout << "physics data size: " << sizeof(PhysicsData);

	//load in weights from data file
	loadWeights();


	return true;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

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

	//Unit* pUnit;
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


	displayWeights();


	//write text at mouse position
	mpGraphicsSystem->writeText(*mpFont, (float)x, (float)y, mousePos.str(), BLACK_COLOR);

	//test of fill region
	//mpGraphicsSystem->fillRegion(*pDest, Vector2D(300, 300), Vector2D(500, 500), RED_COLOR);
	mpGraphicsSystem->swap();

	mpMessageManager->processMessagesForThisframe();

	mpInputManager->getInput(gpGame, mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));

}
void Game::endGame()
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


void Game::loadWeights()
{
	std::ifstream fin;
	fin.open(WEIGHT_DATA_FILE);
	fin >> mSeparateWeight;
	fin >> mAlignWeight;
	fin >> mCohesionWeight;
	fin.close();
}
void Game::modifyWeight(int weight, float mod)
{
	switch (weight) {
	case(0):
		mSeparateWeight += mod;
		break;
	case(1):
		mSeparateWeight += mod;
		break;
	case(2):
		mSeparateWeight += mod;
		break;
	};
}

void Game::displayWeights()
{
	//Create weight text
	std::stringstream weightA;
	std::stringstream weightB;

	std::stringstream selectedWeight;
	weightA << "Alignment Weight: ";
	switch (currentWeight)
	{
	case(0):
		selectedWeight << mAlignWeight;
		weightB << "  Separation Weight: " << mSeparateWeight << "  CohesionWeight: " << mCohesionWeight;
		
		mpGraphicsSystem->writeText(*mpFont, (float)5, (float)5, weightA.str(), BLACK_COLOR);
		mpGraphicsSystem->writeText(*mpFont, (float)255, (float)5, selectedWeight.str(), RED_COLOR);
		mpGraphicsSystem->writeText(*mpFont, (float)307, (float)5, weightB.str(), BLACK_COLOR);

		break;
	case(1):
		weightA << mAlignWeight << "  Separation Weight: ";
		selectedWeight << mSeparateWeight;
		weightB << "  CohesionWeight: " << mCohesionWeight;

		mpGraphicsSystem->writeText(*mpFont, (float)5, (float)5, weightA.str(), BLACK_COLOR);
		mpGraphicsSystem->writeText(*mpFont, (float)590, (float)5, selectedWeight.str(), RED_COLOR);
		mpGraphicsSystem->writeText(*mpFont, (float)620, (float)5, weightB.str(), BLACK_COLOR);

		break;
	case(2):
		weightA << mAlignWeight << "  Separation Weight: " << mSeparateWeight << "  CohesionWeight: ";
		selectedWeight << mCohesionWeight;
		weightB << "";

		mpGraphicsSystem->writeText(*mpFont, (float)5, (float)5, weightA.str(), BLACK_COLOR);
		mpGraphicsSystem->writeText(*mpFont, (float)845, (float)5, selectedWeight.str(), RED_COLOR);
		break;
	}
//write weights
	//weightA <<"Alignment Weight " <<  mAlignWeight << "  Separation Weight: " << mSeparateWeight << "  CohesionWeight: ";
//	mpGraphicsSystem->writeText(*mpFont, (float)5, (float)5, weightA.str(), BLACK_COLOR);

}

void Game::saveWeights()
{
	std::ofstream ofs;
	ofs.open(WEIGHT_DATA_FILE, std::ofstream::out | std::ofstream::trunc);
	ofs << mSeparateWeight << "\n";
	ofs << mAlignWeight << "\n";
	ofs << mCohesionWeight;
	ofs.close();
}