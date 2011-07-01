/**
 * @file
 * This file contains the declaration of Container, which is
 * a generic container of RbObject objects.
 *
 * @brief Declaration of Container
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id:$
 */

#ifndef Container_H
#define Container_H

#include "RbComplex.h"

#include <ostream>
#include <string>
#include <vector>

class RbObject;
class VectorNatural;
class VectorIndex;
class VectorInteger;
class VectorString;


/**
 * This class is used for all complex data types seen by the parser
 * as being multidimensional. Unlike all other RbObject objects, 
 * their object type does not correspond to their language type.
 * Instead, their language type is the same as their dim and element
 * type. This allows the parser to handle the container and its
 * elements as one unit.
 *
 * For instance, the language type of a MatrixReal, which is a
 * specialized type of Container, is Real[][] and not MatrixReal,
 * its object type returned by the getType() function.
 *
 * The getTypeSpec() function should be used to get the language type
 * of the object; for a MatrixReal, the getTypeSpec() function returns
 * a TypeSpec object containing type Real and dim 2. For RbObject
 * types other than Container types, the TypeSpec object returns the
 * object type and dim 0. For instance, a Simplex object, which is a member
 * object, should have dim 0 and type Simplex, that is, in the language it
 * has type Simplex. An object of dim 0 can still support subscripting
 * by overriding the appropriate functions in MemberObject. The parser
 * will then rely entirely on the object itself in getting and setting
 * elements of the object. Such objects will typically not want the
 * parser to directly mess with their elements (setElement should
 * throw an error), while they should return the value of their elements
 * through implementing the getElement function.
 */
class Container : public RbComplex {

    public:
        virtual                        ~Container(void) {}                                                  //!< Virtual destructor

        // Basic utility functions you have to override (also getClass()!)
        virtual Container*              clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class
        virtual void                    printValue(std::ostream& o) const = 0;                              //!< Print value for user
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object

        // Container functions and basic utility functions you should not have to override
        ContainerIterator               begin(void) const;                                                  //!< Begin iterator
        ContainerIterator               end(void) const;                                                    //!< End iterator
        size_t                          getDim(void) const { return length.size(); }                        //!< Get number of dimensions (1 for vector, 2 for matrix, etc)
        const std::string&              getElementType(void) const { return elementType; }                  //!< Get element type (this method returns a string because containers of containers are not allowed so dim of elements is always 0)
        size_t                          getIndexOfName(size_t k, const std::string& s) const;               //!< Get index in dimension k corresponding to name s
        VectorInteger                   getIntegerIndex(const VectorIndex& index) const;                    //!< Get integer index to a single container element or to a subcontainer from generic index
        const std::vector<size_t>&      getLength(void) const { return length; }                            //!< Get length in each dim (length is a vector of lengths in the different dimensions according to Sebastian's wishes)
        VectorNatural                   getNaturalIndex(const VectorIndex& index) const;                    //!< Get natural index to a single container element from generic index
        const TypeSpec                  getTypeSpec(void) const;                                            //!< Get language type of the object (the container)
        bool                            isTypeSpec(const TypeSpec& typeSpec) const;                         //!< Does the language type of the object fit type specification typeSpec?
        DAGNode*                        wrapIntoVariable(void);                                             //!< Wrap value into variable

        // Container function you may want to override
        virtual void                    setLength(const std::vector<size_t>& len);                          //!< Reorganize container

        // Container functions you have to override
        virtual void                    clear(void) = 0;                                                    //!< Clear
        virtual void                    resize(const std::vector<size_t>& len) = 0;                         //!< Resize
        virtual size_t                  size(void) const = 0;                                               //!< Get number of elements

	protected:
                                        Container(const TypeSpec& typeSpec);                                //!< Default constructor (set type of elements and dim of container)

        // Help functions you do not have to override
        Container&                      operator=(const Container& x);                                      //!< Assignment operator checks type and dimension (but length can differ)
        void                            getContainerSubscriptIndices( const VectorInteger& index,
                                                                            VectorInteger& containerIndex,
                                                                            VectorInteger& subscriptIndex
                                                                    ) const;                                //!< Divide up an index into container and subscript indices
        size_t                          getOffset(const VectorNatural& index) const;                        //!< Get offset in elements vector

        // Parser help function you have to override
        virtual DAGNode*                getElement(VectorIndex& index) = 0;                                 //!< Get (or make) element or subcontainer for parser

        // Member variables
        const std::string&              elementType;                                                        //!< Type of elements
        std::vector<size_t>             length;                                                             //!< Length in each dimension (size_t because this is natural (STL) type for sizes)
        std::vector<std::vector<std::string> >  names;                                                      //!< Names for string indexing
};

#endif

