/* 
 * File:   MultivariatePhyloProcess.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 20:17
 */

#ifndef MULTIVARIATEPHYLOPROCESS_H
#define	MULTIVARIATEPHYLOPROCESS_H


#include "MatrixReal.h"
#include "PrecisionMatrix.h"
#include "MemberObject.h"
#include "TimeTree.h"
#include "ContinuousCharacterData.h"

#include <vector>
#include <string>

namespace RevBayesCore {
    
    class MultivariatePhyloProcess : public MatrixReal, public MemberObject<double> {
    
    public:
        MultivariatePhyloProcess(void);                                                                                                                 //!< Default constructor
        MultivariatePhyloProcess(const TimeTree* intree, size_t indim);                                                                                             //!< Default constructor
        MultivariatePhyloProcess(const MultivariatePhyloProcess& t);                                                                                    //!< Copy constructor
        virtual                                    ~MultivariatePhyloProcess(void);                                                                                                        //!< Destructor
    
        MultivariatePhyloProcess&                   operator=(const MultivariatePhyloProcess& t);
    
        // Basic utility functions
        MultivariatePhyloProcess*                   clone(void) const;                                                                                  //!< Clone object
    
        void                                        executeMethod(const std::string &n, const std::vector<const DagNode*> &args, double &rv) const;     //!< Map the member methods to internal function calls
        const TimeTree*                             getTimeTree() const {return tree;}
        size_t                                      getDim() const {return getNumberOfColumns();}
        

        // MultivariatePhyloProcess functions
        double                                      getRootVal(int k) const;
        double                                      getMean(int k) const;
        double                                      getMeanOverTips(int k) const;
        double                                      getStdev(int k) const;
        void                                        recursiveGetStats(int k, const TopologyNode& from, double& e1, double& e2, int& n) const;
        void                                        recursiveGetStatsOverTips(int k, const TopologyNode& from, double& e1, double& e2, int& n) const;

        void                                        printBranchContrasts(std::ostream& os) const;
        PrecisionMatrix                             getBranchContrasts(int& nnode) const ;

        bool                                        isClamped(size_t index, size_t k) const;
        void                                        clampAt(const ContinuousCharacterData* data, size_t k, size_t l);
        void                                        recursiveClampAt(const TopologyNode& from, const ContinuousCharacterData* data, size_t k, size_t l);
        
    protected:

        void                                        recursiveGetBranchContrasts(const TopologyNode& from, PrecisionMatrix& c, int& n) const;

    private:

        const TimeTree*                     tree;
        std::vector<std::vector<bool> >     clampVector;

    };

    std::ostream&                       operator<<(std::ostream& o, const MultivariatePhyloProcess& x);                                         //!< Overloaded output operator

}

#endif	/* MULTIVARIATEPHYLOPROCESS_H */

