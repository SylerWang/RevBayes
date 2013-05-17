//
//  AdmixtureEdgeReplaceResidualWeights.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/3/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "AdmixtureEdgeReplaceResidualWeights.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"

#include <list>
#include <cmath>

using namespace RevBayesCore;

AdmixtureEdgeReplaceResidualWeights::AdmixtureEdgeReplaceResidualWeights(StochasticNode<AdmixtureTree> *v, StochasticNode<double>* r, DeterministicNode<std::vector<double> >* res, ConstantNode<int>* dt, bool asa, double w) : Move( v, w), variable( v ), rate(r), residuals(res), delayTimer(dt), changed(false), failed(false), failedAdd(false), allowSisterAdmixture(asa) {
    
    nodes.insert(rate);
    nodes.insert(residuals);
    nodes.insert(dt);
    
}



/* Clone object */
AdmixtureEdgeReplaceResidualWeights* AdmixtureEdgeReplaceResidualWeights::clone( void ) const {
    
    return new AdmixtureEdgeReplaceResidualWeights( *this );
}



const std::string& AdmixtureEdgeReplaceResidualWeights::getMoveName( void ) const {
    static std::string name = "AdmixtureEdgeReplaceResidualWeights";
    
    return name;
}

void AdmixtureEdgeReplaceResidualWeights::findDescendantTips(std::set<AdmixtureNode*>& s, AdmixtureNode* p)
{
    if (p->getNumberOfChildren() == 0)
    {
        //std::cout << p << "\t" << p->getIndex() << "\n";
        s.insert(p);
    }
    else
    {
        for (size_t i = 0; i < p->getNumberOfChildren(); i++)
            findDescendantTips(s, &p->getChild(i));
    }
}


