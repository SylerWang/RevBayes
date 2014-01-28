#ifndef DiversityDependentPureProcess_H
#define DiversityDependentPureProcess_H

#include "BirthDeathProcess.h"

namespace RevBayesCore {
    
    class Clade;
    
    /**
     * @brief Diversity-dependent pure-birth (Yule) process.
     *
     *
     * The diversity-dependent pure-birth process is a species tree distribution without extinction events.
     * The speciation rate depends on the current diversity by the function:
     * lambda = (1.0 - n/K) * lambda_0
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-01-17, version 1.0
     *
     */
    class DiversityDependentPureBirthProcess : public BirthDeathProcess {
        
    public:
        DiversityDependentPureBirthProcess(const TypedDagNode<double> *o, const TypedDagNode<double> *s, const TypedDagNode<int> *k, 
                                           const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt, 
                                           unsigned int nTaxa, const std::vector<std::string> &tn, const std::vector<Clade> &c);                                //!< Constructor
        
        // public member functions
        DiversityDependentPureBirthProcess*                 clone(void) const;                                                                                  //!< Create an independent clone
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                            //!< Implementation of swaping parameters
        
    private:
        
        // helper functions
        double                                              lnSpeciationRate(double t) const;                                                                   //!< log-transformed speciation rate at time t.
        double                                              rateIntegral(double t_low, double t_high) const;                                                    //!< rate integral in the inveral t_low to t_high
        double                                              pSurvival(double start, double end) const;                                      //!< Probability of survival (non-extinction) if started at time start with one species
        std::vector<double>                                 simSpeciations(size_t n, double origin, double r) const;                                                             //!< Simulate a speciation event.
        
        // members
        const TypedDagNode<double>*                         initialSpeciation;                                                                                  //!< The initial speciation rate (lambda_0).
        const TypedDagNode<int>*                            capacity;                                                                                           //!< The carrying capacity (K).
        
    };
    
}

#endif