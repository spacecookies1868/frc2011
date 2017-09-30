/*
 *  SimElevatorModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimElevatorModule.h"
#include "FieldObject/Components/Prism.h"
#include "SimulatedRobot/SimModules/SimRobotBody.h"
#include "SimulatedRobot/SimModules/SimPotModule.h"
#include "SimulatedRobot/SimModules/SimEncoderModule.h"
#include "SimulatedRobot/SimModules/SimGearingModule.h"
#include "SimulatedRobot/SimModules/SimMotorModule.h"

static const double ELEVATOR_LENGTH=.272;
static const double ELEVATOR_WIDTH=.167;
static const double ELEVATOR_HEIGHT=2.5;//1.435;
static const double ARM_INIT_ANGLE=-M_PI/2;
static const Prism elevatorPrism(ELEVATOR_LENGTH,ELEVATOR_WIDTH,ELEVATOR_HEIGHT,4);

SimElevatorModule::SimElevatorModule(SimRobotBody* body)
: m_body(body)
{
	m_polyhedronindex=m_body->addPolyhedron(elevatorPrism, Vector3(body->GetLength()/2-ELEVATOR_WIDTH/2,0,body->GetHeight()), Vector3(0,0,0));
	m_body->init_bottom();
	m_sources.push_back(m_body->GetSimModule());	
	
    NumericalVector<2> errorRange;
    
	m_elevatorMotor=new SimMotorModule(60.0, errorRange);
	m_armMotor=new SimMotorModule(60.0, errorRange);
	
	
	double elevatorGearingRatio=50; //damper on elevator speed
	m_elevatorGearing=new SimGearingModule(m_elevatorMotor,elevatorGearingRatio);
	m_armGearing=new SimGearingModule(m_armMotor,1);
	
	m_armPot=new SimPotModule(m_armGearing,1);
	m_elevatorEncoder=new SimEncoderModule(m_elevatorGearing);
	m_elevatorEncoder->SetEncoderEnabled(true);
}

SimElevatorModule::~SimElevatorModule() {
	delete m_elevatorMotor;
	delete m_armMotor;
	delete m_elevatorGearing;
	delete m_armGearing;
	delete m_armPot;
	delete m_elevatorEncoder;
}

void SimElevatorModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	//constants
	static const double MAX_ANGLE=0;
	static const double MIN_ANGLE=ARM_INIT_ANGLE;
	static const double MAX_HEIGHT=ELEVATOR_HEIGHT;
	static const double MIN_HEIGHT=0;
	
	double height=m_elevatorEncoder->GetEncoderValue();
	if(height>MAX_HEIGHT)
		m_elevatorMotor->SetDrive(-1);
	else if(height<MIN_HEIGHT)
		m_elevatorMotor->SetDrive(1);
	
	double angle=m_armPot->GetPotValue()+ARM_INIT_ANGLE;
	if(angle>MAX_ANGLE)
		m_armMotor->SetDrive(-1);
	else if(angle<MIN_ANGLE)
		m_armMotor->SetDrive(1);
}

double SimElevatorModule::GetArmPos() const
{
	return m_elevatorEncoder->GetEncoderValue();
}

double SimElevatorModule::GetArmAngle() const
{
	return m_armPot->GetPotValue()+ARM_INIT_ANGLE;
}