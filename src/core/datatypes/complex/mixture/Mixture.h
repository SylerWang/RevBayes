/**
 * @file
 * This file contains the declaration of Mixture, a type
 * used to hold a mixture model, where elements are assigned different parameter values.
 *
 * @brief Declaration of Mixture
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#ifndef Mixture_h
#define Mixture_h

#include "MutableMemberObject.h"
#include <string>

class Mixture: public MutableMemberObject {
    
public:
    Mixture(void);                                                                                          //!< Default constructor
    Mixture(VectorNatural allocationVector, Vector parameters);                                             //!< constructor
    Mixture(int numberOfElements, Vector parameters);                                                       //!< constructor
    Mixture(VectorNatural allocationVector, int numberOfClasses);                                           //!< constructor
    Mixture(int numberOfElements, int numberOfClasses);                                                     //!< constructor
    Mixture(const Mixture& m);                                                                              //!< Copy constructor
    ~Mixture(void);                                                                                         //!< Destructor
    
    // Basic utility functions
    Mixture*                        clone(void) const;                                                      //!< Clone object
    const VectorString&             getClass(void) const;                                                   //!< Get class vector
    const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
    void                            printValue(std::ostream& o) const;                                      //!< Print value for user
    std::string                     richInfo(void) const;                                                   //!< Complete info
    
    // Member variable functions
    const MemberRules&              getMemberRules(void) const;                                             //!< Get member rules
    void                            setMemberVariable(const std::string& name, Variable* var);              //!< Catch setting of the topology
    
    // Member method inits
    RbLanguageObject*               executeOperation(const std::string& name, Environment& args);           //!< Execute method
    const MethodTable&              getMethods(void) const;                                                 //!< Get methods
    
    // Mixture functions
    size_t                          getNumberOfClasses();                                                   //!< Get the number of classes in the mixture
    void                            setNumberOfClasses(size_t numClasses);                                  //!< Set the number of classes in the mixture
    void                            addClass();                                                             //!< Add a new class to the mixture
    void                            removeClass();                                                          //!< Remove a class from the mixture
    Vector                          getVectorOfParameters();                                                //!< Get the vector of parameter values associated to the classes of the mixture
    void                            setVectorOfParameters(Vector parameters);                               //!< Set the vector of parameter values associated to the classes of the mixture
    void                            setParameterValue();                                                    //!< Set the value of a parameter associated to a particular class
    VectorNatural                   getAllocationVector();                                                  //!< Get the allocation vector associating class indices to elements
    void                            setAllocationVector(Vector allocationVector);                           //!< Set the allocation vector associating class indices to elements
    void                            allocateElement (int elementId, int classId);                           //!< Change the class of a particular element
    VectorRealPos                   getClassProbabilities();                                                //!< Get the vector containing class probabilities
    void                            setClassProbabilities();                                                //!< Set the vector containing class probabilities
    
private:
    static const TypeSpec           typeSpec_;                                                              
    VectorNatural                   allocationVector_;                                                      //!< Vector allocating elements to cluster indices
    Vector                          parameters_;                                                            //!< Vector of size the number of classes and containing parameters associated to the classes
    Vector                          numberOfElementsInClasses_;                                             //!< Vector giving the number of elements in each class
    VectorRealPos                   classProbabilities_;                                                    //!< Vector giving class probabilities
};

#endif