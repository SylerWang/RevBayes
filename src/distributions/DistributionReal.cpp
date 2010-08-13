/**
 * @file
 * This file contains the partial implementation of DistributionReal, the
 * abstract base class for distributions on real-valued variables.
 *
 * @brief Partial implementation of DistributionReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @interface DistributionReal
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "DistributionReal.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbDouble.h"
#include "RbNames.h"
#include "StringVector.h"

#include <cfloat>


/** Constructor passes member rules and method inits to base class */
DistributionReal::DistributionReal(const MemberRules& memberRules)
    : Distribution(memberRules, getMethodInits()) {
}


/** Map calls to member methods */
const RbObject* DistributionReal::executeOperation(const std::string& name, std::vector<DAGNode*>& args) {

    /* Manage all calls here, for simplicity, instead of relying on inheritance */
    if (name == "cdf") {
        retDouble.setValue(cdf(((RbDouble*)(args[0]->getValue()))->getValue()));
    }
    else if (name == "lnPdf") {
        retDouble.setValue(lnPdf(args[0]->getValue()));
    }
    else if (name == "pdf") {
        retDouble.setValue(pdf(args[0]->getValue()));
    }
    else if (name == "quantile") {
        retDouble.setValue(quantile(((RbDouble*)(args[0]->getValue()))->getValue()));
    }
    else if (name == "rv") {
        if (getVariableType() == PosReal_name)
            return rv();
        RbDouble* randomVal = (RbDouble*)(rv());
        retDouble.setValue(*randomVal);
        delete randomVal;
    }
    else
        throw RbException("No member method called '" + name + "'");

    return &retDouble;
}


/** Get class vector describing type of object */
const StringVector& DistributionReal::getClass(void) const {

    static StringVector rbClass = StringVector(DistributionReal_name) + Distribution::getClass();
    return rbClass;
}


/** Get max value of distribution */
const RbDouble* DistributionReal::getMax(void) {

    static RbDouble rbMax = RbDouble(DBL_MAX);
    return &rbMax;
}


/** Get min value of distribution */
const RbDouble* DistributionReal::getMin(void) {

    static RbDouble rbMin = RbDouble(DBL_MIN);
    return &rbMin;
}


/** Get variable type */
const std::string& DistributionReal::getVariableType(void) const {

    return RbDouble_name;
}


/** Get method specifications */
const MethodTable& DistributionReal::getMethodInits(void) const {

    static MethodTable   methodInits;
    static ArgumentRules cdfArgRules;
    static ArgumentRules quantileArgRules;
    static bool          initsSet = false;

    if (!initsSet) {

        /* We inherit lnPdf, pdf and rv functions */
        methodInits = Distribution::getMethodInits();
        
        cdfArgRules.push_back(new ArgumentRule("q", PosReal_name));
        methodInits.addFunction("cdf", new MemberFunction(RbDouble_name, cdfArgRules));

        quantileArgRules.push_back(new ArgumentRule("p", PosReal_name));
        methodInits.addFunction("quantile", new MemberFunction(RbDouble_name, quantileArgRules));

        initsSet = true;
    }

    return methodInits;
}

