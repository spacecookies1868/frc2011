/*
 *  Prism.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Prism.h"
#include <math.h>
#include <assert.h>
Prism::Prism()
: m_length(1)
, m_width(1)
, m_height(1)
{
	init();
}

Prism::Prism(double length, double width, double height, int numFaceVertices)
: m_length(length)
, m_width(width)
, m_height(height)
, m_numFaceVertices(numFaceVertices)
{
	init();
}

void Prism::init()
{
	m_vertices.clear();
	m_edges.clear();
	//vars
	double halfwidth=m_width/2;
	double halflength=m_length/2;
	double increment=2*M_PI/m_numFaceVertices;
	double rectAng=m_numFaceVertices==4?M_PI/4:M_PI/2; //gives proper rotation
	double rectFact=m_numFaceVertices==4?2/sqrt(2):1; //compensates if drawing a box
	//gen vertices
	for(int i=0;i<m_numFaceVertices*2;i++)
		m_vertices.push_back(Vector3(rectFact*halfwidth*cos(increment*(i%m_numFaceVertices)-rectAng),
									 rectFact*halflength*sin(increment*(i%m_numFaceVertices)-rectAng),
									 i/m_numFaceVertices?0:m_height));
                                     
	
	//optimize later
	//gen edges
	std::vector<Vector3> birdsEyeVertices;
	m_edges.resize(m_numFaceVertices*3);
	for(int i=0;i<m_numFaceVertices;i++) {
		m_edges[i]=Edge(m_vertices[i],m_vertices[(i+1)%m_numFaceVertices]);
		m_edges[i+m_numFaceVertices]=Edge(m_vertices[m_numFaceVertices+i],m_vertices[m_numFaceVertices+(i+1)%m_numFaceVertices]);
		m_edges[i+m_numFaceVertices*2]=Edge(m_vertices[i],m_vertices[i+m_numFaceVertices]);
		birdsEyeVertices.push_back(m_vertices[i]);
	}

	m_birdsEye=ConvexPolygon(birdsEyeVertices);
	//GenBirdsEye();
}