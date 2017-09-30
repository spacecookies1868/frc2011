/*
 *  SimWheelModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimWheelModule.h"
#include <math.h>

SimWheelModule::SimWheelModule(RotationInterface* sourceRot, double wheelRadius, const Vector3& wheelDir, double accelFactor, double friction)
: m_rotSource(sourceRot)
, m_wheelRadius(wheelRadius)
, m_wheelDirection(wheelDir)
, m_rotVal(0)
, m_accel(accelFactor)
, m_currFrict(friction)
{	
	m_sources.push_back(sourceRot);
	m_wheelDirection.normalize();
	m_pRotWrapper = new RotationWrapper(this);
	m_pDistanceWrapper = new DistanceWrapper(this);
}

Vector3 SimWheelModule::GetDistanceTraveled() const
{
	Vector3 wheelDistance = m_wheelDirection * ((2 * M_PI * m_wheelRadius) * this->GetRotationValue());
	return wheelDistance;
}

void SimWheelModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	m_rotVal=m_rotSource->GetRotationValue();
	/* need to fix
	
	///* acceleration shizz - update when actual robot is tested
	double src = m_rotSource->GetRotationValue();
	double acc = m_accel*delta_simTimeSec;
	double frict=m_currFrict*delta_simTimeSec;
	int src_sign=fabs(src)<.0001?0:src/fabs(src);
	int rot_sign=fabs(m_rotVal)<.0001?0:m_rotVal/fabs(m_rotVal);
	
	//simplified friction:
	//if want to turn faster
	m_rotVal+=src_sign*acc;

	//otherwise add friction
	if(src_sign==0)
		m_rotVal-=rot_sign*frict;

	//if going the opposite direction
	else if(src_sign==-rot_sign)
		m_rotVal+=src_sign*acc;
	
	//cap max speed
	if((src>0 && m_rotVal>src) || (src_sign<0 && m_rotVal<src))
		m_rotVal=src;
	
	//cleanup
	if(src=0 && fabs(m_rotVal)<=frict)
		m_rotVal=0;
	
	printf("fric: %f\n",frict);
	printf("src: %f\nrot: %f\n",src,m_rotVal);
	 */
}