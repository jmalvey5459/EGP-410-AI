#pragma once

#include "Trackable.h"
#include "CircularQueue.h"

class GameMessage;

class GameMessageManager: public Trackable
{
public:
	explicit GameMessageManager(Uint32 size=128);
	~GameMessageManager();

	void addMessage( GameMessage* pMessage, int delay );
	void processMessagesForThisframe();

private:

	CircularQueue<GameMessage*>* mMessages;
};

