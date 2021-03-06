
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Mcmc.h"
#include "Model.h"
#include "Natural.h"
#include "OptionRule.h"
#include "RbException.h"
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlString.h"
#include "TypeSpec.h"
#include "WorkspaceVector.h"


using namespace RevLanguage;

Mcmc::Mcmc() : WorkspaceObject<RevBayesCore::Mcmc>() {
    
}


/** Clone object */
Mcmc* Mcmc::clone(void) const {
    
	return new Mcmc(*this);
}


void Mcmc::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new MCMC object
    const RevBayesCore::Model&                              mdl     = static_cast<const Model &>( model->getRevObject() ).getValue();
    const RevBayesCore::RbVector<RevBayesCore::Move>&       mvs     = static_cast<const WorkspaceVector<Move> &>( moves->getRevObject() ).getVectorRbPointer();
    const RevBayesCore::RbVector<RevBayesCore::Monitor>&    mntr    = static_cast<const WorkspaceVector<Monitor> &>( monitors->getRevObject() ).getVectorRbPointer();
    const std::string &                                     sched   = static_cast<const RlString &>( moveSchedule->getRevObject() ).getValue();
    value = new RevBayesCore::Mcmc(mdl, mvs, mntr);
    value->setScheduleType( sched );
}


/* Map calls to member methods */
RevPtr<Variable> Mcmc::executeMethod(std::string const &name, const std::vector<Argument> &args) {
    
    if (name == "run") 
    {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        value->run( gen );
        
        return NULL;
    } 
    else if (name == "burnin") 
    {
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        int tuningInterval = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
        value->burnin( gen, tuningInterval );
        
        return NULL;
    } 
    else if ( name == "operatorSummary") 
    {
        value->printOperatorSummary();
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args );
}


/** Get Rev type of object */
const std::string& Mcmc::getClassType(void) { 
    
    static std::string revType = "MCMC";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Mcmc::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceObject<RevBayesCore::Mcmc>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Mcmc::getMemberRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        memberRules.push_back( new ArgumentRule("model", true, Model::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("monitors", true, WorkspaceVector<Monitor>::getClassTypeSpec() ) );
        memberRules.push_back( new ArgumentRule("moves", true, WorkspaceVector<Move>::getClassTypeSpec() ) );

        std::vector<RlString> options;
        options.push_back( RlString("sequential") );
        options.push_back( RlString("random") );
        options.push_back( RlString("single") );
        
        memberRules.push_back( new OptionRule( "moveschedule", new RlString( "random" ), options ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/* Get method specifications */
const MethodTable& Mcmc::getMethods(void) const {
    
    static MethodTable   methods    = MethodTable();
    static bool          methodsSet = false;
    
    if ( methodsSet == false ) {
        ArgumentRules* runArgRules = new ArgumentRules();
        runArgRules->push_back( new ArgumentRule("generations", true, Natural::getClassTypeSpec()) );
        methods.addFunction("run", new MemberProcedure( RlUtils::Void, runArgRules) );
        
        ArgumentRules* burninArgRules = new ArgumentRules();
        burninArgRules->push_back( new ArgumentRule("generations", true, Natural::getClassTypeSpec()) );
        burninArgRules->push_back( new ArgumentRule("tuningInterval", true, Natural::getClassTypeSpec()) );
        methods.addFunction("burnin", new MemberProcedure( RlUtils::Void, burninArgRules) );
        
        ArgumentRules* operatorSummaryArgRules = new ArgumentRules();
        methods.addFunction("operatorSummary", new MemberProcedure( RlUtils::Void, operatorSummaryArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &RevObject::getMethods() );
        methodsSet = true;
    }
    
    return methods;
}

/** Get type spec */
const TypeSpec& Mcmc::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Mcmc::printValue(std::ostream &o) const {
    
    o << "Mcmc";
}


/** Set a member variable */
void Mcmc::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "model") {
        model = var;
    } 
    else if ( name == "moves") {
        moves = var;
    }
    else if ( name == "monitors") {
        monitors = var;
    }
    else if ( name == "moveschedule") {
        moveSchedule = var;
    }
    else {
        RevObject::setConstMemberVariable(name, var);
    }
}
