#pragma once
#include <Trackable.h>
#include "Steering.h"
#include "Game.h"
#include "Vector2D.h"
#include "UnitManager.h"
//Formulas for projectiles modeled after: https://www.gamasutra.com/blogs/KainShin/20090515/83954/Predictive_Aim_Mathematics_for_AI_Targeting.php "Predictive Aim Mathematics for AI Targeting" by Kain Shin, 05/15/09
class Trajectory : public Steering
{
public:
	Trajectory(const UnitID& ownerID, const Vector2D& targetLoc);
protected:	
	
	

	//Target Initial Pos
	Vector2D mPti;
	//velocity of target
	Vector2D mVt;
	// acceleration of target
	Vector2D mAt;

	//goal bullet velocity
	Vector2D mVb;

	//bullet initial position
	Vector2D mPbi = gpGame->getUnitManager()->getSpawnLoc();
	//bullet speed
	float mSb = 1000.0f;
	//bullet acceleration
	Vector2D mAb;

	float mT;
	

	///int mSadIter = 1;

	
	//first time estimate
///	float mTo;
	//used for every iteration after that. there is no mT2
	///float mTa;
///	float mTb;
	void precisionAim(PhysicsData& data);
	void lobAim(PhysicsData& data);
	virtual Steering* getSteering();
};