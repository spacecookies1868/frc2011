/*
 *  ConvexPolygon.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "FieldObject/Components/ConvexPolygon.h"
#include "Shared/Vector3.h"
#include "FieldObject/Components/Edge.h"

ConvexPolygon::ConvexPolygon(std::vector<Vector3> vertices)
: m_vertices(vertices)
{
	for(int i=0;i<m_vertices.size();i++)
		m_edges.push_back(Edge(m_vertices[i],m_vertices[(i+1)%m_vertices.size()]));
}

bool ConvexPolygon::IsIntersecting(const ConvexPolygon& p) const
{
	//NOTE: only checks vertices at the moment :(
	const std::vector<Edge>& myEdges=m_edges;
	const std::vector<Vector3>& pVertices=p.m_vertices;
	
	for(int i=0;i<pVertices.size();i++) {
		const Vector3& vertex=pVertices[i];
		bool isIntersecting=true;
		for(int j=0;j<myEdges.size();j++) {
			const Vector3& edge=myEdges[j].GetValue();
			double max=edge.dotProd(myEdges[j].GetEnds()[0]);
			double min=edge.dotProd(myEdges[j].GetEnds()[1]);
			if(max<min) {
				double tmp=max;
				max=min;
				min=tmp;
			}
			double dotprod=vertex.dotProd(edge);
			if((dotprod<min && fabs(fabs(dotprod)-fabs(min))>.01) || (dotprod>max && fabs(fabs(dotprod)-fabs(max))>.01)) {
				isIntersecting=false;
				j=myEdges.size();
			}
		}
		if(isIntersecting)
			return true;
	}
	return false;
}

Vector3	ConvexPolygon::GetCenter() const
{
	Vector3 sum(0,0,0);
	for(int i=0;i<m_vertices.size();i++)
		sum+=m_vertices[i];
	return sum/m_vertices.size();
}


Vector3 ConvexPolygon::GetNormal() const
{
	return m_edges[0].GetValue().crossProd(m_edges[1].GetValue()).normalize();
}
