/*
 *  ConvexPolyhedron.cpp
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "FieldObject/Components/ConvexPolyhedron.h"
#include "Shared/Vector3.h"
#include "FieldObject/Components/Edge.h"
#include "Shared/Matrix3x3.h"
#include <vector>
#include <assert.h>

ConvexPolyhedron::ConvexPolyhedron(const ConvexPolyhedron& polyhedron)
: m_vertices(polyhedron.m_vertices)
, m_edges(polyhedron.m_edges)
//, m_faces(polyhedron.m_faces)
, m_birdsEye(polyhedron.m_birdsEye)
{
}
ConvexPolyhedron::ConvexPolyhedron(std::vector<Vector3> vertices, std::vector<Edge> edges)
: m_vertices(vertices)
, m_edges(edges)
{
}

ConvexPolyhedron ConvexPolyhedron::GetAbsolutePolyhedron(Vector3 pos, Vector3 angle) {
	
	//add in x and y angle multiplication
	Matrix3x3 rotMat=Matrix3x3::RotMatrix(angle.z, Vector3(0,0,1));
	
	//vertices
	std::vector<Vector3> vertices;
	std::vector<Edge> edges;
	
	//shift vertices
	for(int i=0;i<m_vertices.size();i++)
		vertices.push_back(rotMat*m_vertices[i]+pos);
	
	//shift edges
	for(int i=0;i<m_edges.size();i++) { //for each face
		Vector3 ends[2];
		for(int j=0;j<2;j++)
			ends[j]=rotMat*m_edges[i].GetEnds()[j]+pos;
		edges.push_back(Edge(ends));
	}
	
	return ConvexPolyhedron(vertices,edges);
}

bool ConvexPolyhedron::IsIntersecting(const ConvexPolyhedron& p) const
{
	//TODO: AABB COLLISIONS
	
	//EBAKAN IMPLEMENTATION
	
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

Vector3	ConvexPolyhedron::GetCenter() const
{
	Vector3 sum(0,0,0);
	for(int i=0;i<m_vertices.size();i++)
		sum+=m_vertices[i];
	return sum/m_vertices.size();
}

void ConvexPolyhedron::setBirdsEye(ConvexPolygon face)
{
	m_birdsEye=face;
}