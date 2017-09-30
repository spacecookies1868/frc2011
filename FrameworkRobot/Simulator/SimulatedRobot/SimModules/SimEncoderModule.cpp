/*
 *  SimEncoderModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimEncoderModule.h"
#include "SimulatedRobot/SimModules/Interfaces/RotationInterface.h"
#include <time.h>

SimEncoderModule::SimEncoderModule(RotationInterface* rotSource)
: m_rotSource(rotSource)
, m_bEnabled(false)
, m_encoderVal(0.0)
{
	m_sources.push_back(rotSource);
}

void SimEncoderModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	if (m_bEnabled) {
		double randfactor=.05;
		double randNum = (16-(rand() & 0x1F))/32.0;
		double randmult=1+randNum*randfactor;
		m_encoderVal += randmult*m_rotSource->GetRotationValue();
	}
}
void SimEncoderModule::SetEncoderEnabled(bool bEnabled)
{
	if (bEnabled ^ m_bEnabled) {
		m_bEnabled = bEnabled;
		m_encoderVal = 0.0;
	}
}