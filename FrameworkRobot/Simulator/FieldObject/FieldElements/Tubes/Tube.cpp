/*
 *  Tube.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/19/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tube.h"

static const std::string DEFAULT_NAME="Tube";

Tube::Tube(ConvexPolyhedron outer, ConvexPolyhedron inner, Vector3 pos, Vector3 angle, std::string descriptor)
: FieldObject(outer, pos, angle, FieldObject::neutral, descriptor)
{
	addPolyhedron(inner, Vector3(0,0,0), Vector3(0,0,0));
	m_static=0;
	m_uid=FieldObject::tube;
	init_bottom();
}

bool Tube::Intersects(FieldObject& rhs)
{
	std::vector<ConvexPolyhedron> rhsPolyhedra=rhs.GetAbsolutePolyhedra();
	for(int i=0;i<2;i++)
		for(int j=0;j<rhsPolyhedra.size();j++)
			if(rhsPolyhedra[j].IsIntersecting(m_prevpolyhedra[0]) /*&& !rhsPolyhedra[j].IsIntersecting(m_prevpolyhedra[1])*/)
				return true;
	return false;
}