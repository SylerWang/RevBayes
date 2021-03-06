/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of a screen monitor.
 *
 * @brief Declaration of RlScreenmonitor
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef Mntr_Screen_H
#define Mntr_Screen_H

#include "ScreenMonitor.h"
#include "RlMonitor.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Mntr_Screen : public Monitor {
        
    public:
        
        Mntr_Screen(void);                                                                                      //!< Default constructor (0.0)
        
        // Basic utility functions
        virtual Mntr_Screen*                        clone(void) const;                                                      //!< Clone object
        void                                        constructInternalObject(void);                                          //!< We construct the a new internal Filemonitor.
        static const std::string&                   getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                 //!< Get class type spec
        const MemberRules&                          getMemberRules(void) const;                                             //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                //!< Get language type of the object
        virtual void                                printValue(std::ostream& o) const;                                      //!< Print value (for user)
        
    protected:
        
        void                                        setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);     //!< Set member variable
        
        std::set<RevPtr<const Variable> >           vars;
        RevPtr<const Variable>                      printgen;
        RevPtr<const Variable>                      separator;
        RevPtr<const Variable>                      prior;
        RevPtr<const Variable>                      posterior;
        RevPtr<const Variable>                      likelihood;
        
    };
    
}

#endif
