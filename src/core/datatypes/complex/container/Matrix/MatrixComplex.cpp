/**
 * @file
 * This file contains the implementation of MatrixComplex, a complex type
 * used to hold matrices of complex values.
 *
 * @brief Implementation of MatrixComplex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Complex.h"
#include "ConstantNode.h"
#include "ContainerNode.h"
#include "MatrixComplex.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "RbNames.h"
#include "UserInterface.h"
#include "VectorComplex.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>



/** Default constructor resulting in an empty Complex matrix */
MatrixComplex::MatrixComplex(void) : Matrix(Complex_name) {

}


/** Construct matrix of specified dimensions (length), initialize it with double x (default 0.0) */
MatrixComplex::MatrixComplex(const size_t nRows, const size_t nCols, std::complex<double> x) : Matrix(Complex_name) {

    length[0] = nRows;
    length[1] = nCols;

    for ( size_t i = 0; i < length[0]; i++ )
        matrix.push_back( VectorComplex( length[1], x ) );
}


/** Construct matrix from a two-dimensional set of STL vectors */
MatrixComplex::MatrixComplex(const std::vector<std::vector<std::complex<double> > >& x) : Matrix(Complex_name) {

    size_t numRows = x.size();
    size_t numCols = x[0].size();
    for (size_t i = 1; i < x.size(); i++) 
        {
        if ( x[i].size() != numCols )
            throw RbException( "Invalid attempt to initialize a matrix container using a jagged matrix" );
        }

    length[0] = numRows;
    length[1] = numCols;

    for ( size_t i = 0; i < length[0]; i++ )
        matrix.push_back( VectorComplex( x[i] ) );
}


/** Construct matrix from a length specification [ nRows, nCols ] and a single vector of values */
MatrixComplex::MatrixComplex(const std::vector<size_t>& len, const std::vector<std::complex<double> >& x) : Matrix(Complex_name) {

    length = len;

    size_t index = 0;
    for (size_t i=0; i<len[0]; i++) 
        {
        VectorComplex y;
        for (size_t j=0; j<len[1]; j++) 
            {
            y.push_back( x[index++] );
            }
        matrix.push_back( y );
        }
}


/** Index operator (const) */
const VectorComplex& MatrixComplex::operator[]( const size_t i ) const {

    if ( i >= length[0] )
        throw RbException( "Index to " + Complex_name + "[][] out of bounds" );

    return matrix[i];
}


/** Index operator */
VectorComplex& MatrixComplex::operator[]( const size_t i ) {

    if ( i >= length[0] )
        throw RbException( "Index to " + Complex_name + "[][] out of bounds" );

    return matrix[i];
}


/** Overloaded container clear function */
void MatrixComplex::clear( void ) {

    matrix.clear();
    length[0] = 0;
    length[1] = 0;
}


/** Clone function */
MatrixComplex* MatrixComplex::clone(void) const {

    return new MatrixComplex(*this);
}


/** Get class vector describing type of object */
const VectorString& MatrixComplex::getClass(void) const {

    static VectorString rbClass = VectorString(MatrixComplex_name) + Matrix::getClass();
    return rbClass;
}


/** Get matrix content as an STL vector of doubles */
std::vector<std::complex<double> > MatrixComplex::getContent( void ) const {

    std::vector<std::complex<double> > temp;
    for (size_t i=0; i<length[0]; i++)
        for (size_t j=0; j<length[1]; j++)
            temp.push_back( matrix[i][j] );
    return temp;
}


/** Overloaded container method to get element or subcontainer for parser */
DAGNode* MatrixComplex::getElement( const VectorInteger& index ) {

    if ( index.size() > 2 )
        throw RbException( "Too many indices for " + Complex_name + "[][]" );

    // Disregard irrelevant negative value(s) at back of index
    size_t numIndices = index.size();
    for ( int i = static_cast<int>( index.size() ) - 1; i >= 0; i-- ) 
        {
        if ( index[i] < 0 )
            numIndices--;
        else
            break;
        }

    if ( numIndices == 0 ) 
        {
        return new ContainerNode( this->clone() );
        }

    else if ( numIndices == 1 ) 
        {
        // Row submatrix, this is easy
        if ( index[0] > static_cast<int>( length[0] ) )
            throw RbException( "Index out of bound for " + Complex_name + "[][]" );
        return new ContainerNode( matrix[index[0]].clone() );
        }
        
    else /* if ( numIndices == 2 ) */ 
        {
        if ( index[0] < 0 ) 
            {
            // We want a column submatrix, which is a little tricky
            if ( size_t( index[0] ) > length[0] )
                throw RbException( "Index out of bound for " + Complex_name + "[][]" );
            
            VectorComplex* temp = new VectorComplex();
            for ( size_t j = 0; j < length[1]; j++ )
                temp->push_back( matrix[index[0]][j] );
            return new ContainerNode( temp );
            }
        else 
            {
            // We want an element, this is easy
            if ( size_t( index[0] ) > length[0] || size_t( index[1] ) > length[1] )
                throw RbException( "Index out of bound for " + Complex_name + "[][]" );
            return ( new Complex(matrix[index[0]][index[1]]) )->wrapIntoVariable();
            }
        }
}


