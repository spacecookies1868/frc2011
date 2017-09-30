/*
 *  SimulatedRobotModel.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 12/2/10.
 *
 */

#ifndef SIMULATED_ROBOT_MODEL_H
#define SIMULATED_ROBOT_MODEL_H

#include "Shared/RobotModel.h"
#include "Shared/Vector3.h"
#include "SimulatedRobot/SimulatedField.h"
#include "SimulatedRobot/SimModules/SimModuleIncludes.h"

class FieldObject;

class SimulatedRobotModel : public virtual RobotModel
{

public:

	virtual void    SetWheelMotorValue(WheelType wheel_id, double drive);

    virtual double  GetWheelEncoderValue(WheelType wheel_id) const;

    virtual void    EnableWheelEncoder(WheelType wheel_id);

    virtual void    DisableWheelEncoder(WheelType wheel_id);

    virtual void	ResetWheelEncoder(WheelType wheel_id);

	virtual double	GetGyroValue() const;

	virtual void	ClearGyro();

	virtual void	SetGearType(GearSet gear_id) {}

	virtual void	EnableCompressor() {}

	virtual void	DisableCompressor() {}

    virtual bool	GetCompressorState() const { return true; }

    virtual GearSet	GetGear() const { return kLowGear; }

	virtual double	GetTimeSec() const;

	virtual uint32_t GetTimeMilliSec() const;

    virtual void    WaitTimeMilliSec(uint32_t waitMS) const;

	explicit SimulatedRobotModel(SimulatedField* field, const Vector3& initialPos, Vector3 initialAngle, FieldObject::Alliance alliance);
	virtual ~SimulatedRobotModel();
    
    FieldObject*    GetRobotFieldObject() const;

protected:

	friend class SimHardware;

    Vector3         GetPosition() const;
	Vector3			GetRotationAngle() const;

	void			UpdateSimulation(uint32_t simTimeMS);


private:

	int32_t	m_LastSimulatedUpdateTime;
    uint32_t            m_TimeBaseline;
	SimMotorModule*		m_pMotors[RobotModel::kNumWheels];
	SimEncoderModule*	m_pEncoders[RobotModel::kNumWheels];
	SimWheelModule*		m_pWheels[RobotModel::kNumWheels];
	SimGyroModule*		m_pGyro;
	SimRobotBody*		m_pRobotBody;
	SimulatedField*		m_field;
};

#endif
