/*
 *  SimMotorModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <assert.h>

#include "SimMotorModule.h"

SimMotorModule::SimMotorModule(double motorRPS, NumericalVector<2> const& errorRange)
: m_driveSpeed(0)
, m_motorRPS(motorRPS)
, m_lastMotorRotation(0.0)
, m_ErrorRange(errorRange)
{
	assert(m_ErrorRange[1] >= m_ErrorRange[0]);
}

void SimMotorModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	double randNum = (rand() & 0x1F) / 32.0;
    double randFactor = (randNum * (m_ErrorRange[1] - m_ErrorRange[0])) + m_ErrorRange[0];

	double motorDrive = m_driveSpeed * delta_simTimeSec;
	motorDrive = motorDrive * (1.0 + randFactor);
	m_lastMotorRotation = motorDrive;
}

void SimMotorModule::SetDrive(double drive)
{
	if (fabs(drive) > 1)
		drive = (drive > 0) ? 1 : -1;
	m_driveSpeed = PWMtoRPM(drive);
}

double SimMotorModule::PWMtoRPM(double pwm)
{
	return pwm*m_motorRPS;
	/*
	double maxrpm;
	double accelcoefs[4];
	if(pwm==0)
		return 0;
	if(pwm>0) {
		maxrpm=4.4890;
		accelcoefs[0]= 22.0087;
		accelcoefs[1]=-39.0928;
		accelcoefs[2]= 23.6647;
		accelcoefs[3]=- 0.7287;
	}
	else {
		maxrpm=4.6396;
		accelcoefs[0]= 16.2742;
		accelcoefs[1]=-31.9954;
		accelcoefs[2]= 20.8889;
		accelcoefs[3]=- 0.1869;
	}
	 */
	
}