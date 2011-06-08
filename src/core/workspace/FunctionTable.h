/**
 * @file
 * This file contains the declaration of FunctionTable, which is
 * used to hold global functions in the base environment (the
 * global workspace) and the user workspace.
 *
 * @brief Declaration of FunctionTable
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2009-11-18 01:05:57 +0100 (Ons, 18 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2009-09-09
 * @extends RbObject
 *
 * $Id: FunctionTable.h 63 2009-11-18 00:05:57Z ronquist $
 */

#ifndef FunctionTable_H
#define FunctionTable_H

#include "RbFunction.h"     // For typedef of ArgumentRules
#include "RbInternal.h"

#include <map>
#include <ostream>
#include <string>
#include <vector>

class Argument;
class ArgumentRule;
class RbObject;

class FunctionTable : public RbInternal {

    public:
                        FunctionTable(FunctionTable* parent=NULL);  //!< Empty table
                        FunctionTable(const FunctionTable& x);      //!< Copy constructor
        virtual        ~FunctionTable();                           //!< Delete functions

        // Assignment operator
        FunctionTable&  operator=(const FunctionTable& x);          //!< Assignment operator 

        // Static help function that can be used by other objects, like MethodTable

        // Basic utility functions
        std::string     briefInfo(void) const;                                  //!< Brief info to string
        FunctionTable*  clone(void) const { return new FunctionTable(*this); }  //!< Clone object
        std::string     richInfo(void) const;                                   //!< Complete info to string
        void            printValue(std::ostream& o) const;                      //!< Print table for user

        // FunctionTable functions
        virtual void    addFunction(const std::string name, RbFunction* func);  //!< Add function
        void            clear(void);                                            //!< Clear table
        DAGNode*        executeFunction( const std::string&             name,
                                         const std::vector<Argument>&   args) const;                        //!< Evaluate function (once)
        void            eraseFunction(const std::string& name);                                             //!< Erase a function (all versions)
        RbFunction*     getFunction(const std::string& name, const std::vector<Argument>& args) const;      //!< Get function (a copy)
        bool            isDistinctFormal(const ArgumentRules& x, const ArgumentRules& y) const;             //!< Are formals unique?

    protected:
        RbFunction*     findFunction(   const std::string&              name,
                                        const std::vector<Argument>&    args,
                                        bool                            evaluateOnce) const;     //!< Find function, process args
        
        std::multimap<std::string, RbFunction*>     table;          //!< Table of functions
        FunctionTable*                              parentTable;    //!< Enclosing table
};

#endif
