#include "GameMessageManager.h"
#include "GameMessage.h"
#include "SpawnSystem.h"
#include "CreateRandomMessage.h"
#include "Game.h"
void SpawnSystem::spawn()
{
	GameMessage* pMessage = new CreateRandomMessage();
	MESSAGE_MANAGER->addMessage(pMessage, 0);
}