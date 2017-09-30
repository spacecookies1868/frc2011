/*
 *  TubeSquare.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TubeSquare.h"

static const float TUBE_WIDTH=.610;
static const float TUBE_HEIGHT=.203;
static const float TUBE_THICKNESS=.19;
static const float TUBE_INNER_DIAMETER=TUBE_WIDTH-2*TUBE_THICKNESS;
static const Prism inner(TUBE_INNER_DIAMETER,TUBE_INNER_DIAMETER,TUBE_HEIGHT,4);
static const Prism outer(TUBE_WIDTH,TUBE_WIDTH,TUBE_HEIGHT,4);
static const std::string DEFAULT_NAME="Square Tube";

TubeSquare::TubeSquare(Vector3 pos, Vector3 angle, std::string descriptor)
: Tube(outer,inner,pos,angle,descriptor)
{
	m_uid=FieldObject::tubesquare;
}