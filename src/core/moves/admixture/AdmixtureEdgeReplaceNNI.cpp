//
//  AdmixtureEdgeReplaceNNI.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 1/29/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeReplaceNNI.h"
#include "DistributionBeta.h"
#include "DistributionBinomial.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeReplaceNNI::AdmixtureEdgeReplaceNNI(StochasticNode<AdmixtureTree> *v, DeterministicNode<std::vector<double> >* res, double d, int ag, int me, bool asa, double w) : MoveOld( v, w), variable( v ), residuals(res), changed(false), failed(false), activeGen(ag), delta(d), maxEvents(me), allowSisterAdmixture(asa) {
    
    //nodes.insert(rate);
    nodes.insert(residuals);
}



/* Clone object */
AdmixtureEdgeReplaceNNI* AdmixtureEdgeReplaceNNI::clone( void ) const {
    
    return new AdmixtureEdgeReplaceNNI( *this );
}



const std::string& AdmixtureEdgeReplaceNNI::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeReplaceNNI";
    
    return name;
}

void AdmixtureEdgeReplaceNNI::findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p)
{
    if (p->getNumberOfChildren() == 0)
    {
        s.insert(p);
    }
    else
    {
        for (size_t i = 0; i < p->getNumberOfChildren(); i++)
            findDescendantTips(s, &p->getChild(i));
    }
}

void AdmixtureEdgeReplaceNNI::storeAdmixtureEventsForLineage(AdmixtureNode* p)
{
    AdmixtureTree& tau = variable->getValue();
    AdmixtureNode* q_it = &p->getParent();
    while (q_it->getNumberOfChildren() != 2)
    {
        AdmixtureNode* qp = q_it;
        
        // adm parent
        if (&q_it->getAdmixtureChild() != NULL)
            ;
        
        // adm child
        else if (&q_it->getAdmixtureParent() != NULL)
            qp = &q_it->getAdmixtureParent();
        
        // divergence
        else
            break;
        
        AdmixtureNode* qc = &qp->getAdmixtureChild();
        AdmixtureNode* qpc = &qp->getChild(0);
        AdmixtureNode* qcc = &qc->getChild(0);
        AdmixtureEdgePosition ap(qp, qc, qpc, qcc, qc->getAge(), qc->getWeight());
        
        storedAdmixtureEdges.push_front(ap);
        
        tau.removeAdmixtureEdge(qp);
        tau.eraseAdmixtureNode(qp);
        tau.eraseAdmixtureNode(qc);
        
        q_it = &p->getParent();
    }
}

