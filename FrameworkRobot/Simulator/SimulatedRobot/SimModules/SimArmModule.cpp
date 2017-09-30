/*
 *  SimArmModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimArmModule.h"
#include "SimulatedRobot/SimModules/SimElevatorModule.h"
#include "SimulatedRobot/SimModules/SimRobotBody.h"
#include "FieldObject/FieldElements/Tubes/Tube.h"
#include "SimulatedRobot/SimulatedField.h"
#include "Shared/Matrix3x3.h"

static const float ARM_LENGTH=.167;
static const float ARM_WIDTH=.483;
static const float ARM_HEIGHT=.167;
static const Prism armPrism(ARM_LENGTH, ARM_WIDTH, ARM_HEIGHT, 4);
static const float GRABBER_SIDE=.2;
static const Prism grabber(GRABBER_SIDE,GRABBER_SIDE,GRABBER_SIDE,4);

SimArmModule::SimArmModule(SimElevatorModule* elevator)
: m_elevator(elevator)
, m_body(elevator->m_body)
, m_tube(NULL)
{
	m_polyhedronindex=m_body->addPolyhedron(armPrism,
											m_body->m_relativeposs[m_elevator->m_polyhedronindex]+Vector3(0,0,m_elevator->GetArmPos())+Vector3(ARM_WIDTH/2+.09,0,0),
											Vector3(0,m_elevator->GetArmAngle(),0)+m_body->m_relativeangles[m_elevator->m_polyhedronindex]);
	m_grabberindex=m_body->addPolyhedron(grabber,
										 m_body->m_relativeposs[m_polyhedronindex]+Vector3(ARM_WIDTH/2+.09,0,0)+Vector3(GRABBER_SIDE/2,0,0),
										 m_body->m_relativeangles[m_polyhedronindex]);
	m_body->init_bottom();
	m_sources.push_back(elevator);
  
	
}
void SimArmModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	//arm
	m_body->m_relativeangles[m_polyhedronindex]=Vector3(0,m_elevator->GetArmAngle(),0)+m_body->m_relativeangles[m_elevator->m_polyhedronindex];
	m_body->m_relativeposs[m_polyhedronindex]=m_body->m_relativeposs[m_elevator->m_polyhedronindex]+Vector3(0,0,m_elevator->GetArmPos())+Vector3(.09,0,0);
	m_body->m_relativeposs[m_polyhedronindex].x+=ARM_WIDTH/2*cos(-m_body->m_relativeangles[m_polyhedronindex].y);
	m_body->m_relativeposs[m_polyhedronindex].z+=ARM_WIDTH/2*sin(-m_body->m_relativeangles[m_polyhedronindex].y);
	
	//grabber
	m_body->m_relativeangles[m_grabberindex]=m_body->m_relativeangles[m_polyhedronindex];
	m_body->m_relativeposs[m_grabberindex]=m_body->m_relativeposs[m_polyhedronindex];
	m_body->m_relativeposs[m_grabberindex].x+=(ARM_WIDTH/2+GRABBER_SIDE/2)*cos(-m_body->m_relativeangles[m_grabberindex].y);
	m_body->m_relativeposs[m_grabberindex].z+=(ARM_WIDTH/2+GRABBER_SIDE/2)*sin(-m_body->m_relativeangles[m_grabberindex].y);
	
	if(m_tube) {
		//absolute
		m_tube->m_pos=m_body->m_pos;
		m_tube->m_angle=m_body->m_angle;
		//relative
		Matrix3x3 relative_rot=Matrix3x3::RotMatrix(m_body->m_angle);
		m_tube->m_pos+=relative_rot*m_body->m_relativeposs[m_grabberindex];
		m_tube->m_angle+=relative_rot*m_body->m_relativeangles[m_grabberindex];

	}
	 
}