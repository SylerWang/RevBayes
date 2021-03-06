//
//  AdmixtureEdgeReplaceSubtreeRegraft.h
//  rb_mlandis
//
//  Created by Michael Landis on 2/5/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeReplaceSubtreeRegraft__
#define __rb_mlandis__AdmixtureEdgeReplaceSubtreeRegraft__



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
    
    class AdmixtureEdgeReplaceSubtreeRegraft : public MoveOld {

    public:
        AdmixtureEdgeReplaceSubtreeRegraft( StochasticNode<AdmixtureTree> *n, DeterministicNode<std::vector<double> >* res, double d, int ag, int me, bool asa, double weight);                                            //!<  constructor
        
        // Basic utility functions
        AdmixtureEdgeReplaceSubtreeRegraft*       clone(void) const;                                                                  //!< Clone object
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
        
        void                            findNewBrothers(std::vector<AdmixtureNode *> &b, AdmixtureNode &p, AdmixtureNode& c, AdmixtureNode *n);
        void                            findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p);
        void                            storeAdmixtureEventsForLineage(AdmixtureNode* p);
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        DeterministicNode<std::vector<double> >* residuals;
        bool changed;
        bool failed;
        unsigned long activeGen;
        size_t numEvents;
        size_t maxEvents;
        bool allowSisterAdmixture;
        double delta;
        
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

#endif /* defined(__rb_mlandis__AdmixtureEdgeReplaceSubtreeRegraft__) */
