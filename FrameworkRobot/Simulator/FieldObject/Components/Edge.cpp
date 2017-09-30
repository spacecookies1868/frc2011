/*
 *  Edge.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Edge.h"
#include "Shared/Vector3.h"
#include <math.h>
#include <assert.h>

Edge::Edge(Vector3* ends)
{
	m_ends[0]=ends[0];
	m_ends[1]=ends[1];
}

Edge::Edge(Vector3 e1, Vector3 e2)
{
	m_ends[0]=e1;
	m_ends[1]=e2;
}

Vector3 Edge::GetValue() const
{
	return m_ends[1]-m_ends[0];
}