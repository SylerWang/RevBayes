#include <iostream>
#include <list>
#include <string>
#include <sstream>

#include "Argument.h"
#include "Environment.h"
#include "Integer.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RbException.h"
#include "RevObject.h"
#include "RbUtil.h"
#include "RbOptions.h"
#include "RlString.h"
#include "SyntaxFunctionCall.h"
#include "Variable.h"
#include "Workspace.h"
#include "SyntaxVariable.h"

using namespace RevLanguage;


/** Construct from identifier and index */
SyntaxVariable::SyntaxVariable( const std::string &n, std::list<SyntaxElement*>* indx ) :
    SyntaxElement(),
    identifier( n ),
    functionCall( NULL ),
    expression( NULL ),
    index( indx ),
    baseVariable( NULL )
{
}


/** Construct from function call and index */
SyntaxVariable::SyntaxVariable( SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx ) :
    SyntaxElement(),
    identifier( "" ),
    functionCall( fxnCall ),
    expression( NULL ),
    index( indx ),
    baseVariable( NULL )
{
}


/** Construct from expression and index */
SyntaxVariable::SyntaxVariable( SyntaxElement* expr, std::list<SyntaxElement*>* indx ) :
    SyntaxElement(),
    identifier( "" ),
    functionCall( NULL ),
    expression( expr ),
    index( indx ),
    baseVariable( NULL )
{
}


/** Construct from base variable (member object), identifier and index */
SyntaxVariable::SyntaxVariable( SyntaxVariable* baseVar, const std::string &n, std::list<SyntaxElement*>* indx ) :
    SyntaxElement(),
    identifier( n ),
    functionCall( NULL ),
    expression( NULL ),
    index( indx ),
    baseVariable( baseVar )
{
}


/** Construct from base variable (member object), function call and index */
SyntaxVariable::SyntaxVariable( SyntaxVariable* baseVar, SyntaxFunctionCall* fxnCall, std::list<SyntaxElement*>* indx ) :
    SyntaxElement(),
    identifier( "" ),
    functionCall( fxnCall ),
    expression( NULL ),
    index( indx ),
    baseVariable( baseVar )
{
}


/** Deep copy constructor */
SyntaxVariable::SyntaxVariable(const SyntaxVariable& x) :
    SyntaxElement( x ),
    identifier( x.identifier )
{
    if ( x.functionCall != NULL )
        functionCall = x.functionCall->clone();
    else
        functionCall = NULL;
    
    if ( x.expression != NULL )
        expression = x.expression->clone();
    else
        expression = NULL;

    if ( x.index != NULL )
    {
        index = new std::list<SyntaxElement*>();
        for ( std::list<SyntaxElement*>::const_iterator it = x.index->begin(); it != x.index->end(); ++it )
            index->push_back( (*it)->clone() );
    }
    else
        index = NULL;

    if ( x.baseVariable != NULL )
        baseVariable = x.baseVariable->clone();
    else
        baseVariable = NULL;
}


/** Destructor deletes base variable, expression, and index */
SyntaxVariable::~SyntaxVariable()
{
    if ( functionCall != NULL )
        delete functionCall;
    
    if ( expression != NULL )
        delete expression;
    
    if ( index != NULL )
    {
        for ( std::list<SyntaxElement*>::iterator it = index->begin(); it != index->end(); ++it )
            delete *it;
        delete index;
    }

    if ( baseVariable != NULL )
        delete baseVariable;
}


/** Assignment operator */
SyntaxVariable& SyntaxVariable::operator=( const SyntaxVariable& x )
{
    if (&x != this)
    {
        SyntaxElement::operator=( x );

        if ( functionCall != NULL )
            delete functionCall;

        if ( expression != NULL )
            delete expression;
        
        if ( index != NULL )
        {
            for ( std::list<SyntaxElement*>::iterator it = index->begin(); it != index->end(); ++it )
                delete *it;
            delete index;
            index = NULL;
        }
        
        if ( baseVariable != NULL )
            delete baseVariable;

        identifier   = x.identifier;

        if ( x.functionCall != NULL )
            functionCall = x.functionCall->clone();
        else
            functionCall = NULL;
        
        if ( x.expression != NULL )
            expression = x.expression->clone();
        else
            expression = NULL;
        
        if ( x.index != NULL )
        {
            index = new std::list<SyntaxElement*>();
            for ( std::list<SyntaxElement*>::const_iterator it = x.index->begin(); it != x.index->end(); ++it )
                index->push_back( (*it)->clone() );
        }
        else
            index = NULL;

        if ( x.baseVariable != NULL )
            baseVariable = x.baseVariable->clone();
        else
            baseVariable = NULL;
    }

    return (*this);
}



