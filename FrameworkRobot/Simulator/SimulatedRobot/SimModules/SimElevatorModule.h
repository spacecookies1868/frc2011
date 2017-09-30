/*
 *  SimElevatorModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_ELEVATOR_MODULE_H
#define SIM_ELEVATOR_MODULE_H

#include "FieldObject/FieldObject.h"
#include "SimulatedRobot/SimModules/SimMotorModule.h"

class SimRobotBody;
class RotationInterface;
class SimPotModule;
class SimEncoderModule;
class SimGearingModule;

/*
if we want to be completely true to real life,
we could add a motor and a gearbox for
changing the position/angle of the arm
*/

class SimElevatorModule : public SimModuleBase
{
public:
	SimElevatorModule(SimRobotBody* body);
	~SimElevatorModule();
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	void SetElevatorMotorDrive(double drive) {m_elevatorMotor->SetDrive(drive);}
	void SetArmMotorDrive(double drive) {m_armMotor->SetDrive(drive);}
	double GetArmPos() const;
	double GetArmAngle() const;
	
private:
	SimRobotBody* m_body;
	RotationInterface* m_elevatorRotsrc;
	RotationInterface* m_armRotsrc;
	int m_polyhedronindex;
	Vector3 m_relpos;
	SimMotorModule* m_elevatorMotor;
	SimMotorModule* m_armMotor;
	SimPotModule* m_armPot;
	SimEncoderModule* m_elevatorEncoder;
	SimGearingModule* m_elevatorGearing;
	SimGearingModule* m_armGearing;
	
	friend class SimArmModule;
};
#endif