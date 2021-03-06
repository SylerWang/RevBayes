/**
 * @file
 * This file contains the declaration of Container, a complex type
 * that acts as a base class for all Containers.
 *
 * @brief Declaration of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends Container
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Container.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RevPtr.h"
#include "TypeSpec.h"
#include "Variable.h"
#include "VectorIndexOperator.h"

#include <algorithm>

using namespace RevLanguage;


/**
 * Default constructor. Construct empty container.
 */
Container::Container( void ) :
    RevObject()
{
}


/**
 * Construct empty container and set member rules for derived
 * objects that have member variables.
 */
Container::Container( const MemberRules& memberRules ) :
    RevObject( memberRules )
{
}


/** Get Rev type of object */
const std::string& Container::getClassType( void )
{
    static std::string revType = "Container";
    
	return revType; 
}


/** Get class type spec describing type of object */
const TypeSpec& Container::getClassTypeSpec(void)
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &RevObject::getClassTypeSpec() );
    
	return revTypeSpec; 
}


/**
 * Get an element using a simple index. We simply defer this to the generic getElement function
 * by providing a one-element vector of indices.
 */
RevPtr<Variable> Container::getElement( const size_t oneOffsetIndex )
{
    std::vector<size_t> oneOffsetIndices;
    
    oneOffsetIndices.push_back( oneOffsetIndex );
    
    return getElement( oneOffsetIndices );
}


/**
 * Get method specifications for container. Here we provide the "size" method.
 * Note that also the subscript operator method is supported by executeMethod.
 *
 * @todo Expose the subscript operator method
 */
const MethodTable& Container::getMethods(void) const {
    
    static MethodTable methods = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        ArgumentRules* sizeArgRules = new ArgumentRules();
        methods.addFunction("size", new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}


/* Map calls to member methods */
RevPtr<Variable> Container::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "size") 
    {
        return new Variable( new Natural( size() ) );
    } 

    return RevObject::executeMethod( name, args );
}


/** Print structure of a container */
void Container::printStructure( std::ostream& o ) const
{
    RevObject::printStructure( o );
    o << "_size         = " << size() << std::endl;
}

