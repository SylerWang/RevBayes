//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "F81RateMatrixFunction.h"
#include "RlF81RateMatrixFunction.h"
#include "RateMatrix_F81.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
F81RateMatrixFunction::F81RateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
F81RateMatrixFunction* F81RateMatrixFunction::clone( void ) const {
    
    return new F81RateMatrixFunction( *this );
}


RevObject* F81RateMatrixFunction::execute() {
    
    RevBayesCore::TypedDagNode<std::vector<double> >* bf = static_cast<const Simplex &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::F81RateMatrixFunction* f = new RevBayesCore::F81RateMatrixFunction( bf );
    RevBayesCore::DeterministicNode<RevBayesCore::RateMatrix> *detNode = new RevBayesCore::DeterministicNode<RevBayesCore::RateMatrix>("", f);
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& F81RateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "baseFrequencies", true, Simplex::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& F81RateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "F81";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& F81RateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& F81RateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& F81RateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}