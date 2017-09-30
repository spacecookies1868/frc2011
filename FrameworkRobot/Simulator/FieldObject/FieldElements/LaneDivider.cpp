/*
 *  LaneDivider.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "LaneDivider.h"
#include "FieldObject/Components/Prism.h"
#include <string>

//NEED DIMENSIONS

static const float WIDTH=0;
static const float LENGTH=0;
static const float HEIGHT=0;
static const Prism prism(LENGTH,WIDTH,HEIGHT,4);
static const std::string DEFAULT_STRING="Lane Divider";

LaneDivider::LaneDivider(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor)
: FieldObject(prism, pos, angle, alliance, descriptor)
{
	m_uid=FieldObject::lanedivider;
}