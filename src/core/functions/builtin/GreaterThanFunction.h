/**
 * @file
 * This file contains the declaration of the deterministic variable class for Vectors.
 * This class is derived from the deterministic node and each instance will represent a deterministic variable
 * computing the Vector of its parameters.
 *
 * @brief Declaration of the deterministic variable for Vectors.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-07-06, version 1.0
 * @interface TypedDagNode
 *
 * $Id$
 */



#ifndef GreaterThanFunction_H
#define GreaterThanFunction_H

#include "TypedFunction.h"

#include <vector>

namespace RevBayesCore {
    
    template <class leftValueType, class rightValueType>
    class GreaterThanFunction : public TypedFunction<bool> {
        
    public:
        GreaterThanFunction(const TypedDagNode<leftValueType> * l, const TypedDagNode<rightValueType> *r);
        GreaterThanFunction(const GreaterThanFunction &n);                                                                                        //!< Copy constructor
        virtual                                            ~GreaterThanFunction(void);                                                       //!< Virtual destructor
        
        // public member functions
        GreaterThanFunction*                                   clone(void) const;                                                          //!< Create an independent clone
        void                                                update(void);
        
    protected:
        void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Implementation of swaping parameters
        
    private:
        
        // members
        const TypedDagNode<leftValueType>*                  left;
        const TypedDagNode<rightValueType>*                 right;
        
    };
    
}

template <class leftValueType, class rightValueType>
RevBayesCore::GreaterThanFunction<leftValueType,rightValueType>::GreaterThanFunction(const TypedDagNode<leftValueType> *l, const TypedDagNode<rightValueType> *r) : TypedFunction<bool>( new bool(false) ), left( l ), right( r ) {
    // add the parameters as parents
    this->addParameter( left );
    this->addParameter( right );
    
    update();
}


template <class leftValueType, class rightValueType>
RevBayesCore::GreaterThanFunction<leftValueType,rightValueType>::GreaterThanFunction(const GreaterThanFunction<leftValueType,rightValueType> &n) : TypedFunction<bool>( n ), left( n.left ), right( n.right ) {
    // no need to add parameters, happens automatically
    
    update();
}


template <class leftValueType, class rightValueType>
RevBayesCore::GreaterThanFunction<leftValueType,rightValueType>::~GreaterThanFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



template <class leftValueType, class rightValueType>
RevBayesCore::GreaterThanFunction<leftValueType,rightValueType>* RevBayesCore::GreaterThanFunction<leftValueType,rightValueType>::clone( void ) const {
    return new GreaterThanFunction<leftValueType,rightValueType>( *this );
}


template <class leftValueType, class rightValueType>
void RevBayesCore::GreaterThanFunction<leftValueType,rightValueType>::update( void ) {
    
    *this->value = (left->getValue() > right->getValue());
    
}



template <class leftValueType, class rightValueType>
void RevBayesCore::GreaterThanFunction<leftValueType,rightValueType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if ( oldP == left ) {
        left = static_cast<const TypedDagNode<leftValueType>* >( newP );
    }
    if ( oldP == right ) {
        right = static_cast<const TypedDagNode<rightValueType>* >( newP );
    }
    
}

#endif
