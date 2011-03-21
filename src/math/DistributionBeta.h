/**
 * @file DistributionBeta
 * This file contains the functions of the beta distribution.
 *
 * @brief Implementation of the beta distribution.
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

class RandomNumberGenerator;
    
namespace RbStatistics 
{
    
	namespace Beta {
        
        double                      pdf(double a, double b, double x);                                    /*!< Beta(a,b) probability density */
        double                      lnPdf(double a, double b, double x);                                  /*!< Beta(a,b) log_e probability density */
        double                      cdf(double a, double b, double x);                                    /*!< Beta(a,b) cumulative probability */
        double                      quantile(double a, double b, double p);                               /*!< Beta(a,b) quantile */
        double                      rv(double a, double b, RandomNumberGenerator* rng);                   /*!< Beta(a,b) random variable */
        double                      rv_old(double a, double b, RandomNumberGenerator* rng);                   /*!< Beta(a,b) random variable */
	}
    
}