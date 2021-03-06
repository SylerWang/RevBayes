//
//  MoveSlide.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Move_WeightedNodeTimeSlide.h"
#include "Natural.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_WeightedNodeTimeSlide::Move_WeightedNodeTimeSlide() : Move() {
    
}


/** Clone object */
Move_WeightedNodeTimeSlide* Move_WeightedNodeTimeSlide::clone(void) const {
    
	return new Move_WeightedNodeTimeSlide(*this);
}


void Move_WeightedNodeTimeSlide::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    int b = static_cast<const Natural &>( blocks->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::WeightedNodeTimeSlide(t, size_t(b), w);
}


/** Get Rev type of object */
const std::string& Move_WeightedNodeTimeSlide::getClassType(void) { 
    
    static std::string revType = "Move_Move_WeightedNodeTimeSlide";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_WeightedNodeTimeSlide::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_WeightedNodeTimeSlide::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "tree", false, TimeTree::getClassTypeSpec() ) );
        nniMemberRules.push_back( new ArgumentRule( "blocks", true, Natural::getClassTypeSpec(), new Natural(8) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& Move_WeightedNodeTimeSlide::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_WeightedNodeTimeSlide::printValue(std::ostream &o) const {
    
    o << "Move_WeightedNodeTimeSlide(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_WeightedNodeTimeSlide::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else if ( name == "blocks" ) {
        blocks = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
