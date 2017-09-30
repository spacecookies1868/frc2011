/*
 *  TubeRing.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TubeRing.h"

static const int CIRCLE_RESOLUTION=16;
static const float TUBE_WIDTH=.737;
static const float TUBE_HEIGHT=.165;
static const float TUBE_THICKNESS=.203;
static const float TUBE_INNER_DIAMETER=TUBE_WIDTH-2*TUBE_THICKNESS;
static const Prism inner(TUBE_INNER_DIAMETER,TUBE_INNER_DIAMETER,TUBE_HEIGHT,CIRCLE_RESOLUTION);
static const Prism outer(TUBE_WIDTH,TUBE_WIDTH,TUBE_HEIGHT,CIRCLE_RESOLUTION);
static const std::string DEFAULT_NAME="Ring Tube";

TubeRing::TubeRing(Vector3 pos, Vector3 angle, std::string descriptor)
: Tube(outer,inner,pos,angle,descriptor)
{
	m_uid=FieldObject::tubering;
}