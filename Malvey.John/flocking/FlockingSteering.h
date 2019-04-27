#include <Trackable.h>
#include "Steering.h"
#include "SeparationSteering.h"
#include "AlignmentSteering.h"
#include "CohesionSteering.h"
#include <fstream>
/// class set up modeled after examples at: https://gamedevelopment.tutsplus.com/tutorials/3-simple-rules-of-flocking-behaviors-alignment-cohesion-and-separation--gamedev-3444

class FlockingSteering : public Steering
{
public:
	FlockingSteering(const UnitID& ownerID);
	~FlockingSteering();

	void loadWeights();
	float mSeparateWeight, mAlignWeight, mCohesionWeight;

protected:
	virtual Steering* getSteering();

	friend class SeparationSteering;
	friend class AlignmentSteering;
	friend class CohesionSteering;
};
