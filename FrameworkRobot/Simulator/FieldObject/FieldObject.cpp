/*
 *  FieldObject.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */


#include "FieldObject/FieldObject.h"
#include "Shared/Vector3.h"
#include "Shared/Matrix3x3.h"

#include "FieldObject/Components/ConvexPolyhedron.h"
#include "FieldObject/Components/ConvexPolygon.h"
#include "SimulatedRobot/SimModules/SimRobotBody.h"

#include <vector>

FieldObject::FieldObject()
{
	init_top();
	init_bottom();
}

FieldObject::FieldObject(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor)
{
	init_top();
	m_pos=pos;
	m_angle=angle;
	m_descriptor=descriptor;
	m_alliance=alliance;
	init_bottom();
}

FieldObject::FieldObject(ConvexPolyhedron polyhedron, Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor)
{
	init_top();
	addPolyhedron(polyhedron, Vector3(0,0,0), Vector3(0,0,0));
	m_pos=pos;
	m_angle=angle;
	m_descriptor=descriptor;
	m_alliance=alliance;
	init_bottom();
}

FieldObject::~FieldObject()
{
	delete m_simModule;
}

bool FieldObject::IsIntersecting(FieldObject& rhs)
{
	if(m_static && rhs.m_static) //don't check if both are static
		return false;
	return Intersects(rhs) || rhs.Intersects(*this);
}

void FieldObject::HandleCollision(FieldObject& rhs)
{
	printf("Collision detected between %s and %s\n",m_descriptor.c_str(),rhs.m_descriptor.c_str());
}

const std::vector<ConvexPolyhedron>& FieldObject::GetAbsolutePolyhedra()
{
	if(!m_static)
		for(int i=0;i<m_polyhedra.size();i++)
			m_prevpolyhedra[i]=m_polyhedra[i].GetAbsolutePolyhedron(m_pos+m_relativeposs[i], m_angle+m_relativeangles[i]);
		
	return m_prevpolyhedra;
}

bool FieldObject::Intersects(FieldObject& rhs)
{
	const std::vector<ConvexPolyhedron>& myPolyhedra=GetAbsolutePolyhedra();
	const std::vector<ConvexPolyhedron>& rhsPolyhedra=rhs.GetAbsolutePolyhedra();
	for(int i=0;i<myPolyhedra.size();i++)
		for(int j=0;j<rhsPolyhedra.size();j++)
			if(myPolyhedra[i].IsIntersecting(rhsPolyhedra[j]))
				return true;
	return false;
}

void FieldObject::Update(uint32_t simTimeMS, double delta_simTimeSec)
{
	m_pos+=m_vel*delta_simTimeSec;
	m_angle+=m_rotvel*delta_simTimeSec;
}

void FieldObject::init_top()
{
	m_pos=Vector3(0,0,0);
	m_angle=Vector3(0,0,0);
	m_vel=Vector3(0,0,0);
	m_rotvel=Vector3(0,0,0);
	m_static=1;
	m_alliance=neutral;
	m_uid=fieldobject;
	m_descriptor="Field Object";
	m_simModule=new SimModuleWrapper(this);
}

void FieldObject::init_bottom()
{
	m_prevpolyhedra.clear();
	for(int i=0;i<m_polyhedra.size();i++)
		m_prevpolyhedra.push_back(m_polyhedra[i].GetAbsolutePolyhedron(m_pos,m_angle));
}

int FieldObject::addPolyhedron(const ConvexPolyhedron& polyhedron, const Vector3& pos, const Vector3& angle)
{
	m_polyhedra.push_back(polyhedron);
	m_relativeposs.push_back(pos);
	m_relativeangles.push_back(angle);
	return m_polyhedra.size()-1;
}