/** Get matrix value as an STL vector<vector> of doubles */
std::vector<std::vector<std::complex<double> > > MatrixComplex::getValue( void ) const {

    std::vector<std::vector<std::complex<double> > > temp;
    for ( size_t i = 0; i < length[0]; i++ )
        temp.push_back( matrix[i].getValue() );
    return temp;
}


/** Calculates the number of digits to the left and right of the decimal point */
bool MatrixComplex::numFmt(int& numToLft, int& numToRht, std::string s) const {

    int ba = 0;
    int n[2] = { 0, 0 };
    bool foundDecimalPoint = false;
    for (size_t i=0; i<s.size(); i++)
        {
        if ( isdigit(s[i]) != 0 )
            n[ba]++;
        else if (s[i] == '.')
            {
            ba = 1;
            foundDecimalPoint = true;
            }
        }
    numToLft = n[0];
    numToRht = n[1];
    return foundDecimalPoint;
}


/** Print value for user */
void MatrixComplex::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    // find the maximum number of digits to the left and right of the decimal place
    int maxToLft = 0, maxToRht = 0;
    bool foundDecimalPoint = false;
    for (size_t i=0; i<length[0]; i++)
        {
        for (size_t j=0; j<length[1]; j++)
            {
            std::ostringstream v;
            v << matrix[i][j];
            int numToLft, numToRht;
            if (numFmt(numToLft, numToRht, v.str()) == true)
                foundDecimalPoint = true;
            if (numToLft > maxToLft)
                maxToLft = numToLft;
            if (numToRht > maxToRht)
                maxToRht = numToRht;
            }
        }
        
    // print the matrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i<length[0]; i++)
        {
        std::string lineStr = "";
        if (i == 0)
            lineStr += "[[ ";
        else 
            lineStr += pad  + " [ ";
        for (size_t j=0; j<length[1]; j++)
            {
            std::ostringstream v;
            v << matrix[i][j];
            int numToLft, numToRht;
            numFmt(numToLft, numToRht, v.str());
            for (int k=0; k<maxToLft-numToLft; k++)
                lineStr += " ";
            lineStr += v.str();
            if (numToRht == 0 && foundDecimalPoint == true)
                lineStr += ".";
            for (int k=0; k<maxToRht-numToRht; k++)
                lineStr += "0";
            if (j+1 < length[1])
                lineStr += ", ";
            }
        if (i == length[0]-1)
            lineStr += " ]]";
        else 
            lineStr += " ],\n";

        o << lineStr;
        //RBOUT(lineStr);
        }
    
    o.setf(previousFlags);
    o << o.precision(previousPrecision);
}


/** Push back a row vector */
void MatrixComplex::push_back( const VectorComplex& x ) {

    if ( size() == 0 )
        length[1] = x.size();
    else if ( x.size() != length[1] )
        throw RbException( "Cannot make matrix with rows of unequal size" );

    matrix.push_back( x );
    length[0]++;
}


/** Overloaded container resize method */
void MatrixComplex::resize( const std::vector<size_t>& len ) {

    if ( len.size() != 2 )
        throw RbException( "Invalid length specification in attempt to resize " + Complex_name + "[][]" );

    if ( len[0] < length[0] || len[1] < length[1] )
        throw RbException( "Invalid attempt to shrink " + Complex_name + "[][]" );

    // First add the new rows with the right number of columns
    for ( size_t i = length[0]; i < len[0]; i++ )
        matrix.push_back( VectorComplex( len[1] ) );

    // Now add columns to the old rows
    for ( size_t i = 0; i < length[0]; i++ )
        matrix[i].resize( len[1] );

    // Set new length specification
    length = len;
}


/** Complete info about object */
std::string MatrixComplex::richInfo(void) const {

    // TODO: Replace with something that makes sense
    std::ostringstream o;
    printValue(o);

    return o.str();
}


