// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Constant_h_
#define __Constant_h_

#include "Element_ABC.h"
#include "DataType_ABC.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <boost/shared_ptr.hpp>
#include <xeumeuleu/xml.hpp>

namespace indicators
{

// =============================================================================
/** @class  Constant
    @brief  Constant
*/
// Created: SBO 2009-03-17
// =============================================================================
template< typename T >
class Constant : public Element_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Constant( const std::string& id, boost::shared_ptr< const DataType_ABC > type, const T& value )
                 : Element_ABC( id ), type_( type ), value_( value ) {}
    virtual ~Constant() {}
    //@}

    //! @name Accessors
    //@{
    virtual const DataType_ABC& GetType() const;
    virtual std::string GetValue() const;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< Element_ABC > element );
    virtual void Serialize( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const;
    virtual void SerializeDeclaration( xml::xostream& xos ) const;
    //@}

protected:
    //! @name Member data
    //@{
    boost::shared_ptr< const DataType_ABC > type_;
    const T value_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Constant::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
template< typename T >
const DataType_ABC& Constant< T >::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: Constant::GetValue
// Created: SBO 2009-04-17
// -----------------------------------------------------------------------------
template< typename T >
std::string Constant< T >::GetValue() const
{
    return boost::lexical_cast< std::string, T >( value_ );
}

// -----------------------------------------------------------------------------
// Name: Constant::AddParameter
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
template< typename T >
void Constant< T >::AddParameter( boost::shared_ptr< Element_ABC > element )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: Constant::Serialize
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
template< typename T >
void Constant< T >::Serialize( xml::xostream&, ElementDeclarator_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Constant::SerializeDeclaration
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
template< typename T >
void Constant< T >::SerializeDeclaration( xml::xostream& xos ) const
{
    xos << xml::start( "constant" )
            << xml::attribute( "id", GetInput() )
            << xml::attribute< T >( "value", value_ )
            << xml::attribute( "type", type_->ToString() )
        << xml::end;
}

}

#endif // __Constant_h_
