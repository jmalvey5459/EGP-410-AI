#include "ChangeSelectedWeightMessage.h"
class Game;

ChangeSelectedWeightMessage::ChangeSelectedWeightMessage(bool posOrNeg) :GameMessage(SELECT_WEIGHT),mPositive(posOrNeg)
{
}
void ChangeSelectedWeightMessage::process()
{

	//increment
	if (mPositive)
		gpGame->currentWeight++;
	else
		gpGame->currentWeight--;

	//wrap back around
	if (gpGame->currentWeight > 2)
		gpGame->currentWeight = 0;
	else if (gpGame->currentWeight < 0)
		gpGame->currentWeight = 2;
}