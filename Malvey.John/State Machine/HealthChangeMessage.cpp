#include "Game.h"
#include "GameMessageManager.h"
#include "HealthChangeMessage.h"
#include "UnitManager.h"

/*
Changing this to use Arrive
*/



HealthChangeMessage::HealthChangeMessage(const UnitID unitID, const int change)
	:GameMessage(HEALTH_MESSAGE)
	, mUnitID(unitID), mChange(change)
{
}

HealthChangeMessage::~HealthChangeMessage()
{

}

void HealthChangeMessage::process()
{
	gpGame->getUnitManager()->getUnit(mUnitID)->setHealth(mChange);

}
