#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_geom.h"
#include "GeometricDistribution.h"
#include "Natural.h"
#include "Probability.h"
#include "StochasticNode.h"

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_geom::Dist_geom() : TypedDistribution<Natural>() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself 
 */
Dist_geom* Dist_geom::clone( void ) const 
{
    
    return new Dist_geom(*this);
}


/**
 * Create a new internal distribution object.
 *
 * This function simply dynamically allocates a new internal distribution object that can be 
 * associated with the variable. The internal distribution object is created by calling its
 * constructor and passing the distribution-parameters (other DAG nodes) as arguments of the 
 * constructor. The distribution constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
RevBayesCore::GeometricDistribution* Dist_geom::createDistribution( void ) const
{
    // get the parameters
    RevBayesCore::TypedDagNode<double>* prob    = static_cast<const Probability &>( p->getRevObject() ).getDagNode();
    RevBayesCore::GeometricDistribution* d      = new RevBayesCore::GeometricDistribution( prob );
    
    return d;
}



/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_geom::getClassType(void) 
{ 
    
    static std::string revType = "Dist_geom";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_geom::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<Natural>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the geometric distribution are:
 * (1) the rate lambda which must be a positive real between 0 and 1 (= a probability).
 *
 * \return The member rules.
 */
const MemberRules& Dist_geom::getMemberRules(void) const 
{
    
    static MemberRules distGeomMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distGeomMemberRules.push_back( new ArgumentRule( "p", true, Probability::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distGeomMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_geom::getTypeSpec( void ) const 
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}



/** 
 * Set a member variable.
 * 
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
void Dist_geom::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) 
{
    
    if ( name == "p" ) 
    {
        p = var;
    }
    else 
    {
        TypedDistribution<Natural>::setConstMemberVariable(name, var);
    }
    
}
