#include <Trackable.h>
#include "Steering.h"
class FlockingSteering;

class SeparationSteering : public Steering
{
public:
	SeparationSteering(const UnitID& ownerID);
	PhysicsData computeSeparation();
private:
	float mSeparationRadius = 1000;
protected:
	virtual Steering* getSteering();
};