#ifndef Move_RandomIntegerWalk_H
#define Move_RandomIntegerWalk_H

#include "RandomIntegerWalkMove.h"
#include "RlMove.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the random-integer-walk move.
     *
     * The RevLanguage wrapper of the random-integer-walk move simply
     * manages the interactions through the Rev with our core.
     * That is, the internal move object can be constructed and hooked up
     * in a DAG-nove (variable) that it works on.
     * See the RandomIntegerWalkMove.h for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-28, version 1.0
     *
     */
    class Move_RandomIntegerWalk : public Move {
        
    public:
        
        Move_RandomIntegerWalk(void);                                                                                                            //!< Default constructor
        
        // Basic utility functions
        virtual Move_RandomIntegerWalk*              clone(void) const;                                                                          //!< Clone object
        void                                        constructInternalObject(void);                                                              //!< We construct the a new internal SlidingMove.
        static const std::string&                   getClassType(void);                                                                         //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                     //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                                                 //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                    //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                                          //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);          //!< Set member variable
        
        RevPtr<const Variable>                      x;                                                                                          //!< The variable on which the move works
        
    };
    
}

#endif
