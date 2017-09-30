#ifndef ACCELERATORARCADE_H_
#define ACCELERATORARCADE_H_
#include "Shared/CommonIncludes.h"
#include "Shared/NumericalVector.hpp"
#include "Shared/RobotModel.h"

class AcceleratorArcade  {
public:
	explicit AcceleratorArcade(float multiplier);
    NumericalVector<2> update(float throttle, float turnaxis, RobotModel::GearSet gear, bool bQuickTurn);
private:
	float m_turnVelocity;
	float m_accelerator_multiplier;
};
#endif
