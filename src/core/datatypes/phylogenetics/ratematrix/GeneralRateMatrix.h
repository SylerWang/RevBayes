#ifndef GeneralRateMatrix_H
#define GeneralRateMatrix_H

#include "AbstractRateMatrix.h"
#include <complex>
#include <vector>


namespace RevBayesCore {
    
    class EigenSystem;
    class Simplex;
    class TransitionProbabilityMatrix;
    
    
    /**
     * \brief Abstract class for general rate matrices.
     *
     * This is an abstract class for general rate matrices that implementes the RateMatrix interface.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2013-04-25
     *
     */
    class GeneralRateMatrix : public AbstractRateMatrix {
        
    public:
        GeneralRateMatrix(size_t n);                                                                                                    //!< Construct rate matrix with n states
        virtual                            ~GeneralRateMatrix(void);                                                                    //!< Destructor
        
        
        // public methods
        
        // pure virtual methods you have to overwrite
        virtual double                      averageRate(void) const = 0;                                                                //!< Calculate the average rate
        void                                calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const = 0;       //!< Calculate the transition probabilities for the rate matrix
        virtual GeneralRateMatrix*          clone(void) const = 0;
        const std::vector<double>&          getTransitionRates(void) const;
        const std::vector<double>&          getStationaryFrequencies(void) const;                                                       //!< Return the stationary frequencies
        bool                                isTimeReversible(void);                                                                     //!< Return whether the rate matrix is time reversible
        void                                setTransitionRates(const std::vector<double> &tr);
        void                                setStationaryFrequencies(const std::vector<double>& f);                                     //!< Directly set the stationary frequencies
        void                                updateMatrix(void);                                                                         //!< Update the rate entries of the matrix (is needed if stationarity freqs or similar have changed)
        
    protected:
        
        // protected methods
        void                                calculateStationaryFrequencies(void);                                                       //!< Calculate the stationary frequencies for the rate matrix
        
        // members
        std::vector<double>                 stationaryFreqs;
        std::vector<double>                 transitionRates;
        
    };
    
    
}

#endif