/** Perform the move */
double AdmixtureEdgeReplaceResidualWeights::performSimpleMove( void ) {
    
    std::cout << "replace_RW\n";
    failed = false;
    failedAdd = false;
    storedResiduals = residuals->getValue();
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    AdmixtureTree& tau = variable->getValue();
    AdmixtureNode* root = &tau.getRoot();

    std::vector<AdmixtureNode*> admixtureParents = tau.getAdmixtureParents();
    
    // if no admixtureParent exists, the proposal fails
    if (admixtureParents.size() == 0)
    {
        failed = true;
        return RbConstants::Double::neginf;
    }
    
    // otherwise, proceed
    else
    {
        // proposal densities
        double fwdProposal = 1.0;
        double bwdProposal = 1.0;
        
        failed = false;
        failedAdd = false;

        // sample a random admixture parent node
        double u = rng->uniform01();
        size_t index = std::floor(admixtureParents.size() * u);
        
       // std::cout << "\n\treplace, before all\n";
       // tau.checkAllEdgesRecursively(root);

       //  std::cout << "\n\treplace, remove\n";
        
        // store admixture edge position
        storedAdmixtureParent = admixtureParents[index];
        storedAdmixtureChild = &storedAdmixtureParent->getAdmixtureChild();
        storedAdmixtureChildParent = &storedAdmixtureChild->getParent();
        storedAdmixtureChildChild = &storedAdmixtureChild->getChild(0);
        storedAdmixtureParentParent = &storedAdmixtureParent->getParent();
        storedAdmixtureParentChild = &storedAdmixtureParent->getChild(0);
       
        
        /*
        std::cout << "\n";
        std::cout << "stored AP " << storedAdmixtureParentParent << " -> " << storedAdmixtureParent << " -> " << storedAdmixtureParentChild << "\n";
        std::cout << "new    AP " << newAdmixtureParentParent << " -> " << storedAdmixtureParent << " -> " << newAdmixtureParentChild << "\n";
        std::cout << "stored AC " << storedAdmixtureChildParent << " -> " << storedAdmixtureChild << " -> " << storedAdmixtureChildChild << "\n";
        std::cout << "new    AC " << newAdmixtureChildParent << " -> " << storedAdmixtureChild << " -> " << newAdmixtureChildChild << "\n";
        */

        
        // remove admixture edge from graph
        storedAdmixtureChild->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChildChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChildChild);
        
        storedAdmixtureParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParentChild->setParent(storedAdmixtureParentParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParentChild);
        
        // get age for admixture event
        storedAdmixtureAge = storedAdmixtureChild->getAge();
        storedAdmixtureWeight = storedAdmixtureChild->getWeight();
        
        // initialize NA__
        newAdmixtureChildChild = storedAdmixtureChildChild;
        newAdmixtureChildParent = storedAdmixtureChildParent;
        newAdmixtureParentChild = storedAdmixtureParentChild;
        newAdmixtureParentParent = storedAdmixtureParentParent;
        
        //tau.checkAllEdgesRecursively(root);
        
        //////////////////////////////////////////////////
        // add event ...
        //std::cout << "\n\treplace, add\n";
        
        
        size_t numTaxa = tau.getNumberOfTips()-2;
        AdmixtureNode* nodeSrc = NULL;
        AdmixtureNode* nodeDst = NULL;
        size_t k_a = 0;
        size_t k_b = 0;
       // std::cout << "numTaxa\t" << numTaxa << "\n";
        
        double v = rng->uniform01();
        
        if (v < 0.5)
        {
            
            // get sum of positive residuals for each taxon against all other taxa
            std::vector<double> residualWeights_a(numTaxa,0.0);
            
            double sumResidualWeights_a = 0.0;
            for (size_t i = 0; i < numTaxa; i++)
            {
                for (size_t j = 0; j < numTaxa; j++)
                {
                    double r = storedResiduals[i * numTaxa + j];
                    if (r > 0.0 && i != j)
                    {
                        residualWeights_a[i] += r;
                        sumResidualWeights_a += r;
                    }
                }
            }
            
            // if there are no positive residuals, abort move
            if (sumResidualWeights_a == 0.0)
            {
                std::cout << "no pos residuals\n";
                failedAdd = true;
                return RbConstants::Double::neginf;
            }
            
            std::cout << "rw_a\t";
            for (size_t i = 0; i < numTaxa; i++)
                std::cout << residualWeights_a[i] / sumResidualWeights_a << "\t";
            std::cout << "\n";
            
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
                    nodeDst = &tau.getNodeByIndex(k_a);
                    break;
                }
            }
            fwdProposal *= (residualWeights_a[k_a] / sumResidualWeights_a);
            
            // get sum of positive residuals for each taxon wrt taxon A
            std::vector<double> residualWeights_b(numTaxa,0.0);
            double sumResidualWeights_b = 0.0;
            for (size_t i = 0; i < numTaxa; i++)
            {
                double r = storedResiduals[k_a * numTaxa + i];
                if (r > 0.0 && k_a != i)
                {
                    residualWeights_b[i] += r;
                    sumResidualWeights_b += r;
                }
            }
            
                std::cout << "rw_b\t";
            for (size_t i = 0; i < numTaxa; i++)
                std::cout << residualWeights_b[i] / sumResidualWeights_b << "\t";
            std::cout << "\n";
            
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
                    nodeSrc = &tau.getNodeByIndex(k_b);
                    break;
                }
            }
            fwdProposal *= (residualWeights_b[k_b] / sumResidualWeights_b);
        }
        else
        {
            //std::cout << "random\n";
            k_a = rng->uniform01() * numTaxa;
            do
            {
                k_b = rng->uniform01() * numTaxa;
            } while(k_a == k_b);
            nodeDst = &tau.getNodeByIndex(k_a);
            nodeSrc = &tau.getNodeByIndex(k_b);
        }
        
        std::cout << "taxa pair\t" << k_a << "\t" << k_b << "\n";
        //std::cout << "fwdProposal\t" << fwdProposal << "\tlnFwdProposal\t" << log(fwdProposal) << "\n";
        
        // get path from tip to root for both nodes
        AdmixtureNode* nd_a = nodeDst;
        std::list<AdmixtureNode*> path_a;
    //    std::cout << "path_a : tip -> root\t" << nd_a << "\n";
        while (nd_a != NULL)
        {
    //        std::cout << "\t" << nd_a << "\t" << nd_a->getAge() << "\n";
            path_a.push_back(nd_a);
            nd_a = &nd_a->getParent();
        }
        nd_a = path_a.back();
        
        AdmixtureNode* nd_b = nodeSrc;
        std::list<AdmixtureNode*> path_b;
    //    std::cout << "path_b : tip -> root\t" << nd_b << "\n";
        while (nd_b != NULL)
        {
     //       std::cout << "\t" << nd_b << "\t" << nd_b->getAge() << "\n";
            path_b.push_back(nd_b);
            nd_b = &nd_b->getParent();
        }
        nd_b = path_b.back();
        
        // find the node where the paths diverge by traversing both paths from root to tip
        AdmixtureNode* mrca = nd_a;
     //   std::cout << "mrca : root -> tip\n";
        while (nd_a == nd_b && !path_a.empty() && !path_b.empty())
        {
            mrca = nd_a;
     //       std::cout << "\t" << mrca << "\t" << mrca->getAge() << "\n";
            nd_a = path_a.back();
            nd_b = path_b.back();
            path_a.pop_back();
            path_b.pop_back();
        }
        
        // sample u.a.r. between nd_b and present
        double minAge = nodeSrc->getAge();
        if (nodeDst->getAge() < minAge)
            minAge = nodeDst->getAge();
        double maxAge = mrca->getAge();
        
        int mrcaChIdx = 0;
        // if (allowSisterAdmixture == false)
        if (allowSisterAdmixture == false && mrca->getTopologyChild(0).isTip() == false && mrca->getTopologyChild(1).isTip() == false)
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
     //       std::cout << "maxAge <= minAge\t" << maxAge << " < " << minAge << "\n";
            failedAdd = true;
            return RbConstants::Double::neginf;
        }
        //maxAge = mrca->getAge();
        double admixtureAge = rng->uniform01() * (maxAge - minAge) + minAge;
        
        double a = 1.0;
        double b = 1.0;
        double admixtureWeight = RbStatistics::Beta::rv(a, b, *rng);
        admixtureWeight /= 2;
      
        // attach edge as appropriate from aPath to bPath
        // front=0, back=1
        
     //   std::cout << "a_path : find admixtureAge\n";
        while (nd_a->getAge() > admixtureAge && !path_a.empty())
        {
            nd_a = path_a.back();
       //     std::cout << "\t" << nd_a << "\t" << nd_a->getAge() << "\n";
            path_a.pop_back();
            
            if (nd_a->getParent().getAge() > admixtureAge && nd_a->getAge() < admixtureAge)
                break;
        }
        
        // ... prob of selecting a certain source branch
        // double t_a = nd_a->getParent().getAge();
        // fwdProposal *= t_a / (t_a - nodeSrc->getAge());
        
       // std::cout << "b_path : find admixtureAge\n";
        while (nd_b->getAge() > admixtureAge && !path_b.empty())
        {
            nd_b = path_b.back();
        //    std::cout << "\t" << nd_b << "\t" << nd_b->getAge() << "\n";
            path_b.pop_back();
            
            if (nd_b->getParent().getAge() > admixtureAge && nd_b->getAge() < admixtureAge)
                break;
        }
        
        
        //////////////////////////////////////////////////
        
        
        
        
        // placeholder for add
        // nd_a = storedAdmixtureParentChild;
        // nd_b = storedAdmixtureChildChild;
        double newAge = admixtureAge;
        double newWeight = admixtureWeight;
       // std::cout << "age\t" << newAge << "\n";
      //  std::cout << "wt \t" << newWeight << "\n";

        // add into graph
                
        // store adjacent nodes to new parent node
        newAdmixtureParentChild = nd_a;
        newAdmixtureParentParent = &nd_a->getParent();
        
        // insert admixtureParent into graph
        storedAdmixtureParent->setAge(newAge);
        storedAdmixtureParent->setParent(root);
        newAdmixtureParentChild->setParent(storedAdmixtureParent);
        newAdmixtureParentParent->addChild(storedAdmixtureParent);
        newAdmixtureParentParent->removeChild(newAdmixtureParentChild); // 
        storedAdmixtureParent->addChild(newAdmixtureParentChild);
        storedAdmixtureParent->setParent(newAdmixtureParentParent);
        
        // store adjacent nodes to new child node
        newAdmixtureChildChild = nd_b;
        newAdmixtureChildParent = &nd_b->getParent();
        
        // insert admixtureChild into graph
        storedAdmixtureChild->setAge(newAge);
        storedAdmixtureChild->setWeight(newWeight);
        storedAdmixtureChild->setParent(root);
        newAdmixtureChildChild->setParent(storedAdmixtureChild);
        newAdmixtureChildParent->addChild(storedAdmixtureChild);
        newAdmixtureChildParent->removeChild(newAdmixtureChildChild);
        storedAdmixtureChild->addChild(newAdmixtureChildChild);
        storedAdmixtureChild->setParent(newAdmixtureChildParent);
        
        // update with outgroup settings
        storedAdmixtureChild->setOutgroup(storedAdmixtureChildParent->isOutgroup());
        storedAdmixtureParent->setOutgroup(storedAdmixtureParentParent->isOutgroup());
        
     //   tau.checkAllEdgesRecursively(root);
        
        // ln hastings ratio
        double lnFwdProposal = log(fwdProposal);
        double lnBwdProposal = log(bwdProposal);
        
        std::cout << "replace lnPropRat\t" << lnBwdProposal - lnFwdProposal << " = " << lnBwdProposal << " - " << lnFwdProposal << ";\t";
        std::cout << storedAdmixtureAge << " -> " << newAge << "\n";
        
       // return lnBwdProposal - lnFwdProposal;
        
       /*
        std::cout << "\n";
        std::cout << "stored AP " << storedAdmixtureParentParent << " -> " << storedAdmixtureParent << " -> " << storedAdmixtureParentChild << "\n";
        std::cout << "new    AP " << newAdmixtureParentParent << " -> " << storedAdmixtureParent << " -> " << newAdmixtureParentChild << "\n";
        std::cout << "stored AC " << storedAdmixtureChildParent << " -> " << storedAdmixtureChild << " -> " << storedAdmixtureChildChild << "\n";
        std::cout << "new    AC " << newAdmixtureChildParent << " -> " << storedAdmixtureChild << " -> " << newAdmixtureChildChild << "\n";
*/

        
        return 0.0;
        //return -1000.0;
    }
}


