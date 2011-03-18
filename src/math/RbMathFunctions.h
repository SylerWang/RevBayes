/**
 * @file RbMathFunctions
 * This file contains some useful (standard) math functions.
 *
 * @brief Implementation of standard math functions.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-03-17, version 1.0
 *
 * $Id$
 */

#include "MatrixReal.h"
#include "VectorReal.h"
#include <vector>

namespace RbMath {
    
    double                      beta(double a, double b);                                                       //!< Xxx 
    int                         chebyshev_init(double *dos, int nos, double eta);
    double                      chebyshev_eval(double x, const double *a, const int n);
    double                      gamma(double x);                                                                //!< Calculate the Gamma function 
    double                      gamma_old(double x);                                                                //!< Calculate the Gamma function 
    double                      incompleteBeta(double a, double b, double x);                                   //!< Xxx 
    double                      incompleteGamma(double x, double alpha, double scale);                          //!< Xxx 
    double                      incompleteGamma_old(double x, double alpha, double scale);                          //!< Xxx 
    double                      lnGamma_sign(double a, int *sgn);                                               //!< Calculate the log of the Gamma function
    double                      lnGamma(double a);                                                              //!< Calculate the log of the Gamma function
    double                      lnGamma_old(double a);                                                          //!< Calculate the log of the Gamma function
    double                      lnGammacor(double x);
    double                      lnBeta(double a, double b);                                                     //!< Calculate the log of the Beta function 
    double                      rbEpsilon(void);                                                                //!< Xxx 
    double                      stirlerr(double n);
}