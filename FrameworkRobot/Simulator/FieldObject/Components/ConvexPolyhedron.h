/*
 *  ConvexPolyhedron.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONVEX_POLYHEDRON_H
#define CONVEX_POLYHEDRON_H

#include "ConvexPolygon.h"
#include <vector>

class Vector3;
class Edge;

/**
 * Simulates a convex polyhedron.
 *
 * A convex polyhedron is a three-dimensional figure such that a line connecting each
 * interior point to another is in the interior of the figure.
 * @author Eric Bakan
 */
class ConvexPolyhedron {
public:
	/**
	 * Creates a convex polyhedron that is a copy of the supplied convex polyhedron.
	 * @param polyhedron the polyhedron to copy
	 */
	ConvexPolyhedron(const ConvexPolyhedron& polyhedron);
	ConvexPolyhedron(std::vector<Vector3> vertices, std::vector<Edge> edges);

	/**
	 * Creates a convex polyhedron with the specified vertices and edges.
	 * @param vertices the vertices of the polyhedron
	 * @param edges the edges of the polyhedron
	 */
	ConvexPolyhedron () {}
	virtual ~ConvexPolyhedron() {}
	
	/**
	 * Gets absolute coordinates of a polyhedron given a position and an angle
	 * @param pos position of polyhedron
	 * @param angle angle of polyhedron
	 */
	ConvexPolyhedron GetAbsolutePolyhedron(Vector3 pos, Vector3 angle);
	
	/**
	 * Determines whether two convex polyhedrons are intersecting
	 * @param p the polyhedron to check
	 * @return whether this and the supplied polyhedron intersect
	 */
	bool IsIntersecting(const ConvexPolyhedron& p) const;
	
	/**
	 * Calculates and returns the center of this polyhedron.
	 * @return coordinates indicating the center of the polyhedrons
	 */
	Vector3 GetCenter() const;
	const std::vector<Vector3>& GetVertices() const {return m_vertices;}
	const std::vector<Edge>& GetEdges() const {return m_edges;}
	const ConvexPolygon& GetBirdsEye() const {return m_birdsEye;}
	void setBirdsEye(ConvexPolygon face);
	
protected:	
	/**
	 * The polygon's list of vertices.
	 */
	std::vector<Vector3>		m_vertices;
	
	/**
	 * The polygon's list of edges.
	 */
	std::vector<Edge>			m_edges;
	
	//std::vector<ConvexPolygon>	m_faces;
	/**
	 * A ConvexPolygon representing the top view of this.
	 */
	ConvexPolygon				m_birdsEye;
};

#endif
