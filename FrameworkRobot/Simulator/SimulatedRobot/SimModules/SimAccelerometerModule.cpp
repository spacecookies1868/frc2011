/*
 *  SimAccelerometerModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimAccelerometerModule.h"
#include "SimulatedRobot/SimModules/SimRobotBody.h"

SimAccelerometerModule::SimAccelerometerModule(SimRobotBody* body)
: m_body(body)
, m_prevVel(0,0,0)
{
	m_sources.push_back(body->GetSimModule());
}

void SimAccelerometerModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	const Vector3& currVel = m_body->GetVel();
	m_prevAcc=(currVel-m_prevVel)/delta_simTimeSec;
	m_prevVel=currVel;
}