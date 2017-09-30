#include "AutoStateMachine.h"
#include "JoystickDriveController.h"

#define DO_PERIODIC( period_count, stmt ) do { static int __count__ = 0; if (( __count__++ % ( period_count )) == 0 ) { stmt; }} while(0)

#define FORW_DIST 14.50

AutoStateMachine::AutoStateMachine(RobotModel* pRobot, DriveStateMachine* driveSM, ArmStateMachine* armSM, HumanDriveControl* pHumanDriveControl) : FiniteStateMachine(pRobot, 0)
{
	m_pRobotModel = pRobot;
	m_pHumanDriveControl = pHumanDriveControl;
	m_DriveStateMachine = driveSM;
	m_ArmStateMachine = armSM;
	
	m_driveStateVal = kDriveInitialize;
	m_armStateVal = kArmInitialize;
	
	m_bGoForTwoTubes = false ;
	
	m_ForwardDriveDistance = FORW_DIST;
	m_ForwardDriveSpeed = 0.85;
	
	m_BackwardDriveSpeed = -0.85;
	m_BackwardDriveDistance = m_ForwardDriveDistance ;	//was 8 needs to be 1.25 more  than forward to end up where it will pivot
	
	m_StartOpenClawWaitTime = 0.0;
	m_OpenClawWaitTime = 1.0;			//1.0 for both wait times worked
	
	m_StartDriveBackWaitTime = 0.0;
	m_DriveBackWaitTime = 1.5;
	
	m_WaitToStartSecondTubeDriveAngleStart = 0;
	m_SecondTubeDriveAngleStartWait = 0.5;
	
	m_SecondTubeWaitToBackwardsDriveStart = 0;
	m_SecondTubeWaitBackwardsDrive = 0.75;
	
	m_StartSecondTubeOpenClawWaitTime = 0;
	m_SecondTubeOpenClawWaitTime = 0.75;
	
	m_StartPowerDownWaitTime = 0;
	m_PowerDownWaitTime = 1;
	
	
	m_bReadyForLiftArm = false;
		 
}

void AutoStateMachine::UpdateState(double c, double d)
{
	/*
	 * There are actually 2 separate state machines contained within AutoStateMachine.
	 * One controls the drive, and one controls the arm. UpdateState updates the state
	 * of these two machines.
	 */
	
	AutoStateMachine::UpdateDriveState(c,d);
	AutoStateMachine::UpdateArmState(c,d);
}

