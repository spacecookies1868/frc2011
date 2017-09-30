/*
 *  AllianceWall.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/19/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AllianceWall.h"
#include "FieldObject/Components/Prism.h"

static const float WIDTH=.05;
static const float LENGTH=8.23;
static const float HEIGHT=1.98;
static Prism prism(LENGTH,WIDTH,HEIGHT,4);
static const std::string DEFAULT_NAME="Alliance Wall";

AllianceWall::AllianceWall(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor)
: FieldObject(prism, pos, angle, alliance, descriptor)
{
	m_uid=FieldObject::alliancewall;
}