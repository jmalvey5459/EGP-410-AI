#include "ChangeModeMessage.h"
#include "Game.h"
#include "GameApp.h"
ChangeModeMessage::ChangeModeMessage(PathType type)
	:GameMessage(CHANGE_MODE_MESSAGE)
	, mType(type)
{

}

ChangeModeMessage::~ChangeModeMessage()
{
}

void ChangeModeMessage::process()
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	pGame->setPathfinder(mType);
}
