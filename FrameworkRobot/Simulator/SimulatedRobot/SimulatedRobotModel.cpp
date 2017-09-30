/*
 *  SimulatedRobotModel.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 12/2/10.
 *
 */

#include "SimulatedRobotModel.h"
#include "Shared/Vector3.h"
#include "SimulatedRobot/SimModules/SimModuleBase.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <SDL.h>
#include <SDL_thread.h>
#include <list>
#include "SimGUI/SimHardware.h"
#include "FieldObject/FieldElements/Tubes/TubeRing.h"

using namespace std;

static Vector3 DegreesToRadians(Vector3 inDegrees)
{
    return inDegrees * M_PI / 180.f;
}

static Vector3 RadiansToDegrees(Vector3 inRadians)
{
    return  inRadians * 180.f / M_PI;
}

SimModuleBase::ModuleList	SimModuleBase::s_AllModules;
SimModuleBase::ModuleList	SimModuleBase::s_ModuleUpdateList;

SimulatedRobotModel::SimulatedRobotModel(SimulatedField* field, const Vector3& initialPos, Vector3 initialAngle, FieldObject::Alliance alliance)
: m_field(field)
, m_LastSimulatedUpdateTime(0)
, m_TimeBaseline(0)
{
	//initialize robot modules & body
    static const double sLeftMotorErrorRange[2] = {-0.1, 0.1};
    static const double sRightMotorErrorRange[2] = {-0.1, 0.1};
    
    static const NumericalVector<2> sMotorError[kNumWheels] = 
    {
        NumericalVector<2>(sLeftMotorErrorRange),
        NumericalVector<2>(sRightMotorErrorRange)
    };
    
    static const double robotAccelFactor = .01;

	for (int i=0; i<RobotModel::kNumWheels; i++)
	{
		WheelType wheel_id = (WheelType)i;
		SimMotorModule* motor = new SimMotorModule(this->GetMaxMotorRPS(), sMotorError[i]);
		SimGearingModule* gearing = new SimGearingModule(motor, RobotModel::GetWheelGearingRatio(wheel_id));
		SimWheelModule* wheel = new SimWheelModule(gearing, RobotModel::GetWheelRadius(wheel_id), Vector3(0,1,0), robotAccelFactor, m_field->GetRollingResistance());
		SimEncoderModule* encoder = new SimEncoderModule(wheel->GetRotationInterface());
	
		m_pMotors[i] = motor;
		m_pEncoders[i] = encoder;
		m_pWheels[i]=wheel;
	}

	m_pRobotBody = new SimRobotBody(m_pWheels, GetLength(), GetWidth(),
									GetHeight(), GetMass(), GetRotationTensor(),
									alliance);
	m_pRobotBody->InitializeLocation(initialPos, DegreesToRadians(initialAngle));
	m_field->AddFieldObject((FieldObject*) m_pRobotBody);

	Vector3 gyroVect=Vector3(0,1,1).normalize();
	m_pGyro=new SimGyroModule(m_pRobotBody,gyroVect);

	

}

FieldObject* SimulatedRobotModel::GetRobotFieldObject() const
{
    return m_pRobotBody;
}

SimulatedRobotModel::~SimulatedRobotModel()
{
	SimModuleBase::DeleteAllModules();
}

void
SimulatedRobotModel::UpdateSimulation(uint32_t simTimeMS)
{
	uint32_t timeDiffMS = simTimeMS - m_LastSimulatedUpdateTime;
	double delta_time = timeDiffMS / 1000.0;
	SimModuleBase::UpdateAllModules(simTimeMS, delta_time);
	m_LastSimulatedUpdateTime = simTimeMS;
}


/*

 Basic Robot Interface

 */

void
SimulatedRobotModel::SetWheelMotorValue(WheelType wheel_id, double drive)
{
	assert(wheel_id < kNumWheels);
	m_pMotors[wheel_id]->SetDrive(drive);
}

double
SimulatedRobotModel::GetWheelEncoderValue(WheelType wheel_id) const
{
	assert(wheel_id < kNumWheels);
	return m_pEncoders[wheel_id]->GetEncoderValue();
}

void
SimulatedRobotModel::EnableWheelEncoder(WheelType wheel_id)
{
	assert(wheel_id < kNumWheels);
	m_pEncoders[wheel_id]->SetEncoderEnabled(true);
}

void
SimulatedRobotModel::DisableWheelEncoder(WheelType wheel_id)
{
	assert(wheel_id < kNumWheels);
	m_pEncoders[wheel_id]->SetEncoderEnabled(false);
}

void
SimulatedRobotModel::ResetWheelEncoder(WheelType wheel_id)
{
    assert(wheel_id < kNumWheels);
	m_pEncoders[wheel_id]->SetEncoderEnabled(false);
    m_pEncoders[wheel_id]->SetEncoderEnabled(true);
}


double
SimulatedRobotModel::GetGyroValue() const
{
	return m_pGyro->GetGyroValue().z;
}

void
SimulatedRobotModel::ClearGyro()
{
	m_pGyro->ClearGyro();
}

Vector3
SimulatedRobotModel::GetPosition() const
{
    return m_pRobotBody->GetPos();
}

Vector3
SimulatedRobotModel::GetRotationAngle() const
{
    return RadiansToDegrees(m_pRobotBody->GetAngle());
}

double
SimulatedRobotModel::GetTimeSec() const
{
    return SimHardware::GetInstance()->GetTimeSec();
}

uint32_t
SimulatedRobotModel::GetTimeMilliSec() const
{
    return SimHardware::GetInstance()->GetTimeMilliSec();
}

void
SimulatedRobotModel::WaitTimeMilliSec(uint32_t waitMS) const
{
    return SimHardware::GetInstance()->WaitTimeMilliSec(waitMS);
}
