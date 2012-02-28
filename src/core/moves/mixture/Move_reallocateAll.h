/**
 * @file
 * This file contains the declaration of Move_reallocateAll, which changes
 * the class of all data elements in a mixture.
 *
 * @brief Declaration of Move_reallocateAll
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-02-26 01:17:07 -0800 (Sun, 26 fév 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: Move_mscale.h 1247 2012-02-25 09:17:07Z hoehna $
 */

#ifndef Move_reallocateAll_H
#define Move_reallocateAll_H

#include <ostream>
#include <set>
#include <string>

#include "RandomNumberFactory.h"
#include "MoveSimple.h"


class Move_reallocateAll : public MoveSimple {
    
public:
    Move_reallocateAll(void);                                                      //!< Parser constructor
    
    // Basic utility functions
    Move_reallocateAll*                    clone(void) const;                                                      //!< Clone object
    static const std::string&       getClassName(void);                                                     //!< Get class name
    static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
    const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
    
    // Member variable inits
    const MemberRules&              getMemberRules(void) const;                                             //!< Get member rules
    void                            setMemberVariable(const std::string& name, Variable* var);              //!< Set member variable (ensure topologyProb is updated)
    
    // Return variable type
    const TypeSpec                  getVariableType(void) const;                                            //!< Get move variable type
    
protected:
    double                          perform(void);                                                          //!< Perform move
    
    // parameters
   // RbVariablePtr                   lambda;
    RbVariablePtr                       mixture;

};

#endif
