/**
 * @file
 * This file contains the declaration of RbFunction, which is
 * the interface and abstract base class for RevBayes functions.
 *
 * @brief Declaration of RbFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#ifndef RbFunction_H
#define RbFunction_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "Argument.h"
#include "ArgumentFrame.h"

class ArgumentRule;
class DAGNode;
class FunctionNode;
class VectorInteger;
class VectorString;

typedef std::vector<ArgumentRule*> ArgumentRules;

/**
 * This is the interface and abstract base class for functions in
 * RevBayes. Function instances are put in the function table in the
 * relevant frame (user workspace or base environment) if they are
 * global. If they are member functions of user-defined type, they
 * are instead associated with the function table of the approprioate
 * class in the class table of the user workspace.
 * 
 * A function instance knows its argument rules and can process a
 * vector of labeled argument values according to these rules to pro-
 * duce a ready-to-use list of arguments. The processing involves label
 * matching as well as filling of missing values with default values.
 * The processing of labeled argument values is done in the function
 * processArguments, which will throw an error if the provided
 * arguments do not match.
 *
 */
class RbFunction :  public RbObject {

    friend class                                FunctionNode;                                                                       //!< Give FunctionNode direct access to function frame

    public:
        virtual                                ~RbFunction(void) {}                                                                 //!< Destructor

        // Basic utility functions you have to override
        virtual RbFunction*                     clone(void) const = 0;                                                              //!< Clone object
    	virtual const VectorString&             getClass(void) const;                                                               //!< Get class vector

        // Basic utility functions you may want to override
        virtual std::string                     briefInfo(void) const;                                                              //!< Brief info about object
        virtual std::string                     richInfo(void) const;                                                               //!< Complete info about object

        // Basic utility functions you should not have to override
    	void                                    printValue(std::ostream& o) const;                                                  //!< Print the general information on the function ('usage')

        // RbFunction functions you have to override
        virtual DAGNode*                        execute(void) = 0;                                                                  //!< Execute function
        virtual const ArgumentRules&            getArgumentRules(void) const = 0;                                                   //!< Get argument rules
        virtual const TypeSpec                  getReturnType(void) const = 0;                                                      //!< Get type of return value

        // RbFunction function you may want to override
        virtual bool                            processArguments(const std::vector<Argument>&    passedArgs,
                                                                 bool                            evaluateOnce,
                                                                 VectorInteger*                  matchScore=NULL);                  //!< Process args, return a match score if pointer is not null
        virtual bool                            throws(void) { return false; }                                                      //!< Does the function throw exceptions?

        // RbFunction functions you should not override
        void                                    clearArgs(void);                                                                    //!< Clear argument frame "args"
        const ArgumentFrame&                    getArgs(void) const { return args; }                                                //!< Get processed arguments in argument frame "args"

	protected:
                                                RbFunction(void);                                                                   //!< Basic constructor

        // Member variables
        ArgumentFrame                           args;                                                                               //!< Frame for passed arguments
        bool                                    argsProcessed;                                                                      //!< Are arguments processed?
};

#endif
