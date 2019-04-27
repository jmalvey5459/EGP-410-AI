#include "InputManager.h"
#include "EndGameMessage.h"
#include "GameMessageManager.h"
#include "GameMessage.h"
char prevScanCode = NULL;
//all this should be moved to InputManager
float vel = 8;
void InputManager:: getInput()
{
	int x, y;
	SDL_PumpEvents();

	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//if escape key was down then exit the loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		GameMessage* pMessage = new EndGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
	if (state[SDL_SCANCODE_A])
	{
		if (prevScanCode != SDL_SCANCODE_D)
		{
			
			if (gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX() >= 86.0f)
			{
				if(!gpGame->getUnitManager()->getPlayerUnit()->shouldFlip())
					gpGame->getUnitManager()->getPlayerUnit()->flip();
				gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->modPosition(Vector2D(-vel, 0.0f));
			
				
			}
		}
		prevScanCode = SDL_SCANCODE_A;

	}
	else if (prevScanCode == SDL_SCANCODE_A)
	{
		prevScanCode = NULL;
	}

	if (state[SDL_SCANCODE_D])
	{
		if (prevScanCode != SDL_SCANCODE_A)
		{
			if (gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX() <= 882.0f)
			{
				
				if (gpGame->getUnitManager()->getPlayerUnit()->shouldFlip())
					gpGame->getUnitManager()->getPlayerUnit()->flip();
				gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->modPosition(Vector2D(vel, 0.0f));

			}
		}
		prevScanCode = SDL_SCANCODE_D;
		
	}
	else if (prevScanCode == SDL_SCANCODE_D)
	{
		prevScanCode = NULL;
	}


	if (gpGame->getUnitManager()->getPlayerUnit() != NULL)
	{
		if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			Vector2D pos(x, y);
			//GameMessage* pMessage = new PlayerMoveToMessage(pos);
			//MESSAGE_MANAGER->addMessage(pMessage, 0);
		}
	}

}