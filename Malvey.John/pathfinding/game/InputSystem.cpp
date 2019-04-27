#include "InputSystem.h"
#include "DeanLibDefines.h"
#include "GameMessageManager.h"
#include "GameMessage.h"
#include "Game.h"
#include "Vector2D.h"
#include "PathToMessage.h"
#include "ChangeModeMessage.h"
#include "Grid.h"
#include "GameApp.h"
char prevScancode = NULL;

void InputSystem::getInput()
{
	SDL_PumpEvents();

	int x, y;

	
	if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		static Vector2D lastPos(0.0f, 0.0f);
		Vector2D pos(x, y);
		GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
		Grid* pGrid = pGame->getGrid();
		if ((int)pGrid->getValueAtPixelXY((int)pos.getX(), (int)pos.getY()) != 1)
		{
			if (lastPos.getX() != pos.getX() || lastPos.getY() != pos.getY())
			{
				GameMessage* pMessage = new PathToMessage(lastPos, pos);
				MESSAGE_MANAGER->addMessage(pMessage, 0);
				lastPos = pos;
			}
		}
	}
	//get keyboard state
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	//if escape key was down then exit the loop
	if (state[SDL_SCANCODE_ESCAPE])
	{ 
		gpGame->markForExit();
	}

	if (state[SDL_SCANCODE_F])
	{
		if (prevScancode != SDL_SCANCODE_F)
		{
			GameMessage* pMessage = new ChangeModeMessage(DFS_TYPE);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		
		}
		prevScancode = SDL_SCANCODE_F;
	}
	else if (prevScancode == SDL_SCANCODE_F)
	{
		prevScancode = NULL;

	}



	if (state[SDL_SCANCODE_D])
	{
		if (prevScancode != SDL_SCANCODE_D)
		{
			GameMessage* pMessage = new ChangeModeMessage(DIJKSTRA_TYPE);
			MESSAGE_MANAGER->addMessage(pMessage, 0);

		}
		prevScancode = SDL_SCANCODE_D;
	}
	else if (prevScancode == SDL_SCANCODE_D)
	{
		prevScancode = NULL;

	}


	if (state[SDL_SCANCODE_A])
	{
		if (prevScancode != SDL_SCANCODE_A)
		{
			GameMessage* pMessage = new ChangeModeMessage(ASTAR_TYPE);
			MESSAGE_MANAGER->addMessage(pMessage, 0);
		
		}
		prevScancode = SDL_SCANCODE_A;
	}
	else if (prevScancode == SDL_SCANCODE_A)
	{
		prevScancode = NULL;

	}
	state = NULL;




}