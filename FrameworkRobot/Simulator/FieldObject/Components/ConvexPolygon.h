/*
 *  ConvexPolygon.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONVEX_POLYGON_H
#define CONVEX_POLYGON_H

#include "FieldObject/Components/Edge.h"
#include <vector>

class Vector3;
class Edge;

/**
 * Simulates a convex polygon.
 *
 * A convex polygon is a two-dimensional figure such that a line connecting each
 * interior point to another is in the interior of the figure.
 * @author Eric Bakan
 */
class ConvexPolygon {
public:
	/**
	 * Creates a ConvexPolygon with the specified vertices.
	 * @param the coordinates of the vertices of the ConvexPolygon
	 */
	ConvexPolygon(std::vector<Vector3> vertices);

	/**
	 * Default constructor.
	 */
	ConvexPolygon() {}
	~ConvexPolygon() {};

	/**
	 * Determines whether or not two convex polygons intersect.
	 * @param the other convex polygon to check for intersection
	 * @return true if this is intersecting p
	 */
	bool IsIntersecting(const ConvexPolygon& p) const;
	
	/**
	 * Calculates and returns the center of this polygon.
	 * @return coordinates indicating the center of the polygon
	 */
	Vector3 GetCenter() const;

	/**
	 * Calculates the normal vector of this polygon.
	 * @return the normal vector from this polygon
	 */
	Vector3	GetNormal()	const;
	const std::vector<Vector3>&	GetVertices() const {return m_vertices;}
	const std::vector<Edge>& GetEdges() const {return m_edges;}
	
private:
	/**
	 * A list of the vertices in this polygon.
	 */
	std::vector<Vector3> m_vertices;
	
	/**
	 * A list of the edges in this polygon.
	 */
	std::vector<Edge> m_edges;
	
};
#endif
