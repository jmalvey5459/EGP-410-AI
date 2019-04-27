#include "Game.h"
#include "GameMessageManager.h"
#include "PlayerMoveToMessage.h"
#include "UnitManager.h"

/*
	Changing this to use Arrive
*/



PlayerMoveToMessage::PlayerMoveToMessage( const Vector2D& pos )
:GameMessage(PLAYER_MOVETO_MESSAGE)
,mPos(pos)
{
}

PlayerMoveToMessage::~PlayerMoveToMessage()
{

}

void PlayerMoveToMessage::process()
{
	//gpGame->getUnitManager()->getUnit(0)->setSteering(Steering::ARRIVE, mPos);
	//still memory leaks >:(
	gpGame->getUnitManager()->getPlayerUnit()->setSteering(Steering::ARRIVE, mPos);
	
}

