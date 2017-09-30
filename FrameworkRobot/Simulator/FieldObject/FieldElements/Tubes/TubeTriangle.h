/*
 *  TubeTriangle.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TUBE_TRIANGLE_H
#define TUBE_TRIANGLE_H

#include "FieldObject/FieldELements/Tubes/Tube.h"
#include "FieldObject/Components/Prism.h"

/**
 * An object simulating a tube triangle.
 *
 * @author Eric Bakan
 */
class TubeTriangle : public Tube
{
public:
	/**
	 * Creates a TubeTriangle object with specified position, angle, and descriptor.
	 *
	 * @param pos the position of the object
	 * @param angle the angle the object is facing along
	 * @param descriptor a description of the object
	 */
	TubeTriangle(Vector3 pos, Vector3 angle, std::string descriptor);
	~TubeTriangle() {}
};
#endif
