#ifndef XBOX_CONTROLLER_CRIO_DRIVER_H_
#define XBOX_CONTROLLER_CRIO_DRIVER_H_

#include "StudentRobotProgram/XBoxDriveController.h"
#include "WPILib.h"

/**
 * An implementation of a physical XBox controller providing input to the robot.
 */
class XBoxControllerCRIODriver : public virtual XBoxDriveControl 
{
	
public:

	
	/**
	 * Creates a controller with the specified port number.
	 */
	explicit XBoxControllerCRIODriver(int portNum);
	virtual ~XBoxControllerCRIODriver();
	
protected:
	
	virtual bool getButton(Button b);
	virtual float getAxis(Axis a);
	
private:
	
	/**
	 * The port of the controller.
	 */
	Joystick	m_j1;
	

	
};

#endif
