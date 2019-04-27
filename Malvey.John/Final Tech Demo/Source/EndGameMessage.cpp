#include "EndGameMessage.h"
class Game;

void EndGameMessage::process()
{
	gpGame->exitGame();
}