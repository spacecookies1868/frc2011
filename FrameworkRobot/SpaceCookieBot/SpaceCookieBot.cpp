#include "Shared/CommonIncludes.h"
#include "SpaceCookieBot.h"
#include "WPILib.h"
#include "LinearizedMotorResponse.h"
#include "RobotPorts2011.h"

#define PRACTICE_BOT_ENCODERS 0

SpaceCookieBot::SpaceCookieBot()
:m_robotLength(37 * 2.54 /100)
,m_robotWidth(27 * 2.54 / 100)
,m_rotationTensor(.003)
,m_wheelRadius((3.863/2.0) * 2.54 / 100.0)
,m_lowGear((45.0/12.0) * (64.0/14.0)) //17.14
,m_highGear(45.0/12.0)
,m_maxMotorDrive(60)
,m_currentGear(RobotModel::kLowGear)
{
	m_v1 = new Victor(LEFT_MOTOR_A_PWM_PORT);
	m_v2 = new Victor(LEFT_MOTOR_B_PWM_PORT);
	m_v7 = new Victor(RIGHT_MOTOR_A_PWM_PORT);
	m_v8 = new Victor(RIGHT_MOTOR_B_PWM_PORT);
	
	m_v3 = new Victor(ARM_MOTOR_A_PWM_PORT);		// two motors for the arm,
	m_v4 = new Victor(ARM_MOTOR_B_PWM_PORT);		// driven in the same direction. Brake jumper is set.
	
	m_WheelEncoder[kLeftWheel] = 
		new Encoder(LEFT_WHEEL_ENCODER_A_PWM_PORT, LEFT_WHEEL_ENCODER_B_PWM_PORT, true);
	m_WheelEncoder[kRightWheel] = 
		new Encoder(RIGHT_WHEEL_ENCODER_A_PWM_PORT, RIGHT_WHEEL_ENCODER_B_PWM_PORT, true);
	m_ArmEncoder = 
		new Encoder(ARM_ENCODER_A_PWM_PORT, ARM_ENCODER_B_PWM_PORT, true);
	
	m_gyro = new Gyro(GYRO_PORT);
	m_gyro->Reset();
	m_gyro->SetSensitivity(.007);	// used to be .0033, last year used .007
	for (int i=0; i < RobotModel::kNumWheels; i++){
		m_WheelEncoderEnabled[i] = false;
	}
	m_ArmEncoderEnabled = false;
	
	//Compressor and Solenoid Initialization
	m_compressor = new Compressor(COMPRESSOR_PRESSURE_SWITCH_CHAN, COMPRESSOR_RELAY_CHAN);

	m_GearShiftSolenoid = new Solenoid(GEAR_SHIFT_SOLENOID_CHAN);
	m_ClawOpenSolenoid = new Solenoid(CLAW_OPEN_SOLENOID_CHAN);
	m_ClawClosedSolenoid = new Solenoid(CLAW_CLOSED_SOLENOID_CHAN);
	m_ArmExtendedSolenoid = new Solenoid(ARM_EXTENDED_SOLENOID_CHAN);
	m_ArmRetractedSolenoid = new Solenoid(ARM_RETRACTED_SOLENOID_CHAN);
	m_MinibotDeploySolenoid = new Solenoid(MINIBOT_DEPLOY_SOLENOID_CHAN);
	m_MinibotLowerSolenoid = new Solenoid(MINIBOT_LOWER_SOLENOID_CHAN);
	m_MinibotRaiseSolenoid = new Solenoid(MINIBOT_RAISE_SOLENOID_CHAN);
	
	m_timer = new Timer();
	m_timer->Start();		
	
}

SpaceCookieBot::~SpaceCookieBot()
{
    delete m_v1;
    delete m_v2;
    delete m_v7;
    delete m_v8;
    delete m_v3;
    delete m_v4;
    delete m_WheelEncoder[kLeftWheel];
    delete m_WheelEncoder[kRightWheel];
    delete m_GearShiftSolenoid;
    delete m_ClawOpenSolenoid;
    delete m_ClawClosedSolenoid;
    delete m_ArmExtendedSolenoid;
    delete m_ArmRetractedSolenoid;
    delete m_MinibotDeploySolenoid;
    delete m_MinibotLowerSolenoid;
    delete m_MinibotRaiseSolenoid;
    delete m_compressor;
    delete m_timer;
}

