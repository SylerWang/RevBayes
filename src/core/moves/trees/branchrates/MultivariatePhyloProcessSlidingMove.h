/* 
 * File:   MultivariatePhyloProcessSlidingMove.h
 * Author: nl
 *
 * Created on 16 juillet 2014, 23:19
 */

#ifndef MULTIVARIATEPHYLOPROCESSSLIDINGMOVE_H
#define	MULTIVARIATEPHYLOPROCESSSLIDINGMOVE_H

#include "SimpleMove.h"
#include "StochasticNode.h"

#include "MatrixReal.h"
#include "MultivariatePhyloProcess.h"
#include <ostream>
#include <string>


namespace RevBayesCore {
    

    class MultivariatePhyloProcessSlidingMove : public SimpleMove {
        
    public:
        MultivariatePhyloProcessSlidingMove(StochasticNode<MultivariatePhyloProcess>* n, double l, bool t, double w);                         //!< Constructor
        
        // Basic utility functions
        MultivariatePhyloProcessSlidingMove*         clone(void) const;                                                                  //!< Clone this object.
        const std::string&                          getMoveName(void) const;                                                            //!< Get the name of the move for summary printing.
        void                                        swapNode(DagNode *oldN, DagNode *newN);                                             //!< Swap the variable if it was replaced.
        
    protected:
        
        double                                  performSimpleMove(void);                                                            //!< Perform move
        void                                    printParameterSummary(std::ostream &o) const;
        void                                    rejectSimpleMove(void);
        void                                    acceptSimpleMove(void);
        void                                    tune(void);
        void                                    touch( DagNode *toucher );
        
    private:
        
        StochasticNode<MultivariatePhyloProcess>*   variable;

        double                                      lambda;                 
        double                                      storedValue;
        size_t                                      nodeindex;
        size_t                                      compindex;
    };
    
}

#endif	/* MULTIVARIATEPHYLOPROCESSSLIDINGMOVE_H */

