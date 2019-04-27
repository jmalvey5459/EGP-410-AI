#include "InputManager.h"
#include "AddBoidsMessage.h"
#include "EndGameMessage.h"
#include "DeleteBoidMessage.h"
#include "GameMessageManager.h"
#include "ChangeSelectedWeightMessage.h"
#include "ModifyWeightMessage.h"
#include "GameMessage.h"
char prevScanCode = NULL;
//all this should be moved to InputManager!!!
void InputManager:: getInput(Game* mpGame,  Sprite* pArrowSprite)
{
	SDL_PumpEvents();

	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//if escape key was down then exit the loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		GameMessage* pMessage = new EndGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}


	//ADD AND DELETE BOIDS
	if (state[SDL_SCANCODE_A])
	{
		if (prevScanCode != SDL_SCANCODE_A)
		{
			GameMessage* pMessage = new AddBoidsMessage();
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}
		prevScanCode = SDL_SCANCODE_A;
	}

	else if (prevScanCode == SDL_SCANCODE_A)
	{
		prevScanCode = NULL;
	}

	if (state[SDL_SCANCODE_D])
	{
		if (prevScanCode != SDL_SCANCODE_D)
		{
			GameMessage* pMessage = new DeleteBoidMessage();
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}
		prevScanCode = SDL_SCANCODE_D;
	}
	else if (prevScanCode == SDL_SCANCODE_D)
	{
		prevScanCode = NULL;
	}


	//CHANGE WHICH WEIGHT VALUE IS BEING MODIFIED
	if (state[SDL_SCANCODE_RIGHT])
	{
		if (prevScanCode != SDL_SCANCODE_RIGHT)
		{
			GameMessage* pMessage = new ChangeSelectedWeightMessage(true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}
		prevScanCode = SDL_SCANCODE_RIGHT;
	}
	else if (prevScanCode == SDL_SCANCODE_RIGHT)
	{
		prevScanCode = NULL;
	}

	if (state[SDL_SCANCODE_LEFT])
	{
		if (prevScanCode != SDL_SCANCODE_LEFT)
		{
			GameMessage* pMessage = new ChangeSelectedWeightMessage(false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}
		prevScanCode = SDL_SCANCODE_LEFT;
	}
	else if (prevScanCode == SDL_SCANCODE_LEFT)
	{
		prevScanCode = NULL;
	}


	//MODIFY WEIGHT VALUES

	if (state[SDL_SCANCODE_UP])
	{
		if (prevScanCode != SDL_SCANCODE_UP)
		{
			GameMessage* pMessage = new ModifyWeightMessage(true);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}
		prevScanCode = SDL_SCANCODE_UP;
	}
	else if (prevScanCode == SDL_SCANCODE_UP)
	{
		prevScanCode = NULL;
	}



	if (state[SDL_SCANCODE_DOWN])
	{
		if (prevScanCode != SDL_SCANCODE_DOWN)
		{
			GameMessage* pMessage = new ModifyWeightMessage(false);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		}
		prevScanCode = SDL_SCANCODE_DOWN;
	}
	else if (prevScanCode == SDL_SCANCODE_DOWN)
	{
		prevScanCode = NULL;
	}

}