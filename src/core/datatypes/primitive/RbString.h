/**
 * @file
 * This file contains the declaration of RbString, which is
 * a RevBayes wrapper around a regular string.
 *
 * @brief Declaration of RbString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RbString_H
#define RbString_H

#include "RbBoolean.h"
#include "RbLanguageObject.h"

#include <fstream>
#include <string>


class RbString : public RbLanguageObject {

    public:
                                    RbString(void) : RbLanguageObject(), value() {}     //!< Default: empty string
                                    RbString(int i);                                    //!< Constructor from int
                                    RbString(long i);                                   //!< Constructor from long
                                    RbString(double i);                                 //!< Constructor from double
                                    RbString(const std::string& v);                     //!< Constructor from string

        // Overloaded operators
                                    operator std::string() const;                       //!< Type conversion
        RbString                    operator+(const RbString& s) const;                 //!< String concatenation
        RbString                    operator+(const std::string& s) const;              //!< String concatenation
        friend std::ostream&        operator<< (std::ostream& o, RbString const& s);    //!< String output with << operator
        bool                        operator==(const RbString& x) const;                //!< Equals operator
//        bool                        operator<(const RbString& x) const;                 //!< Smaller comparison

        // the value type definition
        typedef std::string valueType;

        // Basic utility functions
	    RbString*                   clone(void) const;                                  //!< Copy
        static const std::string&   getClassName(void);                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
        RbValue<void*>              getLeanValue(void) const;                           //!< Transform the object into a basic element pointer for fast access.
        size_t                      memorySize() const { return sizeof( value ); }      //!< Get the size
        void                        printValue(std::ostream& o) const;                  //!< Print value (for user)
        void                        setLeanValue(const RbValue<void*> &val);            //!< Set the value of the object from a basic (lean) element pointer.

        // RbString functions
        void                        append(const RbString& s) { value += s.value; }     //!< Append string
        void                        append(const char* s) { value += s; }               //!< Append C string
        const char*                 c_str(void) { return value.c_str(); }               //!< Return C-style string
        std::string                 getValue(void) const { return value; }              //!< Get value
        void                        setValue(const std::string& x) { value = x; }       //!< Set value

    private:

        std::string                 value;                                              //!< Value member
    
};

std::string                         operator+(const std::string& A, const RbString& B); //!< Append to std::string
RbBoolean                           operator==(const RbString& A, const RbString& B);   //!< operator == 
RbBoolean                           operator!=(const RbString& A, const RbString& B);   //!< operator != 
RbBoolean                           operator< (const RbString& A, const RbString& B);   //!< operator < 

#endif

