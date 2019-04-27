#include "ExitGameMessage.h"
#include "Game.h"

void ExitGameMessage::process()
{
	gpGame->exitGame();
}