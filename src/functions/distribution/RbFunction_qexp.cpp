/**
 * @file
 * This file contains the implementation of RbFunction_qexp, the
 * qexp() function.
 *
 * @brief Implementation of RbFunction_qexp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#include "RbFunction_qexp.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include "RbStatistics.h"

const StringVector RbFunction_qexp::rbClass = StringVector("qexp") + RbFunction::rbClass;

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_qexp = SymbolTable::globalTable().add("qexp", new RbFunction_qexp());


/** Default constructor, allocate workspace */
RbFunction_qexp::RbFunction_qexp(void)
    : RbFunction(), value(new RbDouble(0)) {

	argRules.push_back( ArgumentRule("p", "double", NULL, new RbDouble(0.0), new RbDouble(0.0) ) );
	argRules.push_back( ArgumentRule("rate", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	returnType = "double";
} 

/** Copy constructor */
RbFunction_qexp::RbFunction_qexp(const RbFunction_qexp& s)
    : RbFunction(s), value(new RbDouble(0)) {
    
	argRules.push_back( ArgumentRule("p", "double", NULL, new RbDouble(0.0), new RbDouble(0.0) ) );
	argRules.push_back( ArgumentRule("rate", "double", new RbDouble(1.0), new RbDouble(0.0), NULL) );
	returnType = "double";
}

/** Destructor, delete workspace */
RbFunction_qexp::~RbFunction_qexp() {

    delete value;
}

/**
 * @brief clone function
 *
 * This function creates a deep copy of this object.
 *
 * @see RbObject.clone()
 * @returns           return a copy of this object
 *
 */
RbObject* RbFunction_qexp::clone(void) const {

    RbObject *x = new RbFunction_qexp( *this );
    return x;
}

RbObject& RbFunction_qexp::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbFunction_qexp& x = dynamic_cast<const RbFunction_qexp&> (obj);

        RbFunction_qexp& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbFunction_qexp& x = dynamic_cast<const RbFunction_qexp&> (*(obj.convertTo("qexp")));

            RbFunction_qexp& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to qexp");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbFunction_qexp& RbFunction_qexp::operator=(const RbFunction_qexp& obj) {

    argRules = obj.argRules;
    returnType = obj.returnType;
    (*value) = (*obj.value);
    return (*this);
}

/**
 * @brief print function
 *
 * This function prints this object.
 *
 * @see RbObject.print()
 * @param c           the stream where to print to
 *
 */
void RbFunction_qexp::print(std::ostream &c) const {

    c << "RbFunction_qexp" << std::endl;
}

void RbFunction_qexp::printValue(std::ostream &o) const {

    o << value << std::endl;
}

/**
 * @brief dump function
 *
 * This function dumps this object.
 *
 * @see RbObject.dump()
 * @param c           the stream where to dump to
 *
 */
void RbFunction_qexp::dump(std::ostream& c){
    //TODO implement

    std::string message = "Dump function of RbFunction_qexp not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

/**
 * @brief resurrect function
 *
 * This function resurrects this object.
 *
 * @see RbObject.resurrect()
 * @param x           the object from which to resurrect
 *
 */
void RbFunction_qexp::resurrect(const RbDumpState& x){
    //TODO implement
    std::string message = "Resurrect function of RbFunction_qexp not fully implemented!";
    RbException e;
    e.setMessage(message);
    throw e;
}

std::string RbFunction_qexp::toString(void) const {

	char temp[30];
	sprintf(temp, "%1.6lf", value->getValue());
	std::string tempStr = temp;
    return "Value = " + tempStr;
}


/**
 * @brief overloaded == operators
 *
 * This function compares this object
 *
 * @param o           the object to compare to
 *
 */
bool RbFunction_qexp::equals(const RbObject* o) const {

    return false;
}


/** Get number of argument rules */
const int RbFunction_qexp::getNumberOfRules(void) const {
    return 1;
}

/** Execute function */
RbObject* RbFunction_qexp::executeOperation(const std::vector<DAGNode*>& arguments) {

    /* Get actual argument */
    RbDouble *x      = (RbDouble*) arguments[0]->getValue();
    RbDouble *lambda = (RbDouble*) arguments[1]->getValue();

    value->setValue(RbStatistics::Exponential::quantile(*lambda,*x));

    return value;
}

RbObject* RbFunction_qexp::convertTo(const std::string& type) const {

    return NULL;
}

/**
 * @brief is convertible to
 *
 * This function checks if this data type can be converted into the given data type.
 *
 * @param dt         the data type we want to convert to
 * @returns          true, if it can be converted
 *
 */
bool RbFunction_qexp::isConvertibleTo(const std::string& type) const {

    return false;
}