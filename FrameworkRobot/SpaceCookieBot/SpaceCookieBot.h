#ifndef SPACE_COOKIE_BOT_H_INCLUDED
#define SPACE_COOKIE_BOT_H_INCLUDED

#include "Shared/CommonIncludes.h"
#include "Shared/RobotModel.h"
#include "WPILib.h"

/**
 * Defines the physical characteristics of the robot SpaceCookieBot.
 */
class SpaceCookieBot : public RobotModel
{
   public:
		/**
		 * SpaceCookieBot Constructor.
		 *
		 * Initializes all the member variables of the SpaceCookieBot Class. Multiplying by 2.54 and dividing
		 * by 100 converts a value in inches to meters. The rotation tensor is currently not mathematically
		 * correct. The wheel radius is the diameter (3.863 inches) divided by 2 (to get the radius) and
		 * converted to meters. The values for the gears is the actual equation used to calculate gearing
		 * ratio with the real gear numbers. Drive type is linearized or un-linearized, starts linearized.
		 * The Victors are created, the parameters are the pwm channels the Victors are plugged into. The
		 * numbers for the encoders are the ports those are plugged into. A new timer is created and started.
		 * The encoders start out disabled so the for loop goes through and tells the robot its encoders are
		 * disabled.
		 */
        explicit SpaceCookieBot();
        virtual ~SpaceCookieBot();
        
        /**
         * Gets the length of the robot in meters.
         * @return m_robotLength The length of the robot in meters
         */
        virtual double GetLength() const;

        /**
         * Gets the width of the robot in meters.
         * @return m_robotWidth The width of the robot in meters
         */
        virtual double GetWidth() const;

        virtual double GetRotationTensor() const;

        virtual double GetWheelRadius(WheelType wheel_id) const;
        
    	virtual double GetMaxMotorRPS() const {return 88.5;}
    	
    	virtual double GetEfficiencyLoss() const {return (0.8909);}

        /**
         * Gets the current gearing ratio.
         * @return The current gearing ratio
         */
        virtual double GetWheelGearingRatio(WheelType wheel_id) const;

        virtual uint32_t GetMaxMotorDrive(WheelType wheel_id) const;

        // Sets the arm motor speed.
        virtual void SetArmMotorValue(double speed);
        
        
        virtual double GetArmEncoderValue() const;
        
        virtual void EnableArmEncoder();

        virtual void DisableArmEncoder();
                
        virtual void ResetArmEncoder();
        
        
        /**
         * Sets the pwm value for the wheels.
         *
         * Makes sure that motorVal is set to a min and max of -1 and 1 respectively.
         * Checks to see if the value wants to be linear and linearizes it if is
         * supposed to be linear. Manually sets the pwm value for each Victor.
         * @param wheel_id The side of the motors to be set
         * @param drive The value the motors are to be set to
         */
        virtual void SetWheelMotorValue(WheelType wheel_id, double drive);  
        /**
         * Gets the encoder value of the wheel
         * The wheel id is necessary to see if the value needs to be flipped so that
         * positive always corresponds to the robot going forward. Divides by 256 to
         * convert raw encoder values to rotations per second.
         * @param wheel_id The wheel to retrieve the encoder from
         * @return The rotations per second of the wheel the encoder is attached too
         */
        virtual double GetWheelEncoderDistance(WheelType wheel_id) const;
        
        virtual double GetWheelEncoderValue(WheelType wheel_id) const;

        virtual void EnableWheelEncoder(WheelType wheel_id);

        virtual void DisableWheelEncoder(WheelType wheel_id);
        
        virtual void ResetWheelEncoder(WheelType wheel_id);
        
        /*
         * Sensors 
         * 
         */
        
        virtual double	GetGyroValue() const;
    	
    	virtual void	ClearGyro();
    	
    	virtual double	GetAccelerometerX() const {return 0.0;}
    	
    	virtual double	GetAccelerometerY() const {return 0.0;}
    	
    	virtual double	GetAccelerometerZ() const {return 0.0;}
    	

    	
        /**
         * Starts the compressor.
         */
        virtual void EnableCompressor();
        
