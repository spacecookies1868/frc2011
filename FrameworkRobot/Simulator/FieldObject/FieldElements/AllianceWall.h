/*
 *  AllianceWall.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/19/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ALLIANCE_WALL_H
#define ALLIANCE_WALL_H

#include "FieldObject/FieldObject.h"

/**
 * An object simulating an alliance's wall.
 *
 * @author Eric Bakan
 */
class AllianceWall : public FieldObject {
public:	
	/**
	 * Creates a AllianceWall object with specified position, angle, alliance, and descriptor.
	 *
	 * @param pos the position of the object
	 * @param angle the angle the object is facing along
	 * @param alliance the alliance of the object
	 * @param descriptor a description of the object
	 */
	AllianceWall(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor);
	~AllianceWall() {};
};

#endif
