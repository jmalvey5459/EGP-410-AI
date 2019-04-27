#include "ModifyWeightMessage.h"
class Game;

ModifyWeightMessage::ModifyWeightMessage(bool posOrNeg) :GameMessage(MODIFY_WEIGHT), mPositive(posOrNeg)
{
}
void ModifyWeightMessage::process()
{
	//find weight being changed
	float weightToModify;
	switch (gpGame->currentWeight)
	{
	case(0):
		weightToModify = gpGame->mAlignWeight;
		break;
	case(1):
		weightToModify = gpGame->mSeparateWeight;
		break;
	case(2):
		weightToModify = gpGame->mCohesionWeight;
		break;
	}

	//increment
	if (mPositive)
		weightToModify += 0.1f;
	else if (weightToModify >= 0.2f)
		weightToModify -= 0.1f;
	//minimum weight. no negatives!
	else
		weightToModify = 0;

	switch (gpGame->currentWeight)
	{
	case(0):
		gpGame->mAlignWeight = weightToModify;
		break;
	case(1):
		gpGame->mSeparateWeight = weightToModify;
		break;
	case(2):
		gpGame->mCohesionWeight = weightToModify;
		break;
	}
}