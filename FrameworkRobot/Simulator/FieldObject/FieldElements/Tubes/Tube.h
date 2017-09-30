/*
 *  Tube.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/19/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TUBE_H
#define TUBE_H

#include "FieldObject/FieldObject.h"

/**
 * An object simulating a tube.
 *
 * @author Eric Bakan
 */
class Tube : public FieldObject {
public:
	/**
	 * Creates a Tube object with specified outer and inner polyhedrons as well as
	 * position, angle, and descriptor.
	 *
	 * @param outer the outer polyhedron of the tube
	 * @param inner the inner polyhedron of the tube
	 * @param pos the position of the object
	 * @param angle the angle the object is facing along
	 * @param descriptor a description of the object
	 */
	Tube(ConvexPolyhedron outer, ConvexPolyhedron inner, Vector3 pos, Vector3 angle, std::string descriptor);
	virtual ~Tube() {}
protected:
	virtual bool Intersects(FieldObject& rhs);
	friend class SimArmModule;
};
#endif
