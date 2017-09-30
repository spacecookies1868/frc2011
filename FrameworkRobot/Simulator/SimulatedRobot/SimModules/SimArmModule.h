/*
 *  SimArmModule.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/23/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIM_ARM_MODULE_H
#define SIM_ARM_MODULE_H

#include "FieldObject/FieldObject.h"
#include "FieldObject/Components/Prism.h"

class SimElevatorModule;
class SimRobotBody;
class Tube;

class SimArmModule : public SimModuleBase
{
public:
	SimArmModule(SimElevatorModule* elevator);
	~SimArmModule() {}
	void GrabTube(Tube* tube) {m_tube=tube;}
	void ReleaseTube() {m_tube=NULL;}
	void Update(uint32_t simTimeMS, double delta_simTimeSec);
	
private:
	SimElevatorModule* m_elevator;
	SimRobotBody* m_body;
	Tube* m_tube;
	Vector3 m_origTubePosDiff;
	Vector3 m_origTubeAngleDiff;
	int m_polyhedronindex;
	int m_grabberindex;
};
#endif