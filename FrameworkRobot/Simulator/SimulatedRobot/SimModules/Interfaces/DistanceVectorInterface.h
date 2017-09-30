/*
 *  DistanceVectorInterface.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef DISTANCE_VECTOR_INTERFACE_H
#define DISTANCE_VECTOR_INTERFACE_H

#include "SimulatedRobot/SimModules/SimModuleBase.h"
#include "Shared/Vector3.h"

// interface for modules that have a vector property
class DistanceVectorInterface : public SimModuleBase
{
public:
	virtual const Vector3 GetDistanceVector() const = 0;
	virtual ~DistanceVectorInterface() {}
};
#endif