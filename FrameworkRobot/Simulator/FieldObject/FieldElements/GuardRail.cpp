/*
 *  GuardRail.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "GuardRail.h"
#include "FieldObject/Components/Prism.h"

static const float WIDTH=16.46;
static const float LENGTH=.05;
static const float HEIGHT=.51;
static const Prism prism(LENGTH,WIDTH,HEIGHT,4);
static const std::string DEFAULT_STRING="Guard Rail";

GuardRail::GuardRail(Vector3 pos, Vector3 angle, std::string descriptor)
: FieldObject(prism, pos, angle, FieldObject::neutral, descriptor)
{
	m_uid=FieldObject::guardrail;
}