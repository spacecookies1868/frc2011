/*
 *  TubeTriangle.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TubeTriangle.h"

static const float TUBE_WIDTH=.787;
static const float TUBE_LENGTH=.724;
static const float TUBE_HEIGHT=.152;
static const float TUBE_INNER_WIDTH=.241;
static const float TUBE_INNER_LENGTH=.229;
static const Prism inner(TUBE_INNER_LENGTH,TUBE_INNER_WIDTH,TUBE_HEIGHT,3);
static const Prism outer(TUBE_LENGTH,TUBE_WIDTH,TUBE_HEIGHT,3);
static const std::string DEFAULT_NAME="Triangle Tube";

TubeTriangle::TubeTriangle(Vector3 pos, Vector3 angle, std::string descriptor)
: Tube(outer,inner,pos,angle,descriptor)
{
	m_uid=FieldObject::tubetriangle;
}