#ifndef XBOX_DRIVE_CONTROL_H_
#define XBOX_DRIVE_CONTROL_H_

#include "Shared/HumanDriveControl.h"
#include "AcceleratorArcade.h"

/**
 * An implementation of a physical XBox controller providing input to the robot.
 */
class XBoxDriveControl : public virtual HumanDriveControl {

public:
	enum Button {
			A, B, X, Y, LB, RB,
			BACK, START, LStickClick, RStickClick, NumButtons
		};
		enum Axis {
			LeftStickX, LeftStickY, LeftTrigger, RightTrigger, RightStickX, RightStickY, NumAxes
		};

    virtual double getLeftMotorDesiredSpeed() const{ return m_motorLeft;}
	virtual double getRightMotorDesiredSpeed() const{ return m_motorRight;}
	virtual RobotModel::GearSet getDesiredGear() const;

    /**
	 * Processes button input and updates robot.
	 */
	virtual void update();

	/**
	 * Creates a controller with the specified port number.
	 */
	explicit XBoxDriveControl();
	virtual ~XBoxDriveControl();



protected:
	virtual bool getButton(Button b) = 0;
	virtual float getAxis(Axis a) = 0;

private:
	/**
	 * The desired speed for the left motor.
	 */
	double		m_motorLeft;
	/**
	 * The desired speed for the right motor.
	 */
	double		m_motorRight;

	/**
	 * Indicates what gear the robot is using.
	 */
	RobotModel::GearSet  m_gearType;

	/**
	 * Indicates whether or not a button is currently pressed.
	 */
	bool		m_gearActive;
	/**
	 * Indicates the accleration for the motors
	 */
	float		m_acceleration;
	
	AcceleratorArcade		m_arcade;
};

#endif
