#ifndef CharacterMatrix_H
#define CharacterMatrix_H

#include "MemberObject.h"
#include <set>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class VectorCharacterObservations;
class VectorString;


class CharacterMatrix : public MemberObject {

    public:
                                                    CharacterMatrix(void);                                             //!< Default constructor
    
        // Basic utility functions
        CharacterMatrix*                            clone(void) const;                                                 //!< Clone object
        const VectorString&                         getClass(void) const;                                              //!< Get class vector   

        // Member variable rules
        const MemberRules&                          getMemberRules(void) const;                                        //!< Get member rules
        void                                        setVariable(const std::string& name, DAGNode* var);                //!< Only constants allowed

        // Member method inits
        const MethodTable&                          getMethods(void) const;                                            //!< Get methods
        
        // CharacterMatrix functions
        void                                        addTaxonName(const std::string tName);                             //!< Add taxon name
        void                                        addTaxonObservations(VectorCharacterObservations* obs);            //!< Add obserervations
        void                                        excludeCharacter(size_t i);                                        //!< Exclude character
        void                                        excludeTaxon(size_t i);                                            //!< Exclude taxon
        void                                        excludeTaxon(std::string& s);                                      //!< Exclude taxon
        size_t                                      getNumCharacters(void);                                            //!< Number of characters
        size_t                                      getNumTaxa(void) { return taxonObservations.size(); }              //!< Number of taxa
        bool                                        isCharacterExcluded(size_t i);                                     //!< Is the character excluded
        bool                                        isTaxonExcluded(size_t i);                                         //!< Is the taxon excluded
        bool                                        isTaxonExcluded(std::string& s);                                   //!< Is the taxon excluded
        void                                        restoreCharacter(size_t i);                                        //!< Restore character
        void                                        restoreTaxon(size_t i);                                            //!< Restore taxon
        void                                        restoreTaxon(std::string& s);                                      //!< Restore taxon
        void                                        setDataType(const std::string dt) { dataType = dt; }               //!< Set the data type
        void                                        setFileName(const std::string fn) { fileName = fn; }               //!< Set the file name

	protected:
        DAGNode*                                    executeOperation(const std::string& name, ArgumentFrame& args);    //!< Execute method
    
    private:
        size_t                                      indexOfTaxonWithName(std::string& s);                              //!< Get the index of the taxon
        std::vector<VectorCharacterObservations*>   taxonObservations;                                                 //!< Vector of character observations
        std::vector<std::string>                    taxonNames;                                                        //!< Vector of taxon names
        std::set<size_t>                            deletedTaxa;                                                       //!< Set of deleted taxa
        std::set<size_t>                            deletedCharacters;                                                 //!< Set of deleted characters
        std::string                                 dataType;                                                          //!< Type of data in matrix
        std::string                                 fileName;                                                          //!< The path/filename from where this matrix originated
};

#endif

