#include <cassert>
#include "Game.h"
#include "GameMessageManager.h"
#include "GameMessage.h"

using namespace std;

GameMessageManager::GameMessageManager(Uint32 size)
{
	mMessages = new CircularQueue<GameMessage *>(size);
}

GameMessageManager::~GameMessageManager()
{
	GameMessage* pMessage;
	while( mMessages->popFront(pMessage))
	{
		delete pMessage;
	}

	delete mMessages;
}

void GameMessageManager::processMessagesForThisframe()
{
	double currentTime = gpGame->getCurrentTime();
	GameMessage* pMessage;

	while(mMessages->popFront(pMessage))
	{
		if( pMessage->getScheduledTime() <= currentTime )
		{
			pMessage->process();
			delete pMessage;
		}
		else
		{
			//not time to process yet - push to back
			mMessages->pushBack(pMessage);
		}
	}
}

void GameMessageManager::addMessage( GameMessage* pMessage, int delay )
{
	double currentTime = gpGame->getCurrentTime();

	//set frame numbers
	pMessage->mSubmittedTime = currentTime;
	pMessage->mScheduledTime = currentTime + delay;

	//put it in the message list
	bool success = mMessages->pushBack( pMessage );

	//too many enemies attacking at once overloads the message manager, it's easier to delete the extra messages than crash the program
	if (!success)
	{
		delete pMessage;
		pMessage = NULL;
	}
}