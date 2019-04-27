#include "AddBoidsMessage.h"
class SpriteManager;
class Game;

void AddBoidsMessage::process()
{
	for (int i = 0; i < 10; i++)
	{
		gpGame->getUnitManager()->createRandomUnit(gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID));
	}
}