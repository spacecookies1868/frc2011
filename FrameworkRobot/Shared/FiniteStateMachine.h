/*
 *  FiniteStateMachine.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 1/21/11.
 *
 */

#ifndef FINITE_STATE_MACHINE
#define FINITE_STATE_MACHINE

#include "Shared/CommonIncludes.h"


class RobotModel;

/**
 * Models a finite-state machine which can be updated over time.
 *
 * @author Dmitriy Solomonov
 */
class FiniteStateMachine 
{
public:
    
	/**
	 * Updates the state of the machine.
	 *
	 * @param currTime_sec the time which to update the machine state
	 * @param deltaTime_sec the length of time over which to update
	 */
    virtual void UpdateState(double currTime_Sec, double deltaTime_sec) = 0;
    
    virtual bool IsMachineDone() = 0;
    
    /**
     * Constructor with specified robot model and initial state.
     */
    explicit FiniteStateMachine(RobotModel* pRobot, uint32_t initialState)
    : m_pRobot(pRobot)
    , m_stateVal(initialState)
    {}
    
    virtual ~FiniteStateMachine() {}
    
protected:
    
    /**
     * The robot characteristics of the machine.
     */
    RobotModel*     m_pRobot;

    /**
     * The value of the machine's state.
     */
    uint32_t        m_stateVal;
};


#endif
