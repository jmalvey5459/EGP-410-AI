#include <Trackable.h>
#include "Steering.h"
/// class set up modeled after examples at: https://gamedevelopment.tutsplus.com/tutorials/3-simple-rules-of-flocking-behaviors-alignment-cohesion-and-separation--gamedev-3444
class AlignmentSteering : public Steering
{
public:
	AlignmentSteering(const UnitID& ownerID);
	PhysicsData computeAlignment();

private:
	int	mAlignRadius = 1000;
protected:
	virtual Steering* getSteering();
};