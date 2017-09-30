#pragma once

#include "Shared/FiniteStateMachine.h"
#include "Shared/HumanDriveControl.h"
#include "Shared/RobotModel.h"


class ClawStateMachine : public virtual FiniteStateMachine
{
private:
	
	enum ClawState {
		//states
		kIdle, kReset, kInitialize, kOpen, kClosed
	};
	

	
	// inputs
	HumanDriveControl* m_pHumanController;
	RobotModel*	m_pRobotModel; //input and output
	bool	m_bRequestClawOpen;
	bool	m_bRequestClawClosed;
	
	uint32_t nextState;
	
	//outputs
	bool	m_bClawDone;
	
public:
	
	
	void RequestClawOpen();
	void RequestClawClosed();
	
	bool IsClawDone() const { return m_bClawDone; }
	
	void UpdateState(double currTime_Sec, double deltaTime_sec) ;
	
	bool IsMachineDone(){return false;};
	
	explicit ClawStateMachine(RobotModel* pRobot) : FiniteStateMachine(pRobot, kInitialize)
	    {
		}

};
