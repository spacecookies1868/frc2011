#ifndef POLY_FUNCTION_HPP
#define POLY_FUNCTION_HPP

#include "Shared/CommonIncludes.h"

/**
 * This class implements a generic polynomial function of order n
 * The basic use case is to initialize it with the n+1 constants
 * in order such that the function
 * f = c[0] * X^n + c[1] * X^(n-1) + ... + c[n-1] * X + c[n] * 1
 * 
 */
template<uint32_t polyOrder>
class PolyFunction
{

public:
	
	/**
	 * Creates a polynomial function with specified coefficients.
	 *
	 * @param constants The array of constants for the polynomial function
	 * This array should have polyOrder+1 elements
	 */
	explicit PolyFunction(const double *constants)
	{
		// Initialize the constants array from the array passed in
		for (uint32_t i=0; i<=polyOrder; i++)
			m_polyConstants[i] = constants[i];
	}
	
	/**
	 * Computes the value of the polynomial given an input value.
	 *
	 * @param x the input value
	 * @return the value of the polynomial
	 */
	double	operator()(double x) const
	{
		// create a temporary array for values of x to the ith order
		double polyVals[polyOrder+1];
		
		// initialize the x^0 term
		polyVals[polyOrder]=1;
				
		// initialize the output y to the x^0 term 
		double y=m_polyConstants[polyOrder];
		
		if (polyOrder > 0)
		{
			// walking backwards, compute the x^n terms by multiplying x 
			// by the previously calculated x^(n-1)
			// also, take this opportunity to calculate y
			for (int i=(polyOrder-1); i>=0; i--)
			{
				polyVals[i] = x * polyVals[i+1];
				y += (polyVals[i]*m_polyConstants[i]);
			}
		}
		return y;
	}
	
	virtual ~PolyFunction()
	{
	}

private:
	/**
	 * An array of the polynomial coefficients.
	 */
	double m_polyConstants[polyOrder+1];	

};

#endif
