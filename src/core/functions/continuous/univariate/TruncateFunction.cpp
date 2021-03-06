//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TruncateFunction.h"
#include <cmath>

using namespace RevBayesCore;

TruncateFunction::TruncateFunction(const TypedDagNode<double> *x) : TypedFunction<int>( new int(0) ), a( x ) {
    addParameter( x );
    
}


TruncateFunction* TruncateFunction::clone( void ) const {

    return new TruncateFunction(*this);

}


void TruncateFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {

    if (oldP == a) 
    {
        a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void TruncateFunction::update( void ) {
    
    *value = int( a->getValue() );

}


