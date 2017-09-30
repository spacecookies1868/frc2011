/*
 *  StudentProgram.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/31/10.
 *  Copyright 2010 Apple Inc.. All rights reserved.
 *
 */

#include "Shared/CommonIncludes.h"
#include "StudentProgram.h"
#include "Shared/Vector3.h"
#include "Shared/RobotModel.h"
#include "Shared/HumanDriveControl.h"
#include "Shared/FieldModel.h"
#include "Shared/FiniteStateMachine.h"
#include "RobotTuningTests.h"
#include "Shared/FieldModel.h"
#include "LinearizeMotorTest.h"
#include "AutoStateMachine.h"

void StudentProgram::operatorControl(double curr_time, double delta_time)
{
		m_pDriveController->update();
		//printf("drivestatemachine updated");
		if(m_pDriveController->getDesiredGear() == RobotModel::kHighGear)
			m_pRobot->SetGearType(RobotModel::kHighGear);
		else
			m_pRobot->SetGearType(RobotModel::kLowGear);
		
		if(m_pDriveController->getDesiredClaw() == RobotModel::kOpen)
			m_pRobot->SetClawType(RobotModel::kOpen);
		else
			m_pRobot->SetClawType(RobotModel::kClosed);
		
		if(m_pDriveController->getDesiredRetract() == RobotModel::kRetract)
					m_pRobot->SetRetractType(RobotModel::kExtend);
				else
					m_pRobot->SetRetractType(RobotModel::kRetract);
		
		if(m_pDriveController->getDesiredDeploy() == RobotModel::kDeploy)
					m_pRobot->SetDeployType(RobotModel::kDeploy);
				else 
					m_pRobot->SetDeployType(RobotModel::kUndeploy);
		
		if(m_pDriveController->getDesiredLower() == RobotModel::kLower)
					m_pRobot->SetLowerType(RobotModel::kLower);
				else
					m_pRobot->SetLowerType(RobotModel::kRaise);
		
		
		m_DriveStateMachine->UpdateState(curr_time, delta_time);
		m_ArmStateMachine->UpdateState(curr_time, delta_time);

}

void StudentProgram::autonomousInit()
{
	m_AutoStateMachine->Reset();
	m_DriveStateMachine->Reset();
	m_ArmStateMachine->Reset();
	m_pDriveController->update();
	printf("Autonomous init complete\n");
}

void StudentProgram::autonomousControlPeriodic(double time, double delta_time)
{
    __unused static uint32_t exec_test = 0;
    static int i = 0;
 
    if ( i++ % 100 == 0 ) printf( "%s\n", __FUNCTION__ );
 
    m_AutoStateMachine->UpdateState(time, delta_time);
    m_DriveStateMachine->UpdateState(time, delta_time);
    m_ArmStateMachine->UpdateState(time, delta_time);

    /*if (exec_test)
    {
        switch (exec_test) {
            case 1:
                if (!m_pLinearizeMotorsTest->IsMachineDone())
                    m_pLinearizeMotorsTest->UpdateState(time, delta_time);
                break;
            default:
                break;
        }
    }*/

}

void StudentProgram::teleopInit(){
	m_AutoStateMachine->Reset();
	m_DriveStateMachine->Reset(); 
	m_ArmStateMachine->Reset();
}

void StudentProgram::autonomousControl()
{
    __unused static const uint32_t autonomousContinuosTest = 2;

    /*switch (autonomousContinuosTest) {
        case 0:
        default:
            break;
        case 1:
            FindEncoderConstant(m_pRobot);
            break;
        case 2:
            FindAcceleration(1.0, m_pRobot);
            FindAcceleration(-1.0, m_pRobot);
            break;
        case 3:
            TestPID(m_pRobot, 1.0, 5.0);
            break;
        case 4:
            TestPID(m_pRobot, -1.0, 5.0);
            break;
    }*/

}

StudentProgram::StudentProgram(RobotModel* model, HumanDriveControl* driveControl)
: m_pRobot(model)
, m_pDriveController(driveControl)
, m_pLinearizeMotorsTest(NULL)
, m_bAutonomousComplete(false)
{
    m_pLinearizeMotorsTest = new LinearizeMotorTest(m_pRobot);
    m_DriveStateMachine = new DriveStateMachine(m_pRobot, m_pDriveController);
    m_ArmStateMachine = new ArmStateMachine(m_pRobot, m_pDriveController);
    m_AutoStateMachine = new AutoStateMachine(m_pRobot, m_DriveStateMachine, m_ArmStateMachine, m_pDriveController);
}

StudentProgram::~StudentProgram()
{
    delete m_pLinearizeMotorsTest;
}

Vector3 StudentProgram::getAssumedFieldPosition() const
{
    return Vector3(-100,-100,0);
}

Vector3 StudentProgram::getAssumedFieldAngle() const
{
    return Vector3(0,0,0);
}

void
StudentProgram::setInitialPositionInformation( const Vector3 &inPosition, const Vector3& inPositionError,
                                      const Vector3& inAngle,  const Vector3& inAngleError)
{
    m_InitialPosition = inPosition;
    m_InitialPositionTolerance = inPositionError;
    m_InitialAngle = inAngle;
    m_InitialAngleTolerance = inAngleError;
}
