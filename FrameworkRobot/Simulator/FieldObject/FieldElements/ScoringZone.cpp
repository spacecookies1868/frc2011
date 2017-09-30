/*
 *  ScoringZone.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ScoringZone.h"
#include "FieldObject/Components/Prism.h"

static const float WIDTH=2.13;
static const float LENGTH=5.49;
static const float HEIGHT=.1;
static const Prism prism(LENGTH,WIDTH,HEIGHT,4);
static const std::string DEFAULT_STRING="Scoring Zone";

ScoringZone::ScoringZone(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor)
: FieldObject(prism, pos, angle, alliance, descriptor)
{
	m_uid=FieldObject::scoringzone;
}