/** Type-safe clone of syntax element */
SyntaxVariable* SyntaxVariable::clone () const
{
    return new SyntaxVariable( *this );
}


/**
 * Get constant (static) value of one-offset indices. We use this when the
 * variable is part of a left-hand side expression, and when it is part
 * of a static right-hand side expression.
 */
std::vector<size_t> SyntaxVariable::computeIndex( Environment& env )
{
    std::vector<size_t> oneOffsetIndices;
    
    size_t count = 1;
    for ( std::list<SyntaxElement*>::iterator it = index->begin(); it != index->end(); ++it, ++count )
    {
        if ( (*it) == NULL )
        {
            oneOffsetIndices.push_back( 0 );
        }
        else
        {
            RevPtr<Variable> indexVar = (*it)->evaluateContent( env );
            
            if ( indexVar->getRevObject().isTypeSpec( Natural::getClassTypeSpec() ) )
            {
                // Get a Natural one-offset index
                size_t oneOffsetIndex = static_cast<Natural &>( indexVar->getRevObject() ).getValue();

                // Check validity
                if ( oneOffsetIndex < 1 )
                {
                    std::ostringstream msg;
                    msg << "Index " << count << " for ";
                    if ( baseVariable != NULL )
                        msg << baseVariable->getFullName( env ) << ".";
                    msg << identifier;
                    msg << " smaller than 1";
                    throw RbException( msg );
                }
                
                // Push the value onto integer index
                oneOffsetIndices.push_back( oneOffsetIndex );
            }
            
            else if ( indexVar->getRevObject().isConvertibleTo( Natural::getClassTypeSpec() ) )
            {
                // Convert to Natural
                RevObject* theNaturalIndex = indexVar->getRevObject().convertTo( Natural::getClassTypeSpec() );
                size_t oneOffsetIndex = static_cast<Natural*>( theNaturalIndex )->getValue();
                delete theNaturalIndex;

                // Check validity
                if ( oneOffsetIndex < 1 )
                {
                    std::ostringstream msg;
                    msg << "Index " << count << " for ";
                    if ( baseVariable != NULL )
                        msg << baseVariable->getFullName( env ) << ".";
                    msg << identifier;
                    msg << " smaller than 1";
                    throw RbException( msg );
                }
                
                // Push the value onto integer index
                oneOffsetIndices.push_back( oneOffsetIndex );
            }
            
            else if ( indexVar->getRevObject().isTypeSpec( RlString::getClassTypeSpec() ) )
            {
                throw RbException( "String indexes not supported (yet)");
#if 0
                //Push string index onto string index vector
                stringIndex.push_back( static_cast<RlString&>( indexVar->getRevObject() ).getValue() );
#endif
            }
            
            else
            {
                std::ostringstream msg;
                msg << "Index " << count << " for ";
                if ( baseVariable != NULL )
                    msg << baseVariable->getFullName( env ) << ".";
                msg << identifier;
                msg << " of wrong type (neither " << Natural::getClassType() << " nor " << RlString::getClassType() << ")";
                throw RbException( msg );
            }
        }
    }
    
    // Return index
    return oneOffsetIndices;
}


/**
 * Evaluate the dynamic content of the one-offset index variables.
 *
 * @todo Add dynamic type conversion
 */
