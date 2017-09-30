/*
 *  NumbericalVector.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 1/23/11.
 *  Copyright 2011 Apple Inc.. All rights reserved.
 *
 */

#ifndef NUMERICAL_VECTOR_H
#define NUMERICAL_VECTOR_H

#include "Shared/CommonIncludes.h"

/**
 * Models an n-length vector.
 *
 * @author Dmitriy Solomonov
 */
template<uint32_t N>
class NumericalVector 
{
public:
	/**
	 * Creates an n-length vector with all values set to 0.
	 */
    explicit NumericalVector()
    {   
        for (uint32_t i=0; i<N; i++)
            m_values[i]=0.0;
    }
    
    /**
     * Creates a numerical vector from a const array
     *
     * @param vec the vector to copy
     */
    explicit NumericalVector(const double* arr)
    {   
        for (uint32_t i=0; i<N; i++)
            m_values[i]=arr[i];
    }
    
    /**
     * Copies a numerical vector.
     *
     * @param vec the vector to copy
     */
    NumericalVector(const NumericalVector& vec)
    {   
        for (uint32_t i=0; i<N; i++)
            m_values[i]=vec.m_values[i];
    }
    
    /**
     * Finds the value of a certain vector element.
     *
     * @param index the vector index to look in
     * @return the value of the element at index
     */
    const double  operator[](uint32_t index) const
    {   return m_values[index];
    }
    
    /**
     * Finds the value of a certain vector element.
     *
     * @param index the vector index to look in
     * @return the value of the element at index
     */
    double&  operator[](uint32_t index)
    {   return m_values[index];
    }
    
    /**
     * Adds the elements of another vector to this.
     *
     * @param rhs the vector to add to this
     * @return the sum of the vectors
     */
    NumericalVector& operator+= (const NumericalVector& rhs)
    {   
        for (uint32_t i=0; i<N; i++)
            m_values[i]+=rhs[i];
        return *this;
    }
    
    /**
     * Subtracts the elements of another vector from this.
     *
     * @param rhs the vector to subtract from this
     * @return the difference of the vectors
     */
    NumericalVector& operator-= (const NumericalVector& rhs)
    {   
        for (uint32_t i=0; i<N; i++)
            m_values[i]-=rhs[i];
        return *this;
    }
    
    /**
      * Adds the elements of another vector to this.
      *
      * @param rhs the vector to add to this
      * @return the sum of the vectors
      */
    NumericalVector operator+ (const NumericalVector& rhs) const
    {
        NumericalVector outVec = *this;
        outVec += rhs;
        return outVec;
    }
    
    /**
     * Subtracts the elements of another vector from this.
     *
     * @param rhs the vector to subtract from this
     * @return the difference of the vectors
     */
    NumericalVector operator- (const NumericalVector& rhs) const
    {
        NumericalVector outVec = *this;
        outVec -= rhs;
        return outVec;
    }
    
    /**
	 * Multiplies this vector by a scalar.
	 *
	 * @param rhs the scalar to multiply by
	 * @return the resultant vector
	 */
    NumericalVector operator* (double rhs) const
    {
        NumericalVector outVec = *this;
        for (uint32_t i=0; i<N; i++)
        {   outVec.m_values[i]*=rhs;
        }
        return outVec;
    }
    
    /**
     * Divides this vector by a scalar.
     *
     * Equivalent to multiplying by the scalar's reciprocal.
     *
     * @param rhs the scalar to divide by
     * @return the resultant vector
     */
    NumericalVector operator/ (double rhs) const
    {
        NumericalVector outVec = *this;
        for (uint32_t i=0; i<N; i++)
        {   outVec.m_values[i]/=rhs;
        }
        return outVec;
    }
    
	/**
	 * Checks if two vectors are equal.
	 *
	 * Two vectors are equal if and only if all elements are equal.
	 *
	 * @param rhs the vector to compare this with
	 * @return whether the vectors are equal
	 */
	bool operator== (const NumericalVector& rhs) const
    {
        bool bRet = true;
        for (uint32_t i=0; i<N; i++)
        {   bRet = bRet && (m_values[i]==rhs[i]);
        }
        return bRet;
    }
    
	/**
	 * Checks if two vectors are not equal.
	 *
	 * Returns the opposite of operator==.
	 *
	 * @param rhs the vector to compare this with
	 * @return !operator===
	 */
	bool operator!= (const NumericalVector& rhs) const
    {
        bool bRet = true;
        for (uint32_t i=0; i<N; i++)
        {   bRet = bRet && (m_values[i]!=rhs[i]);
        }
        return bRet;
    }
    
    
private:
    
	/**
	 * The values in this vector.
	 */
    double m_values[N];
    
};

#endif

