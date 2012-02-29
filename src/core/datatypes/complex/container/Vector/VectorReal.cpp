/**
 * @file
 * This file contains the implementation of VectorReal, a complex type
 * used to hold double vectors.
 *
 * @brief Implementation of VectorReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#include "MemberFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "ValueRule.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>


/** Default constructor */
VectorReal::VectorReal(void) : AbstractVector(Real::getClassTypeSpec()) {
}


/** Construct vector with one double x */
VectorReal::VectorReal(const double x) : AbstractVector(Real::getClassTypeSpec()) {
    
    elements.push_back( x );
}


/** Construct vector with n doubles x */
VectorReal::VectorReal(const size_t n, const double x) : AbstractVector(Real::getClassTypeSpec()) {

    for (size_t i = 0; i < n; i++) {
        elements.push_back( x );
    }
}


/** Constructor from double vector */
VectorReal::VectorReal(const std::vector<double>& x) : AbstractVector(Real::getClassTypeSpec()) {

    elements = x;
}


/** Constructor from VectorRealPos */
VectorReal::VectorReal( const VectorRealPos& x ) : AbstractVector( x ) {

    elements = x.getValue();
}


/** Subscript operator */
double& VectorReal::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Subscript const operator */
const double& VectorReal::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorReal::operator==(const VectorReal& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorReal::operator!=(const VectorReal& x) const {

    return !operator==(x);
}


void VectorReal::clear(void) {
    elements.clear();
}


/** Clone function */
VectorReal* VectorReal::clone(void) const {

    return new VectorReal(*this);
}


/** Can we convert this vector into another object? */
RbObject* VectorReal::convertTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos::getClassTypeSpec()) {
        
        return new VectorRealPos( getValue() );
    }
    
    return AbstractVector::convertTo(type);
}


/** Get class name of object */
const std::string& VectorReal::getClassName(void) { 
    
    static std::string rbClassName = "Real Vector";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& VectorReal::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( AbstractVector::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& VectorReal::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const RbObject& VectorReal::getElement(size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    const RbObject* n = new Real(elements[index]);
    
    return *n;
}


RbObject& VectorReal::getElement(size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbObject* n = new Real(elements[index]);
    
    return *n;
}



/* Get method specifications */
const MethodTable& VectorReal::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) 
    {
        
        // add method for call "x[]" as a function
        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        squareBracketArgRules->push_back( new ValueRule( "index" , Natural::getClassTypeSpec() ) );
        methods.addFunction("[]",  new MemberFunction( Real::getClassTypeSpec(), squareBracketArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &AbstractVector::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Export value as STL vector */
std::vector<double> VectorReal::getValue(void) const {

    return elements;
}


/** Can we convert this vector into another object? */
bool VectorReal::isConvertibleTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos::getClassTypeSpec()) {
        
        for (std::vector<double>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            
            // test whether we can convert this element, otherwise return false
            if (*it < 0) {
                return false;
            }
        }
        
        return true;
    }
    
    return AbstractVector::isConvertibleTo(type);
}


void VectorReal::pop_back(void) {
    elements.pop_back();
}


void VectorReal::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Print value for user */
void VectorReal::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    for (std::vector<double>::const_iterator i = elements.begin(); i!= elements.end(); i++) 
        {
        if (i != elements.begin())
            o << ", ";
        o << (*i);
        }
    o <<  " ]";

    o.setf(previousFlags);
    o.precision(previousPrecision);
}


/** Push an int onto the back of the vector after checking */
void VectorReal::push_back( RbObject* x ) {
    
    if ( x->isTypeSpec( Real::getClassTypeSpec() ) ) {
        elements.push_back(static_cast<Real*>( x )->getValue());
    } else if ( x->isConvertibleTo(Real::getClassTypeSpec()) ) {
        elements.push_back(static_cast<Real*>(x->convertTo(Real::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Real::getClassName() + "[] with invalid value" );
    }
}

/** Append element to end of vector, updating length in process */
void VectorReal::push_back(double x) {
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorReal::push_front( RbObject* x ) {
    
    if ( x->isTypeSpec( Real::getClassTypeSpec() ) ) {
        elements.insert( elements.begin(), static_cast<Real*>( x )->getValue());
    } else if ( x->isConvertibleTo(Real::getClassTypeSpec()) ) {
        elements.insert( elements.begin(), static_cast<Real*>(x->convertTo(Real::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Real::getClassName() + "[] with invalid value" );
    }
}


/** Add element in front of vector, updating length in process */
void VectorReal::push_front(double x) {
    
    elements.insert(elements.begin(), x );
}


void VectorReal::resize(size_t n) {
    elements.resize(n);
}


void VectorReal::setElement(const size_t index, RbLanguageObject* x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( Real::getClassTypeSpec() ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<Real*>( x )->getValue());
    } else if ( x->isConvertibleTo(Real::getClassTypeSpec()) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<Real*>(x->convertTo(Real::getClassTypeSpec()))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Real::getClassName() + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector */
void VectorReal::setValue(const std::vector<double>& x) {

    elements = x;
}   


/** Set value of vector using VectorReal */
void VectorReal::setValue(const VectorReal& x) {

    elements = x.elements;
}     


size_t VectorReal::size(void) const {
    return elements.size();
}


/* Sort the vector */
void VectorReal::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/* Remove consecutive duplicates and resizes the vector */
void VectorReal::unique(void) {
    sort();
    std::vector<double> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    return;
    
}