void AdmixtureEdgeReplaceResidualWeights::rejectSimpleMove( void ) {

    
    if (failedAdd == true)
    {
        // NOTE: root used to protect from infinite recursions caused by AdmixtureNode::flagNewickComputation() from addChild,removeChild,setParent.
        AdmixtureNode* root = &variable->getValue().getRoot();
  
        /*
        std::cout << "\n\tbefore reject no residuals, or maxAge<=minAge\n";
        variable->getValue().checkAllEdgesRecursively(root);
        
        std::cout << storedAdmixtureParent << "\n";
        std::cout << storedAdmixtureParentChild << "\t" << storedAdmixtureParentParent << "\n";
        std::cout << newAdmixtureParentChild << "\t" << newAdmixtureParentParent << "\n";
        std::cout << storedAdmixtureChild << "\n";
        std::cout << storedAdmixtureChildChild << "\t" << storedAdmixtureChildParent << "\n";
        std::cout << newAdmixtureChildChild << "\t" << newAdmixtureChildParent << "\n";
        */
         
        // revert admixture parent
       // storedAdmixtureParent->setParent(root);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent);
        storedAdmixtureParent->addChild(storedAdmixtureParentChild);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild);
        storedAdmixtureParent->setParent(storedAdmixtureParentParent);
        storedAdmixtureParentChild->setParent(storedAdmixtureParent);
        
        storedAdmixtureParent->setAge(storedAdmixtureAge);
        
        // revert admixture child
        //storedAdmixtureChild->setParent(root);
        storedAdmixtureChild->addChild(storedAdmixtureChildChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChild->setParent(storedAdmixtureChildParent);
        storedAdmixtureChildChild->setParent(storedAdmixtureChild);

        storedAdmixtureChild->setAge(storedAdmixtureAge);
        storedAdmixtureChild->setWeight(storedAdmixtureWeight);
        
        //std::cout << "\n\tafter reject\n";
        //variable->getValue().checkAllEdgesRecursively(root);
        
        // revert the age
        
    }
    else if (failed == false)
    {
        // NOTE: root used to protect from infinite recursions caused by AdmixtureNode::flagNewickComputation() from addChild,removeChild,setParent.
        AdmixtureNode* root = &variable->getValue().getRoot();

        /*
        std::cout << "\nreject\n";
        std::cout << "stored AP " << storedAdmixtureParentParent << " -> " << storedAdmixtureParent << " -> " << storedAdmixtureParentChild << "\n";
        std::cout << "new    AP " << newAdmixtureParentParent << " -> " << storedAdmixtureParent << " -> " << newAdmixtureParentChild << "\n";
        std::cout << "stored AC " << storedAdmixtureChildParent << " -> " << storedAdmixtureChild << " -> " << storedAdmixtureChildChild << "\n";
        std::cout << "new    AC " << newAdmixtureChildParent << " -> " << storedAdmixtureChild << " -> " << newAdmixtureChildChild << "\n";

        std::cout << "\n\tbefore reject regular\n";
        variable->getValue().checkAllEdgesRecursively(root);
         */
        
        // remove new admixture edge from graph
        storedAdmixtureChild->removeChild(newAdmixtureChildChild);
        newAdmixtureChildChild->setParent(newAdmixtureChildParent);
        newAdmixtureChildParent->removeChild(storedAdmixtureChild);
        newAdmixtureChildParent->addChild(newAdmixtureChildChild);
        
        storedAdmixtureParent->removeChild(newAdmixtureParentChild);
        newAdmixtureParentChild->setParent(newAdmixtureParentParent);
        newAdmixtureParentParent->removeChild(storedAdmixtureParent);
        newAdmixtureParentParent->addChild(newAdmixtureParentChild);

        // restore admixture edge values
        storedAdmixtureParent->setAge(storedAdmixtureAge);
        storedAdmixtureChild->setAge(storedAdmixtureAge);
        storedAdmixtureChild->setWeight(storedAdmixtureWeight);
        
        // restore stored admixture edge
        storedAdmixtureParent->setParent(root);
        storedAdmixtureParentChild->setParent(storedAdmixtureParent);
        storedAdmixtureParentParent->addChild(storedAdmixtureParent);
        storedAdmixtureParentParent->removeChild(storedAdmixtureParentChild); //
        storedAdmixtureParent->addChild(storedAdmixtureParentChild);
        storedAdmixtureParent->setParent(storedAdmixtureParentParent);

        storedAdmixtureChild->setParent(root);
        storedAdmixtureChildChild->setParent(storedAdmixtureChild);
        storedAdmixtureChildParent->addChild(storedAdmixtureChild);
        storedAdmixtureChildParent->removeChild(storedAdmixtureChildChild);
        storedAdmixtureChild->addChild(storedAdmixtureChildChild);
        storedAdmixtureChild->setParent(storedAdmixtureChildParent);

    
        // update with outgroup settings
        storedAdmixtureChild->setOutgroup(storedAdmixtureChildParent->isOutgroup());
        storedAdmixtureParent->setOutgroup(storedAdmixtureParentParent->isOutgroup());
    }
    
    
    
    else
        std::cout << "rpl_RW failed\n"; 
}

