/*
 *  Matrix3x3.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 12/23/10.
 *  Copyright 2010 Apple Inc.. All rights reserved.
 *
 */

#include "Shared/CommonIncludes.h"
#include "Matrix3x3.h"
#include <assert.h>
#include "Shared/Vector3.h"

using namespace Matrix;

Matrix3x3::Matrix3x3()
{
    m_value[Row1][Col1] = 1;
    m_value[Row1][Col2] = 0;
    m_value[Row1][Col3] = 0;
    
    m_value[Row2][Col1] = 0;
    m_value[Row2][Col2] = 1;
    m_value[Row2][Col3] = 0;
    
    m_value[Row3][Col1] = 0;
    m_value[Row3][Col2] = 1;
    m_value[Row3][Col3] = 0;
}

Matrix3x3::Matrix3x3(const Matrix3x3& rhs)
{
    for (int row=0; row<3; row++)
        for (int col=0; col<3; col++)
            m_value[row][col] = rhs.m_value[row][col];
}


Matrix3x3   
Matrix3x3::operator+(const Matrix3x3& rhs) const
{
    Matrix3x3 outMat(*this);
    for (int row=0; row<3; row++)
        for (int col=0; col<3; col++)
            outMat.m_value[row][col] += rhs.m_value[row][col];
    
    return outMat;
}

Matrix3x3   
Matrix3x3::operator-(const Matrix3x3& rhs) const
{
    Matrix3x3 outMat(*this);
    for (int row=0; row<3; row++)
        for (int col=0; col<3; col++)
            outMat.m_value[row][col] -= rhs.m_value[row][col];
    return outMat;
}

Matrix3x3   
Matrix3x3::operator*(double rhs) const
{
    Matrix3x3 outMat(*this);
    for (int row=0; row<3; row++)
        for (int col=0; col<3; col++)
            outMat.m_value[row][col] *= rhs;
    return outMat;
}

Matrix3x3   
Matrix3x3::operator/(double rhs) const
{
    Matrix3x3 outMat(*this);
    for (int row=0; row<3; row++)
        for (int col=0; col<3; col++)
            outMat.m_value[row][col] /= rhs;
    return outMat;
}

Matrix3x3   
Matrix3x3::operator*(const Matrix3x3& rhs) const
{
    Matrix3x3 outMat;
    
    for (int row=0; row<3; row++)
    {   
        for (int col=0; col<3; col++)
        {   
            outMat.m_value[row][col] = 0;
            for (int k=0; k<3; k++)
            {
                outMat.m_value[row][col] += m_value[row][k] * rhs.m_value[k][col];
            }
        }
    }
    return outMat;
}

Vector3
Matrix3x3::operator*(const Vector3& rhs) const
{
	double vals[3];
	for(int r=0;r<3;r++)
		vals[r]=m_value[r][0]*rhs.x+m_value[r][1]*rhs.y+m_value[r][2]*rhs.z;
	return Vector3(vals[0],vals[1],vals[2]);
}

bool        
Matrix3x3::operator==(const Matrix3x3& mat) const
{
    for (int i=0; i<3; i++)
    {   for (int j=0; j<3; j++)
        {
            if (fabs(m_value[i][j] - mat.m_value[i][j]) > 0.0001)
                return false;
        }
    }
    return true;
}

bool        
Matrix3x3::operator!=(const Matrix3x3& mat) const
{
    return (!(*this == mat));
}

double
Matrix3x3::determinant() const
{
	double a=m_value[0][0];
	double b=m_value[0][1];
	double c=m_value[0][2];
	double d=m_value[1][0];
	double e=m_value[1][1];
	double f=m_value[1][2];
	double g=m_value[2][0];
	double h=m_value[2][1];
	double i=m_value[2][2];
	
	return a*e*i+b*f*g+c*d*h-a*f*h-b*d*i-c*e*g;
}

Matrix3x3
Matrix3x3::adjugate() const
{
	Matrix3x3 outMat;
	
	double a=m_value[0][0];
	double b=m_value[0][1];
	double c=m_value[0][2];
	double d=m_value[1][0];
	double e=m_value[1][1];
	double f=m_value[1][2];
	double g=m_value[2][0];
	double h=m_value[2][1];
	double i=m_value[2][2];
	
	outMat.m_value[0][0]=e*i-f*h;
	outMat.m_value[0][1]=c*h-b*i;
	outMat.m_value[0][2]=b*f-c*e;
	outMat.m_value[1][0]=f*g-d*i;
	outMat.m_value[1][1]=a*i-c*g;
	outMat.m_value[1][2]=c*d-a*f;
	outMat.m_value[2][0]=d*h-e*g;
	outMat.m_value[2][1]=b*g-a*h;
	outMat.m_value[2][2]=a*e-b*d;
	
	return outMat;
	
}

Matrix3x3
Matrix3x3::inverse() const
{
	return this->adjugate()*(1/this->determinant());
}

Matrix3x3   
Matrix3x3::transpose() const
{
    Matrix3x3 outMat;
    const Matrix3x3& myMat = *this;
    outMat(Row1, Col1) = myMat(Row1, Col1);
    outMat(Row1, Col2) = myMat(Row2, Col1);
    outMat(Row1, Col3) = myMat(Row3, Col1);
    
    outMat(Row2, Col1) = myMat(Row1, Col2);
    outMat(Row2, Col2) = myMat(Row2, Col2);
    outMat(Row2, Col3) = myMat(Row3, Col2);
    
    outMat(Row3, Col1) = myMat(Row1, Col3);
    outMat(Row3, Col2) = myMat(Row2, Col3);
    outMat(Row3, Col3) = myMat(Row3, Col3);
    
    return outMat;
}

Matrix3x3    
Matrix3x3::RotMatrix(double angle, const Vector3& axis)
{
    const Vector3 axis_n = axis.normalize();
    
    const double s = sin(angle);
    const double c = cos(angle);
    const double ci = 1 - c;
    
    const double x = axis.x;
    const double y = axis.y;
    const double z = axis.z;
    
    Matrix3x3   outMat;
    
    outMat(Row1,Col1) = (x * x * ci) + c;
    outMat(Row1,Col2) = (x * y * ci) - (z * s);
    outMat(Row1,Col3) = (x * z * ci) + (y * s);

    outMat(Row2,Col1) = (x * y * ci) + (z * s); 
    outMat(Row2,Col2) = (y * y * ci) + c;
    outMat(Row2,Col3) = (y * z * ci) - (x * s);
    
    outMat(Row3,Col1) = (x * z * ci) - (y * s);
    outMat(Row3,Col2) = (z * y * ci) + (x * s);
    outMat(Row3,Col3) = (z * z * ci) + c;
    return outMat;
}

Matrix3x3
Matrix3x3::RotMatrix(Vector3 angle)
{
	Matrix3x3 rotx=RotMatrix(angle.x,Vector3(1,0,0));
	Matrix3x3 roty=RotMatrix(angle.y,Vector3(0,1,0));
	Matrix3x3 rotz=RotMatrix(angle.z,Vector3(0,0,1));
	return rotx*roty*rotz;
}

void
Matrix3x3::printVals() const
{
	for(int row=0;row<3;row++) {
		for(int col=0;col<3;col++)
			printf("%f\t",m_value[row][col]);
		printf("\n");
	}
}

