/*
 *  Matrix3x3.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 12/23/10.
 *
 */

#ifndef MATRIX_3X3_H
#define MATRIX_3X3_H

namespace Matrix
{
    enum RowNumber {
        Row1=0,
        Row2,
        Row3
    };
    
    enum ColNumber {
        Col1=0,
        Col2,
        Col3
    };
};

class Vector3;

/**
 * Models a 3 by 3 matrix.
 *
 * @author Dmitriy Solomonov
 */
class Matrix3x3
{
private:
    
	/**
	 * A two-dimensional array containing the contents of the matrix.
	 */
    double  m_value[3][3];

public:
    
    /**
     * Default constructor.
     *
     * Creates an identity matrix.
     * An identity matrix has all elements set to 0 except for its diagonal elements,
     * which are set to 1.
     */
    Matrix3x3();

    /**
     * Constructor that produces a copy of a supplied matrix.
     *
     * @param mat the matrix to copy
     */
    Matrix3x3(const Matrix3x3& mat);
    ~Matrix3x3() {}
    
    /**
     * Finds the value of a specified row and column.
     *
     * @param row the row of the matrix
     * @param col the column of the matrix
     * @return the value in the row and column
     */
    double&     operator()(Matrix::RowNumber row, Matrix::ColNumber col)
    {   return m_value[row][col];   }
    
    /**
     * Finds the value of a specified row and column.
     *
     * @param row the row of the matrix
     * @param col the column of the matrix
     * @return the value in the row and column
     */
    double      operator()(Matrix::RowNumber row, Matrix::ColNumber col) const
    {   return m_value[row][col];   }
    
    /**
     * Adds another matrix to this.
     *
     * Matrix addition is defined elementwise.
     *
     * @param rhs the matrix to add
     * @return the sum of the matrices
     */
    Matrix3x3   operator+(const Matrix3x3& rhs) const;
    /**
     * Subtracts another matrix from this.
     *
     * Matrix subtraction is defined elementwise.
     *
     * @param rhs the matrix to subtract
     * @return the difference of the matrices
     */
    Matrix3x3   operator-(const Matrix3x3& rhs) const;

    /**
     * Multiply another matrix with this.
     *
     * Matrix multiplication is defined that the element in a row of the matrix AB
     * is equal to the sum of the products of the corresponding row-elements of A
     * and the column elements of B.
     *
     * @param rhs the matrix to multiply by
     * @return the product of the matrices
     */
    Matrix3x3   operator*(const Matrix3x3& rhs) const;

    /**
     * Multiplies this matrix by a scalar.
     *
     * Scalar multiplication is defined elementwise, so each element in the resultant
     * matrix is equal to the scalar times itself.
     *
     * @param rhs the scalar to multiply by
     * @return the resultant matrix
     */
    Matrix3x3   operator*(double rhs) const;

    /**
     * Divides this matrix by a scalar.
     *
     * Equivalent to scalar multiplication by the scalar's reciprocal.
     *
     * @param rhs the scalar to divide by
     * @return the resultant matrix
     */
    Matrix3x3   operator/(double rhs) const;
	
	/**
	 * Multiplies this matrix by a vector.
	 *
	 * Equivalent to multiplying this matrix by a row matrix 1x3 of the vector.
	 *
	 * @param rhs the vector to multiply by
	 * @return the resultant 1x3 matrix expressed as a 3-element vector
	 */
    Vector3		operator*(const Vector3& rhs) const;
    
    /**
     * Determines if this matrix is equal to another.
     *
     * Equivalence is defined elementwise, where two matrices are equal if and only if
     * their corresponding elements are equal.
     *
     * @param rhs the matrix to check for equivalence
     * @return whether these matrices are equal
     */
    bool        operator==(const Matrix3x3& rhs) const;

    /**
     * Determines if this matrix is not equal to another.
     *
     * Equivalence is defined elementwise, where two matrices are equal if and only if
     * their corresponding elements are equal.
     *
     * @param rhs the matrix to check for equivalence
     * @return whether these matrices are not equal
     */
    bool        operator!=(const Matrix3x3& rhs) const;
	
	/**
	 * Calculates the determinant of the matrix.
	 *
	 * The formula for the determinant of a matrix is as follows: <br />
	 * | [a, b, c] | <br />
	 * | [d, e, f] | = a*e*i + b*f*g + c*d*h - a*f*h - b*d*i - c*e*g <br />
	 * | [g, h, i] | <br />
	 *
	 * @return the determinant of the matrix
	 */
	double		determinant() const;
	
	/**
	 * Calculates the adjugate of the matrix.
	 *
	 * @return the adjugate of the matrix
	 */
	Matrix3x3	adjugate() const;
	
	/**
	 * Calculates the inverse of the matrix.
	 *
	 * Multiplies the adjugate of the matrix with the inverse of its determinant.
	 *
	 * @return the inverse of the matrix
	 */
	Matrix3x3	inverse() const;
    
    /**
     * Calculates the transpose of the matrix.
     *
     * The transpose of a matrix redefines its rows to be columns and its columns to be rows.
     *
     * @return the transpose of the matrix
     */
	Matrix3x3   transpose() const;
	
	/**
	 * Prints the values of the matrix.
	 */
	void		printVals() const;
	    
    /**
     * Rotates an axis a given angle to form a rotational matrix.
     *
     * @param angle the angle which to rotate the axis
     * @param axis the axis which to rotate
     * @return the resultant rotational matrix
     */
	static Matrix3x3    RotMatrix(double angle, const Vector3& axis);

	/**
	 * Creates a rotational matrix from a supplied angle.
	 *
	 * @param angle the angle which to derive a rotational matrix from
	 * @return the resultant rotational matrix
	 */
	static Matrix3x3    RotMatrix(Vector3 angle);
    
};

#endif
