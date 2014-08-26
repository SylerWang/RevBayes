#include "SimplexFromVectorFunction.h"
#include "TypedDagNode.h"

using namespace RevBayesCore;


/** Standard constructor from appropriately typed DAG node */
SimplexFromVectorFunction::SimplexFromVectorFunction( const TypedDagNode< std::vector<double> >* vec ) :
    TypedFunction< std::vector<double> >( new std::vector<double>() ),
    realPosVector( vec )
{
    // add the vector parameter as a parent
    this->addParameter( vec );

    // update the value
    update();
}


/** Copy constructor. Note that parameter pointers are copied automatically. */
SimplexFromVectorFunction::SimplexFromVectorFunction( const SimplexFromVectorFunction& f ) :
    TypedFunction< std::vector<double> >( f ),
    realPosVector( f.realPosVector )
{
    update();
}


/**
 * Destructor. Reference decrement and destruction of parameters is delegated to
 * the deterministic node, so there is nothing to do here.
 */
SimplexFromVectorFunction::~SimplexFromVectorFunction( void )
{
}


/** Construct type-safe clone of the object. */
SimplexFromVectorFunction* SimplexFromVectorFunction::clone( void ) const
{
    return new SimplexFromVectorFunction( *this );
}


/** Compute the simplex from the vector. */
void SimplexFromVectorFunction::update( void )
{
    
    // Empty current simplex (value is a std::vector<double>*)
    value->clear();

    // Compute the new simplex
    double sum = 0.0;
    (*value) = realPosVector->getValue();
    for ( size_t i = 0; i < value->size(); ++i )
        sum += (*value)[i];
    for ( size_t i = 0; i < value->size(); ++i )
        (*value)[i] = (*value)[i] / sum;
}


/** Swap parameter */
void SimplexFromVectorFunction::swapParameterInternal( const DagNode *oldP, const DagNode *newP )
{
    if ( oldP == realPosVector )
        realPosVector = static_cast< const TypedDagNode< std::vector<double> >* >( newP );
}
