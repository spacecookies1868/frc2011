/*
 *  SimGyroModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_GYRO_MODULE_H
#define SIM_GYRO_MODULE_H

#include "SimulatedRobot/SimModules/SimModuleBase.h"
#include "SimulatedRobot/SimModules/SimRobotBody.h"
#include "Shared/Matrix3x3.h"

class SimGyroModule : public SimModuleBase
{
public:
	SimGyroModule(SimRobotBody* body, Vector3 normal);
	~SimGyroModule();
	void FillLookup();
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	const Vector3 GetGyroValue() {return m_prevAngle-m_initAngle;}
	void ClearGyro();

private:
	Vector3 GetOffsetAngle(Vector3 vect);
	Vector3 GetNormalAngle(Vector3 vect);
	SimRobotBody* m_body;
	Vector3 m_normal;
	Vector3 m_robotNormal;
	Vector3 m_initAngle;
	Vector3 m_prevAngle;
	
	//offset values
	Vector3 m_alpha;
	Matrix3x3 m_rotng;
	Matrix3x3 m_rotgn;
	int m_lookupRes;
	double* m_lookup[2];
	
};
#endif
