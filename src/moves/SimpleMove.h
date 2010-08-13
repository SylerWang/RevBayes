/**
 * @file
 * This file contains the declaration of SimpleMove, which is the abstract
 * base class for moves updating single stochastic dag nodes in mcmc
 * inference.
 *
 * @brief Declaration of SimpleMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef SimpleMove_H
#define SimpleMove_H

#include "Move.h"

#include <string>
#include <vector>

class VariableNode;
class RandomNumberGenerator;
class StochasticNode;
class StringVector;

class SimpleMove : public Move {

    public:
        virtual                     ~SimpleMove(void) {}                //!< Destructor

        // Basic utility functions
        virtual RbObject*           clone(void) const = 0;              //!< Clone the object
        virtual const StringVector& getClass(void) const;               //!< Get class vector

        // SimpleMove functions
        void                        acceptMove(void);				    //!< Accept the move, update statistics
        void                        performMove(double& lnPriorRatio, double& lnLikelihoodRatio, double& lnHastingsRatio);   //!< Call perform, calculate ratios
        void                        rejectMove(void);                   //!< Reject the move
        void                        setNodePtr(StochasticNode* node) { nodePtr = node; }    //!< Set node ptr

    protected:
                                    SimpleMove(const MemberRules& memberRules);             //!< Parser constructor

        // Function you have to override
        virtual double              perform(std::set<StochasticNode*>& affectedNodes) = 0;  //!< Perform the move

        // Functions you may want to override for additional statistics purposes, e.g.
        virtual void                accept(void) {}                                         //!< Accept the move
        virtual void                reject(void) {}                                         //!< Reject the move

        // Member variable (hidden from parser)
        StochasticNode*             nodePtr;                            //!< The node to change (ptr)
};

#endif