/** Set matrix content from single STL vector of doubles */
void MatrixComplex::setContent( const std::vector<std::complex<double> >& x ) {
    
    if ( x.size() != length[0]*length[1] )
        throw RbException( "Incorrect number of elements in setting " + Complex_name + "[][]" );
    
    matrix.clear();
    
    size_t index = 0;
    for ( size_t i = 0; i < length[0]; i++ ) {
        VectorComplex y;
        for ( size_t j = 0; j < length[1]; j++ ) {
            y.push_back( x[index++] );
        }
        matrix.push_back( y );
    }
}


/** Set matrix value from an STL vector<vector> of complex values */
void MatrixComplex::setValue( const std::vector<std::vector<std::complex<double> > >& x ) {

    if ( x.size() != length[0] )
        throw RbException( "Wrong number of rows in setting value of " + Complex_name + "[][]" );

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i].size() != length[1] )
            throw RbException( "Wrong number of columns in at least one row in setting value of " + Complex_name + "[][]" );
    }

    matrix.clear();
    for ( size_t i = 0; i < length[0]; i++ )
        matrix.push_back( VectorComplex( x[i] ) );
}


/** Overloaded container setElement method */
void MatrixComplex::setElement( const VectorNatural& index, DAGNode* var ) {

    if ( index.size() != 2 )
        throw RbException ( "Wrong number of indices for " + Complex_name + "[][]" );

    if ( index[0] >= length[0] || index[1] >= length[1] )
        throw RbException( "Index out of bounds for " + Complex_name + "[][]" );

    RbObject* value = var->getValue()->clone();
    if ( value == NULL )
        throw RbException( "Cannot set " + Complex_name + "[][] element to NULL" );

    if ( value->isType( Complex_name ) )
        matrix[index[0]][index[1]] = static_cast<Complex*>( value )->getValue();
    else {
        // We rely on convertTo to throw an error with a meaningful message
        matrix[index[0]][index[1]] = static_cast<Complex*>( value->convertTo( Complex_name ) )->getValue();
    }
}


/** Overloaded container setLength method */
void MatrixComplex::setLength( const std::vector<size_t>& len) {

    if ( len[0] * len[1] != size() )
        throw RbException( "New length for " + Complex_name + "[][] does not have the right number of elements" );

    std::vector<std::complex<double> > temp = getContent();
    setContent( temp );
}


/** Overloaded container size method */
size_t MatrixComplex::size( void ) const {

    return length[0] * length[1];
}


/** Transpose the matrix */
void MatrixComplex::transpose( void ) {

    MatrixComplex temp(length[1], length[0]);
    for ( size_t i = 0; i < length[0]; i++ )
        for ( size_t j = 0; j < length[1]; j++ )
            temp[j][i] = matrix[i][j];
}


////////////////////////////////// Global Matrix operators ///////////////////////////////////


/**
 * This function performs addition of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator+ (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A + b
 */
MatrixComplex operator+(const MatrixComplex& A, const double& b) {

	MatrixComplex B;
    B = A;
	for (size_t i=0; i<B.getNumRows(); i++)
		for (size_t j=0; j<B.getNumCols(); j++)
			B[i][j] = A[i][j] - b;
	return B;
}


/**
 * This function performs subtraction of a scalar from
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator- (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A - b
 */
MatrixComplex operator-(const MatrixComplex& A, const double& b) {

	MatrixComplex B;
    B = A;
	for (size_t i=0; i<B.getNumRows(); i++)
		for (size_t j=0; j<B.getNumCols(); j++)
			B[i][j] = A[i][j] - b;
	return B;
}

/**
 * This function performs multiplication of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator* (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A * b
 */
MatrixComplex operator*(const MatrixComplex& A, const double& b) {

	MatrixComplex B;
    B = A;
	for (size_t i=0; i<B.getNumRows(); i++)
		for (size_t j=0; j<B.getNumCols(); j++)
			B[i][j] = A[i][j] * b;
	return B;
}

/**
 * This function performs division with a scalar of
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator/ (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A / b
 */
MatrixComplex operator/(const MatrixComplex& A, const double& b) {

	MatrixComplex B;
    B = A;
	for (size_t i=0; i<B.getNumRows(); i++)
		for (size_t j=0; j<B.getNumCols(); j++)
			B[i][j] = A[i][j] / b;
	return B;
}

/**
 * This function performs addition of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator+ (scalar first)
 * @param a Scalar
 * @param B Matrix
 * @return a + B
 */
MatrixComplex operator+(const double& a, const MatrixComplex& B) {

	MatrixComplex A;
    A = B;
	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
			A[i][j] = a + B[i][j];
	return A;
}

