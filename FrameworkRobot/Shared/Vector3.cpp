/*
 *  Vector3.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 11/27/10.
 *
 */

#include "Shared/CommonIncludes.h"
#include <stdio.h>
#include "Vector3.h"

Vector3::Vector3()
: x(0)
, y(0)
, z(0)
{}

Vector3::Vector3(const Vector3& inVec)
: x(inVec.x)
, y(inVec.y)
, z(inVec.z)
{}

Vector3::Vector3(double _x, double _y, double _z)
: x(_x)
, y(_y)
, z(_z)
{}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
    Vector3 outVec = *this;
    outVec.x += rhs.x;
    outVec.y += rhs.y;
    outVec.z += rhs.z;
    return outVec;
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
    Vector3 outVec = *this;
    outVec.x -= rhs.x;
    outVec.y -= rhs.y;
    outVec.z -= rhs.z;
    return outVec;
}

Vector3 Vector3::operator*(double rhs) const
{
    Vector3 outVec = *this;
    outVec.x *= rhs;
    outVec.y *= rhs;
    outVec.z *= rhs;
    return outVec;
}

Vector3 Vector3::operator/(double rhs) const
{
    Vector3 outVec = *this;
    outVec.x /= rhs;
    outVec.y /= rhs;
    outVec.z /= rhs;
    return outVec;
}

bool Vector3::operator== (const Vector3& rhs) const
{
	return x==rhs.x && y==rhs.y && z==rhs.z;
}

bool Vector3::operator!= (const Vector3& rhs) const
{
	return !(*this==rhs);
}

double Vector3::dotProd(const Vector3& inVec) const
{
    return ((x * inVec.x) + (y * inVec.y) + (z * inVec.z));
}

Vector3 Vector3::crossProd(const Vector3& inVec) const
{
    Vector3 outVec;
    outVec.x = y*inVec.z + z*inVec.y;
    outVec.y = z*inVec.x + x*inVec.z;
    outVec.z = x*inVec.y + y*inVec.x;
    return outVec;
}

double Vector3::magnitude() const
{
    return sqrt((x*x) + (y*y) + (z*z));
}

Vector3 Vector3::normalize() const
{
    const double mag = magnitude();
    if (mag == 0)
        return Vector3();
    else
        return (*this / mag);
}

void Vector3::printVals() const
{
	printf("%f %f %f\n",x,y,z);
}
