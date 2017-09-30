/*
 *  SimEncoderModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_ENCODER_MODULE_H
#define SIM_ENCODER_MODULE_H

#include "SimulatedRobot/SimModules/SimModuleBase.h"

class RotationInterface;

class SimEncoderModule : public SimModuleBase
{
public:
	SimEncoderModule(RotationInterface* rotSource);
	~SimEncoderModule() {}
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	const double& GetEncoderValue() const {	return m_encoderVal;	}
	void	SetEncoderEnabled(bool bEnabled);
	
private:
	RotationInterface*	m_rotSource;
	bool				m_bEnabled;
	double				m_encoderVal;
};
#endif