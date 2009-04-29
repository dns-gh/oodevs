// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ElementType_h_
#define __ElementType_h_

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

namespace indicators
{
    class ElementTypeResolver;

// =============================================================================
/** @class  ElementType
    @brief  ElementType
*/
// Created: SBO 2009-04-06
// =============================================================================
class ElementType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ElementType( xml::xistream& xis );
    explicit ElementType( const std::string& type );
             ElementType( const ElementType& type, boost::shared_ptr< ElementTypeResolver > resolver );
    virtual ~ElementType();
    //@}

    //! @name Operations
    //@{
    std::string ToString() const;
    std::string Resolve() const;
    bool operator!=( const ElementType& rhs ) const;
    bool IsAbstract() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
//    ElementType( const ElementType& type ); //<! Copy constructor  // $$$$ SBO 2009-04-29: $$$$
    ElementType& operator=( const ElementType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::string type_;
    boost::shared_ptr< ElementTypeResolver > resolver_;
    //@}
};

}

#endif // __ElementType_h_
