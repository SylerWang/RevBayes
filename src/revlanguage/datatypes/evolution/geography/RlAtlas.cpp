//
//  RlAtlas.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 4/5/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "ArgumentRule.h"
#include "ModelVector.h"
#include "MemberProcedure.h"
#include "Natural.h"
#include "RlAtlas.h"
#include "RlBoolean.h"
#include "RlTaxonData.h"
#include "TimeAtlas.h"

using namespace RevLanguage;

RlAtlas::RlAtlas(void) : ModelObject<RevBayesCore::TimeAtlas>( ) {

}


RlAtlas::RlAtlas( RevBayesCore::TimeAtlas *v) : ModelObject<RevBayesCore::TimeAtlas>( v ) {

}


RlAtlas::RlAtlas( RevBayesCore::TypedDagNode<RevBayesCore::TimeAtlas> *m) : ModelObject<RevBayesCore::TimeAtlas>( m ) {

}


RlAtlas* RlAtlas::clone() const {
    return new RlAtlas( *this );
}


/* Map calls to member methods */
RevPtr<Variable> RlAtlas::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    
//    if (name == "nAreas")
//    {
//        int n = (int)this->value->getValue().getNumberOfCharacters();
//        return new Natural(n);
//    }
//    else if (name == "getNumberOfStates")
//    {
//        int n = (int)this->value->getValue().getNumberOfStates();
//        return new Natural(n);
//    }
    
    return ModelObject<RevBayesCore::TimeAtlas>::executeMethod( name, args );
}


/* Get Rev type of object */
const std::string& RlAtlas::getClassType(void) {
    
    static std::string revType = "RlAtlas";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& RlAtlas::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get the methods for this vector class */
/* Get method specifications */
const MethodTable& RlAtlas::getMethods(void) const {
    
    static MethodTable    methods                     = MethodTable();
    static bool           methodsSet                  = false;
    
    if ( methodsSet == false ) {
        
        //        // add method for call "x[]" as a function
        //        ArgumentRules* squareBracketArgRules = new ArgumentRules();
        //        squareBracketArgRules->push_back( new ArgumentRule( "index" , true, Natural::getClassTypeSpec() ) );
        //        methods.addFunction("[]",  new MemberProcedure( ModelVector<RealPos>::getClassTypeSpec(), squareBracketArgRules) );
        //
        //        // add method for call "x[]" as a function
        //        ArgumentRules* sizeArgRules = new ArgumentRules();
        //        methods.addFunction("size",  new MemberProcedure( Natural::getClassTypeSpec(), sizeArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &ModelObject<RevBayesCore::TimeAtlas>::getMethods() );
        methodsSet = true;
    }
    
    
    return methods;
}



/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& RlAtlas::getTypeSpec(void) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}
