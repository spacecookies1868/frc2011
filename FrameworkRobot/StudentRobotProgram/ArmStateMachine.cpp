#include "ArmStateMachine.h"

ArmStateMachine::ArmStateMachine(RobotModel* pRobot, 
		HumanDriveControl* m_pHumanDriveControl) : FiniteStateMachine(pRobot, kInitialize)

{
    	m_pRobotModel = pRobot ;
    	m_pHumanController = m_pHumanDriveControl;
    	m_bRequestHumanDrive = false;
    	m_stateVal = kInitialize ; 
    	
    	ArmPositionEncoderValues[kNoPosition] 		= 0; 
    	ArmPositionEncoderValues[kOutsideLow] 		= -332;
    	ArmPositionEncoderValues[kCenterLow] 		= -400;
   		ArmPositionEncoderValues[kOutsideMiddle] 	= -688;
   		ArmPositionEncoderValues[kCenterMiddle]		= -788;
  		ArmPositionEncoderValues[kOutsideHigh]		= -1024;
  		ArmPositionEncoderValues[kCenterHigh] 		= -1072;
  		ArmPositionEncoderValues[kGround] 			= -172;
  		
    	m_error = 1.0;
    	m_largeError= 40.0;
    	last_error = 0.0;
    	count = 0;
    	m_desiredEncoderValue = 0.0;
    	m_bHoldingActive = false;
    	m_bHangMacro = false ;
    	/**
    	m_bHoldingButtonPressed = false;
    	m_bHoldingButtonJustPressed = false;
    	m_bHoldingButtonOn = false;
    	**/
    	m_bArmAboveWanted = false;
    	m_StartTime = 0.0;
    	//printf("armstatemachine constructor reached");
}
	
double ArmStateMachine::ArmPID( ){

	static int count = 0;
	float PFac = 0.006;
	float DFac = 0.04;
	float NError = m_desiredEncoderValue  - m_currentEncoderValue ;
	float pterm = PFac * NError;
	
	/*
	float difErr = fabs(NError - last_error);
	// if (difErr > 10) {printf("Outsize DE term\n");}
	float sign = 1.0;
	if (pterm < 0.0){sign = -1.0;}
	float dterm = (sign * DFac *(difErr));
	*/
	
	float difErr = (NError - last_error);
	float dterm = (DFac *(difErr));
	
	double motorValue = pterm + dterm;
	if (motorValue > 0.0) {motorValue = min(motorValue, 0.70);}
	else {motorValue = max(motorValue, -0.70);}
	if (count++ % 50 == 0) {printf("Motor On: %f err: %f difErr: %f pt: %f dt: %f \n", motorValue, NError,difErr, pterm, dterm);}
	last_error = NError;
	//When value given to arm is -, arm driven up
	return 	motorValue;

}


