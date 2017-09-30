/*
 *  RobotModel.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 12/2/10.
 *
 */

#ifndef ROBOT_MODEL_H
#define ROBOT_MODEL_H

#include "Shared/CommonIncludes.h"
#include "Shared/NumericalVector.hpp"
#include "Shared/Vector3.h"



/**
 * Class that defines the robot characteristics.
 *
 * This class is abstract because it may have an implementation for simulation
 * and the real physical model.
 *
 * @author Dmitriy Solomonov
 */

class RobotModel // I'm just adding a comment here so that TortoiseSVN thinks something has changed.
{
	
public:
	
	virtual ~RobotModel() {}
	
	/**
	 * Definitions of the wheel's type.
	 */
	enum WheelType{
        kLeftWheel=0,
        kRightWheel,
		// add any other wheel definitions here
		kNumWheels
    };
	
	/**
	 * Definition of the lowest, highest, and the number of intermediate gears.
	 */
    enum GearSet{
    	kLowGear, kHighGear, kNumGears
    };
    
    enum ClawSet{
    	kOpen, kClosed, kNumClaw
    };
    
    enum RetractSet{
    	kRetract, kExtend, kNumRetract
    };
    
    enum DeploySet{
    	kDeploy, kUndeploy, kNumDeploy
    };
    
    enum LowerSet{
    	kLower, kRaise, kNumLower
    };
    
	/*
	 Accessor functions for robot properties
	 */
	virtual double GetLength() const 		{ return 1.03; }
	
	virtual double GetWidth() const			{ return .702; }
	
	virtual double GetHeight() const		{ return 0.127; }
	
	virtual double GetMaxMotorRPS() const {return 85.5;}
	
	virtual double GetEfficiencyLoss() const {return (85.0/88.0);}
	
	virtual double GetRotationTensor() const 	{ return 3.1; }
	
	virtual double	GetMass() const { return 53.5;}
	
	// for now, treat both wheels as the same size
	virtual double GetWheelRadius(WheelType wheel_id) const	{	return 0.0445; }
	
	virtual double GetWheelGearingRatio(WheelType wheel_id) const {	return 2.0; }
	
	virtual uint32_t	GetMaxMotorDrive(WheelType wheel_id) const {	return 60; }
	
    /*
     *  Wheel Control
     */
    
    typedef NumericalVector<kNumWheels> WheelValues;
    
	virtual void    SetWheelMotorValue(WheelType wheel_id, double drive) = 0;
	
    virtual double  GetWheelEncoderValue(WheelType wheel_id) const = 0;
    
    virtual double 	GetWheelEncoderDistance(WheelType wheel_id) const = 0;
    
    virtual void    EnableWheelEncoder(WheelType wheel_id) = 0;
    
    virtual void    DisableWheelEncoder(WheelType wheel_id) = 0;
    
    virtual void	ResetWheelEncoder(WheelType wheel_id) = 0;
	
	virtual void	SetGearType(GearSet gear_id) = 0;
	
	virtual void 	SetClawType(ClawSet claw_id) = 0;
	
	virtual void 	SetRetractType(RetractSet retract_id) = 0;
	
	virtual void 	SetDeployType(DeploySet deploy_id) = 0;
	
	virtual void 	SetLowerType(LowerSet lower_id) = 0;
	
	virtual GearSet	GetGear() const = 0;
	
	virtual ClawSet GetClaw() const = 0;
	
	virtual RetractSet GetRetract() const = 0;
	
	virtual DeploySet GetDeploy() const = 0;
	
	virtual LowerSet GetLower() const = 0;
	
	virtual void	EnableCompressor() = 0;
	
	virtual void	DisableCompressor() = 0;
	
	virtual bool	GetCompressorState() const = 0;
	
	virtual void ExtendArm() = 0;
	        
	virtual void RetractArm() = 0;
	        
	virtual void OpenClaw() = 0;
	        
	virtual void CloseClaw() = 0;
	
	virtual void SetArmMotorValue(double speed) = 0;
	        
	virtual double GetArmEncoderValue() const = 0;
	        
	virtual void EnableArmEncoder() = 0;

	virtual void DisableArmEncoder() = 0;
	                
	virtual void ResetArmEncoder() = 0;
	
	
	// Convenience Functions for wheel operations in parallel
    // These are not virtual because they are implemented using the regular control funcs
    WheelValues     GetWheelRadii() const;
    WheelValues     GetWheelGearingRatios() const;
    WheelValues     GetMaxMotorDrives() const;
    void            SetWheelMotorValues(const WheelValues& drive);
    WheelValues     GetWheelEncoderValues() const;
    void            EnableWheelEncoders();
    void            DisableWheelEncoders();
    void            ResetWheelEncoders();
    
    /*
     * Other sensors not tied to specific parts of the robot
     */
	
	virtual double	GetGyroValue() const = 0;
	
	virtual void	ClearGyro() = 0;
	
	/*
	 * Time Functions
	 */
	
	virtual double	GetTimeSec() const = 0;
	
	virtual uint32_t GetTimeMilliSec() const = 0;
    
    virtual void    WaitTimeMilliSec(uint32_t waitMS) const = 0;

	
};

#endif
