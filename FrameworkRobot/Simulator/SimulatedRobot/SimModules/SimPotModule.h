/*
 *  SimPotModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_POT_MODULE_H
#define SIM_POT_MODULE_H

#include "SimulatedRobot/SimModules/SimModuleBase.h"
#include "SimulatedRobot/SimModules/Interfaces/RotationInterface.h"

class SimPotModule : public SimModuleBase
{
public:
	SimPotModule(RotationInterface* rotSource, int numTurns);
	~SimPotModule() {}
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	const double GetPotValue() const {	return m_potPos/m_turns;	}
	
private:
	RotationInterface*	m_rotSource;
	int					m_turns;
	double				m_potPos;
};
#endif