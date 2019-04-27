#include "DeleteRandomMessage.h"
#include "Game.h"
#include "UnitManager.h"

DeleteRandomMessage::DeleteRandomMessage(const UnitID unitID)
	:GameMessage(DELETE_RANDOM_MESSAGE)
	, mUnitID(unitID)
{

}
void DeleteRandomMessage::process()
{
	if (mUnitID == PLAYER_UNIT_ID)
	{
		//std::cout << "Your score is: " << gpGame->getScore()<< "\n";
		gpGame->getUnitManager()->deleteUnit(mUnitID);
	}
	else 
		gpGame->getUnitManager()->deleteUnit(mUnitID);    
}