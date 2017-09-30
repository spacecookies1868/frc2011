/*
 *  Vector3.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 11/27/10.
 *
 */

#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

/**
 * Representation of a three-tuple vector.
 *
 * @author Dmitriy Solomonov
 */
class Vector3
{
public:

    /**
     * Default constructor, initializing all elements to 0.
     */
	explicit Vector3();

    /**
     * Constructor that creates a copy of another vector.
     *
     * @param inVector the vector to copy.
     */
	Vector3(const Vector3& inVector);

    /**
     * Constructor creating a vector with supplied values.
     *
     * @param _x the x-value of the vector
     * @param _y the y-value of the vector
     * @param _z the z-value of the vector
     */
	explicit Vector3(double _x, double _y, double _z);

    /**
     * Adds this vector to another.
     *
     * Vector addition is defined elementwise.
     * e.g. <x1,y1,z1> + <x2,y2,z2> = <x1+x2,y1+y2,z1+z2>
     *
     * @param rhs the other vector to add
     * @return the sum of the vectors
     */
	Vector3 operator+ (const Vector3& rhs) const;

    /**
     * Subtracts the supplied vector from this.
     *
     * Equivalent to adding negative values of the vector.
     *
     * @param rhs the vector to subtract from this
     * @return the difference of the vectors
     */
	Vector3 operator- (const Vector3& rhs) const;

    /**
     * Multiplies this vector by a scalar.
     *
     * e.g. a * <x,y,z> = <a*x, a*y, a*z>
     *
     * @param rhs the scalar to multiply by
     * @return the resultant vector
     */
	Vector3 operator* (double rhs) const;

    /**
     * Divides this vector by a scalar.
     *
     * Equivalent to multiplying by the scalar's reciprocal.
     *
     * @param rhs the scalar to divide by
     * @return the resultant vector
     */
	Vector3 operator/ (double rhs) const;

	/**
	 * Checks if two vectors are equal.
	 *
	 * Two vectors are equal if and only if all elements are equal.
	 *
	 * @param rhs the vector to compare this with
	 * @return whether the vectors are equal
	 */
	bool operator== (const Vector3& rhs) const;

	/**
	 * Checks if two vectors are not equal.
	 *
	 * Returns the opposite of operator==.
	 *
	 * @param rhs the vector to compare this with
	 * @return !operator===
	 */
	bool operator!= (const Vector3& rhs) const;

    /**
     * Calculates the dot product of two vectors.
     *
     * A dot product is a scalar that results from the sum of the products of each element in
     * the vector and its corresponding element.
     * e.g. <x, y, z> dot <a, b, c> = ax + by + cz
     *
     * @param rhs the vector to take the dot product with
     * @return the dot product
     */
	double dotProd(const Vector3& rhs) const;

    /**
     * Calculates the cross product of two vectors.
     *
     * A cross product is the vector that results from "cross-multiplying" unlike elements.
     * i.e. <x, y, z> cross <a, b, c> = <bz + cy, az + cx, ay + bx>
     *
     * @param rhs the vector to take the cross product with
     * @return the cross-product vector
     */
	Vector3 crossProd(const Vector3& rhs) const;

    /**
     * Determines the magnitude of a vector.
     *
     * The magnitude is equivalent to the square root of the sum of the elements in the
     * vector.
     * e.g. ||<x, y, z>|| = sqrt(x^2 + y^2 + z^2)
     *
     * @return the magnitude of a vector
     */
	double  magnitude() const;

    /**
     * Returns the normalized unit vector.
     *
     * The unit vector is determined by dividing this vector by its magnitude.
     * All unit vectors have a magnitude of 1.
     *
     * @return the normalized vector
     */
	Vector3 normalize() const;

	/**
	 * Prints the values of the vector.
	 */
	void	printVals() const;

    /**
     * Adds a vector to this.
     *
     * @param inVec a vector to add
     * @return the sum of the vectors
     */
	inline Vector3& operator+= (const Vector3& inVec)
    {
        *this = *this + inVec;
        return *this;
    }

    /**
     * Subtracts a vector from this.
     *
     * @param inVec a vector to add
     * @return the difference of the vectors
     */
	inline Vector3& operator-= (const Vector3& inVec)
    {
        *this = *this - inVec;
        return *this;
    }

    /**
     * The x-value of the vector.
     */
    double x;

    /**
     * The y-value of the vector.
     */
    double y;

    /**
     * The z-value of the vector.
     */
    double z;
};

#endif

