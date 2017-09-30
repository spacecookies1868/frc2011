#include "RobotConfiguration.h"

RobotConfiguration::RobotConfiguration( RobotBase *robot )
{
	PRINTF(("begin"));
	
	robotDrive = new RobotDrive( 2, 3, 5, 6 );				// Change to whatever ports the four drive Victors are plugged into.
	armVictor = new Victor( 4 );							// Change to whatever port the arm victor is plugged into.
	
	armSolenoid = new Solenoid( 1 );						// Numbers are the output ports the solenoids are plugged into.
	clawSolenoid = new Solenoid( 2 );
	shiftSolenoid = new Solenoid( 3 );
	
	mainCompressor = new Compressor( 1, 1 );
	
	leftWheelEncoder = new Encoder( (UINT32)2, 3, true );	// true means reverse the direction of the encoder.
	rightWheelEncoder = new Encoder( (UINT32)4, 5);			// The numbers are the analog input ports the encoders are plugged into.
	mainGyro = new Gyro( 1 );								// 1 is the analog port the gyro is plugged into.
	mainGyro->SetSensitivity( 0.007 );
	
#if USE_CAMERA
	AxisCamera &camera = AxisCamera::GetInstance();
	camera.WriteResolution(AxisCamera::kResolution_320x240);
	camera.WriteBrightness(0);
	Wait(3.0);
#endif
	
	driverStation = DriverStation::GetInstance();
	leftDriveJoystick = new Joystick( 1 );					// USB port #s
	rightDriveJoystick = new Joystick( 2 );
	operatorJoystick = new Joystick( 3 );
	
	mainWatchdog = &robot->GetWatchdog();
	mainWatchdog->SetExpiration( 0.1 );						// 0.1 sec
	mainWatchdog->SetEnabled( true );
	
	leftWheelEncoder->Start();
	rightWheelEncoder->Start();
	double feetPerPulse = ( (12.75/12.0)/300.0 );			// These numbers depend on the wheel radius and the type of encoder.
	leftWheelEncoder->SetDistancePerPulse(feetPerPulse);
	rightWheelEncoder->SetDistancePerPulse(feetPerPulse);
	
	
}

RobotConfiguration::~RobotConfiguration()
{
}
