//
//  AdmixtureBipartitionMonitor.h
//  rb_mlandis
//
//  Created by Michael Landis on 5/27/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __rb_mlandis__AdmixtureBipartitionMonitor__
#define __rb_mlandis__AdmixtureBipartitionMonitor__

#include "Monitor.h"
#include "AdmixtureTree.h"
#include "TypedDagNode.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace RevBayesCore {
    
    // store results
    struct AdmixtureEdgeRecord
    {
        int cycle;
        bool isAdmixtureEvent;
        double weight;
        double age;
        double brlen;
        double rate;
        
        AdmixtureEdgeRecord(int c, bool tf, double w, double a, double b, double r)
        {
            cycle = c;
            isAdmixtureEvent = tf;
            weight = w;
            age = a;
            brlen = b;
            rate = r;
        }
    };
    
    // sort results
    struct AdmixtureBipartitionCompare {
        
        bool operator() (const std::pair<std::pair<std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureEdgeRecord> >& lhs, const std::pair<std::pair<std::vector<bool>, std::vector<bool> >, std::vector<AdmixtureEdgeRecord> >& rhs) const
        {
            return lhs.second.size() > rhs.second.size();
        }
    };
    
    struct TreeBipartitionCompare {
        bool operator() (const std::pair<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >& lhs, const std::pair<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >& rhs) const
        {
            return lhs.second.size() > rhs.second.size();
        }
    };
    
    class AdmixtureBipartitionMonitor : public Monitor {
        
    public:
        // Constructors and Destructors
        AdmixtureBipartitionMonitor(TypedDagNode<AdmixtureTree> *t, TypedDagNode< std::vector< double > >* br, TypedDagNode<int>* dt, int ntr, int nar, int g, const std::string &fname, const std::string &del, bool pp=true, bool l=true, bool pr=true, bool ap = false);
        AdmixtureBipartitionMonitor(const AdmixtureBipartitionMonitor& f);
        
        // basic methods
        AdmixtureBipartitionMonitor*        clone(void) const;                                                  //!< Clone the object
        
        // Monitor functions
        void                                monitor(long gen);                                                  //!< Monitor at generation gen
        void                                swapNode(DagNode *oldN, DagNode *newN);
        
        // FileMonitor functions
        void                                closeStream(void);                                                  //!< Close stream after finish writing
        void                                openStream(void);                                                   //!< Open the stream for writing
        void                                printHeader(void);                                                  //!< Print header
        
    private:
        
        void                                initializeTaxonBipartition(void);
        void                                findTaxonBipartition(std::vector<bool>& b, AdmixtureNode* p);
        void                                flipTaxonBipartitionToMinor(std::vector<bool>& b);
        //void                                updateBipartitions(void);
        std::string                         buildBipartitionString(void);
        //std::string                         buildAdmixtureBipartitionString(void);
        
        // the stream to print
        std::fstream                        outStream;
        
        // parameters
        TypedDagNode<AdmixtureTree>*        tree;
        TypedDagNode< std::vector< double > >* branchRates;
        TypedDagNode<int>* delayTimer;
        
        std::set<DagNode *>                 nodeVariables;
        std::string                         filename;
        std::string                         separator;
        bool                                posterior;
        bool                                prior;
        bool                                likelihood;
        bool                                append;
        
        std::map<std::vector<bool>, std::vector<AdmixtureEdgeRecord> >                              treeBipartitions;
        std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureEdgeRecord> > > AdmixtureBipartitions;
        
        
        
        size_t numTaxa;
        size_t numSamples;
        size_t numTreeResults;
        size_t numAdmixtureResults;
        
    };
    
    std::ostream& operator<<(std::ostream& o, const std::map<std::vector<bool>, std::map<std::vector<bool>,std::vector<AdmixtureNode*> > >& x);
    
}

#endif /* defined(__rb_mlandis__AdmixtureBipartition__) */
