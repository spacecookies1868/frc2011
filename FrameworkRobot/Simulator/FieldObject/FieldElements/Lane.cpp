/*
 *  Lane.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Lane.h"
#include "FieldObject/Components/Prism.h"

static const float WIDTH=5.94;
static const float LENGTH=1.30;
static const float HEIGHT=.1;
static const Prism prism(LENGTH,WIDTH,HEIGHT,4);
static const std::string DEFAULT_STRING="Lane";

Lane::Lane(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor)
: FieldObject(prism, pos, angle, alliance, descriptor)
{
	m_uid=FieldObject::lane;
}