        /**
         * Stop the compressor.
         */
        virtual void DisableCompressor();
        
        /**
         * Determines if the compressor is enabled.
         */
        virtual bool GetCompressorState() const;
        
        virtual GearSet GetGear () const { return m_currentGear; }
        
        virtual ClawSet GetClaw () const { return m_currentClaw; }
        
        virtual RetractSet GetRetract () const { return m_currentRetract; }
        
        virtual DeploySet GetDeploy () const { return m_currentDeploy; }
        
        virtual LowerSet GetLower () const { return m_currentLower; }
        
        virtual void	SetGearType(GearSet gear_id);
        
        virtual void    SetClawType(ClawSet claw_id);
        
        virtual void 	SetRetractType(RetractSet retract_id);
        
        virtual void 	SetDeployType(DeploySet deploy_id);
        
        virtual void 	SetLowerType (LowerSet lower_id);
        /* These functions control the pistons on the arm and claw.
         * 
         */
        
        virtual void ExtendArm();
        
        virtual void RetractArm();
        
        virtual void OpenClaw();
        
        virtual void CloseClaw();
        
        
        
        
        /*
         * Time Implementation Functions
         */
        
        virtual double GetTimeSec() const;

        virtual uint32_t GetTimeMilliSec() const;

        virtual void WaitTimeMilliSec(uint32_t waitMS) const;
                

    private:
    	/**
    	 * The length of the robot.
    	 */
        double m_robotLength;

        /**
         * The width of the robot.
         */
        double m_robotWidth;

        /**
         * The rotation tensor of the robot.
         */
        double m_rotationTensor;

        /**
         * The robot's wheel radius.
         */
        double m_wheelRadius;

        /**
         * The value for the robot's low gear.
         */
        double m_lowGear;

        /**
         * The value for the robot's high gear.
         */
        double m_highGear;

        /**
         * The robot's maximum motor drive.
         */
        uint32_t m_maxMotorDrive;

        /**
         * Left Wheel Motor Controllers
         */
        Victor* m_v1, *m_v2;
        
        /**
         * Right Motor Victor Controllers
         */
        Victor* m_v7, *m_v8;
        
        /**
         * Arm Motor Victor Controllers
         */
        Victor* m_v3, *m_v4;
                
        
        /**
         * The robot's encoder for each wheel.
         */
        Encoder* m_WheelEncoder[kNumWheels];
        
        /**
         * The robot's encoder for the arm shaft.
         */
        Encoder* m_ArmEncoder;
        
        
        /**
         * Whether or not the the encoder is enabled for each wheel.
         */
    	bool m_WheelEncoderEnabled[kNumWheels];
    	
    	/**
    	 * Whether or not the the arm encoder is enabled.
    	 */
    	bool m_ArmEncoderEnabled;
    	
    	
    	/**
    	 * Records the current time of the robot.
    	 */
    	Timer* m_timer;
    	/**
    	 * Indicates the robot's gyro.
    	 */
    	Gyro* m_gyro;
    	/**
    	 * Indicates the robot's compressor.
    	 */
    	Compressor* m_compressor;
    	/**
    	 * Indicates the robot's solenoid.
    	 */
    	Solenoid* m_GearShiftSolenoid;
    	Solenoid* m_ClawClosedSolenoid;
    	Solenoid* m_ClawOpenSolenoid;
    	Solenoid* m_ArmRetractedSolenoid;
    	Solenoid* m_ArmExtendedSolenoid;
    	Solenoid* m_MinibotDeploySolenoid;
    	Solenoid* m_MinibotLowerSolenoid;
    	Solenoid* m_MinibotRaiseSolenoid;
     	/**
     	 * Indicates the robot's current gear set.
     	 */
    	GearSet m_currentGear;
    	
    	ClawSet m_currentClaw;
    	
    	RetractSet m_currentRetract;
    	
    	DeploySet m_currentDeploy;
    	
    	LowerSet m_currentLower;
};



#endif // SpaceCookieBot_H_INCLUDED
