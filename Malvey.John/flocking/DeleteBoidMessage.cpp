#include "DeleteBoidMessage.h"
class Game;

void DeleteBoidMessage::process()
{

	gpGame->getUnitManager()->deleteRandomUnit();

}