/**
 * @file
 * This file contains the declaration of SyntaxUnaryExpr, which is
 * used to hold unary expressions in the syntax tree.
 *
 * @brief Declaration of SyntaxUnaryExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef SyntaxUnaryExpr_H
#define SyntaxUnaryExpr_H

#include "SyntaxElement.h"

#include <iostream>
#include <vector>


/**
 * This is the class used to hold binary expressions in the syntax tree.
 *
 * We store the operands and a flag signalling the type of operation to
 * be performed when getValue is called or to be represented when
 * getDAGNodeExpr is called.
 *
 */

namespace RevLanguage {

    /**
     * @brief Unary expression syntax element
     *
     * This syntax element is used to hold binary expressions. Member
     * variables are the operand and the operator type.
     *
     * Unary expressions are expressions of the type
     *
     *    -a
     *
     * and they effectively correspond to calls of internal univariate
     * functions.
     *
     * Just like function calls, unary expressions evaluate in
     * differently ways in dynamic and static expression contexts. For
     * more explanation of this, see the function call syntax element.
     *
     * The operand argument of unary expressions is understood to
     * be a dynamic argument. See the function call syntax element
     * for more details on different argument types.
     */
    class SyntaxUnaryExpr : public SyntaxElement {

    public:
        // Unary operator types
        enum                        operatorT { UMinus, UPlus, UNot, UAnd };                        //!< Operator types

        static std::string          opCode[];                                                       //!< Operator codes for printing

        // Constructors and destructor
        SyntaxUnaryExpr(SyntaxUnaryExpr::operatorT op, SyntaxElement* oper);                        //!< Standard constructor
        SyntaxUnaryExpr(const SyntaxUnaryExpr& x);                                                  //!< Copy constructor
	    virtual                    ~SyntaxUnaryExpr();                                              //!< Destroy operands

        // Assignment operator
        SyntaxUnaryExpr&            operator=(const SyntaxUnaryExpr& x);                            //!< Assignment operator

        // Basic utility functions
        SyntaxUnaryExpr*            clone() const;                                                  //!< Clone object
        void                        printValue(std::ostream& o) const;                              //!< Print info about object

        // Regular functions
        RevPtr<Variable>            evaluateContent(Environment& env);                              //!< Get semantic value (static version)
        RevPtr<Variable>            evaluateDynamicContent(Environment& env);                       //!< Get semantic value (dynamic version)
        bool                        isConstExpression(void) const;                                  //!< Is the expression constant?

    protected:
        SyntaxElement*              operand;                                                        //!< The operand
        enum operatorT              operation;                                                      //!< The type of operation
    
    };
    
}

#endif