std::vector< RevPtr<Variable> > SyntaxVariable::computeDynamicIndex( Environment& env )
{
    std::vector< RevPtr<Variable> > indexVars;

    std::list<SyntaxElement*>::const_iterator it;
    for ( it = index->begin(); it != index->end(); ++it )
    {
        RevPtr<Variable> theIndex = ( *it )->evaluateDynamicContent( env );
    
        // We ensure that the variables are of type Natural or can be converted to Natural numbers.
        // No sense in checking indices against permissible range here; errors are thrown later if
        // we are out of range by the container or member object.
        // TODO: This is static type conversion, needs to be replaced with dynamic type conversion
        if ( !theIndex->getRevObject().isTypeSpec( Natural::getClassTypeSpec() ) )
        {
            if (theIndex->getRevObject().isConvertibleTo( Natural::getClassTypeSpec() ) )
            {
                RevObject* convObj = theIndex->getRevObject().convertTo( Natural::getClassTypeSpec() );
                
                theIndex = new Variable( convObj );
            }
            else
                throw RbException( "No known conversion of type '" + theIndex->getRevObject().getType() + "' to 'Natural', required for index");
        }
        
        indexVars.push_back( theIndex );
    }
    
    return indexVars;
}

        
/**
 * @brief Evaluate semantic content (r-value) (general case)
 *
 * The variable can either be a member or a base variable. In the latter
 * case, its "baseVariable" member is NULL. If the element is a base variable,
 * we get the semantic value of the element by looking it up in the frame.
 * If it is a member variable, we try to find it in the member variable
 * frame of a composite variable found by another SyntaxVariable element.
 *
 * An expression, such as a function call, may replace the identifier. If so,
 * then there will always be a list of indices, whereas the list of indices is
 * optional for an identifier.
 */
RevPtr<Variable> SyntaxVariable::evaluateContent( Environment& env) {
    
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall != NULL )
        {
            // Get the return variable of the function call
            theVar = functionCall->evaluateContent( env );
        }
        else if ( expression != NULL )
        {
            // Get the return variable of the expression
            theVar = expression->evaluateContent( env );
        }
        else
        {
            // Get named variable from the environment
            theVar = env.getVariable( identifier );
        }
    }
    else
    {
        // Get the base variable
        theVar = baseVariable->evaluateContent( env );
        
        if ( functionCall != NULL )
        {
            // Get the return variable of the function call
            functionCall->setBaseVariable( baseVariable );
            theVar = functionCall->evaluateContent( env );
            functionCall->removeBaseVariable();   // Otherwise double deletion...
        }
        else
        {
            // Find member variable
            theVar = theVar->getRevObject().getMember( identifier );
        }
    }
    
    // Get static index
    std::vector< size_t > oneOffsetIndices = computeIndex( env );
    
    // Get element if indices are provided.
    if ( !oneOffsetIndices.empty() )
    {
        if ( theVar->getRevObject().isTypeSpec( Container::RevObject::getClassTypeSpec() ) )
            theVar = theVar->getRevObject().getElement( oneOffsetIndices );
        else
        {
            // Find the index operator

            // TODO: This is the old code, see if it is possible to find
            // a more elegant solution

            // iterate over the index
            for ( std::list<SyntaxElement*>::const_iterator it= index->begin(); it!=index->end(); it++)
            {
                SyntaxElement*         indexSyntaxElement     = *it;
                RevPtr<Variable>       indexVar               = indexSyntaxElement->evaluateContent(env);

                // create the new variable name
                std::string varName = theVar->getName() + "[" + indexVar->getRevObject().toString() + "]";
                
                // convert the value into a member object
                RevObject &mObject = theVar->getRevObject();
                
                // get the method table for this member object
                // \TODO: We should not allow const casts
                MethodTable& mt = const_cast<MethodTable&>( mObject.getMethods() );
                
                // create the arguments which consist only of the single paramater inside the square brackets
                std::vector<Argument> args;
                args.push_back( Argument( indexVar ) );
                
                // get the member function with name "[]"
                MemberProcedure* theMemberFunction = static_cast<MemberProcedure*>( mt.getFunction( "[]", args ).clone() );
                theMemberFunction->processArguments( args );
                // We need to clone because otherwise we overwrite all methods for this object
                
                // set the member object for the member function
                theMemberFunction->setMemberObject( theVar );
                
                theVar = theMemberFunction->execute();
                if ( theVar->getName() == "" )
                    theVar->setName( varName );

                delete theMemberFunction;
            }
        }
    }

    // Return the variable for assignment
    return theVar;
}


/**
 * @brief Evaluate left-hand-side content
 *
 * This function is similar to evaluateContent(). However, we
 * do not throw an error if the variable does not exist in the
 * frame; instead, we create and return a new null variable.
 */
