#include"Trajectory.h"
#include <math.h>
class Game;
class UnitManager;
class Unit;
class ComponentManager;
class PhysicsComponent;
class PositionComponent;
class SteeringComponent;
class Steering;
class Vector2D;
Trajectory::Trajectory(const UnitID& ownerID, const Vector2D& targetLoc){
mType = Steering::PROJECTILE;
setOwnerID(ownerID);
setTargetLoc(targetLoc);

//player's initial position
mPti = targetLoc;

mAb = Vector2D(1.0f, 9.81f);

mVt = gpGame->getUnitManager()->getPlayerUnit()->getPhysicsComponent()->getVelocity();
};

Steering* Trajectory::getSteering()
{
	mAb = Vector2D(0.0f, 9.81f);
	//Player's initial velocity

	//Player's acceleration
	mAt = gpGame->getUnitManager()->getPlayerUnit()->getPhysicsComponent()->getAcceleration();

	Vector2D test = mAb;
	float check = test.getX();
	check = test.getY();
	Vector2D diff = mTargetLoc - mPbi;
	
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	if (diff.getLength() > 50)
		pOwner->getPhysicsComponent()->modifyMaxAcc((2.0f - (1.0f / diff.getLength())));
	else
		pOwner->getPhysicsComponent()->modifyMaxAcc(1.0f);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	if (!data.mLobMode)
	{
		precisionAim(data);
		
	}
	else if (mTargetLoc.getY() < 25.0f)
	{
		if (pOwner->getPositionComponent()->getPosition().getY() < -70.0f)
		{
			
			pOwner->setSteering(PROJECTILE, Vector2D(gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getX() + 128.0f, gpGame->getUnitManager()->getPlayerUnit()->getPositionComponent()->getPosition().getY() + 128.0f), PLAYER_UNIT_ID);
			mPbi = pOwner->getPositionComponent()->getPosition();
		}	
		mSb = 250.0f; 
		precisionAim(data);
	}
	else
	{
		mSb = 1000.0f;
		precisionAim(data);
		
	}
	float angle = fmod(atan2(diff.getY(), diff.getX()) + (3.14159f / 2), 2 * 3.14159f);
	pOwner->getPositionComponent()->setFacing(angle);
	data.rotVel = 0.0f;
	data.acc = diff;
	mData = data;
	return this;
}

void Trajectory::precisionAim(PhysicsData& data)
{
	//Strategy 3
	///Law of Cosines
	///------------------------------------------------
	//the distance from the player's location to the spawn point of the fireball
	double A = (mPti - mPbi).getLength(); 
	 // the distance traveled in mT seconds
	//double B = (mVt*mT).getLength();
	//the speed of the projectile multiplied by the time it takes to get to the location the player is going (the distance the bullet should travel)
	//double C = (mSb*mT);
	//normalize the vector between the projectile spawn and the location of the target at the time of calculation
	Vector2D normDiff = (mPbi - mPti);
	//AND normalize the velocity of the player
	//std::cout << normDiff.getX() << ", " << normDiff.getY() << "\n";
	normDiff.normalize();
//	std::cout << normDiff.getX() << ", " << normDiff.getY() << "\n \n";

	Vector2D targetVel = (mVt);
//	std::cout << normVel.getX() << ", " << normVel.getY() << "\n";
	targetVel.normalize();
	//std::cout << normVel.getX() << ", " << normVel.getY() << "\n";

	//the cosign of the angle between the launch point of the projectiles and the target's location is the dot product 
	double cosTheta = ((normDiff.getX()*targetVel.getX()) + (normDiff.getY()*targetVel.getY()));
	//std::cout << cosTheta;
	// using this  cleaned up formula, ((A*A) + (B*B) - 2.0f * A * B * cosTheta), get it set up for the quadratic formula to find what mT should be
	//To make this easier to follow:
	//	(A*A) is going to be A^2, (B*B) is going to be B^2, and (C*C) will be C^2
	///-------------------------------------------------
	///Quadratic formula
	// A^2 + B^2 - 2 * A * B * cosTheta = C^2
	//Let's break B back into its base components
	// A^2 + B^2 - 2 * A * Length(mVt) * mT * cosTheta = C^2
	//To clean it up a bit more, the length of the target velocity will now be represented with S 

	double S = mVt.getLength();
	// A^2 + (S * mT)^2 - 2 * A * S * mT * cosTheta = C^2
	// A^2 + S^2 * mT^2 - 2 * A * S * mT * cosTheta = C^2
	/// Now comes the fun!
	// mT^2 + mSb^2 - A^2 - S^2 * mT^2  *  2 * A  * S * mT * cosTheta  =  0
	// mT^2 + mSb^2 - S^2 * mT^2 + 2 * A  * S * mT * cosTheta - A^2 = 0
	// ( mSb^2 - S^2) * mT^2 + 2 * A  * S * mT * cosTheta - A^2 = 0
	///Quadratic Formula for equation: a*mT^2 + mVt*mT^2 + mSb*mT  = 0
	///
	//double a = ((mSb*mSb) - (S*S));
	//double b = 2.0 * A *S*cosTheta;
	//double c = -(A*A);
	//mT = ((0.0-b) + sqrt((b*b) - 4.0 * a*c)) / (2.0 * a);
	mT =(float) (-2.0 * A*S*cosTheta + sqrt(((2.0 * A*S*cosTheta)*(2.0 * A*S*cosTheta)) + 4.0 * ((mSb*mSb) - S)*(A*A)) / (2.0 * ((mSb*mSb) - (S*S))));
	///Getting our final vector mVb:
	//isolate mVb from: mPbi + mVb*mT = mPti + mVt*mT
	mVb = mVt + ((mPti - mPbi) / mT);
	
	//check if this will hit
	Vector2D endpos(mPbi.getX() + (mVb.getX() / mT), (mPbi.getY() + (mVb.getY() / mT)));
	Vector2D playEnd = (mPti + (mVt / mT));
	if( endpos.getLength() - playEnd.getLength() < 32.0f)
		data.vel = mVb;

}
