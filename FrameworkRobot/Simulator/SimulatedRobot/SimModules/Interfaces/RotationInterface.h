/*
 *  RotationInterface.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ROTATION_INTERFACE_H
#define ROTATION_INTERFACE_H

#include "SimulatedRobot/SimModules/SimModuleBase.h"

// interface for modules that have a rotational property, measured in amount of rotation
class RotationInterface : public SimModuleBase
{
public:
	virtual const double GetRotationValue() const = 0;
	virtual ~RotationInterface() {}
};
#endif