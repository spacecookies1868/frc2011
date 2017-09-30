#include "JoystickDriveController.h"
#include "Shared/NumericalVector.hpp"
#include "AcceleratorArcade.h"
#include "ArmStateMachine.h"
#include <math.h>

void JoystickDriveController::update()
{
	//printf("");
	bool gearTarget = getButton(RTrig); 

	if(m_gearActive && !gearTarget)
	{
		if(m_gearType == RobotModel::kHighGear)
			m_gearType = RobotModel::kLowGear;
		else
			m_gearType = RobotModel::kHighGear;
	}

	m_gearActive = gearTarget;
	
	bool clawTarget = getButton(ATrig);
	
	if (m_clawActive && !clawTarget){
		if(m_clawType == RobotModel::kOpen){
			printf("set claw open\n");
			m_clawType = RobotModel::kClosed;
		}else{
			printf("set claw closed\n");
			m_clawType = RobotModel::kOpen;
		}
		}
	m_clawActive = clawTarget;
	
	bool retractTarget = getButton(ATwo);
	
	if (m_retractActive && !retractTarget){
		if(m_retractType == RobotModel::kRetract){
			//printf("set arm retracted\n");
			m_retractType = RobotModel::kExtend;
		}else{
			//printf("set arm extended\n");
			m_retractType = RobotModel::kRetract;
		}
		}
	
	m_retractActive = retractTarget;
	
	bool deployTarget = getButton(LThree);
	
	if(deployTarget){
			m_deployType = RobotModel::kUndeploy;
		}else{
			m_deployType = RobotModel::kDeploy;
		}
	
	bool lowerTarget = getButton(RFour);
	if (m_lowerActive && !lowerTarget){
		if(m_lowerType == RobotModel::kRaise) {
			m_lowerType = RobotModel::kLower;
			printf("Deployment Raised\n");
		}else {
			printf("Deployment Lowered\n");
			m_lowerType = RobotModel::kRaise;
		}
		}
	m_lowerActive = lowerTarget ;
	


	// m_desiredPosition is defined with values correspoinding to the arm positions
	// kNoPosition through kOutsideLow
	m_desiredPosition = 0;
	if (getButton(POne)){
		m_desiredPosition = 1;
	}
	if(getButton(PTwo)){
		m_desiredPosition = 2;
	}
	if(getButton(PThree)){
		m_desiredPosition = 3;
	}
	if(getButton(PFour)){
		m_desiredPosition = 4;
	}
	if(getButton(PFive)){
		m_desiredPosition = 5;
	}
	if(getButton(PSix)){
		m_desiredPosition = 6;
	}
	if(getButton(AThree)){
		m_desiredPosition = 7;
	}

	m_bHangMacro = false;
	if (getButton(AFour)){
		m_bHangMacro = true;
	}
	else {
		m_bHangMacro = false;
	}

	m_desiredJog = 0;
	if(getButton(PSeven)){
		m_desiredJog = 2;
	}
	if(getButton(PEight)){
		m_desiredJog = 1;
	}
	
	if(getButton(PEleven)){
		//printf("Button P11 True\n");
		m_desiredAutonomous = 1;
	}else {
		//printf("Button P11 False\n");
		m_desiredAutonomous = 2;
	}
	
	if(getButton(PTwelve)){
		//printf("Button P12 True\n");
		m_desiredTubes = 1;
	}else{
		//printf("Button P12 False\n");
		m_desiredTubes = 2;
	}
	

	// Switchable Tank or Arcade Drive
	
	bool bQuickTurn = getButton(LTrig);
	NumericalVector<2> driveVec(m_arcade.update(-getAxis(LStickY),getAxis(RStickX),m_gearType, bQuickTurn));
	
	//need to verify polarity of drives
	
	//Use Right stick Z to determine Tank or Arcade Drive
	if(getAxis(RStickZ)< 0){
		//Arade Drive - from m_arcade.update()
		m_motorLeft = driveVec[0];
		m_motorRight = driveVec[1];
	}
	/*else {
		if( bQuickTurn ){ //if quickturn
			
			float axis = getAxis(LStickX);
			const float sign_x = (axis<0.0) ? -1.0 : 1.0;
			
			m_turnVelocity = sign_x * sqrt(fabs(axis)) ;
			// m_turnVelocity = sign_x*fabs(axis)*fabs(axis);	// originally 1.7
			m_motorLeft = m_turnVelocity;
			m_motorRight = -m_turnVelocity;
		}*/
		else {
			// Non quickturn
			double ri = -getAxis(RStickY);
			double le = -getAxis(LStickY);
			if (ri > 0){
				ri = sqrt(ri);
			}
			else {
				ri = -sqrt(fabs(ri));
			}
			if (le > 0){
				le = sqrt(le);
			}
			else {
				le = -sqrt(fabs(le));
			}
			m_motorLeft = le;
			m_motorRight = ri;
		}
	//}
	if( bQuickTurn ){ //if quickturn
		float axis = getAxis(LStickX);
		const float sign_x = (axis<0.0) ? -1.0 : 1.0;
		
		m_turnVelocity = sign_x * sqrt(fabs(axis)) ;
		// m_turnVelocity = sign_x*fabs(axis)*fabs(axis);	// originally 1.7
		m_motorLeft = m_turnVelocity;
		m_motorRight = -m_turnVelocity;
	}
			
	m_motorArm = -getAxis(AStickY);

	
	if (count%100==0) {
	printf("LeftStickX: %f\nLeftStickY: %f\nLeftStickz: %f\nRightStickX: %f\nRightStickY: %f\nArmStickX: %f\nArmStickY:%f\n",
			getAxis(LStickX),
			getAxis(LStickY),
			getAxis(LStickZ),
			getAxis(RStickX),
			getAxis(RStickY),
			getAxis(AStickX),
			getAxis(AStickY));
	}
	count++;
		
	
}

JoystickDriveController::JoystickDriveController()
: m_motorLeft(0)
, m_motorRight(0)
, m_motorArm(0)
, m_gearType(RobotModel::kLowGear)
, m_clawType(RobotModel::kClosed)
, m_retractType(RobotModel::kRetract)
, m_gearActive(true)
, m_clawActive(true)
, m_retractActive(true)
, m_deployActive(true)
, m_lowerActive(true)
, m_bHangMacro(false)
, m_acceleration(0.0)
, m_desiredPosition(0)
, m_desiredJog(0)
, m_desiredAutonomous(1)
, m_desiredTubes(1)
, m_arcade(0.06)
{
	count = 0;
	m_desiredPosition = ArmStateMachine::kNoPosition;
	m_desiredJog = ArmStateMachine::kNoJog;
}

RobotModel::GearSet JoystickDriveController::getDesiredGear() const{
	return m_gearType;
}

RobotModel::ClawSet JoystickDriveController::getDesiredClaw() const{
	return m_clawType;
}

RobotModel::RetractSet JoystickDriveController::getDesiredRetract() const{
	return m_retractType;
}

RobotModel::DeploySet JoystickDriveController::getDesiredDeploy() const{
	return m_deployType;
}

RobotModel::LowerSet JoystickDriveController::getDesiredLower() const{
	return m_lowerType;
}

JoystickDriveController::~JoystickDriveController()
{
}
