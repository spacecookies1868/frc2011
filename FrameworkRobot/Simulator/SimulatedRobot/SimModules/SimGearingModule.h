/*
 *  SimGearingModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_GEARING_MODULE_H
#define SIM_GEARING_MODULE_H

#include "SimulatedRobot/SimModules/Interfaces/RotationInterface.h"

class SimGearingModule : public RotationInterface
{
public:
	SimGearingModule(RotationInterface* source, double gearingRatio);
	~SimGearingModule() {}
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	const double GetRotationValue() const	{ return m_rotSource->GetRotationValue() / m_gearingRatio; }
private:
	RotationInterface*	m_rotSource;
	double				m_gearingRatio;
	
};
#endif