/**
 * @file
 * This file contains the implementation of Dist_cat, which is used to hold
 * parameters and functions related to a distribution on a categorical variable.
 *
 * @brief Implementation of Dist_cat
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (M�n, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#include "DAGNode.h"
#include "Dist_cat.h"
#include "Move_msimplex.h"
#include "RealPos.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbMath.h"
#include "RbNames.h"
#include "RbStatistics.h"
#include "Real.h"
#include "ReferenceRule.h"
#include "Simplex.h"
#include "VectorRealPos.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cmath>
#include <sstream>
#include <vector>


/** Default constructor for parser use */
Dist_cat::Dist_cat( void ) : DistributionCategorical( getMemberRules() ) {
}


/** Constructor for internal use */
Dist_cat::Dist_cat( std::vector<double> m ) : DistributionCategorical( getMemberRules() ) {

    setValue( "m", new Simplex( m ) );
}


/** Clone this object */
Dist_cat* Dist_cat::clone( void ) const {

    return new Dist_cat( *this );
}


/** Get class vector showing type of object */
const VectorString& Dist_cat::getClass( void ) const {

    static VectorString rbClass = VectorString( Dist_cat_name ) + DistributionCategorical::getClass();
    return rbClass;
}


/** Get default move */
Move* Dist_cat::getDefaultMove( StochasticNode* node ) {

	// TODO: Add a default move, such as random draw from noncurrent states
    return NULL;
}


/** Get member variable rules */
const MemberRules& Dist_cat::getMemberRules( void ) const {

    static MemberRules memberRules;
    static bool        rulesSet = false;

    if ( !rulesSet )
		{
        memberRules.push_back( new ReferenceRule( "m"    , Simplex_name ) );
        memberRules.push_back( new ReferenceRule( "dummy", Categorical_name ) );

        rulesSet = true;
		}

    return memberRules;
}


/** Get the number of states in the distribution */
int Dist_cat::getNumStates( void ) const {

    return int( static_cast<const Simplex*>( getValue("m") )->getSubelementsSize() );
}


/** Get the probability mass vector */
const Simplex* Dist_cat::getProbabilityMassVector( void ) {

    return static_cast<const Simplex*>( getValue("m") );
}


/** Get random variable type */
const TypeSpec Dist_cat::getVariableType( void ) const {

    return getVariable( "dummy" )->getTypeSpec();
}


/**
 * This function calculates the natural log of the probability
 * density for a categorical random variable.
 *
 * @brief Natural log of Dirichlet probability density
 *
 * @param value Observed value
 * @return      Natural log of the probability density
 */
double Dist_cat::lnPdf( const RbObject* value ) {

	// Get the value and the parameters of the categorical distribution
    std::vector<double> m = static_cast<const Simplex*    >( getValue("m") )->getValue();
    int                 x = static_cast<const Categorical*>( value         )->getValue();

    if ( x < 0 )
        return 0.0;
    else
        return std::log( m[x] );
}


/**
 * This function calculates the probability density
 * for a categorical random variable.
 *
 * @brief Categorical probability density
 *
 * @param value Observed value
 * @return      Probability density
 */
double Dist_cat::pdf( const RbObject* value ) {

	// Get the value and the parameter of the categorical distribution
    std::vector<double> m = static_cast<const Simplex*    >( getValue("m") )->getValue();
    int                 x = static_cast<const Categorical*>( value         )->getValue();

	if ( x < 0 )
        return 1.0;
    else
        return m[x];
}


/**
 * This function generates a categorical
 * random variable.
 *
 * @brief Random draw from categorical distribution
 *
 * @return      Random draw from categorical distribution
 */
Categorical* Dist_cat::rv( void ) {

	// Get the parameter of the categorical distribution and the rng
    std::vector<double>    m   = static_cast<const Simplex*    >( getValue( "m" ) )->getValue();
    RandomNumberGenerator* rng = GLOBAL_RNG;

    // Get copy of reference object
    Categorical* draw = static_cast<Categorical*>( getValue( "dummy" )->clone() );

    draw->setValue( int( rng->uniform01() * draw->getNumStates() ) );
    return draw;
}
