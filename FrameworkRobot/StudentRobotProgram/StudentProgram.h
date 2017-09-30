
/*
 *  StudentProgram.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/31/10.
 *  Copyright 2010 Apple Inc.. All rights reserved.
 *
 */

#ifndef STUDENT_PROGRAM_H
#define STUDENT_PROGRAM_H

#include "Shared/RobotProgram.h"
#include "Shared/RobotModel.h"
#include "Shared/FiniteStateMachine.h"
#include "DriveStateMachine.h"
#include "ArmStateMachine.h"
#include "AutoStateMachine.h"

class RobotModel;
class HumanDriveControl;
class FiniteStateMachine;
class DriveStateMachine;

/**
 * A simulated implementation of a robot program.
 *
 * @author Dmitriy Solomonov
 */
class StudentProgram : public virtual RobotProgram
{
public:

	/**
	 * Creates a program with the supplied robot characteristics and input.
	 *
	 * @param model the robot model
	 * @param driveControl the robot's input
	 */
    explicit StudentProgram(RobotModel* model, HumanDriveControl* driveControl);
    virtual ~StudentProgram();


    virtual void autonomousControlPeriodic(double time, double delta_time);
    virtual bool IsAutonomousDone() const   {   return m_bAutonomousComplete;   }
    virtual void autonomousControl();
    virtual void autonomousInit();
    virtual void teleopInit();
    virtual void operatorControl(double curr_time, double delta_time);
    
    virtual Vector3 getAssumedFieldPosition() const;
    virtual Vector3 getAssumedFieldAngle() const;
    
    virtual void setInitialPositionInformation( const Vector3 &inPosition, const Vector3& inPositionError, 
                                          const Vector3& inAngle,  const Vector3& inAngleError);

	/**
	 * Models "Stage 1" autonomously.
	 *
	 * Stage 1 consists of the robot scoring with the ring from its starting position.
	 */
    void        Stage1Autonomous();

protected:

	RobotModel*         m_pRobot;

    HumanDriveControl*  m_pDriveController;


    FiniteStateMachine* m_pLinearizeMotorsTest;
    
    DriveStateMachine* 	m_DriveStateMachine;
    ArmStateMachine*	m_ArmStateMachine;
    AutoStateMachine*	m_AutoStateMachine;
    
    bool                m_bAutonomousComplete;
    
    Vector3             m_InitialPosition;
    Vector3             m_InitialPositionTolerance;
    Vector3             m_InitialAngle;
    Vector3             m_InitialAngleTolerance;

};

#endif
