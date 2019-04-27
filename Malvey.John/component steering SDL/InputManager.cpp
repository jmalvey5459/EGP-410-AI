#include "InputManager.h"

//all this should be moved to InputManager!!!
void InputManager:: getInput(Game& mpGame, Unit *pUnit,  Sprite* pArrowSprite)
{
	int x, y;
	SDL_PumpEvents();

	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//if escape key was down then exit the loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mpGame->endLoop();
	}
	if (state[SDL_SCANCODE_A])
	{
		for (int i = 0; i < 10; i++)
		{
			mpGame->getUnitManager()->createRandomUnit(*pArrowSprite);
		}
	}

	if (state[SDL_SCANCODE_D])
	{
		mpGame->getUnitManager()->deleteRandomUnit();
		
	}

	if( SDL_GetMouseState(&x,&y) & SDL_BUTTON(SDL_BUTTON_LEFT) )
	{
		Vector2D pos( x, y );
		GameMessage* pMessage = new PlayerMoveToMessage( pos );
		MESSAGE_MANAGER->addMessage( pMessage, 0 );
	}

}