/** Perform the move */
double AdmixtureEdgeReplaceNNI::performSimpleMove( void ) {
    
    
    storedAdmixtureEdges.clear();
    newAdmixtureEdges.clear();
    failed = false;
    numEvents = 0;
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    
    // stage NNI pointers
    size_t numIntNodes = tau.getNumberOfInteriorNodes();
    size_t numTaxa = tau.getNumberOfTips();
    pruneNode = NULL;
    while (pruneNode == NULL || pruneNode == &tau.getRoot() || pruneNode->getNumberOfChildren() != 2)
    {
        pruneNode = &tau.getNode(rng->uniform01() * numIntNodes + numTaxa - 1);
    }
    pruneParent = &pruneNode->getTopologyParent();
    size_t chIdx = rng->uniform01() * 2;
    size_t nphIdx = (chIdx == 0 ? 1 : 0);
    pruneChild = &pruneNode->getTopologyChild(chIdx);
    pruneNephew = &pruneNode->getChild(nphIdx);
    int uncIdx = 0;
    if (pruneNode == &pruneParent->getTopologyChild(uncIdx))
        uncIdx = 1;
    regraftChild = &pruneParent->getTopologyChild(uncIdx);
    
    // fail if uncle age is older than node age
    if (regraftChild->getAge() > pruneNode->getAge() || regraftChild->isOutgroup() != pruneNode->isOutgroup())
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
//    std::cout << "\n";  
//    std::cout << "pch  " << pruneChild << " <- " << &pruneChild->getTopologyParent() << "\n";
//    std::cout << "pnpw " << pruneNephew << " <- " << &pruneNephew->getTopologyParent() << "\n";
//    std::cout << "pnd  " << pruneNode << " -> " << &pruneNode->getTopologyChild(0) << " " << &pruneNode->getTopologyChild(1) << "\n";
//    std::cout << "rch  " << regraftChild << " <- " << &regraftChild->getTopologyParent() << "\n";
//    std::cout << "ppa  " << pruneParent << " -> " << &pruneParent->getTopologyChild(0) << " " << &pruneParent->getTopologyChild(1) << "\n";
//    std::cout << "\n";
    
    // clear and store adm events on child lineage
//    std::cout << "clr unc   " << &pruneParent->getTopologyChild(uncIdx) << "\n";
//    std::cout << "clr ch    " << &pruneNode->getTopologyChild(chIdx) << "\n";
    //storeAdmixtureEventsForLineage(&pruneNode->getTopologyChild(chIdx));
    storeAdmixtureEventsForLineage(pruneChild);
    
    // clear and store adm events on uncle lineage
//    std::cout << "clr unc   " << &pruneParent->getTopologyChild(uncIdx) << "\n";
//    std::cout << "clr unc   " << regraftChild << "\n";
    storeAdmixtureEventsForLineage(regraftChild);
    
    // perform NNI
    pruneNode->removeChild(pruneChild,false);
    pruneParent->removeChild(regraftChild,false);
    pruneNode->addChild(regraftChild,false);
    pruneParent->addChild(pruneChild,false);
    pruneChild->setParent(pruneParent,false);
    regraftChild->setParent(pruneNode,false);
    
    // draw new admixture edges

    //double fwdProposal = 1.0;
    //int numEvents = storedAdmixtureEdges.size();
    for (size_t i = 0; i < storedAdmixtureEdges.size(); i++)
    {
        residuals->touch();
        storedResiduals = residuals->getValue();
        
        size_t numTaxa = tau.getNumberOfTips();
        AdmixtureNode* nodeSrc = NULL;
        AdmixtureNode* nodeDst = NULL;
        size_t k_a = 0;
        size_t k_b = 0;
        
        /*
         if (tau.getNumberOfAdmixtureParents() >= maxEvents)
         {
         failed = true;
         return RbConstants::Double::neginf;
         }
         */
        
        double maxStoredResidual = 0.0;
        for (size_t i = 0; i < storedResiduals.size(); i++)
            if (storedResiduals[i] > maxStoredResidual)
                maxStoredResidual = storedResiduals[i];
        double lambda = delta / maxStoredResidual;
        
        // get sum of positive residuals for each taxon against all other taxa
        std::vector<double> residualWeights_a(numTaxa,0.0);
        
        double sumResidualWeights_a = 0.0;
        for (size_t i = 0; i < numTaxa; i++)
        {
            for (size_t j = 0; j < numTaxa; j++)
            {
                double r = storedResiduals[i * numTaxa + j];
                r = exp(lambda*r);
                if (r > 0.0 && i != j)
                    //if (i != j)
                {
                    residualWeights_a[i] += r;
                    sumResidualWeights_a += r;
                }
                
            }
        }
        
        // if there are no positive residuals, abort move
        if (sumResidualWeights_a == 0.0)
        {
            //std::cout << "no pos residuals\n";
            failed = true;
            return RbConstants::Double::neginf;
        }
        
        
//        std::cout << "rw_a\t";
//        for (size_t i = 0; i < numTaxa; i++)
//            std::cout << residualWeights_a[i] / sumResidualWeights_a << "\t";
//        std::cout << "\n";
        
        
        // sample taxon A from weights
        double u_a = rng->uniform01() * sumResidualWeights_a;
        double m_a = 0.0;
        //size_t k_a = 0;
        
        for (size_t i = 0; i < numTaxa; i++)
        {
            m_a += residualWeights_a[i];
            if (m_a > u_a)
            {
                k_a = i;
                nodeDst = &tau.getNode(k_a);
                break;
            }
        }
        //std::cout << "pick " << k_a << "\n";
        //fwdProposal *= (residualWeights_a[k_a] / sumResidualWeights_a);
        
        // get sum of positive residuals for each taxon wrt taxon A
        std::vector<double> residualWeights_b(numTaxa,0.0);
        double sumResidualWeights_b = 0.0;
        for (size_t i = 0; i < numTaxa; i++)
        {
            double r = storedResiduals[k_a * numTaxa + i];
            r = exp(lambda*r);
            if (r > 0.0 && k_a != i)
                //if (k_a != i)
            {
                residualWeights_b[i] += r;
                sumResidualWeights_b += r;
            }
        }
        
        
//        std::cout << "rw_b\t";
//        for (size_t i = 0; i < numTaxa; i++)
//            std::cout << residualWeights_b[i] / sumResidualWeights_b << "\t";
//        std::cout << "\n";
        
        
        // sample taxon B from weights
        double u_b = rng->uniform01() * sumResidualWeights_b;
        double m_b = 0.0;
        //size_t k_b = 0;
        
        for (size_t i = 0; i < numTaxa; i++)
        {
            m_b += residualWeights_b[i];
            if (m_b > u_b)
            {
                k_b = i;
                nodeSrc = &tau.getNode(k_b);
                break;
            }
        }
//        std::cout << "pick " << k_b << "\n\n";
        //fwdProposal *= (residualWeights_b[k_b] / sumResidualWeights_b);
        
        // get path from tip to root for both nodes
        AdmixtureNode* nd_a = nodeSrc;
        std::list<AdmixtureNode*> path_a;
        //std::cout << "path_a : tip -> root\n";
        while (nd_a != NULL)
        {
            //std::cout << "\tnd_a\t" << nd_a->getIndex() << "\t" << nd_a << "\t" << nd_a->getAge() << "\t" << &nd_a->getParent() << "\n";
            path_a.push_back(nd_a);
            nd_a = &nd_a->getParent();
        }
        nd_a = path_a.back();
        
        AdmixtureNode* nd_b = nodeDst;
        std::list<AdmixtureNode*> path_b;
        //std::cout << "path_b : tip -> root\n";
        while (nd_b != NULL)
        {
            //std::cout << "\tnd_b\t" << nd_b->getIndex() << "\t" << nd_b << "\t" << nd_b->getAge() << "\n";
            path_b.push_back(nd_b);
            nd_b = &nd_b->getParent();
        }
        nd_b = path_b.back();
        
        // find the node where the paths diverge by traversing both paths from root to tip
        AdmixtureNode* mrca = nd_a;
        //std::cout << "mrca : root -> tip\n";
        while (nd_a == nd_b && !path_a.empty() && !path_b.empty())
        {
            mrca = nd_a;
            //    std::cout << "\t" << mrca << "\t" << mrca->getAge() << "\n";
            nd_a = path_a.back();
            nd_b = path_b.back();
            path_a.pop_back();
            path_b.pop_back();
        }
        
        // sample u.a.r. between nd_b and present (we disallow sister branches to admix...)
        double minAge = nodeSrc->getAge();
        if (minAge < nodeDst->getAge())
            minAge = nodeDst->getAge();
        double maxAge = mrca->getAge();
        
        int mrcaChIdx = 0;
        
        if (allowSisterAdmixture == false)// && mrca->getTopologyChild(0).isTip() == false && mrca->getTopologyChild(1).isTip() == false)
        {
            maxAge = mrca->getTopologyChild(0).getAge();
            if (maxAge < mrca->getTopologyChild(1).getAge())
            {
                maxAge = mrca->getTopologyChild(1).getAge();
                mrcaChIdx = 1;
            }
        }
        if (maxAge <= minAge)
        {
            failed = true;
            return RbConstants::Double::neginf;
        }
        //maxAge = mrca->getAge();
        //double admixtureAge = rng->uniform01() * (maxAge - minAge) + minAge;
        double exp_lambda = 2.0;
        double admixtureAge = RbStatistics::Beta::rv(1.0,exp_lambda, *rng) * (maxAge - minAge) + minAge;
        
        
        while (nd_a->getAge() > admixtureAge && !path_a.empty())
        {
            nd_a = path_a.back();
            path_a.pop_back();
            
            if (nd_a->getParent().getAge() > admixtureAge && nd_a->getAge() < admixtureAge)
                break;
        }
        
        while (nd_b->getAge() > admixtureAge && !path_b.empty())
        {
            nd_b = path_b.back();
            path_b.pop_back();
            
            if (nd_b->getParent().getAge() > admixtureAge && nd_b->getAge() < admixtureAge)
                break;
        }
        
        if (GLOBAL_RNG->uniform01() < 0.5)
        {
            AdmixtureNode* tmp = nd_a;
            nd_a = nd_b;
            nd_b = tmp;
        }
        
        //std::cout << "ok\n";
        
        // get admixture weight
        double a = 1.0;
        double b = 2.0;
        double admixtureWeight = RbStatistics::Beta::rv(a, b, *rng);
//        double lnW = 0.0;
        admixtureWeight /= 2;
        
        // add nodes to tree
        AdmixtureNode* storedAdmixtureParent = new AdmixtureNode((int)tau.getNumberOfNodes());
        tau.pushAdmixtureNode(storedAdmixtureParent);
        AdmixtureNode* storedAdmixtureChild = new AdmixtureNode((int)tau.getNumberOfNodes());
        tau.pushAdmixtureNode(storedAdmixtureChild);
        
        // add edge
        tau.addAdmixtureEdge(storedAdmixtureParent, storedAdmixtureChild, nd_a, nd_b, admixtureAge, admixtureWeight, true);
        
        AdmixtureEdgePosition ap(storedAdmixtureParent, storedAdmixtureChild, nd_a, nd_b, admixtureAge, admixtureWeight);
        newAdmixtureEdges.push_front(ap);
        
    }
    
    tau.getRoot().flagNewickRecomputation();
    
    return 0.0;
}


