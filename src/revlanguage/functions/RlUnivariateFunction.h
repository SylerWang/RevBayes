/**
 * @file
 * This file contains the declaration of UnivariateFunction, which is
 * the interface and abstract base class for RevLanguage univariate function objects.
 *
 * @brief Declaration of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-03 17:51:49 +0200 (Fri, 03 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface Distribution
 * @package distributions
 * @since Version 1.0, 2012-08-06
 *
 * $Id: Function.h 1734 2012-08-03 15:51:49Z hoehna $
 */

#ifndef RlUnivariateFunction_H
#define RlUnivariateFunction_H

#include "Real.h"
#include "RlTypedFunction.h"
#include "TypedDagNode.h"
#include "UnivariateFunction.h"

namespace RevLanguage {
    
    class UnivariateFunction : public TypedFunction<Real> {
        
    public:
        UnivariateFunction(void);  
        UnivariateFunction(RevBayesCore::TypedDagNode<RevBayesCore::UnivariateFunction> *n);  
        virtual                                         ~UnivariateFunction(void);                                                  //!< Destructor
        UnivariateFunction(const UnivariateFunction &x);                                                                            //!< Copy constuctor
        
        // the value type definition
        typedef RevBayesCore::UnivariateFunction valueType;
      
        // Basic utility functions you have to override
        virtual UnivariateFunction*                     clone(void) const;                                                          //!< Clone object
        static const std::string&                       getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                     //!< Get class type spec
        virtual const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
        virtual const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
        virtual const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get the type spec of the instance

        
        // Distribution functions you have to override
        virtual RevBayesCore::UnivariateFunction*       createFunction(void) const;                                                 //!< Create a random variable from this distribution
        
        
    protected:                                                                                                 //!< Basic constructor
        
    };
    
}

#endif
