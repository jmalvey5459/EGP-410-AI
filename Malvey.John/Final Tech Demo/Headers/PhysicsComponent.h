#pragma once

#include "Component.h"
#include <Vector2D.h>
#include <Trackable.h>
#include <cfloat>

class PositionComponent;


static const float MAX_ROT_ACC = 1.0f;
static const float MAX_ROT_VEL = 5.0f;

struct PhysicsData : public Trackable
{
	 
	PhysicsData(
		const Vector2D& theVel
		, const Vector2D& theAcc
		, float theRotVel
		, float theRotAcc
		, float theMaxSpeed = 3500.0f
		, float theMaxAcc = 1000.0f
		, float theMaxRotVel = MAX_ROT_VEL
		, float theMaxRotAcc = MAX_ROT_ACC) 
		:vel(theVel)
		, acc(theAcc)
		, rotVel(theRotVel)
		, rotAcc(theRotAcc)
		, maxSpeed(theMaxSpeed)
		, maxAccMagnitude(theMaxAcc)
		, maxRotVel(theMaxRotVel)
		, maxRotAcc(theRotAcc)
	{};

	PhysicsData() 
		:vel(ZERO_VECTOR2D)
		, acc(ZERO_VECTOR2D)
		, rotVel(0.0f)
		, rotAcc(0.0f)
		, maxSpeed(1500.0f)
		, maxAccMagnitude(500.0f)
		, maxRotVel(MAX_ROT_VEL)
		, maxRotAcc(MAX_ROT_ACC)
	{};

	Vector2D vel;
	Vector2D acc;
	float rotVel;
	float rotAcc;
	float maxSpeed;
	float maxAccMagnitude;
	float maxRotVel;
	float maxRotAcc;

	bool mLobMode;
};

const PhysicsData ZERO_PHYSICS_DATA;

class PhysicsComponent :public Component
{
public:

	void setData(const PhysicsData& data){ mData = data; };
	void modData(const PhysicsData& data);
	void modifyMaxAcc(float modifier, int fix = 0)
	{
		if (!fix)
		{
			mData.maxAccMagnitude = 1000.0f;
			mData.maxSpeed = 3500.0f;
			mData.maxAccMagnitude *= modifier;
			mData.maxSpeed *= (modifier * 0.75f);
		}
		else
		{
			mData.maxSpeed = 3500.0f;
			mData.maxAccMagnitude = 1000.0f;
		}

	};
	const PhysicsData& getData() const { return mData; };
	const Vector2D& getVelocity() const { return mData.vel; };
	const Vector2D& getAcceleration() const { return mData.acc; };
	float getRotationalVelocity() const { return mData.rotVel; };
	float getRotationalAcceleration() const { return mData.rotAcc; };
	void lob(bool lobstate) { mData.mLobMode = lobstate; };

private:
	PhysicsData mData;
	ComponentID mPositionComponentID;
	PositionComponent* mpPositionComponent=NULL;

	PhysicsComponent(const ComponentID& id, const ComponentID& posID) :Component(id), mPositionComponentID(posID){};
	~PhysicsComponent(){};

	void integrate(PositionComponent& posComponent, float elapsedTime);

	void setPositionComponent(PositionComponent* pComp) { mpPositionComponent = pComp; };

	friend class ComponentManager;
};