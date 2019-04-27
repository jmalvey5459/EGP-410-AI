#pragma once

#include "GameMessage.h"
class Game;
class GameApp;
enum PathType {
	DFS_TYPE,
	DIJKSTRA_TYPE,
	ASTAR_TYPE
};
class ChangeModeMessage :public GameMessage
{
public:
	ChangeModeMessage(PathType type);
	~ChangeModeMessage();


	void process();
private:
	PathType mType;
};