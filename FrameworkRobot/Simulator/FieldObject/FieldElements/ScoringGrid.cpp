/*
 *  ScoringGrid.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ScoringGrid.h"
#include "FieldObject/Components/Prism.h"

static const float TUBE_DIAMETER=.042;
static const float SPACING_WIDTH=.76;
static const float HEIGHT_STAGGER=.20;
static const float PEG_LENGTH=.41;
static const float STARTING_HEIGHT=.76;
static const float HEIGHT_SPACING=.97;
static const float CIRCLE_RESOLUTION=16;
static const Prism sidepole(TUBE_DIAMETER,TUBE_DIAMETER,STARTING_HEIGHT+HEIGHT_SPACING*2,CIRCLE_RESOLUTION);
static const Prism centerpole(TUBE_DIAMETER,TUBE_DIAMETER,STARTING_HEIGHT+HEIGHT_SPACING*2+HEIGHT_STAGGER,CIRCLE_RESOLUTION);
static const Prism peg(TUBE_DIAMETER,TUBE_DIAMETER,PEG_LENGTH,CIRCLE_RESOLUTION);
static const std::string DEFAULT_STRING="Scoring Grid";

ScoringGrid::ScoringGrid(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor)
: FieldObject(pos,angle,alliance,descriptor)
{
	m_uid=FieldObject::scoringgrid;
	
	//main poles
	for(int i=0;i<3;i++) {
		const Prism& pole = i%2?centerpole:sidepole; //adds sidepole, centerpole, sidepole
		addPolyhedron(pole, Vector3(0,SPACING_WIDTH*(1-i),0), Vector3(0,0,0)); //properly spaces poles
	}
	
	//pegs
	for(int i=0;i<3;i++) {
		double ypos=SPACING_WIDTH*(1-i); //sets proper y position based on which pole
		double height=STARTING_HEIGHT;
		if(i==1) height+=HEIGHT_STAGGER; //if center pole
		for(int j=0;j<3;j++)
			addPolyhedron(peg, Vector3(0,ypos,height+HEIGHT_SPACING*j),Vector3(0,M_PI/2,0));
	}
	
	//Set all the right birds eye polygons for the pegs
	for(int i=3;i<m_polyhedra.size();i++) {
		const ConvexPolyhedron& polyhedra=m_polyhedra[i];
		const std::vector<Vector3>& vertices=polyhedra.GetVertices();
		std::vector<Vector3> facevertices;
		int numFaceVertices=vertices.size()/3;
		for(int j=0;j<numFaceVertices;j++)
			facevertices.push_back(vertices[j+numFaceVertices]);
		m_polyhedra[i].setBirdsEye(ConvexPolygon(facevertices));
	}
	init_bottom();
}