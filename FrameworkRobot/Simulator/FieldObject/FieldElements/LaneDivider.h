/*
 *  LaneDivider.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef LANE_DIVIDER_H
#define LANE_DIVIDER_H

#include "FieldObject/FieldObject.h"

/**
 * An object simulating a lane divider.
 *
 * @author Eric Bakan
 */
class LaneDivider : public FieldObject {
public:

	/**
	 * Creates a LaneDivider object with specified position, angle, alliance, and descriptor.
	 *
	 * @param pos the position of the object
	 * @param angle the angle the object is facing along
	 * @param alliance the alliance of the object
	 * @param descriptor a description of the object
	 */
	LaneDivider(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor);
	
	~LaneDivider() {}
};

#endif
