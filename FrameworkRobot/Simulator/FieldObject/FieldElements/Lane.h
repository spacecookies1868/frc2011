/*
 *  Lane.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef LANE_H
#define LANE_H

#include "FieldObject/FieldObject.h"

/**
 * An object simulating a lane.
 *
 * @author Eric Bakan
 */
class Lane : public FieldObject {
public:
	/**
	 * Creates a Lane object with specified position, angle, alliance, and descriptor.
	 *
	 * @param pos the position of the object
	 * @param angle the angle the object is facing along
	 * @param alliance the alliance of the object
	 * @param descriptor a description of the object
	 */
	Lane(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor);
	
	~Lane() {}
};

#endif