void AdmixtureEdgeReplaceNNI::rejectSimpleMove( void ) {
    
    // we undo the proposal only if it didn't fail
    if ( !failed ) {
        //std::cout << "reject\n";
        
        //        std::cout << "reject\n";
        // restore nodes to admixtureTree vector
        AdmixtureTree& tau = variable->getValue();
        
        
        // remove new edges
        for (std::list<AdmixtureEdgePosition>::iterator it = newAdmixtureEdges.begin(); it != newAdmixtureEdges.end(); it++)
        {
            AdmixtureNode* p = (*it).admixtureParent;
            AdmixtureNode* c = (*it).admixtureChild;
            
            tau.removeAdmixtureEdge(p,false);
            
            tau.eraseAdmixtureNode(p);
            tau.eraseAdmixtureNode(c);
            delete p;
            delete c;
        }
        
        // un-NNI
        pruneParent->removeChild(pruneChild,false);
        pruneNode->removeChild(regraftChild,false);
        pruneParent->addChild(regraftChild,false);
        pruneNode->addChild(pruneChild,false);
        pruneChild->setParent(pruneNode,false);
        regraftChild->setParent(pruneParent,false);

        // restore old edges
        for (std::list<AdmixtureEdgePosition>::iterator it = storedAdmixtureEdges.begin(); it != storedAdmixtureEdges.end(); it++)
        {
            AdmixtureNode* p = (*it).admixtureParent;
            AdmixtureNode* c = (*it).admixtureChild;
            AdmixtureNode* pc = (*it).admixtureParentChild;
            AdmixtureNode* cc = (*it).admixtureChildChild;
            
            tau.addAdmixtureEdge(p,c,pc,cc);
            
            tau.pushAdmixtureNode(p);
            tau.pushAdmixtureNode(c);
            
        }
        
        tau.getRoot().flagNewickRecomputation();
    }
}

