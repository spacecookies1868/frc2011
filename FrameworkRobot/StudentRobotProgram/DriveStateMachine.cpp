

#include "DriveStateMachine.h"
#include "../SpaceCookieBot/SpaceCookieBot.h"

DriveStateMachine::DriveStateMachine(RobotModel* pRobot, 
		HumanDriveControl *controller) : FiniteStateMachine(pRobot, kInitialize)
{
	m_pHumanController = controller;
	m_pRobotModel = pRobot;
	count = 0;
	m_feetTraveled = 0;
	m_leftSpeedAdjust = 0;
	m_targetGyroValue = 0;
	m_CurrentGyroValue = 0;
	m_bRequestAutoDrive = false;
	m_bRequestAutoPivot = false;
}

double DriveStateMachine::DrivePID()
{
	static int count = 0;
	static float last_error = 0.0 ;
	float PFac = 0.4;
	float DFac = 11.0;
	float NError = m_RequestedDriveDistance  - m_feetTraveled ;
	
	// more than N feet - saturate
	// also means we start deceleration with N feet to go
	if(NError > 3.0) NError = 3.0;
	
	float pterm = PFac * NError;
	float difErr = (NError - last_error);
	if(difErr >= NError) difErr = 0.0 ; // account for first time
	if(difErr >= fabs(NError)) difErr = 0.0 ; // account for first time


	float dterm = DFac *(difErr) ;
	double motorValue = pterm + dterm;
	
	// saturation of the terms
	if (motorValue > 0.0) {motorValue = min(motorValue, 1.0);}
	else {motorValue = max(motorValue, -1.0);}
	
	//if (count++ % 10 == 0) {printf("Wheel Motor On: %f err: %f difErr: %f pt: %f dt: %f \n", motorValue, NError,difErr, pterm, dterm);}
	//if (count++ % 10 == 0) {printf("Wheel Motor On: %f err: %f difErr: %f pt: %f dt: %f \n", motorValue, NError,difErr, pterm, dterm);
	//printf("distance traveled = %f\n",m_feetTraveled);}
	last_error = NError;
	m_diffDriveError = difErr ;
	return 	motorValue;
}

double DriveStateMachine::PivotPID()
{
	static int count = 0;
	static float last_error = 0.0 ;
	float PFac = 0.15;
	float DFac = 3.0;
	float NError = m_CurrentGyroValue  - m_RequestedPivotAngle ;
	
	// more than N feet - saturate
	// also means we start deceleration with N feet to go
	if(NError > 10.0) NError = 10.0;
	if(NError < -10.0) NError = -10.0 ;
	
	float pterm = PFac * NError;
	
	float difErr = (NError - last_error);
	if(difErr >= NError) difErr = 0.0 ; // account for first time


	float dterm = DFac *(difErr) ;
	double motorValue = pterm + dterm;
	
	// saturation of the terms
	if (motorValue > 0.0) {motorValue = min(motorValue, 1.0);}
	else {motorValue = max(motorValue, -1.0);}
	
	if (count++ % 5 == 0) {
		printf("Wheel Pivot @ %f: motorVal: %f err: %f difErr: %f pt: %f dt: %f \n", m_CurrentGyroValue, motorValue, NError,difErr, pterm, dterm);
	}
	last_error = NError;
	m_diffPivotError = difErr ;

	return 	motorValue;
}