RevPtr<Variable> SyntaxVariable::evaluateLHSContent( Environment& env, const std::string& elemType )
{
    // Get static index. No dynamic evaluation here
    std::vector<size_t> oneOffsetIndices = computeIndex( env );
    
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall != NULL )
        {
            // Get the return variable of the function call
            theVar = functionCall->evaluateContent( env );
        }
        else if ( expression != NULL )
        {
            // Get the return variable of the expression
            theVar = expression->evaluateContent( env );
        }
        else
        {
            // Find or create the variable
            if ( env.existsVariable( identifier ) )
                theVar = env.getVariable( identifier );
            else    // add it
            {
                if ( oneOffsetIndices.size() == 0 )
                    theVar = new Variable( NULL, identifier );
                else
                    theVar = new Variable( Workspace::userWorkspace().makeNewEmptyContainer( elemType, oneOffsetIndices.size() ), identifier );
                env.addVariable( identifier, theVar );
            }
        }
    }
    else {

        // Get the base variable. Note that we do not create the variable in this case.
        theVar = baseVariable->evaluateContent( env );

        if ( functionCall != NULL )
        {
            // Get the return variable of the function call
            functionCall->setBaseVariable( baseVariable );
            theVar = functionCall->evaluateContent( env );
            functionCall->removeBaseVariable();     // Otherwise double deletion...
        }
        else
        {
            // Find member variable based on its name
            theVar = theVar->getRevObject().getMember( identifier );
        }
    }
    
    // Get element of a container if indices are provided
    if ( !oneOffsetIndices.empty() )
    {
        if ( theVar->getRevObject().isTypeSpec( Container::RevObject::getClassTypeSpec() ) )
            theVar = theVar->getRevObject().findOrCreateElement( oneOffsetIndices );
        else
        {
            // Find the index operator
            
            // TODO: This is the old code, see if it is possible to find
            // a more elegant solution
            
            // iterate over the index
            for ( std::list<SyntaxElement*>::const_iterator it= index->begin(); it!=index->end(); it++)
            {
                SyntaxElement*         indexSyntaxElement     = *it;
                RevPtr<Variable>       indexVar               = indexSyntaxElement->evaluateContent(env);
                
                // create the new variable name
                std::string varName = theVar->getName() + "[" + indexVar->getRevObject().toString() + "]";
                
                // convert the value into a member object
                RevObject &mObject = theVar->getRevObject();
                
                // get the method table for this member object
                // \TODO: We should not allow const casts
                MethodTable& mt = const_cast<MethodTable&>( mObject.getMethods() );
                
                // create the arguments which consist only of the single paramater inside the square brackets
                std::vector<Argument> args;
                args.push_back( Argument( indexVar ) );
                
                // get the member function with name "[]"
                MemberProcedure* theMemberFunction = static_cast<MemberProcedure*>( mt.getFunction( "[]", args ).clone() );
                theMemberFunction->processArguments( args );
                // We need to clone because otherwise we overwrite all methods for this object
                
                // set the member object for the member function
                theMemberFunction->setMemberObject( theVar );
                
                theVar = theMemberFunction->execute();
                if ( theVar->getName() == "" )
                    theVar->setName( varName );
                
                delete theMemberFunction;
            }
        }
    }

    // Return the variable for assignment
    return theVar;
}


/**
 * @brief Evaluate dynamic rhs content
 *
 * This function returns the semantic value of the variable expression
 * when it is part of a dynamic expression, that is, the right-hand side
 * of an equation (deterministic) or tilde (stochastic) assignment.
 *
 * It differs from the standard evaluateContent() in several ways. First,
 * control variables need to return clones of themselves (temporary
 * variables) rather than themselves, so that they are not included in
 * the DAG. Second, we cannot compute a static index for indexed variables.
 * Instead, we need to deliver an index conisting of variables resulting
 * from dynamic evaluation of the index variables. These need to be put
 * in a dynamic lookup variable.
 */