void AutoStateMachine::UpdateDriveState(double c, double d)
{
	switch(m_driveStateVal) {
	
	case(kDriveReset):
		//reset
		
		m_bReadyForLiftArm = false;
		
		m_nextDriveState = kDriveInitialize;
		break;
		
	case (kDriveInitialize):
		//initialize
		if ( 2 == m_pHumanDriveControl->getDesiredTubes()){
			m_bGoForTwoTubes = true;
		}else{
			m_bGoForTwoTubes = false;
		}
		
		if (m_bGoForTwoTubes){
			m_ArmPosition = 1;
			printf("Going for TWO TUBES\n");
		}else{
			printf("Going for ONE TUBE\n");
			m_BackwardDriveDistance = FORW_DIST - 1.0 ; // for safety so we dont cross line by accident
			m_ArmPosition = m_pHumanDriveControl->getDesiredAutonomous();
		}
		if(m_ArmPosition == 2){
			m_ForwardDriveDistance = FORW_DIST - 0.5; // adjust a bit for arm angle
		}
		printf("AutoGoal: %d\n", (unsigned int)m_ArmPosition);

		printf("Initializing auto drive.\n");
		m_nextDriveState = kStartFirstTubeDriveForward;
		break;
		
	case (kStartFirstTubeDriveForward):
		printf("Starting  drive forward.\n");
		m_DriveStateMachine->RequestAutoDrive(m_ForwardDriveDistance,m_ForwardDriveSpeed, 0.0);
		m_armStateVal = kStartLiftArm;
		
		m_nextDriveState = kWaitForFirstTubeDriveForward;
		break;
		
	case (kWaitForFirstTubeDriveForward):
		//DO_PERIODIC( 1000, printf( "in kWaitForDriveForward\n") );
		if (m_DriveStateMachine->IsAutoDriveDone()) m_nextDriveState = kDoneFirstTubeDriveForward; 	// we will add more steps after this, but for now,
		else m_nextDriveState = kWaitForFirstTubeDriveForward;										// just go to idle.
		break;
		
	case (kDoneFirstTubeDriveForward):
		printf("Done driving forward.\n");
		m_nextArmState = kReleaseArm;
		m_StartDriveBackWaitTime = c;
		m_nextDriveState = kWaitToFirstTubeDriveBackward;
		//m_nextDriveState = kDriveIdle;
		break;
	
	case (kWaitToFirstTubeDriveBackward):
		//DO_PERIODIC( 100, printf("Waiting to drive backwards.\n"));
		if (c - m_StartDriveBackWaitTime > m_DriveBackWaitTime) m_nextDriveState = kStartFirstTubeDriveBackward;
		else m_nextDriveState = kWaitToFirstTubeDriveBackward;
		
		break;
		
	case (kStartFirstTubeDriveBackward):
		printf("Starting drive backwards.\n");
		m_DriveStateMachine->RequestAutoDrive(m_BackwardDriveDistance, m_BackwardDriveSpeed, 0.0);
		m_nextDriveState = kWaitForFirstTubeDriveBackward;
		break;
		
	case (kWaitForFirstTubeDriveBackward):
		//DO_PERIODIC( 100, printf("Waiting for backwards drive to complete.\n"));
		if (m_DriveStateMachine->IsAutoDriveDone()) m_nextDriveState = kDoneFirstTubeDriveBackward;
		break;
		
	case (kDoneFirstTubeDriveBackward):
		printf("Done driving backwards.\n");
		if(m_bGoForTwoTubes){
			m_nextDriveState = kStartSecondTubePivotOne;
			m_pRobotModel->SetGearType(RobotModel::kLowGear);
		}else{
			m_nextDriveState = kDriveIdle;
		}
			break;
		
	case (kStartSecondTubePivotOne):
		printf("start 2nd tube pivot\n");
		m_DriveStateMachine->RequestAutoPivot(-53 , 1.0);	//try 45 deg pivot to left 50 at 0.75 - 60 at 0.6 speed
		m_nextDriveState = kWaitForAutoPivotOneDone;
		break;
		
	case (kWaitForAutoPivotOneDone):
		if (m_DriveStateMachine->IsAutoDriveDone()) {
			m_nextDriveState = kSecondTubeAutoPivotOneDone;
			m_nextArmState = kDonePowerDownArm;
		}
		break;
	case (kSecondTubeAutoPivotOneDone):
		m_WaitToStartSecondTubeDriveAngleStart = c;
		m_nextDriveState = kWaitToSecondTubeDriveAngle;
		m_pRobotModel->SetGearType(RobotModel::kHighGear);
		break;
	case (kWaitToSecondTubeDriveAngle):
		if (c - m_WaitToStartSecondTubeDriveAngleStart > m_SecondTubeDriveAngleStartWait){
			m_nextDriveState = kStartSecondTubeDriveAngle;
		}else{
			m_nextDriveState = kWaitToSecondTubeDriveAngle;
		}
	case (kStartSecondTubeDriveAngle):
		printf("start 2nd tube drive forward\n");
		m_DriveStateMachine->RequestAutoDrive(1.8, 1.00, -53);	// match angle in SecondTubePivot
		m_nextDriveState = kWaitForSecondTubeDriveAngle;
		break;
	case (kWaitForSecondTubeDriveAngle):
		if (m_DriveStateMachine->IsAutoDriveDone()) {
			printf("start 2nd tube drive forward done\n");
			m_nextArmState = kSecondTubeCloseClaw;	// moved from SeconTubePivtoDone
			m_nextDriveState = kStartSecondTubePivotTwo;
			m_pRobotModel->SetGearType(RobotModel::kLowGear);
		}
		break;
	case (kStartSecondTubePivotTwo):
		printf("start 2nd tube 2nd pivot back\n");
		m_DriveStateMachine->RequestAutoPivot(-0.5,1.00); //try 45 deg pivot right (hopefully) back to straight
		m_nextDriveState = kWaitForAutoPivotTwoDone;
		break;
	case (kWaitForAutoPivotTwoDone):
		if(m_DriveStateMachine->IsAutoDriveDone()) {
			printf("start 2nd tube pivot back done\n");
			m_nextDriveState = kStartSecondTubeDriveStraight;
			m_pRobotModel->SetGearType(RobotModel::kHighGear);
		}
		break;
	case (kStartSecondTubeDriveStraight):
		// match angle below with angle in SecondTubePivotTwo
		m_DriveStateMachine->RequestAutoDrive((m_ForwardDriveDistance - 3.0),m_ForwardDriveSpeed, -0.5);		//13.5 - 3.5 (diag of a 2.5' side isoc. rt. angl triangle
		printf("\nStarting second tube drive straight");
		m_nextDriveState = kWaitForSecondTubeDriveStraight;
		m_ArmPosition = 2;
		m_nextArmState = kSecondTubeStartLiftArm;
		break;
	case (kWaitForSecondTubeDriveStraight):
		if(m_DriveStateMachine->IsAutoDriveDone())
			m_nextDriveState = kSecondTubeDriveStraightDone;
		break;
	case (kSecondTubeDriveStraightDone):
		m_nextArmState = kSecondTubeReleaseArm;
		m_SecondTubeWaitToBackwardsDriveStart = c;
		m_nextDriveState = kWaitToSecondTubeBackwardsDrive;
		break;
	case (kWaitToSecondTubeBackwardsDrive):
		if (c - m_SecondTubeWaitToBackwardsDriveStart > m_SecondTubeWaitBackwardsDrive){
			m_nextDriveState = kStartSecondTubeBackwardsDrive;
		}else{
			m_nextDriveState = kWaitToSecondTubeBackwardsDrive;
		}
		break;
	case (kStartSecondTubeBackwardsDrive):
		m_DriveStateMachine->RequestAutoDrive(7,m_BackwardDriveSpeed, 0.0);
		m_nextDriveState = kWaitForSecondTubeBackwardsDriveDone;
		break;
	case (kWaitForSecondTubeBackwardsDriveDone):
		if(m_DriveStateMachine->IsAutoDriveDone())
			m_nextDriveState = kDriveIdle;
		break;
		
		
	case (kStartDrive):
		
		break;
		
	case (kWaitForDrive):
		
		break;
	
	case (kDoneDrive):
		
		break;
	
		
		
		
		
		
		
	case (kDriveIdle):
		// do nothing
		static int i = 0;
		//if ( i++ % 100 == 0 ) printf( "in kDriveIdle\n");
		m_nextDriveState = kDriveIdle;
		break;
	default:
		// do nothing
		m_nextDriveState = kDriveIdle;
	
	}
	
	if ( m_driveStateVal != m_nextDriveState ) {
		printf( "Changing auto drive state from %d to %d\n", (int)m_driveStateVal, (int)m_nextDriveState );
	}
	m_driveStateVal = m_nextDriveState;
}

