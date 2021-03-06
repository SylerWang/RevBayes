#include "ConstantNode.h"
#include "RlBoolean.h"
#include "Integer.h"
#include "Natural.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/* Default constructor */
Integer::Integer(void) : ModelObject<int>() {
    
}


Integer::Integer( RevBayesCore::TypedDagNode<int> *v ) : ModelObject<int>( v ) {
    
}



/* Construct from int */
Integer::Integer(int v) : ModelObject<int>( new int(v) ) {

}


/* Construct from unsigned int (ambiguous between int and bool otherwise) */
Integer::Integer(unsigned int v) : ModelObject<int>( new int( int(v) ) ) {
    
}


/**
 * Generic addition operator.
 * We test if the rhs is of a type that we use for a specialized addition operation.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
RevObject* Integer::add( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return add( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return add( static_cast<const Integer&>( rhs ) );
    
    return ModelObject<int>::add( rhs );
}


/**
 * Specialized addition operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
Real* Integer::add(const Real &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() + rhs.getValue() );
    
    return n;
}


/**
 * Specialized addition operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the addition operation.
 *
 * \return              A new object holding the sum.
 */
Integer* Integer::add(const Integer &rhs) const
{
    
    Integer *n = new Integer( dagNode->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone object */
Integer* RevLanguage::Integer::clone(void) const {

	return new Integer(*this);
}


/** Convert to type. The caller manages the returned object. */
RevObject* Integer::convertTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec() ) 
        return new RlBoolean( dagNode->getValue() == 0 );

    if ( type == Real::getClassTypeSpec() )
        return new Real( dagNode->getValue() );

    if ( type == RlString::getClassTypeSpec() ) 
    {
        std::ostringstream o;
        printValue( o );
        return new RlString( o.str() );
    }

    if ( type == RealPos::getClassTypeSpec() && dagNode->getValue() > 0 )
        return new RealPos( dagNode->getValue() );

    if ( type == Natural::getClassTypeSpec() && dagNode->getValue() >= 0)
        return new Natural( dagNode->getValue() );

    return RevObject::convertTo( type );
}


/**
  * Specialized decrement operation.
  */
void Integer::decrement( void ) 
{
    
    dagNode->getValue()--;

}


/**
 * Generic division operator.
 * We test if the rhs is of a type that we use for a specialized division operation.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
RevObject* Integer::divide( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return divide( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return divide( static_cast<const Integer&>( rhs ) );
    
    return ModelObject<int>::divide( rhs );
}


/**
 * Specialized division operation between two real numbers.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the divsion operation.
 *
 * \return              A new object holding the ratio.
 */
Real* Integer::divide(const Real &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() / rhs.getValue() );
    
    return n;
}


/**
 * Specialized division operation between two integer numbers.
 * The return value is also of type integer number.
 *
 * \param[in]   rhs     The right hand side operand of the division operation.
 *
 * \return              A new object holding the ratio.
 */
Real* Integer::divide(const Integer &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() / double( rhs.getValue() ) );
    
    return n;
}


/** Get Rev type of object */
const std::string& Integer::getClassType(void) { 
    
    static std::string revType = "Integer";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Integer::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& Integer::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/**
 * Specialized increment operation.
 */
void Integer::increment( void ) 
{
    
    dagNode->getValue()++;
    
}



/** Is convertible to language object of type? */
bool Integer::isConvertibleTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec())
        return true;

    if ( type == Real::getClassTypeSpec() )
        return true;

    if ( type == RlString::getClassTypeSpec() )
        return true;

    if ( type == RealPos::getClassTypeSpec() && dagNode->getValue() > 0 )
        return true;

    if ( type == Natural::getClassTypeSpec() && dagNode->getValue() >= 0 )
        return true;

    return RevObject::isConvertibleTo( type );
}


/**
 * Generic multiplication operator.
 * We test if the rhs is of a type that we use for a specialized multiplication operation.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
RevObject* Integer::multiply( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return multiply( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return multiply( static_cast<const Integer&>( rhs ) );
    
    return ModelObject<int>::multiply( rhs );
}


/**
 * Specialized multiplication operation between an integer and a real number.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
Real* Integer::multiply(const Real &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() * rhs.getValue() );
    
    return n;
}


/**
 * Specialized multiplication operation between two integer numbers.
 * The return value is also of type integer number.
 *
 * \param[in]   rhs     The right hand side operand of the multiplication operation.
 *
 * \return              A new object holding the product.
 */
Integer* Integer::multiply(const Integer &rhs) const
{
    
    Integer *n = new Integer( dagNode->getValue() * rhs.getValue() );
    
    return n;
}


/**
 * Generic subtraction operator.
 * We test if the rhs is of a type that we use for a specialized subtraction operation.
 *
 * \param[in]   rhs     The right hand side operand of the subtraction operation.
 *
 * \return              A new object holding the difference.
 */
RevObject* Integer::subtract( const RevObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec().isDerivedOf( Real::getClassTypeSpec() ) )
        return subtract( static_cast<const Real&>( rhs ) );
    
    if ( rhs.getTypeSpec().isDerivedOf(  Integer::getClassTypeSpec() ) )
        return subtract( static_cast<const Integer&>( rhs ) );
    
    return ModelObject<int>::subtract( rhs );
}


/**
 * Specialized subtraction operation between an integer and a real number.
 * The return value is also of type real number.
 *
 * \param[in]   rhs     The right hand side operand of the subtraction operation.
 *
 * \return              A new object holding the difference.
 */
Real* Integer::subtract(const Real &rhs) const
{
    
    Real *n = new Real( dagNode->getValue() - rhs.getValue() );
    
    return n;
}


/**
 * Specialized subtraction operation between two integer numbers.
 * The return value is also of type ineteger number.
 *
 * \param[in]   rhs     The right hand side operand of the subtraction operation.
 *
 * \return              A new object holding the difference.
 */
Integer* Integer::subtract(const Integer &rhs) const
{
    
    Integer *n = new Integer( dagNode->getValue() - rhs.getValue() );
    
    return n;
}
