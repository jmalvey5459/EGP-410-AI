#include "Game.h"
#include "Grid.h"
#include "GameMessageManager.h"
#include "EnvironmentChangeMessage.h"
#include "UnitManager.h"
EnvironmentChangeMessage::EnvironmentChangeMessage(const Vector2D& pos)
	:GameMessage(ENVIRONMENT_MESSAGE)
	, mPos(pos)
{
}

EnvironmentChangeMessage::~EnvironmentChangeMessage()
{

}

void EnvironmentChangeMessage::process()
{
	/*
	if (gpGame->getGrid()->getValueAtPixelXY(mPos.getX(), mPos.getY()))
		gpGame->getGrid()->setValueAtPixelXY(mPos.getX(), mPos.getY(), 0);
	else
		gpGame->getGrid()->setValueAtPixelXY(mPos.getX(), mPos.getY(), 1);
	std::ofstream theStream("pathgrid.txt");
	gpGame->getGrid()->save(theStream);

	//gpGame->reloadGrid();
	*/

}