double SpaceCookieBot::GetLength() const{ return m_robotLength; }

double SpaceCookieBot::GetWidth() const{ return m_robotWidth; }

double SpaceCookieBot::GetRotationTensor() const{ return m_rotationTensor; }



double SpaceCookieBot::GetWheelRadius(WheelType wheel_id) const{ return m_wheelRadius; }//temp

double SpaceCookieBot::GetWheelGearingRatio(WheelType wheel_id) const{return m_lowGear; }//temp

uint32_t SpaceCookieBot::GetMaxMotorDrive(WheelType wheel_id) const{ return m_maxMotorDrive; }//temp


//Austin Motor response

double victor_linearize(double goal_speed)
{
	const double deadband_value = 0.082;
	if (goal_speed > deadband_value)
		goal_speed -= deadband_value;
	else if (goal_speed < -deadband_value)
		goal_speed += deadband_value;
	else
		goal_speed = 0.0;
	goal_speed = goal_speed / (1.0 - deadband_value);

	double goal_speed2 = goal_speed * goal_speed;
	double goal_speed3 = goal_speed2 * goal_speed;
	double goal_speed4 = goal_speed3 * goal_speed;
	double goal_speed5 = goal_speed4 * goal_speed;
	double goal_speed6 = goal_speed5 * goal_speed;
	double goal_speed7 = goal_speed6 * goal_speed;

	// Constants for the 5th order polynomial
	double victor_fit_e1		= 0.437239;
	double victor_fit_c1		= -1.56847;
	double victor_fit_a1		= (- (125.0 * victor_fit_e1  + 125.0 * victor_fit_c1 - 116.0) / 125.0);
	double answer_5th_order = (victor_fit_a1 * goal_speed5
					+ victor_fit_c1 * goal_speed3
					+ victor_fit_e1 * goal_speed);

	// Constants for the 7th order polynomial
	double victor_fit_c2 = -5.46889;
	double victor_fit_e2 = 2.24214;
	double victor_fit_g2 = -0.042375;
	double victor_fit_a2 = (- (125.0 * (victor_fit_c2 + victor_fit_e2 + victor_fit_g2) - 116.0) / 125.0);
	double answer_7th_order = (victor_fit_a2 * goal_speed7
					+ victor_fit_c2 * goal_speed5
					+ victor_fit_e2 * goal_speed3
					+ victor_fit_g2 * goal_speed);


	// Average the 5th and 7th order polynomials
	double answer =  0.85 * 0.5 * (answer_7th_order + answer_5th_order)
			+ .15 * goal_speed * (1.0 - deadband_value);

	if (answer > 0.001)
		answer += deadband_value;
	else if (answer < -0.001)
		answer -= deadband_value;

	return answer;
}


#if 0
static double LinearizeVictorResponse(double x, RobotModel::WheelType whichWheel)
{
    int whichCol = 1;
    if (whichWheel==RobotModel::kRightWheel)
    	whichCol=2;
    
    if (fabs(x) < 0.1)
        return 0;

    for (int i=0; i<NumMotorResponseRows-1; i++)
    {
        double lowVal = (whichCol == 2) ? MotorResponse[i+1][whichCol] : MotorResponse[i][whichCol];
        double highVal = (whichCol == 2) ? MotorResponse[i][whichCol] : MotorResponse[i+1][whichCol];
        if (x >= lowVal && x <= highVal)
        {
            double weight = (x - lowVal) / (highVal - lowVal);
            double base = MotorResponse[i][0];
            double diff = MotorResponse[i+1][0] - base;
            return ((weight * diff) + base);
        }
    }
    
    // we should never get here
    printf("\n******************\nERROR!!! The Linearization is out of range\n******************\n\n");    
    return x;
}
#else

