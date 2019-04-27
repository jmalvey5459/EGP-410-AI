#include "ItemManager.h"
#include "Item.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GameMessageManager.h"
#include "GameMessage.h"
#include "CollectMessage.h"
#include "HealthChangeMessage.h"
#include "SpriteManager.h"
#include "UnitManager.h"
#include "Unit.h"
//modeled HEAVILY after unitmanager 
class Unit;
ItemID ItemManager::msNextItemID;

ItemManager::ItemManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Item))
{

}

ItemManager::~ItemManager()
{
	for (unsigned int i = 0; i <= mItemMap.size(); i++)
	{
		deleteItem(i);
	}
}

Item* ItemManager::getItem(const ItemID& id) 
{
	auto it = mItemMap.find(id);
	if (it != mItemMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

Item* ItemManager::createRandomItem(Sprite* sprite, Vector2D pos, const ItemID& id, ItemType type)
{

	Item* pItem = NULL;
	switch (type)
	{
	case SPEED:
		pItem = new Item(sprite, pos, type,gpGame->mSpeedUpValue,gpGame->mSpeedUpDuration);
		break;
	case HEALTH:
		pItem = new  Item(sprite, pos, type, gpGame->mHealthUpValue, 0);
		break;
	case STRENGTH:
		pItem = new  Item(sprite, pos, type, gpGame->mDamageUpValue, gpGame->mDamageUpDuration);
		break;
	default:
		pItem = new  Item(sprite, pos, type, 0, 0);
		break;
	}

		ItemID theID = id;
		if (theID ==  -1 )
		{
			theID = msNextItemID;
			msNextItemID++;
		}

		//place in map
		mItemMap[theID] = pItem;

		//assign id and increment nextID counter
		pItem->mID = theID;

	
	return pItem;
}
void ItemManager::deleteItem(const ItemID& id)
{

	auto it = mItemMap.find(id);
	if (it != mItemMap.end())
	{
		Item* pItem= it->second;
		mItemMap.erase(it);

		delete pItem;
	}
}


void ItemManager::drawAll()const
{
	for (auto it = mItemMap.begin(); it != mItemMap.end(); ++it)
	{
		it->second->draw();
	}
}
void ItemManager::updateAll()
{
	for (auto it = mItemMap.begin(); it != mItemMap.end(); ++it)
	{ 
		for (auto em = gpGame->getUnitManager()->mUnitMap.begin(); em != gpGame->getUnitManager()->mUnitMap.end(); ++em)
		{
			if (em->second->getID() != PLAYER_UNIT_ID)
			{
				if (collisionDetect(it->second, em->second))
				{
					if (it->second->getType() != HEALTH)
					{
						GameMessage* pMessage = new CollectMessage(it->second->getType(), em->second);
						MESSAGE_MANAGER->addMessage(pMessage, 0);
					}
					else
					{
						GameMessage* pMessage = new HealthChangeMessage(em->second->getID(), gpGame->mHealthUpValue);
						MESSAGE_MANAGER->addMessage(pMessage, 0);
					}
				}
			}
		}
	}
}

bool ItemManager::collisionDetect(Item* a, Unit* b)
{
	if (a->getPos().getX()
		>= b->getPositionComponent()->getPosition().getX() ||
		a->getPos().getX()
		<= (gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID)->getSourceLoc().getX()
		+ gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID)->getWidth()))
	{
		if (a->getPos().getY() >=
			b->getPositionComponent()->getPosition().getY() ||
			a->getPos().getY() <=
			(gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID)->getSourceLoc().getY() + 
			gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID)->getHeight()))
			return true;
	}
	return false;
}

Item* ItemManager::getClosestItem(Vector2D pos)
{
	Vector2D temp;
	temp.setX(20000000);
	temp.setY(20000000);
	ItemID index = -1;
	for (auto it = mItemMap.begin(); it != mItemMap.end(); ++it)
	{
		if ((it->second->getPos().getLengthSquared() - pos.getLengthSquared() <= (temp.getLengthSquared() - pos.getLengthSquared())))
		{
			index = it->second->getID();
			temp = it->second->getPos();
		}
	}

	return mItemMap.at(index);
}
Item* ItemManager::getClosestItemOfType(Vector2D pos, ItemType type)
{
	Vector2D temp;
	temp.setX(20000000);
	temp.setY(20000000);
	ItemID index = 0;
	for (auto it = mItemMap.begin(); it != mItemMap.end(); ++it)
	{
		if (it->second->getType() == type)
		{
			if ((it->second->getPos().getLengthSquared() - pos.getLengthSquared() <= (temp.getLengthSquared() - pos.getLengthSquared())))
			{
				index = it->second->getID();
				temp = it->second->getPos();
			}
		}
	}
	return mItemMap.at(index);
}

bool ItemManager::itemExists(ItemType check)
{

	for (auto it = mItemMap.begin(); it != mItemMap.end(); ++it)
	{
		if (it->second->getType() == check)
		{
			return true;
		}
	}
	return false;
}