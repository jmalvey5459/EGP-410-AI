#pragma once
#include <SDL.h>
#include "UnitManager.h"
#include "GameMessageManager.h"
#include "Game.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "PlayerMoveToMessage.h"
class Game;

class InputManager
{
	private:

	public:
		void getInput(Game* mpGame, Sprite* pArrowSprite);
		
};