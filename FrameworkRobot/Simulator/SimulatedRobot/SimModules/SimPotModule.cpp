/*
 *  SimPotModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimPotModule.h"

SimPotModule::SimPotModule(RotationInterface* rotSource, int numTurns)
: m_rotSource(rotSource)
, m_turns(numTurns)
, m_potPos(0.0)
{
	m_sources.push_back(rotSource);
}

void SimPotModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	m_potPos += m_rotSource->GetRotationValue();
	if(m_potPos>m_turns)
		m_potPos=m_turns;
}