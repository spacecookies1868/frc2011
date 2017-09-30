#include "WPILib.h"

#ifndef ROBOTCONFIGURATION_H_
#define ROBOTCONFIGURATION_H_

#if 1								// Easy way to turn all PRINTFs on and off.
#define PRINTF(x) printf x
#else
#define PRINTF(x)
#endif

#define USE_CAMERA 1

/* Can define other stuff here too, like this:
 * 
 * #define NAME 0    to turn it off or
 * #define NAME 1    to turn it on.
 * 
*/

class RobotConfiguration
{
private:
	RobotDrive *robotDrive;
	
	Victor *armVictor;
	
	Solenoid *armSolenoid;
	Solenoid *clawSolenoid;
	Solenoid *shiftSolenoid;
	
	Compressor *mainCompressor;
	
	Encoder *leftWheelEncoder;
	Encoder *rightWheelEncoder;
	Encoder *armEncoder;
	Gyro *mainGyro;
	
	DriverStation *driverStation;
	Joystick *leftDriveJoystick;
	Joystick *rightDriveJoystick;
	Joystick *operatorJoystick;
	
	Watchdog *mainWatchdog;
	
public:
	RobotConfiguration( RobotBase *robot );
	virtual ~RobotConfiguration();
	
	RobotDrive *MainDrive()						{ return robotDrive; }
	
	Victor *ArmVictor()							{ return armVictor; }
	
	Solenoid *ArmSolenoid()						{ return armSolenoid; }
	Solenoid *ClawSolenoid()					{ return clawSolenoid; }
	Solenoid *ShiftSolenoid()					{ return shiftSolenoid; }
	
	Compressor *MainCompressor()				{ return mainCompressor; }
	
	Encoder *LeftWheelEncoder()					{ return leftWheelEncoder; }
	Encoder *RightWheelEncoder()				{ return rightWheelEncoder; }
	Encoder *ArmEncoder()						{ return armEncoder; }
	Gyro *MainGyro()							{ return mainGyro; }
	
	DriverStation *MainDriverStation()				{ return driverStation; }
	Joystick *LeftDriveJoystick()				{ return leftDriveJoystick; }
	Joystick *RightDriveJoystick()				{ return rightDriveJoystick; }
	Joystick *OperatorJoystick()				{ return operatorJoystick;}
	
	Watchdog *MainWatchdog()					{ return mainWatchdog; }
	
};

#endif /*ROBOTCONFIGURATION_H_*/
