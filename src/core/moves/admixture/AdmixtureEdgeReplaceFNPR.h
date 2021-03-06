//
//  AdmixtureEdgeReplaceFNPR.h
//  rb_mlandis
//
//  Created by Michael Landis on 1/30/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeReplaceFNPR__
#define __rb_mlandis__AdmixtureEdgeReplaceFNPR__



#include <ostream>
#include <set>
#include <string>
#include <list>

#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "MoveOld.h"
#include "StochasticNode.h"
#include "AdmixtureNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {

    class AdmixtureEdgeReplaceFNPR : public MoveOld {
        
    public:
        AdmixtureEdgeReplaceFNPR( StochasticNode<AdmixtureTree> *n, DeterministicNode<std::vector<double> >* res, double delta, int ag, int me, bool asa, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeReplaceFNPR*       clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        bool                            isActive(unsigned long g) const;
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
    private:
        
        void                            findNewBrothers(std::vector<AdmixtureNode *> &b, AdmixtureNode &p, AdmixtureNode *n);
        void                            findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p);
        void                            storeAdmixtureEventsForLineage(AdmixtureNode* p);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        DeterministicNode<std::vector<double> >* residuals;
        bool changed;
        bool failed;
        unsigned long activeGen;
        double delta;
        size_t numEvents;
        size_t maxEvents;
        bool allowSisterAdmixture;

        
        // stored objects to undo proposal
        AdmixtureNode*                  pruneChild;
        AdmixtureNode*                  pruneParent;
        AdmixtureNode*                  pruneNephew;
        AdmixtureNode*                  pruneNode;
        AdmixtureNode*                  regraftParent;
        AdmixtureNode*                  regraftChild;
        std::vector<double>             storedResiduals;
        AdmixtureTree                   storedAdmixtureTree;
        double storedAge;
        double storedPruneAge;
        
        std::list<AdmixtureEdgePosition> storedAdmixtureEdges;
        std::list<AdmixtureEdgePosition> newAdmixtureEdges;
        
    };
    
}



#endif /* defined(__rb_mlandis__AdmixtureEdgeReplaceFNPR__) */
