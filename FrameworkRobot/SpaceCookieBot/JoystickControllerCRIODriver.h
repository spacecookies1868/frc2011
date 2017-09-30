#ifndef JOYSTICK_CONTROLLER_CRIO_DRIVER_H_
#define JOYSTICK_CONTROLLER_CRIO_DRIVER_H_

#include "StudentRobotProgram/JoystickDriveController.h"
#include "WPILib.h"

/**
 * An implementation of a physical XBox controller providing input to the robot.
 */
class JoystickControllerCRIODriver : public virtual JoystickDriveController 
{
	
public:

	
	/**
	 * Creates a controller with the specified port number.
	 */
	explicit JoystickControllerCRIODriver(int port1Num, int port2Num, int port3Num, int port4Num);
	virtual ~JoystickControllerCRIODriver();
	
protected:
	
	virtual bool getButton(Button b);
	virtual float getAxis(Axis a);
	
private:
	
	/**
	 * The port of the controller.
	 */
	Joystick	m_j1;
	Joystick 	m_j2;
	Joystick 	m_j3;
	Joystick	m_j4; //Button pad

	
};

#endif
