#pragma once

#include "Trackable.h"

enum MessageType
{
	INVALID_MESSAGE_TYPE = -1,
	PLAYER_MOVETO_MESSAGE = 0,
	EXIT_GAME_MESSAGE,
	DELETE_RANDOM_MESSAGE,
	CREATE_RANDOM_MESSAGE,
	PATH_TO_MESSAGE,
	ENVIRONMENT_MESSAGE,
	HEALTH_MESSAGE,
	COLLECT_MESSAGE,
	CREATE_ITEM_MESSAGE,
};

class GameMessage: public Trackable
{
public:
	friend class GameMessageManager;

	GameMessage( MessageType type  );
	~GameMessage();

	double getSubmittedTime() const { return mSubmittedTime; };
	double getScheduledTime() const { return mScheduledTime; };

protected:
	MessageType mType;
	double mSubmittedTime;
	double mScheduledTime;

private:
	virtual void process() = 0; 
};

