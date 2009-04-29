// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ElementFactory_ABC_h_
#define __ElementFactory_ABC_h_

#include <boost/shared_ptr.hpp>

namespace indicators
{
    class Element_ABC;

// =============================================================================
/** @class  ElementFactory_ABC
    @brief  ElementFactory_ABC
*/
// Created: SBO 2009-03-17
// =============================================================================
class ElementFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ElementFactory_ABC() {}
    virtual ~ElementFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< Element_ABC > CreateNumber( double value ) const = 0;
    virtual boost::shared_ptr< Element_ABC > CreateString( const std::string& value ) const = 0;
    virtual boost::shared_ptr< Element_ABC > CreateVariable( const std::string& name ) const = 0;
    virtual boost::shared_ptr< Element_ABC > CreateFunction( const std::string& name ) const = 0;
    //@}
};

}

#endif // __ElementFactory_ABC_h_
