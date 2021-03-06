/* 
 * File:   Move_MultivariatePhyloProcessTranslation.cpp
 * Author: nl
 * 
 * Created on 16 juillet 2014, 19:46
 */

#include "Move_MultivariatePhyloProcessTranslation.h"


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlBoolean.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "RlMultivariatePhyloProcess.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_MultivariatePhyloProcessTranslation::Move_MultivariatePhyloProcessTranslation() : Move() {
    
}


/** Clone object */
Move_MultivariatePhyloProcessTranslation* Move_MultivariatePhyloProcessTranslation::clone(void) const {
    
	return new Move_MultivariatePhyloProcessTranslation(*this);
}


void Move_MultivariatePhyloProcessTranslation::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::MultivariatePhyloProcess> *tmp = static_cast<const MultivariatePhyloProcess &>( process->getRevObject() ).getDagNode();
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    bool t = static_cast<const RlBoolean &>( tuning->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::MultivariatePhyloProcess> *p = static_cast<RevBayesCore::StochasticNode<RevBayesCore::MultivariatePhyloProcess> *>( tmp );
    value = new RevBayesCore::MultivariatePhyloProcessTranslationMove(p, l, t, w);

    // now allocate a new sliding move
    /*
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::NodeTimeSlideUniform(t, w);
    */
    
}


/** Get class name of object */
const std::string& Move_MultivariatePhyloProcessTranslation::getClassType(void) { 
    
    static std::string revClassType = "Move_MultivariatePhyloProcessTranslation";
    
	return revClassType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_MultivariatePhyloProcessTranslation::getClassTypeSpec(void) { 
    
    static TypeSpec revClassTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revClassTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_MultivariatePhyloProcessTranslation::getMemberRules(void) const {
    
    static MemberRules nniMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        nniMemberRules.push_back( new ArgumentRule( "process", false, MultivariatePhyloProcess::getClassTypeSpec() ) );        
        nniMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec(), new RealPos( 1.0 ) ) );
        nniMemberRules.push_back( new ArgumentRule( "tune", true, RlBoolean::getClassTypeSpec(), new RlBoolean( true ) ) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getMemberRules();
        nniMemberRules.insert( nniMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return nniMemberRules;
}

/** Get type spec */
const TypeSpec& Move_MultivariatePhyloProcessTranslation::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_MultivariatePhyloProcessTranslation::printValue(std::ostream &o) const {
    
    o << "Move_MultivariatePhyloProcessTranslation(";
    if (process != NULL) {
        o << process->getName();
    }
    else {
        o << "?";
    }
    if (lambda != NULL) {
        o << lambda->getName();
    }
    else {
        o << "?";
    }
    if (tuning != NULL) {
        o << tuning->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_MultivariatePhyloProcessTranslation::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "process" ) {
        process = var;
    }
    else if ( name == "lambda" ) {
        lambda = var;
    }
    else if ( name == "tune" ) {
        tuning = var;
    }
    else {
        Move::setConstMemberVariable(name, var);
    }
}
