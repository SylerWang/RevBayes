//
//  AdmixtureTree.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/21/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureTree__
#define __revbayes_mlandis__AdmixtureTree__


#include "AdmixtureNode.h"
#include "Tree.h"

#include <map>
#include <vector>
#include <string>

namespace RevBayesCore {
    
    class AdmixtureTree : public Tree {
        
    public:
        AdmixtureTree(void);                                                                                                     //!< Default constructor
        AdmixtureTree(const AdmixtureTree& t);                                                                                        //!< Copy constructor
        virtual                                    ~AdmixtureTree(void);                                                                                                        //!< Destructor
        
        AdmixtureTree&                              operator=(const AdmixtureTree& t);
        
        // Basic utility functions
        AdmixtureTree*                              clone(void) const;                                                      //!< Clone object
        
        // AdmixtureTree functions
        void                                        equalizeBranchLengths(void);                                            //!< Equalizing branch lengths between subtrees so that no rounding errors happen
        void                                        fillNodeTimes(void);                                                    //!< Compute the branch length, age and time of each node.
        double                                      getAge(size_t idx) const;
        double                                      getBranchLength(size_t idx) const;
        std::vector<std::string>                    getNames() const;
        AdmixtureNode&                              getNode(size_t idx);                                                    //!< Get the node at index
        const AdmixtureNode&                        getNode(size_t idx) const;                                              //!< Get the node at index
        std::vector<TopologyNode*>                  getNodes(void) const;                                                   //!< Get a pointer to the nodes in the AdmixtureTree
        AdmixtureNode&                              getNodeByIndex(size_t idt);                                             //!< Get the node with member index == idx
        const AdmixtureNode&                        getNodeByIndex(size_t idt) const;                                       //!< Get the node with member index == idx
        std::vector<TopologyNode*>                  getNodesByIndex(void) const;                                            //!< Get a vector of nodes ordered by index
        size_t                                      getNumberOfInteriorNodes(void) const;                                   //!< Get the number of nodes in the AdmixtureTree
        size_t                                      getNumberOfNodes(void) const;                                           //!< Get the number of nodes in the AdmixtureTree
        size_t                                      getNumberOfTips(void) const;                                            //!< Get the number of tip nodes in the AdmixtureTree
        const AdmixtureNode&                        getInteriorNode(int indx) const;                                        //!< Get a pointer to interior node i
        const std::string&                          getNewickRepresentation() const;                                        //!< Get the newick representation of this AdmixtureTree
        AdmixtureNode&                              getRoot(void);                                                          //!< Get a pointer to the root node of the AdmixtureTree
        const AdmixtureNode&                        getRoot(void) const;                                                    //!< Get a pointer to the root node of the AdmixtureTree
        double                                      getTime(size_t idx) const;
        AdmixtureNode&                              getTipNode(size_t indx);                                                //!< Get a pointer to tip node i
        const AdmixtureNode&                        getTipNode(size_t indx) const;                                          //!< Get a pointer to tip node i
        double                                      getTreeLength(void) const;
        double                                      getTreeHeight(void) const;
        double                                      getUnitTreeLength(void) const;
        bool                                        isBinary(void) const;                                                   //!< Is the AdmixtureTree rooted
        bool                                        isRooted(void) const;                                                   //!< Is the AdmixtureTree rooted
        void                                        setAge(size_t idx, double a);
        void                                        setRooted(bool tf);                                                     //!< Set the rootedness of the AdmixtureTree
        void                                        setRoot(TopologyNode* r);                                               //!< Set the root and bootstrap the AdmixtureTree from it
        void                                        setRoot(AdmixtureNode* r);                                              //!< Set the root and bootstrap the AdmixtureTree from it
       
        size_t                                      getNumberOfAdmixtureParents(void) const;
        size_t                                      getNumberOfAdmixtureChildren(void) const;
        bool                                        updateTopologicalSort(void);
        std::vector<AdmixtureNode*>                 getAdmixtureParents(void) const;
        std::vector<AdmixtureNode*>                 getFirstAdmixtureParentPerLineage(void) const;
        void                                        pushAdmixtureNode(AdmixtureNode* p);
        void                                        eraseAdmixtureNode(AdmixtureNode* p);
        void                                        checkAllEdges(void);
        void                                        checkAllEdgesRecursively(AdmixtureNode* p) const;

    protected:
        void                                        resizeElementVectors(size_t n);
        
    private:
        void                                        equalizeBranchLengths(AdmixtureNode &node);                                  //!< Equalizing branch lengths between subtrees so that no rounding errors happen
        void                                        fillNodeTimes(AdmixtureNode &node);                                          //!< Compute the branch length, age and time of each node.
        void                                        fillNodesByPreorderTraversal(AdmixtureNode* node);                           //!< fill the nodes vector by a preorder traversal recursively starting with this node.
        void                                        fillNodesByPhylogeneticTraversal(AdmixtureNode* node);                       //!< fill the nodes vector by a preorder traversal recursively starting with this node.
        void                                        fillNodesByIndex(void);
        bool                                        fillNodesByTopologicalSort(AdmixtureNode* node, std::map<AdmixtureNode*,bool> markTopologicalSortLocal);                        // sorts tree DAG by conditions, returns false if cycle found
        
        std::vector<AdmixtureNode*>                 nodesByTopologicalSort;
        std::map<AdmixtureNode*,bool>               markTopologicalSortGlobal;
        
        //std::vector<AdmixtureNode*>                 admixtureParents;
        //std::vector<AdmixtureNode*>                 admixtureChildren;
        
        std::vector<AdmixtureNode*>                 nodes;                                                                  //!< Vector of pointers to all nodes
        std::vector<AdmixtureNode*>                 nodesByIndex;                                                           //!< Vector of pointers to all nodes indexed by "index" -- NOTE: THIS IS NOT IMPLEMENTED OR USED FOR ADMIXTURETREE
        AdmixtureNode*                              root;                                                                   //!< Pointer to the root node
        bool                                        binary;                                                                 //!< Is the AdmixtureTree binary?
        size_t                                      numTips;
        size_t                                      numNodes;
        std::vector<double>                         times;
        std::vector<double>                         ages;

    };
    
    // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const AdmixtureTree& x);                                         //!< Overloaded output operator

}

#endif /* defined(__revbayes_mlandis__AdmixtureTree__) */