void AutoStateMachine::UpdateArmState(double c, double d)
{
	switch(m_armStateVal) {
	case(kArmReset):
		//reset
		m_nextArmState = kArmInitialize;
		break;
		
	case(kArmInitialize):
		//initialize
		printf("Initializing auto arm.\n");
		m_pRobotModel->SetRetractType(RobotModel::kRetract);
		m_pRobotModel->SetClawType(RobotModel::kOpen);	// despite it saying kOpen, this actually closes the claw.
		m_nextArmState = kArmIdle;				// we will add actual arm steps, but for now, just go to idle.
		
		break;
		
	case(kStartLiftArm):
		printf("Starting to lift arm.\n");
		m_ArmStateMachine->RequestArmPosition(m_ArmPosition);
		m_nextArmState = kWaitForLiftArm;
		break;
		
	case(kWaitForLiftArm):
		//DO_PERIODIC( 100, printf( "Waiting for arm to reach position.\n"));
		
		if (m_ArmStateMachine->IsHolding()) m_nextArmState = kDoneLiftArm;
	
		break;
	
	case(kDoneLiftArm):
		printf("Done lifting arm.\n");
		m_nextArmState = kExtend;
		break;
		
	case(kExtend):
		printf("Extending arm.\n");
		m_pRobotModel->SetRetractType(RobotModel::kExtend);
		m_nextArmState = kArmIdle;
		break;
	
	case(kReleaseArm):
		printf("Lowering arm.\n");
		m_ArmStateMachine->ReleaseHold();
		m_nextArmState = kWaitToOpenClaw;
		m_StartOpenClawWaitTime = c;
		break;
	
	case (kWaitToOpenClaw):
		DO_PERIODIC(100, printf("Waiting to open claw.\n"));
		if (c - m_StartOpenClawWaitTime > m_OpenClawWaitTime) m_nextArmState = kOpenClaw;
		else m_nextArmState = kWaitToOpenClaw;
		break;
		
	case (kOpenClaw):
		printf("Opening claw.\n");
		m_pRobotModel->SetClawType(RobotModel::kClosed);	// despite it saying kClosed, this actually opens the claw.
		if(m_bGoForTwoTubes){
			m_StartPowerDownWaitTime = c;
			m_nextArmState = kWaitToPowerDownArm;
		}else{
			m_nextArmState = kArmIdle;
		}
		break;
	
	case (kWaitToPowerDownArm):
		if(c - m_StartPowerDownWaitTime > m_PowerDownWaitTime)
			m_nextArmState = kStartPowerDownArm;
		else 
			m_nextArmState = kWaitToPowerDownArm;
		break;
		
	case(kStartPowerDownArm):
			printf("Starting to lift arm.\n");
			m_ArmStateMachine->RequestArmPosition(7);
			m_nextArmState = kWaitForPowerDownArm;
			break;
			
		case(kWaitForPowerDownArm):
			//DO_PERIODIC( 100, printf( "Waiting for arm to reach position.\n"));
			
			if (m_ArmStateMachine->IsHolding()) m_nextArmState = kArmIdle;
		
			break;
		
		case(kDonePowerDownArm):
			m_ArmStateMachine->ReleaseHold();
			//printf("Done lifting arm.\n");
			m_nextArmState = kArmIdle;
			break;
	case (kSecondTubeCloseClaw):
		m_pRobotModel->SetClawType(RobotModel::kOpen); //like above comment, this is backwards but actually closes the claw
		m_nextArmState = kArmIdle;
		break;
		
	case(kSecondTubeStartLiftArm):
			printf("Starting to lift arm.\n");
			m_ArmStateMachine->RequestArmPosition(m_ArmPosition);
			m_nextArmState = kSecondTubeWaitForLiftArm;
			break;
			
		case(kSecondTubeWaitForLiftArm):
			
			if (m_ArmStateMachine->IsHolding()) m_nextArmState = kSecondTubeDoneLiftArm;
			break;
		
		case(kSecondTubeDoneLiftArm):
			printf("Done lifting arm.\n");
			m_nextArmState = kArmIdle;
			break;
			
		case(kSecondTubeReleaseArm):
			printf("Lowering arm.\n");
			m_ArmStateMachine->ReleaseHold();
			m_nextArmState = kSecondTubeWaitToOpenClaw;
			m_StartSecondTubeOpenClawWaitTime = c;
			break;
		
		case (kSecondTubeWaitToOpenClaw):
			//DO_PERIODIC(100, printf("Waiting to open claw.\n"));
			if (c - m_StartSecondTubeOpenClawWaitTime > m_SecondTubeOpenClawWaitTime) m_nextArmState = kSecondTubeOpenClaw;
			else m_nextArmState = kSecondTubeWaitToOpenClaw;
			break;
			
		case (kSecondTubeOpenClaw):
			printf("Opening claw.\n");
			m_pRobotModel->SetClawType(RobotModel::kClosed);	// despite it saying kClosed, this actually opens the claw.
				m_nextArmState = kArmIdle;
			break;
			
		
	case(kArmIdle):
		static int iv = 0;
		//if ( iv++ % 100 == 0 ) printf( "in kArmIdle\n");
		
		/*if (m_bReadyForLiftArm) {
			m_nextArmState = kStartLiftArm;
			m_bReadyForLiftArm = false;
		}
		else m_nextArmState = kArmIdle;*/
		break;
	
	default:
		// do nothing
		m_nextArmState = kArmIdle;
		
	}
	if ( m_armStateVal != m_nextArmState ) {
		// printf( "Changing auto arm state from %d to %d\n", (int)m_armStateVal, (int)m_nextArmState );
	}
	m_armStateVal = m_nextArmState;
	
}

void AutoStateMachine::Reset()
{
	m_driveStateVal = kDriveReset;
	m_armStateVal = kArmReset;
}

/*AutoStateMachine::AutoStateMachine()
{
}

AutoStateMachine::~AutoStateMachine()
{
}
*/
