#ifndef Dist_divDepYuleProcess_H
#define Dist_divDepYuleProcess_H

#include "DiversityDependentPureBirthProcess.h"
#include "RlTypedDistribution.h"
#include "RlTimeTree.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the Dist_divDepYuleProcess
     *
     * The RevLanguage wrapper of the diversity-dependent pure-birth process connects
     * the variables/parameters of the process and creates the internal Dist_divDepYuleProcess object.
     * Please read the Dist_divDepYuleProcess.h for more info.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-26, version 1.0
     *
     */
    class Dist_divDepYuleProcess :  public TypedDistribution<TimeTree> {
        
    public:
        Dist_divDepYuleProcess( void );
        
        // Basic utility functions
        Dist_divDepYuleProcess*                 clone(void) const;                                                                  //!< Clone the object
        static const std::string&                           getClassType(void);                                                                 //!< Get Rev type
        static const TypeSpec&                              getClassTypeSpec(void);                                                             //!< Get class type spec
        const TypeSpec&                                     getTypeSpec(void) const;                                                            //!< Get the type spec of the instance
        const MemberRules&                                  getMemberRules(void) const;                                                         //!< Get member rules (const)
        
        
        // Distribution functions you have to override
        RevBayesCore::DiversityDependentPureBirthProcess*   createDistribution(void) const;                                                     //!< Create an internal object of the diveristy-dependent pure-birth process.
        
    protected:
    
        void                                                setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var);  //!< Set member variable
        
        
    private:
        
        // members        
        RevPtr<const Variable>                              origin;                                                                             //!< The time of the process since the origin
        RevPtr<const Variable>                              condition;                                                                          //!< The condition of the process (none/survival/#Taxa)
        RevPtr<const Variable>                              numTaxa;                                                                            //!< The number of taxa (only needed for simulation)
        RevPtr<const Variable>                              taxonNames;                                                                         //!< The taxon names that will be applied to the initally simulated tree
        RevPtr<const Variable>                              constraints;                                                                        //!< Topological constraints that will be used for calibrations
        RevPtr<const Variable>                              initialLambda;
        RevPtr<const Variable>                              capacity;
        
    };
    
}

#endif
