/*
 *  SimGyroModule.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 12/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SimGyroModule.h"

SimGyroModule::SimGyroModule(SimRobotBody* body, Vector3 normal)
: m_body(body)
, m_initAngle(0,0,0)
, m_prevAngle(0,0,0)
, m_normal(normal)
, m_robotNormal(0,1,0)
, m_alpha(0,0,acos(m_robotNormal.dotProd(m_normal)/(m_robotNormal.magnitude()*m_normal.magnitude())))
, m_rotng(Matrix3x3::RotMatrix(m_alpha.z,m_robotNormal.crossProd(m_normal)))
, m_rotgn(m_rotng.transpose())
, m_lookupRes(1000)
{
	m_sources.push_back(body->GetSimModule());
	FillLookup();
}


void SimGyroModule::Update(uint32_t simTimeMS, double delta_simTimeSec)
{	
	//Vector3 act(0,0,fmod(m_body->GetAngle().z,2*M_PI));
	//Vector3 err=GetOffsetAngle(act);
	//Vector3 deerr=GetNormalAngle(err);
	double randfactor=.05;
	double randNum = (16-(rand() & 0x1F))/32.0;
	double randmult=1+randNum*randfactor;
	m_prevAngle=m_body->GetAngle()*randmult;

}

void SimGyroModule::ClearGyro()
{
	m_initAngle=m_prevAngle;
}

void SimGyroModule::FillLookup()
{
	for(int i=0;i<2;i++)
		m_lookup[i]=new double[m_lookupRes+1];
	
	Vector3 angle(0,0,0);
	double* normAngle=m_lookup[0];
	double* offsetAngle=m_lookup[1];
	double increment=2*M_PI/m_lookupRes;
	
	for(int i=0;i<m_lookupRes;i++) {
		normAngle[i]=increment*i;
		angle.z=normAngle[i];
		offsetAngle[i]=GetOffsetAngle(angle).z;
	}
		
}

Vector3 SimGyroModule::GetOffsetAngle(Vector3 vect)
{
	Vector3 n = m_robotNormal;
	Vector3 g = m_normal;

	Matrix3x3 rot_theta = Matrix3x3::RotMatrix(vect.z, n);
	Matrix3x3 q = m_rotng * rot_theta * m_rotgn;
	Vector3 v=Vector3(1,0,0);
	Vector3 vp = q*v;
	vp.z=0;
	double thetap=acos(vp.dotProd(v)/(vp.magnitude()*v.magnitude()));
	//double thetap=asin(vp.crossProd(v).magnitude()/(vp.magnitude()*v.magnitude()));
	//printf("v':\n%f\t%f\t%f\n",vp.x,vp.y,vp.z);
	
	return Vector3(0,0,thetap);	
}

Vector3 SimGyroModule::GetNormalAngle(Vector3 vect)
{
	double* normAngle=m_lookup[0];
	double* offsetAngle=m_lookup[1];
	double angle=vect.z;
	int index=0;
	while(offsetAngle[index]<angle)
		index++;
	return Vector3(0,0,(normAngle[index-1]+normAngle[index])/2);	
	
}

SimGyroModule::~SimGyroModule()
{
	for(int i=0;i<2;i++)
		free(m_lookup[i]);
}