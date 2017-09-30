#pragma once

#include "Shared/FiniteStateMachine.h"
#include "Shared/HumanDriveControl.h"

class DriveStateMachine : public virtual FiniteStateMachine
{
	
private:
	
	enum DriveState {
		//states
		kIdle, kReset, kInitialize, kAutoDriveStart, kTeleopDrive,
			kAutoDrive, kAutoDriveDone, kAutoPivotStart, kAutoPivot,
			kAutoPivotDone,
			
			kStateCount /* must always be the last state! */
	};
	
	// inputs
	HumanDriveControl* m_pHumanController;
	RobotModel*	m_pRobotModel; //input and output
	bool	m_bRequestTeleopDrive;
	bool	m_bRequestAutoDrive;
	bool	m_bRequestAutoPivot;
	double	m_RequestedDriveDistance;
	double  m_RequestedPivotAngle;
	double	m_CurrentGyroValue;
	double 	m_RequestedDriveSpeed;
	double 	distanceEncoderValues;
	double 	m_feetTraveled;
	double	m_angleCorrection;
	double  m_leftSpeedAdjust;
	double 	m_targetGyroValue;
	double  m_diffDriveError ;
	double  m_diffPivotError ;
	int count;
	
	
	//outputs
	bool	m_bDrivingDone;

	const char *GetStateName( uint32_t state );
	void SetNextState( uint32_t state );

public:
	
	void RequestAutoDrive(double distance, double speed, double angle);
	void RequestAutoPivot(double angle, double speed);
	void RequestTeleopDrive();
	
	bool IsAutoDriveDone() const { return m_bDrivingDone; }
	
	double DrivePID();
	double PivotPID();
	
	void UpdateState(double currTime_Sec, double deltaTime_sec);
	
	bool IsMachineDone(){return false;};
	
	explicit DriveStateMachine(RobotModel* pRobot, HumanDriveControl* controller);
	
	void Reset();

};