void AdmixtureEdgeReplaceResidualWeights::acceptSimpleMove(void)
{
    //std::cout << "add_RW accept\n";
    //  std::cout << "\t" << variable->getValue().getNumberOfAdmixtureChildren();
    //  std::cout << "\n";
}


void AdmixtureEdgeReplaceResidualWeights::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    Move::swapNode(oldN, newN);
    
    if (oldN == variable)
    {
        //std::cout << "AEA\ttau\n";
        variable = static_cast<StochasticNode<AdmixtureTree>* >(newN) ;
    }
    else if (oldN == rate)
    {
        //std::cout << "AEA\trate\n";
        rate = static_cast<StochasticNode<double>* >(newN);
    }
    else if (oldN == delayTimer)
    {
        delayTimer = static_cast<ConstantNode<int>* >(newN);
    }
    else if (oldN == residuals)
    {
        residuals = static_cast<DeterministicNode<std::vector<double> >* >(newN);
    }
}


void AdmixtureEdgeReplaceResidualWeights::rejectMove( void ) {
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).
    rejectSimpleMove();
    
    // touch the node
    variable->touch();
}


void AdmixtureEdgeReplaceResidualWeights::acceptMove( void ) {
    // nothing to do
    changed = false;
    
    std::cout << "rpl_RW accept\n";
    
    acceptSimpleMove();
}

double AdmixtureEdgeReplaceResidualWeights::performMove( double &probRatio ) {
    
    std::cout << "\nAdmix Edge Replace\n";
    
    if (delayTimer->getValue() > 0)
    {
        failed = true;
        //delayTimer->setValue(delayTimer->getValue()-1);
        //        delay--;
        return RbConstants::Double::neginf;
    }
    
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
    
    probRatio = 0.0;
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf ) {
        
        std::set<DagNode* > affectedNodes;
        variable->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i) {
            DagNode* theNode = *i;
            probRatio += theNode->getLnProbabilityRatio();
            std::cout << theNode->getName() << "\t" << probRatio << "\n";
        }
    }
    
    return hr;
}