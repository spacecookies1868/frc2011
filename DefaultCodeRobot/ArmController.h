#include "WPILib.h"
#include "RobotConfiguration.h"

#ifndef ARMCONTROLLER_H_
#define ARMCONTROLLER_H_

class ArmController
{
private:
	Victor *armVictor;
	Joystick *operatorJoystick;
	DriverStation *driverStation;
	Solenoid *armSolenoid;
	Solenoid *clawSolenoid;
	Encoder *armEncoder;
	
public:
	ArmController( RobotConfiguration *config );
	virtual ~ArmController();
};

#endif /*ARMCONTROLLER_H_*/
