/*
 *  SimRobotBody.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_ROBOT_BODY_H
#define SIM_ROBOT_BODY_H

#include "SimulatedRobot/SimModules/SimModuleBase.h"
#include "Shared/Vector3.h"
#include "Shared/RobotModel.h"
#include "SimulatedRobot/SimulatedField.h"

class SimWheelModule;

class SimRobotBody : public FieldObject
{
public:
	SimRobotBody(SimWheelModule* wheels[RobotModel::kNumWheels],
				 double bodyWidth, double bodyLength, double bodyHeight,
				 double bodyMass, double rotationTensor,
				 FieldObject::Alliance alliance);
	~SimRobotBody() {}
    void    InitializeLocation(const Vector3& pos, Vector3 angle);
	const double GetWidth() {return m_bodyWidth;}
	const double GetLength() {return m_bodyLength;}
	const double GetHeight() {return m_bodyHeight;}
	const double GetMass() {return m_bodyMass;}
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	
private:
	SimWheelModule*	m_wheels[RobotModel::kNumWheels];
    Vector3			m_vel;
    double			m_bodyWidth;
	double			m_bodyLength;
	double			m_bodyHeight;
	double			m_bodyMass;
    double			m_bodyRotationTensor;
	int				m_wheelindices[6];
	friend class SimElevatorModule;
	friend class SimArmModule;
};
#endif