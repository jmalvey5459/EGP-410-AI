#include "SpawnEnemyMessage.h"
#include "Game.h"
#include "SpriteManager.h"
#include "UnitManager.h"
#include "Grid.h"
#include "GraphicsSystem.h"
void SpawnEnemyMessage::process()
{
	gpGame->getUnitManager()->createUnit(*gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID), false, PositionData(Vector2D(gpGame->getGraphicsSystem()->getWidth() / 2, gpGame->getGraphicsSystem()->getHeight() / 2), 0.0f));

}