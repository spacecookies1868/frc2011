/*
 *  SimRobotBody.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimRobotBody.h"
#include "SimulatedRobot/SimModules/Interfaces/DistanceVectorInterface.h"
#include "SimulatedRobot/SimModules/SimWheelModule.h"
#include "Shared/Matrix3x3.h"
#include "FieldObject/Components/Prism.h"
#include <math.h>


static const double WHEEL_DIAMETER=.089;
static const double WHEEL_WIDTH=WHEEL_DIAMETER/2;
static const int CIRCLE_RESOLUTION=16;
static const Prism wheelModel(WHEEL_DIAMETER,WHEEL_DIAMETER,WHEEL_WIDTH,CIRCLE_RESOLUTION);


SimRobotBody::SimRobotBody(SimWheelModule*	wheels[RobotModel::kNumWheels],
                           double bodyWidth,
						   double bodyLength,
						   double bodyHeight,
						   double bodyMass,
                           double bodyRotationTensor,
						   FieldObject::Alliance alliance)
: FieldObject(Vector3(0,0,0),Vector3(0,0,0),alliance,"Robot")
, m_bodyWidth(bodyWidth)
, m_bodyLength(bodyLength)
, m_bodyHeight(bodyHeight)
, m_bodyMass(bodyMass)
, m_bodyRotationTensor(bodyRotationTensor)
{
	addPolyhedron(Prism(bodyLength, bodyWidth, bodyHeight,4), Vector3(0,0,0), Vector3(0,0,0));
	for (int i=0; i<RobotModel::kNumWheels; i++) {
		m_wheels[i] = wheels[i];
		m_simModule->AddModuleToSources(m_wheels[i]);
	}
	m_static=0;
	m_uid=FieldObject::robot;
	
	double wheel_spacing=GetWidth()/3;
	
	//put in wheels
	for(int i=0;i<6;i++) {
		int side=i/3?1:-1;
		int pos=i%3;
		m_wheelindices[i]=addPolyhedron(wheelModel, Vector3(wheel_spacing*(pos-1),side*GetLength()/2+WHEEL_WIDTH/2,WHEEL_DIAMETER/2), Vector3(M_PI/2,0,0));
	}
	
	
	
}
void SimRobotBody::InitializeLocation(const Vector3& pos, Vector3 angle)
{
	m_pos = pos; //represents center position on floor
	m_angle = angle;
}

void SimRobotBody::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	//turn wheels
	for(int i=0;i<6;i++) {
		int sign=(2*(i/3)-1);
		m_relativeangles[m_wheelindices[i]].z+=sign*m_wheels[i/3]->GetRotationInterface()->GetRotationValue();
	}
	
	Vector3	wheelDistanceVec[RobotModel::kNumWheels];
	Vector3	wheelVelocityVec[RobotModel::kNumWheels];
	double wheelDistance[RobotModel::kNumWheels];
	
	Vector3	combinedDistanceVec;
	double	combinedDistanceMag=0;
	
	
	for (int i=0; i<RobotModel::kNumWheels; i++)
	{	
		wheelDistanceVec[i] = m_wheels[i]->GetDistanceVectorInterface()->GetDistanceVector();
		wheelVelocityVec[i] = wheelDistanceVec[i] / delta_simTimeSec;
		wheelDistance[i] = wheelDistanceVec[i].y;
		combinedDistanceMag += fabs(wheelDistance[i]);
		combinedDistanceVec += wheelDistanceVec[i];
	}
	
	double distanceTraveled=0.f;
	// if the wheel velocities are different enough
	if (wheelDistance[RobotModel::kLeftWheel] != wheelDistance[RobotModel::kRightWheel]) {
		if (fabs(combinedDistanceMag) > combinedDistanceVec.magnitude())
			distanceTraveled = combinedDistanceVec.magnitude();
		else if (wheelDistanceVec[RobotModel::kLeftWheel].magnitude() >= wheelDistanceVec[RobotModel::kRightWheel].magnitude())
			distanceTraveled = wheelDistance[RobotModel::kRightWheel];
		else 
			distanceTraveled = wheelDistance[RobotModel::kLeftWheel];
		
		
		static const Vector3  wheelRotationRadius[RobotModel::kNumWheels] = 
		{
			Vector3(-1.0 * m_bodyWidth, 0, 0.0),
			Vector3(1.0 * m_bodyWidth, 0, 0.0),
		};
		
		// L = p x r
		// ignore mass as part of p = mv because mass is constant for the robot
		// mass is compensated by I in L = Iw
		// calculate the angular velocity contributed by each wheel
		Vector3 angularVelocity[RobotModel::kNumWheels];
		// perform w = L / I = (v * r) / I 
		for (int i=0; i<RobotModel::kNumWheels; i++)
			angularVelocity[i] = (wheelRotationRadius[i].crossProd(wheelVelocityVec[i])) / m_bodyRotationTensor;
		Vector3 angleDelta = (angularVelocity[RobotModel::kLeftWheel] + angularVelocity[RobotModel::kRightWheel]) * delta_simTimeSec;
		m_angle += angleDelta;
		
	}
	else
		distanceTraveled = wheelDistance[RobotModel::kLeftWheel];
	
	Vector3 distanceDelta;
	distanceDelta.x = distanceTraveled * cos(m_angle.z);
	distanceDelta.y = distanceTraveled * sin(m_angle.z);
	
	m_vel = distanceDelta;
	m_pos += distanceDelta;	
}