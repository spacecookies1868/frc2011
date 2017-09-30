/*
 *  TubeRing.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TUBE_RING_H
#define TUBE_RING_H

#include "FieldObject/FieldELements/Tubes/Tube.h"
#include "FieldObject/Components/Prism.h"

/**
 * An object simulating a tube ring.
 *
 * @author Eric Bakan
 */
class TubeRing : public Tube
{
public:
	/**
	 * Creates a TubeRing object with specified position, angle, and descriptor.
	 *
	 * @param pos the position of the object
	 * @param angle the angle the object is facing along
	 * @param descriptor a description of the object
	 */
	TubeRing(Vector3 pos, Vector3 angle, std::string descriptor);
	~TubeRing() {}
};
#endif
