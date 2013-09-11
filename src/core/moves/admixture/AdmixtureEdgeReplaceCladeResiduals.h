//
//  AdmixtureEdgeReplaceCladeResiduals.h
//  rb_mlandis
//
//  Created by Michael Landis on 8/29/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureEdgeReplaceCladeResiduals__
#define __rb_mlandis__AdmixtureEdgeReplaceCladeResiduals__


#include <ostream>
#include <set>
#include <string>
#include <map>

#include "Move.h"
#include "ConstantNode.h"
#include "DeterministicNode.h"
#include "ContinuousStochasticNode.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"
#include "Tree.h"

namespace RevBayesCore {
    
    class AdmixtureEdgeReplaceCladeResiduals : public Move {
        
    public:
         AdmixtureEdgeReplaceCladeResiduals( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, std::vector< ContinuousStochasticNode*> br, DeterministicNode<std::vector<double> >* res, int ag, bool asa, double weight);
        
        // Basic utility functions
        AdmixtureEdgeReplaceCladeResiduals*              clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        bool                            isActive(int g) const;
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            rejectSimpleMove(void);
        void                            acceptSimpleMove(void);
        
        void                            acceptMove(void);                                                                   //!< Accept the InferenceMoveSimple
        double                          performMove(double& probRatio);                                                     //!< Perform the InferenceMoveSimple
        void                            rejectMove(void);
        
    private:
        
        // member variables
        StochasticNode<AdmixtureTree>*  variable;
        StochasticNode<double>*         rate;
        //StochasticNode<int>*            admixtureCount;
        std::vector< ContinuousStochasticNode*> branchRates;
        DeterministicNode<std::vector<double> >* residuals;
        //ConstantNode<int>*              delayTimer;
        int                             activeGen;
        bool                            changed;
        bool                            failed;
        bool                            failedAdd;
        bool                            allowSisterAdmixture;
        int                             delay;
        int                             maxEvents;
        int                             numEvents;
        int                             numNodes;
        int                             numTips;
        
        // stored objects to undo proposal
        AdmixtureNode*                  storedAdmixtureParent;
        AdmixtureNode*                  storedAdmixtureChild;
        AdmixtureNode*                  storedAdmixtureParentChild;
        AdmixtureNode*                  storedAdmixtureChildChild;
        AdmixtureNode*                  storedAdmixtureParentParent;
        AdmixtureNode*                  storedAdmixtureChildParent;
        AdmixtureNode*                  newAdmixtureChildChild;
        AdmixtureNode*                  newAdmixtureParentChild;
        AdmixtureNode*                  newAdmixtureChildParent;
        AdmixtureNode*                  newAdmixtureParentParent;
        
        double                          storedAdmixtureAge;
        double                          storedAdmixtureWeight;
        std::map<int,double>            storedBranchRates;
        
        std::vector<double>             storedResiduals;
        
        
        // gets residual proposal distn
        std::pair<AdmixtureNode*,AdmixtureNode*>    sampleAdmixtureEdgeFromResiduals(double& lnP);
        void                                        findCladeOrder(std::set<AdmixtureNode*>& cn);
        std::set<AdmixtureNode*>                    findClades(AdmixtureNode* p);
        void                                        findCladeResiduals(void);
        
        std::map<AdmixtureNode*,std::set<AdmixtureNode*> > clades;
        std::vector<std::map<std::pair<AdmixtureNode*, AdmixtureNode*>,double> > cladeResiduals;
        std::vector<double> cladeResidualSums;
        std::vector<std::set<AdmixtureNode*> > cladeOrder;
        std::vector<double> cladeOrderAges;
    };
    
}

#endif /* defined(__rb_mlandis__AdmixtureEdgeReplaceCladeResiduals__) */
