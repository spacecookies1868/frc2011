#ifndef JOYSTICK_DRIVE_CONTROLLER_H_
#define JOYSTICK_DRIVE_CONTROLLER_H_

#include "Shared/HumanDriveControl.h"
#include "AcceleratorArcade.h"

/**
 * An implementation of a physical Dual Joystick controller providing input to the robot.
 */
class JoystickDriveController : public virtual HumanDriveControl {

public:
	enum Button {
			LTrig, LTwo, LThree, LFour, LFive, LSix,
			LSeven, LEight, LNine, LTen, LEleven, LTwelve, RTrig, RTwo, RThree,
			RFour, RFive, RSix, RSeven, REight, RNine, RTen, REleven, RTwelve, 
			ATrig, ATwo, AThree, AFour, AFive, ASix, ASeven, AEight,
			ANine, ATen, AEleven, ATwelve, POne, PTwo, PThree, PFour, PFive,
			PSix, PSeven, PEight, PNine, PTen, PEleven, PTwelve, NumButtons
		};
	enum Axis {
			RStickX, RStickY, RStickZ, LStickX, LStickY, LStickZ, AStickY, AStickX, AStickZ, NumAxes
		};

    virtual double getLeftMotorDesiredSpeed() const{ return m_motorLeft;}
	virtual double getRightMotorDesiredSpeed() const{ return m_motorRight;}
	virtual double getArmMotorDesiredSpeed()  const { return m_motorArm;}
	virtual uint32_t getDesiredArmPosition() const { return m_desiredPosition; }
	virtual uint32_t getDesiredJog() const { return m_desiredJog; }
	virtual uint32_t getDesiredAutonomous() const { return m_desiredAutonomous; }
	virtual uint32_t getDesiredTubes() const { return m_desiredTubes; }
	virtual bool getHangMacro() const { return m_bHangMacro; }
	virtual RobotModel::GearSet getDesiredGear() const;
	virtual RobotModel::ClawSet getDesiredClaw() const;
	virtual RobotModel::RetractSet getDesiredRetract() const;
	virtual RobotModel::DeploySet getDesiredDeploy() const;
	virtual RobotModel::LowerSet getDesiredLower() const;

    /**
	 * Processes button input and updates robot.
	 */
	virtual void update();

	/**
	 * Creates a controller with the specified port number.
	 */
	explicit JoystickDriveController();
	virtual ~JoystickDriveController();



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
	
	double 		m_motorArm;
	
	double 		m_turnVelocity;

	/**
	 * Indicates what gear the robot is using.
	 */
	RobotModel::GearSet  m_gearType;
	
	RobotModel::ClawSet  m_clawType;
	
	RobotModel::RetractSet m_retractType;
	
	RobotModel::DeploySet m_deployType;
	
	RobotModel::LowerSet m_lowerType;

	/**
	 * Indicates whether or not a button is currently pressed.
	 */
	bool		m_gearActive;
	
	bool 		m_clawActive;
	
	bool 		m_retractActive;
	
	bool		m_deployActive;
	
	bool 		m_lowerActive;
	
	bool 		m_bHangMacro;
	/**
	 * Indicates the accleration for the motors
	 */
	float		m_acceleration;
	
	// Indicates the driver's desired position for the arm.
	uint32_t	m_desiredPosition;
	
	uint32_t 	m_desiredJog;
	
	uint32_t 	m_desiredAutonomous;
	
	uint32_t 	m_desiredTubes;
	
	AcceleratorArcade		m_arcade;
	
	int count;
};

#endif
