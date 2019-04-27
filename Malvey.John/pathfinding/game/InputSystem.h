#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
class GameApp;

#define MESSAGE_MANAGER gpGame->getMessageManager()

class InputSystem
{
public:
	 static void getInput();
};

#endif // !INPUT_SYSTEM_H