void DriveStateMachine::UpdateState(double currTime_Sec, double deltaTime_sec){
	
	  uint32_t nextState = m_stateVal;
	  double leftWheelEncoderValue, rightWheelEncoderValue;
	  double leftJoy, rightJoy;
	  double deadZone = 0.01;
	  
	//printf("\nmade it to the beginning of DriveStateMachine::UpdateState with state %ld\n", m_stateVal);
	
	switch (m_stateVal)
	{
	case (kIdle):
		leftJoy = m_pHumanController->getLeftMotorDesiredSpeed();
		rightJoy = m_pHumanController->getRightMotorDesiredSpeed();
		//printf("leftJoy is %f and rightJoy is %f\n", leftJoy, rightJoy);
		if( (leftJoy > deadZone || leftJoy < -deadZone)
		 || (rightJoy > deadZone || rightJoy < -deadZone) )
		{
			//printf( "Switching from kIdle to kTeleopDrive because leftJoy is %f and rightJoy is %f\n", leftJoy, rightJoy);
			nextState = kTeleopDrive; 		// if human wants the motors to be moving, go to teleop
		} else if (m_bRequestAutoDrive)
		{
			printf( "Switching from kIdle to kAutoDriveStart \n");
			nextState = kAutoDriveStart;	// if autonomous has requested a straight drive, go to the "start auto drive" state
		} else if (m_bRequestAutoPivot)
		{
			printf( "Switching from kIdle to kAutoPivotStart \n");
			nextState = kAutoPivotStart;	// if autonomous has requested a pivot turn, go to the "start pivot turn" state
		} else
		{
			nextState = kIdle; 				//otherwise, go to Idle
		}

		static int _i = 0;
		//if (_i++ % 1000 == 0) printf("Current encoder value: %f \n", m_pRobotModel->GetWheelEncoderValue(RobotModel::kRightWheel));
		
	
		//printf("Angle = %f\n",m_pRobotModel->GetGyroValue());
		
		m_bDrivingDone = false;
		break; 
	case (kReset):
		m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, 0.0); 
		m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, 0.0);
		m_pRobotModel->ResetWheelEncoders();
		m_pRobotModel->ClearGyro();
		nextState = kInitialize; //if robot is reset, stop motors, set encoders to 0, move on to initialization
		break;
	case (kInitialize):
		m_pRobotModel->EnableWheelEncoders();
		m_pRobotModel->ResetWheelEncoders();
		m_bRequestTeleopDrive = false;
		m_bRequestAutoDrive = false;
		m_bRequestAutoPivot = false;
		m_RequestedDriveDistance = 0.0;
		m_angleCorrection = 0;
		m_bDrivingDone = false;
		nextState = kIdle; //do not request teleop or autonomous, set requested drive distance to 0
		break;
	case (kAutoDriveStart):
		m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, 0.5);
		m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, 0.5);  //trim for straight ahead paw
		m_pRobotModel->ResetWheelEncoders();
		
		nextState = kAutoDrive; //move robot forward at half speed, reset wheel encoders
		break;
	case (kAutoDrive):
		/*** Assumming target angle 0, one line P loop *** paw */
		// Directional correction
		m_leftSpeedAdjust = (m_pRobotModel->GetGyroValue() - m_targetGyroValue) * (-0.055);
	
		rightWheelEncoderValue = m_pRobotModel->GetWheelEncoderDistance(RobotModel::kRightWheel);
		leftWheelEncoderValue = m_pRobotModel->GetWheelEncoderDistance(RobotModel::kLeftWheel);
		m_feetTraveled = fabs((rightWheelEncoderValue + leftWheelEncoderValue)/2.0);	// PID loop works more easily
																						// when we work with only positive distances
		// Calculate PID for drive
		double currSpeedFactor ;
		currSpeedFactor = DrivePID();
		// PID override
		// currSpeedFactor = 1.0 ;

		// Adjust speed with PID factor
		double currSpeed = m_RequestedDriveSpeed * currSpeedFactor ;
		
		m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, (currSpeed - m_leftSpeedAdjust));
		m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, (currSpeed + m_leftSpeedAdjust));

		
		/**
		if (count % 5 == 0){
			printf("rightencoder = %f,\nleft encoder = %f\n",
			 		rightWheelEncoderValue,leftWheelEncoderValue);
			printf("Have traveled: %f ft @ %f speed\n", m_feetTraveled, currSpeed );
			// printf("speed adjust = %f\n",m_leftSpeedAdjust);
			// printf("Angle = %f\n",m_pRobotModel->GetGyroValue());
		}
		**/
		count++;
		
		if ( (fabs(m_feetTraveled) >= fabs(m_RequestedDriveDistance))  && 
				(fabs(currSpeedFactor < 0.1)) && m_diffDriveError == 0.0 ){	// PID loop has settled
			printf("Drive Done by distance at: %f\n",m_feetTraveled );
			nextState = kAutoDriveDone;//if you've traveled the requested distance, go to end of auto
		}else if ( (fabs(m_RequestedDriveDistance - m_feetTraveled) < 0.5 ) && (m_diffDriveError == 0.0)){
			printf("Drive Done by DiffError settled\n");
			nextState = kAutoDriveDone;//if you've NOT traveled the requested distance, but don't move
		}else {
			nextState = kAutoDrive;
		}
		break;
	case (kAutoDriveDone):
		m_bRequestAutoDrive = false;
		m_bDrivingDone = true;
				
		m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, 0); //stop here - paw
		m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, 0);

		printf("Stopped driving at encoder distance: %f", m_pRobotModel->GetWheelEncoderDistance(RobotModel::kRightWheel));
		
		nextState = kIdle;//once done with autonomous drive, go to idle
		break;
		
	case (kAutoPivotStart):
		// stuff you want to do once at the beginning of a pivot turn
		nextState = kAutoPivot;
		break;
	
	case (kAutoPivot):
		double currPIDvalue ;
		m_CurrentGyroValue = m_pRobotModel->GetGyroValue();			// clockwise = positive
		currPIDvalue = PivotPID();		//  comment out for no PID;
		double currPSpeed = m_RequestedDriveSpeed * currPIDvalue ;	// comment out for no PID

		m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, currPSpeed);
		m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, -currPSpeed);	// note the negative sign

		/**
		if (m_CurrentGyroValue > m_RequestedPivotAngle) {
			// need to turn ccw: left motor back, right motor forward
			m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, m_RequestedDriveSpeed);
			m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, -m_RequestedDriveSpeed);	// note the negative sign
		}
		else if (m_CurrentGyroValue < m_RequestedPivotAngle) {
			// need to turn cw: right motor back, left motor forward
		 	m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, -m_RequestedDriveSpeed);	// note the negative sign
		 	m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, m_RequestedDriveSpeed);	
		}
		**/
		
		if (m_CurrentGyroValue > m_RequestedPivotAngle -0.5 && m_CurrentGyroValue < m_RequestedPivotAngle +0.5){
			nextState = kAutoPivotDone;
			printf("AutoPivot Done at %f\n", m_CurrentGyroValue);
		}else if ( (fabs(m_CurrentGyroValue - m_RequestedPivotAngle) < 3.0 ) && (m_diffPivotError == 0.0)){
			nextState = kAutoPivotDone; //if you've NOT turned the requested angle, but dont move
			printf("AutoPivot Stalled - Done at %f\n", m_CurrentGyroValue);
		}
		

		break;
	
	case (kAutoPivotDone):
		m_bRequestAutoPivot = false;
		m_bDrivingDone = true;
		m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, 0);	//stop here - paw
		m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, 0);
		nextState = kIdle;												//once done with autonomous pivot, go to idle
		break;
		
	case (kTeleopDrive):
		//printf("in teleop drive :)");
		
		// printf("Angle: %f\n", m_pRobotModel->GetGyroValue());
		
		double ri =  m_pHumanController->getRightMotorDesiredSpeed();
		double le = m_pHumanController->getLeftMotorDesiredSpeed();
		
		//adjustments for mom-linearity already done in JoystickController module
		m_pRobotModel->SetWheelMotorValue(RobotModel::kRightWheel, ri);
		m_pRobotModel->SetWheelMotorValue(RobotModel::kLeftWheel, le);
		//printf("%f %f\n", le, ri);
						
		nextState = kTeleopDrive;
		//printf("angle = %f\r",m_pRobotModel->GetGyroValue());
	break;
	
	
	}
	
	SetNextState( nextState );
	
	//printf("\nmade it to the end of DriveStateMachine::UpdateState with state %ld\n", m_stateVal);
	
}

