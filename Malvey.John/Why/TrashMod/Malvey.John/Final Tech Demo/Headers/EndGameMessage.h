
#include "GameMessage.h"
#include "Game.h"

class EndGameMessage : public GameMessage
{

public:
	EndGameMessage() :GameMessage(END_GAME) {};
	~EndGameMessage() {};

	void process();

};