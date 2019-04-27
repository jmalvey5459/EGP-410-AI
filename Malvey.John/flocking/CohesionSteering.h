#include <Trackable.h>
#include "Steering.h"
/// class set up modeled after examples at: https://gamedevelopment.tutsplus.com/tutorials/3-simple-rules-of-flocking-behaviors-alignment-cohesion-and-separation--gamedev-3444
class CohesionSteering : public Steering
{
public:
	CohesionSteering(const UnitID& ownerID);
	PhysicsData computeCohesion();

private:
	int	mCohesiveRadius = 1000;
protected:
	virtual Steering* getSteering();
};