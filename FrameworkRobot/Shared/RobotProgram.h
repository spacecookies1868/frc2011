/*
 *  RobotProgram.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 12/21/10.
 *
 */

#ifndef ROBOT_PROGRAM_H
#define ROBOT_PROGRAM_H

#include "Shared/Vector3.h"

/**
 * An abstract class modeling control of the robot.
 *
 * @author Dmitriy Solomonov
 */
class RobotProgram
{

public:
    
    virtual ~RobotProgram() {}
    
    /**
     * Puts the robot under autonomous control for the current frame of periodic update.
     *
     * @param curr_time the time that the update frame has started
     * @param delta_time the time difference between the start of this frame and the last one
     */
    virtual void autonomousControlPeriodic(double curr_time, double delta_time) = 0;
    
    /**
     * Puts the robot under autonomous control until the function exits.
     */
    virtual void autonomousControl() = 0;
    virtual bool IsAutonomousDone() const = 0;
    
    /**
     * Puts the robot under operator control for the current update frame.
     *
     * @param curr_time the time that the update frame has started
     * @param delta_time the time difference between the start of this frame and the last one
     */
    virtual void operatorControl(double curr_time, double delta_time) = 0;
    
    virtual Vector3 getAssumedFieldPosition() const = 0;
    virtual Vector3 getAssumedFieldAngle() const = 0;
    
    
    virtual void setInitialPositionInformation( const Vector3 &inPosition, const Vector3& inPositionError, 
                                          const Vector3& inAngle,  const Vector3& inAngleError) = 0;
    
};

#endif
