/*
 *  TubeSquare.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TUBE_SQUARE_H
#define TUBE_SQUARE_H

#include "FieldObject/FieldElements/Tubes/Tube.h"
#include "FieldObject/Components/Prism.h"

/**
 * An object simulating a tube square.
 *
 * @author Eric Bakan
 */
class TubeSquare : public Tube
{
public:
	/**
	 * Creates a TubeSquare object with specified position, angle, and descriptor.
	 *
	 * @param pos the position of the object
	 * @param angle the angle the object is facing along
	 * @param descriptor a description of the object
	 */
	TubeSquare(Vector3 pos, Vector3 angle, std::string descriptor);
	~TubeSquare() {}
};
#endif
