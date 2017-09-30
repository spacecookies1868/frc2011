#ifndef HUMANDRIVECONTROL_H_
#define HUMANDRIVECONTROL_H_

#include "RobotModel.h"

/**
 * An abstract class modeling a user interface through a controller.
 *
 * Can be implemented as from simulator or physical input.
 */
class HumanDriveControl {
	
public:
	
    //What the User wants
	
	enum PresetHeight{
		kNoMovement,
		kLow,
		kLowUp,
		kMiddle,
		kMiddleUp,
		kTop,
		kTopUp,
		kNumPresetHeights
	};
	enum LED
	{
		kNoLED,
	    kRedLED,
	    kBlueLED,
	    kWhiteLED,
	    kNumLEDS
	};
    
	/**
	 * Returns the desired speed of the left motor.
	 *
	 * @return the desired speed of the left motor
	 */
	virtual double getLeftMotorDesiredSpeed() const = 0;
    
    /**
	 * Returns the desired speed of the right motor.
	 *
	 * @return the desired speed of the right motor
	 */
	virtual double getRightMotorDesiredSpeed() const = 0;
	
	
	virtual double getArmMotorDesiredSpeed() const = 0;

	virtual uint32_t getDesiredArmPosition() const = 0;
	
	virtual uint32_t getDesiredJog() const = 0;
	
	virtual uint32_t getDesiredAutonomous() const = 0;
	
	virtual uint32_t getDesiredTubes() const = 0;
	/**
	 * Returns the desired gear.
	 *
	 * @return the desired gear
	 */
	virtual RobotModel::GearSet getDesiredGear() const = 0;
	
	
	virtual RobotModel::ClawSet getDesiredClaw() const = 0;
	
	virtual RobotModel::RetractSet getDesiredRetract() const = 0;
	
	virtual RobotModel::DeploySet getDesiredDeploy() const = 0;
	
	virtual RobotModel::LowerSet getDesiredLower() const = 0;
	
	virtual bool getHangMacro() const = 0;
    
    /**
	 * Updates the state of the controller
	 */
	virtual void update() = 0;
	
	virtual ~HumanDriveControl() {}
	
};

#endif