// New Victor response
static double LinearizeVictorResponse(double x, RobotModel::WheelType whichWheel)
{
	if (whichWheel==RobotModel::kRightWheel) return -victor_linearize(x) ;	 
	else return victor_linearize(x);
}

#endif

void SpaceCookieBot::SetArmMotorValue(double speed)
{
	m_v3->Set(.5*speed);
	m_v4->Set(.5*speed);
}

void SpaceCookieBot::SetWheelMotorValue(WheelType wheel_id, double drive)
{
	// found this constant by running acceleration test and looking at what encoder
	// returned for rps of left and right motors when driving forward and back
	//static const double kMotorForwardBackwardDriveStrengthRatio = (4.5828 / 4.6371);
	//static const double kLeftRightDriveTrainEfficiency = 0.8685;
	
	//printf("setting wheel motor value to %f", drive);
	
	float motorVal = (drive > 1) ? 1 : ((drive < -1) ? -1 : drive);
    double linearOutput = motorVal;
    linearOutput = LinearizeVictorResponse(motorVal, wheel_id);
    
    if(wheel_id == 0)
    {
    	// found this factor in experiments that compensates
    	// for differences in drivetrain
    	    	
    	// scale linear output by a little factor found in experimentation
    	//linearOutput *= (kLeftRightDriveTrainEfficiency);
    	
    	m_v1->Set(linearOutput);
    	m_v2->Set(linearOutput);
    } else
    {
    	// do not set to negative because linearization handles it for us
    	m_v7->Set(linearOutput);
        m_v8->Set(linearOutput);		
       
    }
}
void SpaceCookieBot::SetGearType(GearSet gear_id){
	if(gear_id == RobotModel::kHighGear){
		m_GearShiftSolenoid->Set(false);
	} else if (gear_id == RobotModel::kLowGear) {
		m_GearShiftSolenoid->Set(true);
	}
	m_currentGear = gear_id;
}

void SpaceCookieBot::SetClawType(ClawSet claw_id){
	if(claw_id == RobotModel::kOpen){
		//printf("really setting claw open\n");
		m_ClawOpenSolenoid->Set(false);
		m_ClawClosedSolenoid->Set(true);
	}else if(claw_id == RobotModel::kClosed){
		//printf("really setting claw closed\n");
		m_ClawOpenSolenoid->Set(true);
		m_ClawClosedSolenoid->Set(false);
	}
	m_currentClaw = claw_id;
}

void SpaceCookieBot::SetRetractType(RetractSet retract_id){
	if(retract_id == RobotModel::kRetract){
		//printf("really setting arm retracted\n");
		m_ArmRetractedSolenoid->Set(false);
		m_ArmExtendedSolenoid->Set(true);
	}else if(retract_id == RobotModel::kExtend){
		//printf("really setting arm extended\n");
		m_ArmRetractedSolenoid->Set(true);
		m_ArmExtendedSolenoid->Set(false);
	}
	m_currentRetract = retract_id;
}

void SpaceCookieBot::SetDeployType(DeploySet deploy_id){
	if(deploy_id == RobotModel::kDeploy){
		m_MinibotDeploySolenoid->Set(false);
		//printf("setting Minibot to False\n\n");
	}else if(deploy_id == RobotModel::kUndeploy){
		m_MinibotDeploySolenoid->Set(true);
		//printf("setting Minibot to True\n\n");
	}
	m_currentDeploy = deploy_id;
}

void SpaceCookieBot::SetLowerType(LowerSet lower_id){
	if(lower_id == RobotModel::kLower){
		m_MinibotLowerSolenoid->Set(false);
		m_MinibotRaiseSolenoid->Set(true);
	}else if (lower_id == RobotModel::kRaise){
		m_MinibotLowerSolenoid->Set(true);
		m_MinibotRaiseSolenoid->Set(false);
	}
	m_currentLower = lower_id;
}

double SpaceCookieBot::GetArmEncoderValue() const
{
	return(m_ArmEncoder->GetRaw());			// this is NOT the right number. figure out the right encoder number. TBD EKL
}

