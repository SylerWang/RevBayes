#ifndef SyntaxFunctionDef_H
#define SyntaxFunctionDef_H

#include "SyntaxElement.h"
#include "SyntaxFormal.h"
#include "SyntaxVariable.h"
#include "TypeSpec.h"

#include <ostream>
#include <list>
#include <string>

namespace RevLanguage {

    /**
     * @brief Function definition syntax element
     *
     * This syntax element holds function definitions, that is, user-defined
     * Rev functions. Member variables include the function name, the formal
     * argument specifications, the return type and the code.
     *
     * The semantic content of a function definition is void, but evaluating
     * it results in the user-defined function being inserted in the function
     * table in the environment.
     */
    class SyntaxFunctionDef : public SyntaxElement {

    public:
        SyntaxFunctionDef(const std::string &type, const std::string &name, std::list<SyntaxFormal*>*   formals, std::list<SyntaxElement*>*  stmts);    //!< Standard constructor
        SyntaxFunctionDef(const SyntaxFunctionDef& x);                                                                                                  //!< Copy constructor
	    
        virtual                        ~SyntaxFunctionDef();                                                                                            //!< Destructor

        // Assignment operator
        SyntaxFunctionDef&              operator=(const SyntaxFunctionDef& x);                                                                          //!< Assignment operator

        // Basic utility functions
        SyntaxFunctionDef*              clone() const;                                                                                                  //!< Clone object
        void                            printValue(std::ostream& o) const;                                                                              //!< Print info about object

        // Regular functions
        RevPtr<Variable>                evaluateContent(Environment& env);                                                                              //!< Get semantic value

    protected:
        std::list<SyntaxElement*>*      code;                                                                                                           //!< The list of statements
        std::string                     functionName;                                                                                                   //!< The name of the function
        std::list<SyntaxFormal*>*       formalArgs;                                                                                                     //!< The formal arguments
        TypeSpec                        returnType;                                                                                                     //!< The return type specification of the function
    
    };
    
}

#endif

