#pragma once
#include "Defines.h"
#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <map>
#include "Item.h"
class Unit;
class Item;
class Sprite;
class UnitManager;
class SpriteManager;

class ItemManager : public Trackable
{
public:
	ItemManager(Uint32 maxSize);
	~ItemManager();

	Item* createRandomItem(Sprite* sprite, Vector2D pos, const ItemID& id, ItemType type);
	Item* getItem(const ItemID& id);
	std::map<ItemID, Item*> &getItemMap (){ return mItemMap; };


	void drawAll() const;
	void updateAll();

	void deleteItem(const ItemID& id);

	bool itemExists(ItemType check);


	Item* getClosestItem(Vector2D pos);
	Item* getClosestItemOfType(Vector2D pos, ItemType type);
	bool collisionDetect(Item* a, Unit* b);

private:
	static ItemID msNextItemID;
	
	MemoryPool mPool;
	std::map<ItemID, Item*> mItemMap;

	friend class UnitManager;
};