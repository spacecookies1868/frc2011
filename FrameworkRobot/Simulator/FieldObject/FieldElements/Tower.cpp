/*
 *  Tower.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tower.h"
#include "FieldObject/Components/Prism.h"

static const int CIRCLE_RESOLUTION=16;
static const float BASE_WIDTH=.76;
static const float BASE_HEIGHT=.30;
static const float POLE_HEIGHT=3.1;
static const float POLE_WIDTH=.044;
static const Prism base(BASE_WIDTH,BASE_WIDTH,BASE_HEIGHT,CIRCLE_RESOLUTION);
static const Prism pole(POLE_WIDTH,POLE_WIDTH,POLE_HEIGHT,CIRCLE_RESOLUTION);
static const std::string DEFAULT_STRING="Tower";

Tower::Tower(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor)
: FieldObject(base,pos,angle,alliance,descriptor)
{
	addPolyhedron(pole, Vector3(0,0,BASE_HEIGHT/2), Vector3(0,0,0));
	m_uid=FieldObject::tower;
	init_bottom();
}