void SpaceCookieBot::EnableArmEncoder()
{
	if(m_ArmEncoderEnabled == false)
	{	
		printf("Arm Encoder Enabled\n");
		m_ArmEncoder->Reset();
		m_ArmEncoder->Start();
		m_ArmEncoderEnabled = true;
	}
}

void SpaceCookieBot::DisableArmEncoder()
{
    if(m_ArmEncoderEnabled == true)
    	m_ArmEncoder->Stop();
    m_ArmEncoderEnabled = false;
}

void SpaceCookieBot::ResetArmEncoder(){
	if(m_ArmEncoderEnabled == true){
		m_ArmEncoder->Stop();
		m_ArmEncoder->Reset();
		m_ArmEncoder->Start();
	}
}

double SpaceCookieBot::GetWheelEncoderDistance(WheelType wheel_id) const
{
#if PRACTICE_BOT_ENCODERS
	if(wheel_id == kRightWheel){
			return(-(float)m_WheelEncoder[wheel_id]->Get())/142.0;	// about 143.2 counts per foot paw
		}													//       123.2
		else{
			return ((float)m_WheelEncoder[wheel_id]->Get())/142.0;
		}
#else
	if(wheel_id == kRightWheel){
		return(-(float)m_WheelEncoder[wheel_id]->Get())/284.0;
	}
	else{
		return ((float)m_WheelEncoder[wheel_id]->Get())/284.0;
	}
#endif
}

double SpaceCookieBot::GetWheelEncoderValue(WheelType wheel_id) const {
	if(wheel_id == kRightWheel){
		return(-m_WheelEncoder[wheel_id]->Get());
	} else {
		return (m_WheelEncoder[wheel_id]->Get());
	}
	
}

void SpaceCookieBot::EnableWheelEncoder(WheelType wheel_id)
{
	if(m_WheelEncoderEnabled[wheel_id] == false)
	{
		m_WheelEncoder[wheel_id]->Reset();
		m_WheelEncoder[wheel_id]->Start();
		m_WheelEncoderEnabled[wheel_id] = true;
	}
}

void SpaceCookieBot::DisableWheelEncoder(WheelType wheel_id)
{
    if(m_WheelEncoderEnabled[wheel_id] == true)
    	m_WheelEncoder[wheel_id]->Stop();
    m_WheelEncoderEnabled[wheel_id] = false;
}

void SpaceCookieBot::ResetWheelEncoder(WheelType wheel_id){
	if(m_WheelEncoderEnabled[wheel_id]==true){
		m_WheelEncoder[wheel_id]->Stop();
		m_WheelEncoder[wheel_id]->Reset();
		m_WheelEncoder[wheel_id]->Start();
	}
}




double SpaceCookieBot::GetTimeSec() const
{
    return m_timer->Get();
}

uint32_t SpaceCookieBot::GetTimeMilliSec() const
{
    return (uint32_t)(GetTimeSec() * 1000);
}

void SpaceCookieBot::WaitTimeMilliSec(uint32_t waitMS) const
{
    Wait((double)waitMS/1000.0);//the wait function waits in seconds
}

double SpaceCookieBot::GetGyroValue() const {
	return m_gyro->GetAngle();
}

void SpaceCookieBot::EnableCompressor(){
	m_compressor->Start();
}

void SpaceCookieBot::ClearGyro()
{
	m_gyro->Reset();
}
void SpaceCookieBot::DisableCompressor(){
	m_compressor->Stop();
}

bool SpaceCookieBot::GetCompressorState() const{
	return m_compressor->Enabled();
}

void SpaceCookieBot::ExtendArm() {
	m_ArmRetractedSolenoid->Set(false);
	m_ArmExtendedSolenoid->Set(true);
}

void SpaceCookieBot::RetractArm() {
	m_ArmExtendedSolenoid->Set(false);
	m_ArmRetractedSolenoid->Set(true);
}

void SpaceCookieBot::OpenClaw() {
	m_ClawClosedSolenoid->Set(false);
	m_ClawOpenSolenoid->Set(true);
}

void SpaceCookieBot::CloseClaw() {
	m_ClawOpenSolenoid->Set(false);
	m_ClawClosedSolenoid->Set(true);
}
