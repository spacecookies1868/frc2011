/*
 *  SimAccelerometerModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_ACCELEROMETER_MODULE_H
#define SIM_ACCELEROMETER_MODULE_H

#include "SimulatedRobot/SimModules/SimModuleBase.h"
#include "Shared/Vector3.h"

class SimRobotBody;

class SimAccelerometerModule : public SimModuleBase
{
public:
	SimAccelerometerModule(SimRobotBody* body);
	~SimAccelerometerModule() {}
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	const Vector3& GetAccelerometerValue() {return m_prevAcc;}
private:
	SimRobotBody*	m_body;
	Vector3		m_prevAcc;
	Vector3		m_prevVel;
};
#endif