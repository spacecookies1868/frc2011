#ifndef AUTOSTATEMACHINE_H_
#define AUTOSTATEMACHINE_H_

#include "Shared/FiniteStateMachine.h"
#include "Shared/HumanDriveControl.h"
#include "Shared/RobotModel.h"
#include "DriveStateMachine.h"
#include "ArmStateMachine.h"

class AutoStateMachine : public virtual FiniteStateMachine
{
private:
	
	enum AutoDriveState {
		//states
		kDriveReset, kDriveInitialize, kDriveIdle,
		kStartFirstTubeDriveForward, kWaitForFirstTubeDriveForward, kDoneFirstTubeDriveForward, 
		kWaitToFirstTubeDriveBackward, kStartFirstTubeDriveBackward, kWaitForFirstTubeDriveBackward, kDoneFirstTubeDriveBackward, 
		kStartSecTubePivot1, kWaitForSecTubePivot1, kDoneSecTubePivot1,
		kStartSecTubeDriveForward1, kWaitForSecTubeDriveForward1, kDoneSecTubeDriveForward1,
		kStartSecTubePivot2, kWaitForSecTubePivot2, kDoneSecTubePivot2,
		kStartSecTubeDriveForward2, kWaitForSecTubeDriveForward2, kDoneSecTubeDriveForward2,
		kWaitToSecTubeDriveBackward, kStartSecTubeDriveBackward, kWaitForSecTubeDriveBackward, kDoneSecTubeDriveBackward,
		kStartDrive, kWaitForDrive, kDoneDrive,
		kStartPivot, kWaitForPivot, kDonePivot,
		
		//Another set of names of states for second half of two tube auto
		kStartSecondTubePivotOne, kWaitForAutoPivotOneDone, kSecondTubeAutoPivotOneDone, kWaitToSecondTubeDriveAngle, kStartSecondTubeDriveAngle, 
		kWaitForSecondTubeDriveAngle, kStartSecondTubePivotTwo, kWaitForAutoPivotTwoDone, kStartSecondTubeDriveStraight,
		kWaitForSecondTubeDriveStraight, kSecondTubeDriveStraightDone, kWaitToSecondTubeBackwardsDrive, kStartSecondTubeBackwardsDrive,
		kWaitForSecondTubeBackwardsDriveDone
	};
	
	enum AutoArmState {
		kArmReset, kArmInitialize, kArmIdle, kStartLiftArm, kWaitForLiftArm, kDoneLiftArm,
		kExtend, kReleaseArm, kWaitToOpenClaw, kOpenClaw,
		
		//more extra states for two-tubesssss
		kSecondTubeCloseClaw,kSecondTubeStartLiftArm, kSecondTubeWaitForLiftArm, kSecondTubeDoneLiftArm, kSecondTubeReleaseArm,
		kSecondTubeWaitToOpenClaw, kSecondTubeOpenClaw, kStartPowerDownArm, kWaitForPowerDownArm, kDonePowerDownArm, kWaitToPowerDownArm
		
	};
	
	
	
	RobotModel* m_pRobotModel;
	
	HumanDriveControl* m_pHumanDriveControl;
	
	DriveStateMachine* m_DriveStateMachine;
	ArmStateMachine* m_ArmStateMachine;
	
	uint32_t m_driveStateVal;
	uint32_t m_armStateVal;
	
	uint32_t m_nextDriveState;
	uint32_t m_nextArmState;
	uint32_t m_ArmPosition;
	
	bool m_bAutoDone;
	bool m_bReadyForLiftArm;
	bool m_bGoForTwoTubes;
	
	double m_ForwardDriveSpeed;
	double m_BackwardDriveSpeed;
	double m_ForwardDriveDistance;
	double m_BackwardDriveDistance;
	double m_StartOpenClawWaitTime;
	double m_OpenClawWaitTime;
	double m_StartDriveBackWaitTime;
	double m_DriveBackWaitTime;
	double m_WaitToStartSecondTubeDriveAngleStart;
	double m_SecondTubeDriveAngleStartWait;
	double m_SecondTubeWaitToBackwardsDriveStart;
	double m_SecondTubeWaitBackwardsDrive;
	double m_StartSecondTubeOpenClawWaitTime;
	double m_SecondTubeOpenClawWaitTime;
	double m_StartPowerDownWaitTime;
	double m_PowerDownWaitTime;
	
	
	
public:
	
	explicit AutoStateMachine(RobotModel* pRobot, DriveStateMachine* driveSM, ArmStateMachine* armSM, HumanDriveControl* pHumanDriveControl);
	
	virtual bool IsMachineDone() { return m_bAutoDone; }
	
	bool IsAutoDone() const { return m_bAutoDone; }
	
	virtual void UpdateState(double currTime_sec, double deltaTime_Sec);
	
	void UpdateDriveState(double currTime_sec, double deltaTime_Sec);
	void UpdateArmState(double currTime_sec, double deltaTime_Sec);
	void Reset();
	
	/*AutoStateMachine();
	virtual ~AutoStateMachine();*/
};

#endif /*AUTOSTATEMACHINE_H_*/
