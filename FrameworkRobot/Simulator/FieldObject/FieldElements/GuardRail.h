/*
 *  GuardRail.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GUARD_RAIL_H
#define GUARD_RAIL_H

#include "FieldObject/FieldObject.h"

/**
 * An object simulating a guard rail.
 *
 * @author Eric Bakan
 */
class GuardRail : public FieldObject {
public:
	/**
	 * Creates a GuardRail object with specified position, angle, alliance, and descriptor.
	 *
	 * @param pos the position of the object
	 * @param angle the angle the object is facing along
	 * @param alliance the alliance of the object
	 * @param descriptor a description of the object
	 */
	GuardRail(Vector3 pos, Vector3 angle, std::string descriptor);
	
	~GuardRail() {};
};
#endif
