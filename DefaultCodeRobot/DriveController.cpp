#include "DriveController.h"
#include "RobotConfiguration.h"

DriveController::DriveController( RobotConfiguration *config )
{
	drive = config->MainDrive();
	mode = kDriveModeTank;    // ?
	
	shiftSolenoid = config->ShiftSolenoid();
	
	leftWheelEncoder = config->LeftWheelEncoder();
	rightWheelEncoder = config->RightWheelEncoder();
	mainGyro = config->MainGyro();
	leftJoystick = config->LeftDriveJoystick();
	rightJoystick = config->RightDriveJoystick();
	
	driverStation = config->MainDriverStation();

}

DriveController::~DriveController()
{
}