void AdmixtureEdgeReplaceNNI::acceptSimpleMove(void) {
    
    if (!failed)
    {
        //std::cout << "accept\n";
        
        for (std::list<AdmixtureEdgePosition>::iterator it = storedAdmixtureEdges.begin(); it != storedAdmixtureEdges.end(); it++)
        {
            AdmixtureNode* p = NULL;
            (*it).admixtureChild->setParent(p);
            (*it).admixtureParent->setParent(p);
            
            delete (*it).admixtureChild;
            delete (*it).admixtureParent;
        }
        
        AdmixtureTree& tau = variable->getValue();
        tau.getRoot().flagNewickRecomputation();
    }
}


void AdmixtureEdgeReplaceNNI::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        //std::cout << "AER\ttau\n";
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    }
    else if (oldN == residuals)
    {
        residuals = static_cast<DeterministicNode<std::vector<double> >* >(newN);
    }
}


void AdmixtureEdgeReplaceNNI::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeReplaceNNI::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    acceptSimpleMove();
}

bool AdmixtureEdgeReplaceNNI::isActive(unsigned long g) const {
    
    return g > activeGen;
}

double AdmixtureEdgeReplaceNNI::performMove( double &probRatio ) {
    
    
    if (changed) {
        throw RbException("Trying to execute a simple moves twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = performSimpleMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf ) {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    variable->touch();
    probRatio = variable->getLnProbabilityRatio();
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            //std::cout << theNode->getName() << "\t" << probRatio << "\n";
        }
    }
    
    return hr;
}
