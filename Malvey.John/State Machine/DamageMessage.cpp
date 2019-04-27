#include "DamageMessage.h"
#include "GameMessageManager.h"
#include "GameMessage.h"
DamageMessage::DamageMessage(const UnitID id, const int dmg) :
	GameMessage(DAMAGE_MESSAGE)
	, mUnitID(id), mDMG(dmg)
{}
DamageMessage::~DamageMessage()
{

}

void DamageMessage::process()
{
	gpGame->getUnitManager()->getUnit(mUnitID)->modHealth(mDMG);
}