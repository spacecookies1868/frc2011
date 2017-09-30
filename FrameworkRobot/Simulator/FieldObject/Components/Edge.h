/*
 *  Edge.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef EDGE_H
#define EDGE_H

#include "Shared/Vector3.h"
#include <vector>

/**
 * Represents an edge (a line segment connecting two points).
 *
 * @author Eric Bakan
 */
class Edge {
public:	
	Edge(Vector3* ends);
	/**
	 * Creates an edge with specified endpoints.
	 * @param e1 the first endpoint of the edge.
	 * @param e2 the second endpoint of the edge.
	 */
	Edge(Vector3 e1, Vector3 e2);
	
	Edge() {};
	
	~Edge() {}
	/**
	 * Creates a vector with its initial point on the first endpoint
	 * and its terminal point on the second.
	 * @return a vector representing this edge
	 */
	Vector3 GetValue() const;
	
	const Vector3* GetEnds() const {return m_ends;}
	
private:
	/**
	 * The first and second endpoint of the edge.
	 */
	Vector3 m_ends[2];	
};

#endif