void ArmStateMachine::UpdateState(double c /*currTime_Sec*/, double d/*deltaTime_Sec*/)
{
	
	//nextState =  m_stateVal;
	//printf("Arm State: %d\n", m_stateVal);
				
	m_desiredArmSpeed = m_pHumanController->getArmMotorDesiredSpeed();
	if (m_desiredArmSpeed > 0.1 || m_desiredArmSpeed < -0.1){

		m_bRequestHumanDrive = true;
	} else {
		m_bRequestHumanDrive = false;
	}	
	
	
	// Sets RequestArmPosition and the desiredEncoderValue if the driver is pressing a button.
	if (m_pHumanController->getDesiredArmPosition() > kNoPosition) {
		m_RequestArmPosition = m_pHumanController->getDesiredArmPosition();
		m_desiredEncoderValue = ArmPositionEncoderValues[m_RequestArmPosition];
	} 

	if(m_pHumanController->getDesiredJog()> kNoJog){
		m_RequestJog = m_pHumanController->getDesiredJog();
	}
	
	if(m_pHumanController->getHangMacro()== true && m_stateVal != kHangMacro ){
		printf("HangMacro enabled\n");
		m_bHangMacro = true;
	}

	
	if (count % 50 == 0) { printf("m_desiredEncoderValue: %f \n", m_desiredEncoderValue);}
	
	
	// Toggles holding on and off.
	/**
	m_bHoldingButtonPressed = m_pHumanController->getHoldingButton();
	if (m_bHoldingButtonPressed && !m_bHoldingButtonJustPressed) {
		if (m_bHoldingButtonOn) m_bHoldingButtonOn = false;
		else m_bHoldingButtonOn = true;
	}
	m_bHoldingButtonJustPressed = m_bHoldingButtonPressed;
	**/
	

	// Encoder Readings are negative - become more negative as arm goes up
	m_currentEncoderValue = m_pRobotModel->GetArmEncoderValue();
	if (count % 100 == 0) {
		printf("Arm encoder value: %f (JoyS):%f\n", m_currentEncoderValue, m_pHumanController->getArmMotorDesiredSpeed());
	}
	count++;
		
	switch (m_stateVal){
	case (kReset):
		nextState = kInitialize ;	
		m_pRobotModel->EnableArmEncoder();
		break;
		
	case (kInitialize) : 
		m_bRequestHumanDrive = false;
		m_RequestArmPosition = kNoPosition;	
		m_bHoldingActive = false;
		m_pRobotModel->EnableArmEncoder();
		nextState = kIdle;
		//printf("initialize has been reached \n");
		break;
		
	case (kIdle):		
		if (m_bRequestHumanDrive){
			nextState = kHuman;
			printf("to kHuman\n");
		}
		else if (m_RequestArmPosition > kNoPosition)
			// making sure there has been a button pressed
		{ 
			nextState = kSeekingPosition;
		}	
		else {
		nextState = kIdle;
		}
		break;
		
	case (kSeekingPosition):
		//printf("kSeekingPosition reached \n");
		
		
		if (m_desiredEncoderValue > m_currentEncoderValue) m_bArmAboveWanted = true; 	// arm is above wanted position
		else m_bArmAboveWanted = false;													// arm is below wanted position
			

		if (m_bRequestHumanDrive){
			nextState = kHuman;
			m_pRobotModel->SetArmMotorValue(0.0);
		}
		
		else if (m_currentEncoderValue < (m_desiredEncoderValue + m_largeError) && 
				m_currentEncoderValue > (m_desiredEncoderValue - m_largeError)){
			m_RequestArmPosition = kNoPosition;
			m_pRobotModel->SetArmMotorValue(0.0);
			last_error = m_desiredEncoderValue - m_currentEncoderValue;
			nextState = kHoldingPosition;
		}	
		else {
			if (m_bArmAboveWanted){ // if the arm is above wanted position
				// drive down with low speed
				m_pRobotModel->SetArmMotorValue(0.185);// TEST FOR RIGHT SPEED
				//positive value drives downwards 
				//printf("motor driving down \n");
			}
			else if(!m_bArmAboveWanted){//if arm is below wanted position
				//drive upwards with more speed (fighting gravity)
				m_pRobotModel->SetArmMotorValue(-0.60);// TEST FOR RIGHT SPEED
				//printf("motor driving up \n");
			}
			//nextState = kSeekingPosition;
			nextState = kHoldingPosition;
			
		}
		break;
		
	case (kHuman):
		//printf("In kHuman \n");
		
		m_bHoldingActive = false;
		float axis = m_pHumanController->getArmMotorDesiredSpeed() ;
		const float sign_x = (axis<0.0) ? -1.0 : 1.0;	
		// m_pRobotModel->SetArmMotorValue(axis); // linear
		m_pRobotModel->SetArmMotorValue(sign_x * (axis * axis)); // square func
	
#if ONOFFHOLDING	
		if (m_RequestArmPosition > kNoPosition) {
			nextState = kSeekingPosition;
		}
	/** else {
		if (m_bHoldingButtonOn && !m_bRequestHumanDrive){
			m_desiredEncoderValue = m_currentEncoderValue;
			nextState = kHoldingPosition;
		}
		**/
		else {
			nextState = kHuman;
		}
	// }		
#else
	if (!m_bHoldingButtonOn){
		if (m_RequestArmPosition > kNoPosition){
			nextState = kSeekingPosition;
		}
		else {
			nextState = kHuman;			
		}
	}
#endif 
	/*else {	
		// allows to hold from kHuman... to test different holding loops
		if (!m_bRequestHumanDrive this should be commented out:&& m_bHoldingButtonOn){
			m_desiredEncoderValue = m_currentEncoderValue;
			nextState = kHoldingPosition;
			//printf("to kHoldingPosition\n");
		}
		else {
			nextState = kHuman;
		}	
	}
	*/	
		break;
		
	case (kHoldingPosition):
		// printf("in kHoldingPosition: %f %f\n", m_currentEncoderValue, m_desiredEncoderValue);
			
		m_bHoldingActive = true;
		
		if (m_bHoldingActive){
			if (m_RequestJog == 1){
				m_desiredEncoderValue = m_desiredEncoderValue + 6;
				m_RequestJog = 0;
			}
			else if (m_RequestJog == 2){
				m_desiredEncoderValue = m_desiredEncoderValue - 6;
				m_RequestJog = 0;
			}
		
			double motorValue = ArmPID();
			m_pRobotModel->SetArmMotorValue(motorValue);
			//When value given to arm is -, arm driven up
			//SetArmMotorValue takes given value and multiples by 0.5
			//This way we give it .1 power for holding, everything needs testing...
		}
		else {
			//printf("Moton OFF\n");
			m_pRobotModel->SetArmMotorValue(0.0);
		}
	
	
		// Switching States
		if (m_bRequestHumanDrive){
			m_bHoldingActive = false;
			m_RequestArmPosition = kNoPosition;
			nextState = kHuman;
			//printf("to kHuman\n");
		}
		else if (m_RequestArmPosition > kNoPosition /*0*/)
			// making sure there has been a button pressed
		{ 
			m_bHoldingActive = false;
			nextState = kSeekingPosition;
		}
		else if (m_bHangMacro){
			m_RequestArmPosition = kNoPosition;
			nextState = kHangMacro;
			printf("to kHangMacros at : %f\n", c);
			m_HangStartTime = c;	// note the time
		}
		else {
			nextState = kHoldingPosition;
		}
		break;
		
	case (kHangMacro):
		
		// printf("In Hang Macro at : %f \n", c);
		m_pRobotModel->SetArmMotorValue(0.0);
		
		// Switching States
		if (m_bRequestHumanDrive){
			m_bHangMacro = false;
			m_RequestArmPosition = kNoPosition;
			nextState = kHuman;
		}
	
		if ( (c - m_HangStartTime)  > 0.75){		// more than .75 seconds
			nextState = kHuman;
			m_bHangMacro = false;
			m_pRobotModel->SetClawType(RobotModel::kClosed);	// despite it saying kClosed, this actually opens the claw.
			printf("Claw being opened from Hang macro\n");	
		}
		else {
			// printf("Continue in HangMacro\n");
			nextState = kHangMacro;
		}

			
		break;

	}
	m_stateVal = nextState;	
}

void ArmStateMachine::RequestArmPosition(uint32_t pos){ 
	m_RequestArmPosition = pos;
	m_desiredEncoderValue = ArmPositionEncoderValues[m_RequestArmPosition];
	}

void ArmStateMachine::Reset() {
	m_stateVal = kReset;
}

void ArmStateMachine::ReleaseHold() {
	m_RequestArmPosition = kNoPosition;
	m_bHoldingActive = false;
	m_stateVal = kHuman;
}

