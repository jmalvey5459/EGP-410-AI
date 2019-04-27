#include "InputSystem.h"
#include "DeanLibDefines.h"
#include "GameMessageManager.h"
#include "GameMessage.h"
#include "Game.h"
#include "Vector2D.h"
#include "PlayerMoveToMessage.h"
#include "ExitGameMessage.h"
#include "DeleteRandomMessage.h"
#include "CreateRandomMessage.h"
#include "CollectMessage.h"
#include "EnvironmentChangeMessage.h"
#include "CreateItemMessage.h"
char prevScancode = NULL;

void InputSystem::getInput()
{
	SDL_PumpEvents();

	int x, y;

	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		static Vector2D lastPos(0.0f, 0.0f);
		Vector2D pos(x, y);
		if (lastPos.getX() != pos.getX() || lastPos.getY() != pos.getY())
		{
			GameMessage* pMessage = new PlayerMoveToMessage(pos);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			lastPos = pos;
		}
	}
	//LETS MODIFY THAT GRID MY DUDES!
	/*
	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		static Vector2D lastPos(0.0f, 0.0f);
		Vector2D pos(x, y);
		if (lastPos.getX() != pos.getX() || lastPos.getY() != pos.getY())
		{
			GameMessage* pMessage = new EnvironmentChangeMessage(pos);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
			lastPos = pos;
		}
	}*/
	//all this should be moved to InputManager!!!

	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//if escape key was down then exit the loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		GameMessage* pMessage = new ExitGameMessage();
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}

	if (state[SDL_SCANCODE_S])
	{
		if (prevScancode != SDL_SCANCODE_S)
		{
			GameMessage* pMessage = new CreateItemMessage(HEALTH);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		
			/*GameMessage* pMessage = new CreateRandomMessage();
			MESSAGE_MANAGER->addMessage(pMessage, 0);*/
		}
		prevScancode = SDL_SCANCODE_S;
	}
	else if (prevScancode == SDL_SCANCODE_S)
	{
		prevScancode = NULL;

	}
	state = NULL;




}