void DriveStateMachine::RequestTeleopDrive(){
	
	m_bRequestTeleopDrive = true;
	
	
}

void DriveStateMachine::RequestAutoDrive(double distanceFeet, double speed, double Angle){
	printf("=========> RequestAutoDrive: distance: %f speed: %f angle: %f\n", distanceFeet, speed, Angle);
	
	m_bRequestAutoDrive = true;
	m_bDrivingDone = false;
	m_feetTraveled = 0.0 ;
	m_RequestedDriveDistance = distanceFeet;
	m_RequestedDriveSpeed = speed;
	m_targetGyroValue = Angle ;
	m_pRobotModel->ResetWheelEncoders();	
}

void DriveStateMachine::RequestAutoPivot(double angle, double speed) {
	m_bRequestAutoPivot = true;
	m_bDrivingDone = false;
	m_RequestedPivotAngle = angle;
	m_RequestedDriveSpeed = speed;
}

void DriveStateMachine::Reset() {
	SetNextState( kReset );
	m_bRequestTeleopDrive = false;
	m_bRequestAutoDrive = false;
	m_bRequestAutoPivot = false;
	m_RequestedDriveDistance = 0.0;

}

const char *DriveStateMachine::GetStateName( uint32_t state ) {
	static char *stateNames[] = { "kIdle", "kReset", "kInitialize", "kAutoDriveState", "kTeleopDrive", 
			"kAutoDrive", "kAutoDriveDone", "kAutoPivotStart", "kAutoPivot", "kAutoPivotDone" };

	if ( state <= kAutoPivotDone ) {
		return stateNames[ state ];
	} else {
		return "UNKNOWN";
	}
}


void DriveStateMachine::SetNextState( uint32_t nextState ) {
	if ( m_stateVal != nextState ) {
		printf( "DriveStateMachine state change: %s -> %s\n", GetStateName( m_stateVal ), GetStateName( nextState ));
		m_stateVal = nextState;
	}
}
