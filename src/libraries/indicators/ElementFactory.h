// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ElementFactory_h_
#define __ElementFactory_h_

#include "ElementFactory_ABC.h"

namespace indicators
{
    class DataTypeFactory;
    class ElementTypeResolver;
    class Primitives;
    class Variables;

// =============================================================================
/** @class  ElementFactory
    @brief  ElementFactory
*/
// Created: SBO 2009-03-17
// =============================================================================
class ElementFactory : public ElementFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ElementFactory( const Primitives& primitives, const Variables& variables );
    virtual ~ElementFactory();
    //@}

    //! @name Operations
    //@{
    virtual boost::shared_ptr< Element_ABC > CreateNumber( double value ) const;
    virtual boost::shared_ptr< Element_ABC > CreateString( const std::string& value ) const;
    virtual boost::shared_ptr< Element_ABC > CreateVariable( const std::string& name ) const;
    virtual boost::shared_ptr< Element_ABC > CreateFunction( const std::string& name ) const;
    //@}

private:
    //! @name Helpers
    //@{
    std::string NextId() const;
    //@}

private:
    //! @name Member data
    //@{
    const Primitives& primitives_;
    const Variables& variables_;
    std::unique_ptr< DataTypeFactory > types_;
    boost::shared_ptr< ElementTypeResolver > resolver_;
    mutable unsigned long id_;
    //@}
};

}

#endif // __ElementFactory_h_