/**
 * This function subtracts each element of a
 * a matrix from a scalar and returns the
 * resulting matrix.
 *
 * @brief operator- (scalar first)
 * @param a Scalar
 * @param B Matrix
 * @return a - B
 */
MatrixComplex operator-(const double& a, const MatrixComplex& B) {

	MatrixComplex A;
    A = B;
	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
			A[i][j] = a - B[i][j];
	return A;
}

/**
 * This function performs multiplication of a scalar to
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator* (scalar first)
 * @param a Scalar
 * @param B Matrix
 * @return a * B
 */
MatrixComplex operator*(const double& a, const MatrixComplex& B) {

	MatrixComplex A;
    A = B;
	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
			A[i][j] = a * B[i][j];
	return A;
}

/**
 * This function performs division of a scalar by
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator/ (scalar first)
 * @param a Scalar
 * @param B Matrix
 * @return a / B
 */
MatrixComplex operator/(const double& a, const MatrixComplex& B) {

	MatrixComplex A;
    A = B;
	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
			A[i][j] = a / B[i][j];
	return A;
}

/**
 * This function performs division of a scalar by
 * each element of a matrix and returns the
 * resulting matrix.
 *
 * @brief operator/ (scalar first)
 * @param A Matrix
 * @param B Matrix
 * @return A / B (actually, A * B^(-1))
 */
MatrixComplex operator/(const MatrixComplex& A, const MatrixComplex& B) {

    if ( A.getNumRows() != A.getNumCols() )
        throw RbException("Cannot divide non-square matrices");
	if ( A.getNumCols() != B.getNumCols() )
        throw RbException("Cannot divide matrices of differing dimension");
        
	size_t N = A.getNumCols();
	MatrixComplex invB(N, N, 0.0);
    RbMath::matrixInverse(B, invB);

	MatrixComplex C(N, N, 0.0);
	for (size_t i=0; i<N; i++) 
        {
		for (size_t j=0; j<N; j++) 
            {
			std::complex<double> sum = std::complex<double>(0.0,0.0);
			for (size_t k=0; k<N; k++)
				sum += A[i][k] * B[k][j];
			C[i][j] = sum;
            }
        }
	return C;    
}

/**
 * This function performs addition of a scalar to
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * @brief operator+= (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A += b
 */
MatrixComplex &operator+=(MatrixComplex& A, const double& b) {

	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
			A[i][j] += b;
	return A;
}

/**
 * This function performs subtraction of a scalar from
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * @brief operator-= (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A -= b
 */
MatrixComplex &operator-=(MatrixComplex& A, const double& b) {

	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
			A[i][j] -= b;
	return A;
}

/**
 * This function performs multiplication of a scalar to
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * @brief operator*= (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A *= b
 */
MatrixComplex &operator*=(MatrixComplex& A, const double& b) {

	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
			A[i][j] *= b;
	return A;
}

/**
 * This function performs division with a scalar of
 * each element of a matrix in place and returns the
 * resulting matrix.
 *
 * @brief operator/= (scalar)
 * @param A Matrix
 * @param b Scalar
 * @return A /= b
 */
MatrixComplex &operator/=(MatrixComplex& A, const double& b) {

	for (size_t i=0; i<A.getNumRows(); i++)
		for (size_t j=0; j<A.getNumCols(); j++)
			A[i][j] /= b;
	return A;
}

/**
 * This function performs elementwise addition of two
 * matrices and returns the resulting matrix. If the
 * matrices are not conformant, a null matrix is returned.
 *
 * @brief operator+
 * @param A Matrix 1
 * @param B Matrix 2
 * @return A + B, null matrix on failure
 */
MatrixComplex operator+(const MatrixComplex& A, const MatrixComplex& B) {

	size_t m = A.getNumRows();
	size_t n = A.getNumCols();
	if (B.getNumRows() != m || B.getNumCols() != n)
        throw RbException("Cannot add matrices A and B: the matrices are not of the same dimension");
	else 
        {
		MatrixComplex C(m, n, 0.0);
		for (size_t i=0; i<m; i++) 
            {
			for (size_t j=0; j<n; j++)
				C[i][j] = A[i][j] + B[i][j];
            }
		return C;
        }
}

/**
 * This function performs elementwise subtraction of two
 * matrices and returns the resulting matrix. If the
 * matrices are not conformant, a null matrix is returned.
 *
 * @brief operator-
 * @param A Matrix 1
 * @param B Matrix 2
 * @return A - B, null matrix on failure
 */
