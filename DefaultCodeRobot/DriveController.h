#include "WPILib.h"
#include "RobotConfiguration.h"

#ifndef DRIVECONTROLLER_H_
#define DRIVECONTROLLER_H_

class DriveController
{
private:
	enum DriveMode {
		kDriveModeNone = 0,
		kDriveModeTank,
		kDriveModeTankFast,
		kDriveModeArcade,
		kDriveModeAutonomous
	};
	RobotDrive *drive;
	enum DriveMode mode;	
	
	Solenoid *shiftSolenoid;
	Encoder *leftWheelEncoder;
	Encoder *rightWheelEncoder;
	Gyro *mainGyro;
	Joystick *leftJoystick;
	Joystick *rightJoystick;
	
	DriverStation *driverStation;

	
public:
	DriveController( RobotConfiguration *config );
	virtual ~DriveController();
};

#endif /*DRIVECONTROLLER_H_*/
