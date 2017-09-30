/*
 *  SimGearingModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimGearingModule.h"

SimGearingModule::SimGearingModule(RotationInterface* source, double gearingRatio)
: m_rotSource(source)
, m_gearingRatio(gearingRatio)
{
	m_rotSource = source;
	m_sources.push_back(m_rotSource);
}

void SimGearingModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	// do nothing now, may extend this class to have error in the gearing
}