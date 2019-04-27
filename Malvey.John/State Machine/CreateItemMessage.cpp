#include "CreateItemMessage.h"
#include "Game.h"
#include "SpriteManager.h"
#include "ItemManager.h"
#include "GraphicsSystem.h"
#include "Vector2D.h"
class Vector2D;
void CreateItemMessage::process()
{
	mPos.setX(rand() % (gpGame->getGraphicsSystem()->getWidth() - 64 + 1 - 64) + 64);
	mPos.setY(rand() % (gpGame->getGraphicsSystem()->getHeight() - 64 + 1 - 64) + 64);
	switch (mType)
	{
	case SPEED:
		gpGame->getItemManager()->createRandomItem(gpGame->getSpriteManager()->getSprite(BOOST_ID), mPos, -1.0f, mType);
		break;
	case HEALTH:
		gpGame->getItemManager()->createRandomItem(gpGame->getSpriteManager()->getSprite(HEAL_ID), mPos,-1.0f, mType);
		break;
	case STRENGTH:
		gpGame->getItemManager()->createRandomItem(gpGame->getSpriteManager()->getSprite(STRENGTH_ID), mPos, -1.0f, mType);

		break;
	}
}

