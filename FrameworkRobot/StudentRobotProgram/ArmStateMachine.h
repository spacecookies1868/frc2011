#pragma once

#include "Shared/CommonIncludes.h"
#include "Shared/FiniteStateMachine.h"
#include "Shared/HumanDriveControl.h"
#include "Shared/RobotModel.h"
#include "SpaceCookieBot/SpaceCookieBot.h"

#define ONOFFHOLDING 1


class ArmStateMachine : public virtual FiniteStateMachine
{
	
private:
	
	/*		Pole-peg postions 
			2  1  2
			4  3  4
			6  5  6
	*/
	
	
	// inputs
	HumanDriveControl* m_pHumanController;
	RobotModel*	m_pRobotModel; //input and output
	
	uint32_t m_RequestArmPosition; // the requested arm position value
	uint32_t m_RequestJog;
	
	bool	m_bHangMacro;
	bool	m_bRequestHumanDrive;
	/**
	bool	m_bHoldingButtonPressed;
	bool	m_bHoldingButtonJustPressed;
	bool	m_bHoldingButtonOn;
	**/
	bool	m_bHoldingActive;
	bool	m_bArmAboveWanted;
	

	// Variables
	uint32_t m_stateVal;
	uint32_t nextState;
	double m_currentEncoderValue;
	uint32_t m_ButtonResult;
	double m_desiredArmSpeed;
	double m_desiredEncoderValue;
	double m_error;
	double m_largeError;
	double m_StartTime;
	double m_HangStartTime;
	float last_error;
	int count;
	

	
public:
	
	enum ArmState {
		//states 
		kReset, kInitialize, kIdle,
		kSeekingPosition, kHuman, kHoldingPosition, kHangMacro
	};
		
	enum ArmPosition {
		//various positions
		kNoPosition,
		kCenterHigh,kOutsideHigh,
		kCenterMiddle, kOutsideMiddle, 
		kCenterLow, kOutsideLow, 
		kGround, kNumArmPositions
	};
	
	enum ArmJog	{
		kNoJog, KJogDown, KJogUp
	};
	
	// Arm Encoder values - at the desired arm positions
	int ArmPositionEncoderValues[kNumArmPositions] ;
				
	//Functions
 	void RequestArmPosition(uint32_t pos);
 	
	
    explicit ArmStateMachine(RobotModel* pRobot, 
    		 HumanDriveControl* m_pHumanDriveControl) ;
    	
	virtual void UpdateState(double x /*currTime_Sec*/ , double y /*currTime_Sec*/);
	
	double ArmPID();
	
	bool IsMachineDone() {return false;};
	
	bool IsHolding() { return m_bHoldingActive; };
	
	void Reset();
	
	void ReleaseHold();
	

};
