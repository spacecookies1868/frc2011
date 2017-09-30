#include "ArmController.h"
#include "RobotConfiguration.h"

ArmController::ArmController( RobotConfiguration *config)
{
	armVictor = config->ArmVictor();
	operatorJoystick = config->OperatorJoystick();
	driverStation = config->MainDriverStation();
	armSolenoid = config->ArmSolenoid();
	clawSolenoid = config ->ClawSolenoid();
	armEncoder = config ->ArmEncoder();
	
}

ArmController::~ArmController()
{
}
