/*
 *  SimWheelModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_WHEEL_MODULE_H
#define SIM_WHEEL_MODULE_H

#include "SimulatedRobot/SimModules/SimModuleBase.h"
#include "SimulatedRobot/SimModules/Interfaces/DistanceVectorInterface.h"
#include "SimulatedRobot/SimModules/Interfaces/RotationInterface.h"
#include "Shared/Vector3.h"
#include "SimulatedRobot/SimulatedField.h"

class SimWheelModule : public SimModuleBase
{
public:
	SimWheelModule(RotationInterface* sourceRot, double wheelRadius, const Vector3& wheelDir, double accelFactor, double friction);
	~SimWheelModule() {}
	// could update this class to handle skidding, accel, and friction
	const double& GetRotationValue() const	{ return m_rotVal; }
	Vector3	GetDistanceTraveled() const;
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	void UpdateFriction(double frict) { m_currFrict=frict; }
	RotationInterface*	GetRotationInterface() const	{ return m_pRotWrapper;}
	DistanceVectorInterface*	GetDistanceVectorInterface() const	{ return m_pDistanceWrapper;}
	
private:
	class RotationWrapper : public RotationInterface
	{
	public:
		RotationWrapper(SimWheelModule* pWheelModule)
		: m_pWheelModule(pWheelModule)
		{	m_sources.push_back(pWheelModule);
		}
		
		virtual ~RotationWrapper() {}
		
		virtual void Update(uint32_t simTimeMS, double delta_simTimeSec) {}
		
		virtual const double GetRotationValue() const	{ return m_pWheelModule->GetRotationValue(); }
		
	private:
		SimWheelModule*	m_pWheelModule;
	};
	
	class DistanceWrapper : public DistanceVectorInterface
	{
	public:
		DistanceWrapper(SimWheelModule* pWheelModule)
		: m_pWheelModule(pWheelModule)
		{	m_sources.push_back(pWheelModule);
		}
		
		virtual ~DistanceWrapper() {}
		
		virtual void Update(uint32_t simTimeMS, double delta_simTimeSec) {}
		
		virtual const Vector3 GetDistanceVector() const {	return m_pWheelModule->GetDistanceTraveled(); }
		
	private:
		SimWheelModule*	m_pWheelModule;
	};
	
	RotationInterface*	m_rotSource;
	SimulatedField*		m_field;
	double				m_wheelRadius;
	double				m_currFrict;
	double				m_rotVal;
	double				m_accel;
	Vector3				m_wheelDirection;
	RotationWrapper*	m_pRotWrapper;
	DistanceWrapper*	m_pDistanceWrapper;
};
#endif