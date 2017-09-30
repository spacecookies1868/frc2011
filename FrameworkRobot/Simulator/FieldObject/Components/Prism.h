/*
 *  Prism.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PRISM_H
#define PRISM_H

#include "FieldObject/Components/ConvexPolyhedron.h"
/**
 * A subclass of ConvexPolyhedron, representing a prism.
 *
 * A prism has an n-sided base, a translated copy of the base, and n parallelogram
 * faces connected them.
 *
 * @author Eric Bakan
 */
class Prism : public ConvexPolyhedron
{
public:
	/**
	 * Default constructor initializing all variables to default values.
	 */
	Prism();
	
	/**
	 * Constructor creating a prism with specified length, width, height, and number of vertices.
	 * @param length length of the prism
	 * @param height height of the prism
	 * @param width of the prism
	 */
	Prism(double length, double width, double height, int numFaceVertices);
	
	~Prism() {}

private:
	/**
	 * Initializes the prism.
	 *
	 * This method generates the vertices and edges of the prism.
	 */
	void	init();
	
	/**
	 * The length of the prism. Default value is 1.
	 */
	double	m_length;
	
	/**
	 * The width of the prism. Default value is 1.
	 */
	double	m_width;
	
	/**
	 * The height of the prism. Default value is 1.
	 */
	double	m_height;
	
	/**
	 * The number of faces in the prism.
	 */
	int		m_numFaceVertices;
};
#endif