MatrixComplex operator-(const MatrixComplex& A, const MatrixComplex& B) {

	size_t m = A.getNumRows();
	size_t n = A.getNumCols();
	if (B.getNumRows() != m || B.getNumCols() != n)
        throw RbException("Cannot subtract matrices A and B: the matrices are not of the same dimension");
	else 
        {
		MatrixComplex C(m, n, 0.0);
		for (size_t i=0; i<m; i++) 
            {
			for (size_t j=0; j<n; j++)
				C[i][j] = A[i][j] - B[i][j];
            }
		return C;
        }
}

/**
 * Compute C = A*B, where C[i][j] is the dot-product of 
 * row i of A and column j of B. Note that this operator
 * does not perform elementwise multiplication. If the 
 * matrices do not have the right dimensions for matrix
 * multiplication (that is, if the number of columns of A
 * is different from the number of rows of B), the function
 * returns a null matrix.
 *
 * @brief Matrix multiplication
 * @param A An (m X n) matrix
 * @param B An (n X k) matrix
 * @return A * B, an (m X k) matrix, or null matrix on failure
 */
MatrixComplex operator*(const MatrixComplex& A, const MatrixComplex& B) {

	if ( A.getNumCols() != B.getNumRows() )
        throw RbException("Cannot multiply matrices A and B: the number of columns of A does not equal the number of rows in B");
	size_t M = A.getNumRows();
	size_t N = A.getNumCols();
	size_t K = B.getNumCols();
	MatrixComplex C(M, K, 0.0);
	for (size_t i=0; i<M; i++) 
        {
		for (size_t j=0; j<K; j++) 
            {
			std::complex<double> sum = std::complex<double>(0.0,0.0);
			for (size_t k=0; k<N; k++)
				sum += A[i][k] * B[k][j];
			C[i][j] = sum;
            }
        }
	return C;
}

/**
 * This function performs elementwise addition on two
 * matrices and puts the result in the first matrix.
 * If the two matrices are nonconformant, the first
 * matrix is left intact.
 *
 * @brief operator+=
 * @param A Matrix 1
 * @param B Matrix 2
 * @return A += B, A unmodified on failure
 */
MatrixComplex&  operator+=(MatrixComplex& A, const MatrixComplex& B) {

	size_t m = A.getNumRows();
	size_t n = A.getNumCols();
	if (B.getNumRows() == m && B.getNumCols() == n) 
        {
		for (size_t i=0; i<m; i++) 
            {
			for (size_t j=0; j<n; j++)
				A[i][j] += B[i][j];
            }
        }
	return A;
}

/**
 * This function performs elementwise subtraction on two
 * matrices and puts the result in the first matrix.
 * If the two matrices are nonconformant, the first
 * matrix is left intact.
 *
 * @brief operator-=
 * @param A Matrix 1
 * @param B Matrix 2
 * @return A -= B, A unmodified on failure
 */
MatrixComplex&  operator-=(MatrixComplex& A, const MatrixComplex& B) {

	size_t m = A.getNumRows();
	size_t n = A.getNumCols();
	if (B.getNumRows() == m && B.getNumCols() == n) 
        {
		for (size_t i=0; i<m; i++) 
            {
			for (size_t j=0; j<n; j++)
				A[i][j] -= B[i][j];
            }
        }
	return A;
}

/**
 * Compute C = A*B, where C[i][j] is the dot-product of 
 * row i of A and column j of B. Then assign the result to
 * A. Note that this operator does not perform elementwise
 * multiplication. If the matrices are not both square of the
 * same dimension, then the operation is not possible to
 * perform and we return an unomidified A.
 *
 * \brief Matrix multiplication with assignment (operator *=)
 * \param A An (n X n) matrix
 * \param B An (n X n) matrix
 * \return A = A * B, an (n X n) matrix, or unmodified A on failure
 */
MatrixComplex &operator*=(MatrixComplex& A, const MatrixComplex& B) {

	if ( A.getNumRows() == A.getNumCols() && B.getNumRows() == B.getNumCols() && A.getNumRows() == B.getNumRows() ) 
        {
		size_t N = A.getNumRows();
		MatrixComplex C(N, N, 0.0);
		for (size_t i=0; i<N; i++) 
            {
			for (size_t j=0; j<N; j++) 
                {
                std::complex<double> sum = std::complex<double>(0.0,0.0);
				for (size_t k=0; k<N; k++)
					sum += A[i][k] * B[k][j];
				C[i][j] = sum;
                }
            }
		A = C;
        }
	return A;
}
