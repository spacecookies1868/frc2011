#ifndef LEDSTATEMACHINE_H_
#define LEDSTATEMACHINE_H_

#include "Shared/FiniteStateMachine.h"
#include "Shared/HumanDriveControl.h"
#include "Shared/RobotModel.h"

class LEDStateMachine : public virtual FiniteStateMachine
{
	
private:
	
	enum LEDState {
		//states
		kReset, kInitialize, kIdle, kRed, kWhite, kBlue
	};
	
	
	//inputs
	HumanDriveControl* m_pHumanController;
	RobotModel* m_pRobotModel; // input and output
	bool	m_bRequestRed;
	bool	m_bRequestWhite;
	bool	m_bRequestBlue;
	bool	m_bLEDDone;
	
	uint32_t m_stateVal;
	uint32_t nextState;
	
	//outputs
	//?
	
public:
	
	
	void RequestRed();
	void RequestWhite();
	void RequestBlue();
	
	explicit LEDStateMachine(RobotModel* pRobot): FiniteStateMachine (pRobot, kInitialize)
	{
		m_pRobotModel = pRobot;
		m_stateVal = kInitialize;
	}

	bool AreLEDsDone() const { return m_bLEDDone; }
	
	virtual void UpdateState(double currTime_Sec, double deltaTime_sec);
	
	bool IsMachineDone(){return false;};
	
};

#endif /*LEDSTATEMACHINE_H_*/
