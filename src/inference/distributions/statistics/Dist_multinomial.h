/**
 * @file
 * This file contains the declaration of Dist_multinomial, which is used to hold
 * parameters and functions related to a multinomial distribution.
 *
 * @brief Declaration of Dist_multinomial
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Dist_multinomial_H
#define Dist_multinomial_H

#include "DistributionDiscrete.h"
#include "RbVector.h"

#include <ostream>
#include <string>

class Real;

class Dist_multinomial: public DistributionDiscrete {

    public:
                                                Dist_multinomial(void);                                             //!< constructor

        // Basic utility functions
        Dist_multinomial*                       clone(void) const;                                                  //!< Clone object
        static const std::string&               getClassName(void);                                                 //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                             //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                            //!< Get language type of the object

        // Member variable setup
        const MemberRules&                      getMemberRules(void) const;                                         //!< Get member variable rules
        void                                    setMemberVariable(const std::string& name, const Variable* var);    //!< Catching the setting of the member variables.

        // Multinomial distribution functions
        size_t                                  getNumberOfStates(void) const;                                      //!< Get number of states
        const Simplex&                          getProbabilityMassVector(void);                                     //!< Get probability mass vector
        const std::vector<RbLanguageObject*>&   getStateVector(void) const;
        const TypeSpec&                         getVariableType(void) const;                                        //!< Get random variable type (VectorNatural)
        double                                  lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
        double                                  pdf(const RbLanguageObject& value) const;                           //!< Probability density
        const RbLanguageObject&                 rv(void);                                                           //!< Generate random variable
    
    private:

        // parameters
        RbConstVariablePtr                      probabilities;
    
        // memberfunction return values
        RbVector<Natural>                       randomVariable;

};

#endif
