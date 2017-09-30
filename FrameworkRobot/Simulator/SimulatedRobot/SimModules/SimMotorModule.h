/*
 *  SimMotorModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_MOTOR_MODULE_H
#define SIM_MOTOR_MODULE_H

#include "SimulatedRobot/SimModules/Interfaces/RotationInterface.h"
#include "Shared/NumericalVector.hpp"

class SimMotorModule : public RotationInterface
{
public:
	SimMotorModule(double motorRPS, NumericalVector<2> const& errorRange);
	~SimMotorModule() {}
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	void SetDrive(double drive);
	const double GetRotationValue() const	{ return m_lastMotorRotation; }
	double PWMtoRPM(double pwm);

private:
	double		m_driveSpeed;
	double      m_motorRPS;
	double		m_lastMotorRotation;
    NumericalVector<2>  m_ErrorRange;
};
#endif
