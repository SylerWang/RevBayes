/**
 * @file
 * This file contains the declaration of Func__ne, which is used
 * to compare two variables for inequality.
 *
 * @brief Declaration of Func__ne
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func__ne_H
#define Func__ne_H

#include "RbFunction.h"

#include <map>
#include <string>

class DAGNode;
class VectorString;

template <typename firstValType, typename secondValType>
class Func__ne :  public RbFunction {

    public:
        // Basic utility functions
        Func__ne*                   clone(void) const;                                      //!< Clone the object
    	const VectorString&         getClass(void) const;                                   //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                           //!< Get argument rules
        int                         getReturnDim(void) const;                               //!< Get dim of return value
        const std::string&          getReturnType(void) const;                              //!< Get type of return value

	protected:
        DAGNode*                   executeOperation(const std::vector<DAGNode*>& args);    //!< Execute operation
};

#endif

#include "ArgumentRule.h"
#include "Boolean.h"
#include "DAGNode.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "MatrixReal.h"
#include "VectorString.h"


/** Clone object */
template <typename firstValType, typename secondValType>
Func__ne<firstValType, secondValType>* Func__ne<firstValType, secondValType>::clone(void) const {

    return new Func__ne(*this);
}


/** Execute function: Bool <- Integer != Integer */
template <>
DAGNode* Func__ne<Integer,Integer>::executeOperation(const std::vector<DAGNode*>& args) {

    int val1  = ((Integer*)(args[0])->getValue())->getValue();
    int val2  = ((Integer*)(args[1])->getValue())->getValue();
    bool comp = (val1 != val2);
    return new ConstantNode( new Boolean(comp));
}


/** Execute function: Bool <- Real != Real */
template <>
DAGNode* Func__ne<Real,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = ((Real*)(args[0])->getValue())->getValue();
    double val2 = ((Real*)(args[1])->getValue())->getValue();
    bool comp   = !RbMath::compApproximatelyEqual(val1, val2, 0.00001);
    return new ConstantNode( new Boolean(comp));
}


/** Execute function: Bool <- Integer != Real */
template <>
DAGNode* Func__ne<Integer,Real>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = (double)(((Integer*)(args[0])->getValue())->getValue());
    double val2 = ((Real*)(args[1])->getValue())->getValue();
    bool comp   = !RbMath::compApproximatelyEqual(val1, val2, 0.00001);
    return new ConstantNode( new Boolean(comp));
}


/** Execute function: Bool <- Real != Integer */
template <>
DAGNode* Func__ne<Real,Integer>::executeOperation(const std::vector<DAGNode*>& args) {

    double val1 = ((Real*)(args[0])->getValue())->getValue();
    double val2 = (double)(((Integer*)(args[1])->getValue())->getValue());
    bool comp   = !RbMath::compApproximatelyEqual(val1, val2, 0.00001);
    return new ConstantNode( new Boolean(comp));
}


/** Get argument rules */
template <typename firstValType, typename secondValType>
const ArgumentRules& Func__ne<firstValType, secondValType>::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
        {
        firstValType*  dummy1 = new firstValType();
        secondValType* dummy2 = new secondValType();
        argumentRules.push_back(new ArgumentRule("", dummy1->getType()));
        argumentRules.push_back(new ArgumentRule("", dummy2->getType()));
        rulesSet = true;
        delete dummy1;
        delete dummy2;
        }

    return argumentRules;
}


/** Get class vector describing type of object */
template <typename firstValType, typename secondValType>
const VectorString& Func__ne<firstValType, secondValType>::getClass(void) const {

    firstValType*  dummy1 = new firstValType();
    secondValType* dummy2 = new secondValType();
    
    std::string funcAddName = "Func__ne<" + dummy1->getType() + "," + dummy2->getType() + ">"; 
    static VectorString rbClass = VectorString(funcAddName) + RbFunction::getClass();
    
    delete dummy1;
    delete dummy2;
    
    return rbClass;
}


/** Get return dimension */
template <typename firstValType, typename secondValType>
int Func__ne<firstValType, secondValType>::getReturnDim(void) const {

    return 1;
}


/** Get return type */
template <typename firstValType, typename secondValType>
const std::string& Func__ne<firstValType, secondValType>::getReturnType(void) const {

    return Boolean_name;
}