RevPtr<Variable> SyntaxVariable::evaluateDynamicContent( Environment& env) {
    
    RevPtr<Variable> theVar;
    
    if ( baseVariable == NULL )
    {
        if ( functionCall != NULL )
        {
            // Get the dynamic return variable of the function call
            theVar = functionCall->evaluateDynamicContent( env );
        }
        else if ( expression  != NULL )
        {
            // Get the dynamic return variable of the expression
            theVar = expression->evaluateDynamicContent( env );
        }
        else
        {
            // Get variable from the environment (no dynamic version of identifier)
            theVar = env.getVariable( identifier );
        }
    }
    else {
        
        // Get the base variable
        theVar = baseVariable->evaluateDynamicContent( env );
        
        if ( functionCall == NULL )
        {
            // Get the return variable of the function call
            functionCall->setBaseVariable( baseVariable );
            theVar = functionCall->evaluateDynamicContent( env );
            functionCall->removeBaseVariable();     // Otherwise double deletion
        }
        else
        {
            // Find member variable (no dynamic version of identifier)
            theVar = theVar->getRevObject().getMember( identifier );
        }
    }
    
    // Get index
    std::vector< RevPtr<Variable> > oneOffsetIndices = computeDynamicIndex( env );
    
    // Get dynamic element of a container if indices are provided. Otherwise we
    // need an indirect reference. Note that if we have a function call, then we
    // always have indices; this is a syntactic requirement because otherwise the
    // syntax element will be a function call and not a variable. This means that
    // we do not have to test for functionCall being NULL in the second branch.
    if ( !oneOffsetIndices.empty() )
    {
        if ( theVar->getRevObject().isTypeSpec( Container::RevObject::getClassTypeSpec() ) )
            theVar = new Variable( theVar->getRevObject().makeElementLookup( oneOffsetIndices ) );
        else
        {
            // Find the index operator
            
            // TODO: This is the old code, see if it is possible to find
            // a more elegant solution.
            
            // TODO: This is not correct for the dynamic evaluation case.
            // It uses static evaluation of the index variables, and does
            // not produce a lookup variable.
            
            // iterate over the index
            for ( std::list<SyntaxElement*>::const_iterator it= index->begin(); it!=index->end(); it++)
            {
                SyntaxElement*         indexSyntaxElement     = *it;
                RevPtr<Variable>       indexVar               = indexSyntaxElement->evaluateContent(env);
                
                // create the new variable name
                std::string varName = theVar->getName() + "[" + indexVar->getRevObject().toString() + "]";
                
                // convert the value into a member object
                RevObject &mObject = theVar->getRevObject();
                
                // get the method table for this member object
                // \TODO: We should not allow const casts
                MethodTable& mt = const_cast<MethodTable&>( mObject.getMethods() );
                
                // create the arguments which consist only of the single paramater inside the square brackets
                std::vector<Argument> args;
                args.push_back( Argument( indexVar ) );
                
                // get the member function with name "[]"
                MemberProcedure* theMemberFunction = static_cast<MemberProcedure*>( mt.getFunction( "[]", args ).clone() );
                theMemberFunction->processArguments( args );
                // We need to clone because otherwise we overwrite all methods for this object
                
                // set the member object for the member function
                theMemberFunction->setMemberObject( theVar );
                
                theVar = theMemberFunction->execute();
                if ( theVar->getName() == "" )
                    theVar->setName( varName );
                
                delete theMemberFunction;
            }
        }
    }
    
    // Check whether we have a control variable and make a clone in that case
    if ( theVar->isControlVar() )
        theVar = new Variable( theVar->getRevObject().clone() );
    
    // Return the variable for assignment
    return theVar;
}


/**
 * Return nice representation of the syntax element.
 */
std::string SyntaxVariable::getFullName( Environment& env ) const
{
    std::ostringstream theName;
    if ( baseVariable != NULL )
        theName << baseVariable->getFullName( env ) << ".";
    
    if ( functionCall != NULL )
        theName << "<functionCall>";
    else if ( expression != NULL )
        theName << "unnamed";
    else
        theName << identifier;

    for ( size_t i = 0; i < index->size(); ++i )
        theName << "[...]";

    return theName.str();
}


/** Print info about the syntax element */
void SyntaxVariable::printValue(std::ostream& o) const
{
    o << "<" << this << "> SyntaxVariable:" << std::endl;
    o << "identifier      = \"" << identifier << "\"" << std::endl;

    if ( functionCall != NULL )
    {
        o << "function call   = <" << functionCall << "> ";
        functionCall->printValue(o);
        o << std::endl;
    }
    
    if ( expression != NULL )
    {
        o << "expression      = <" << expression << "> ";
        expression->printValue(o);
        o << std::endl;
    }
    
    size_t count = 1;
    for ( std::list<SyntaxElement*>::const_iterator it = (*index).begin(); it != (*index).end(); ++it, ++count )
    {
        if ( (*it) != NULL ) {
            o << "index " << count << " = <" << (*it) << "> ";
            (*it)->printValue(o);
            o << std::endl;
        }
        else {
            o << "index " << count << " = < NULL >" << std::endl;
        }
    }
    o << std::endl;

    if ( baseVariable != NULL )
    {
        o << "base variable   = <" << baseVariable << "> ";
        baseVariable->printValue(o);
        o << std::endl;
    }
}

