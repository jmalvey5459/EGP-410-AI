#include "CollectMessage.h"
#include "Game.h"
#include "ItemManager.h"

void CollectMessage::process()
{
	if (mpUnit->getFSM()->getCurrentState()->getName() == SEEK_BOOST || mpUnit->getFSM()->getCurrentState()->getName() == SEEK_HEALTH)
	{
		mpUnit->getFSM()->objectiveSuccess();
		//std::cout << "Succefully picked up seek or health item!" << std::endl;
	}
	gpGame->getItemManager()->getItem(mItemID)->~Item();
}