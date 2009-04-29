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
#include "ElementType.h"
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.h>

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
             Constant( const std::string& id, const std::string& type, const T& value )
                 : Element_ABC( id ), type_( type ), value_( value ) {}
    virtual ~Constant() {}
    //@}

    //! @name Accessors
    //@{
    virtual const ElementType& GetType() const;
    virtual std::string GetValue() const;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< Element_ABC > element );
    virtual void Serialize( xml::xostream& xos, ElementDeclarator_ABC& declarator ) const;
    virtual void SerializeDeclaration( xml::xostream& xos ) const;
    virtual Element_ABC& Clone() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Constant( const Constant& );            //!< Copy constructor
    Constant& operator=( const Constant& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    const ElementType type_;
    const T value_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: Constant::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
template< typename T >
const ElementType& Constant< T >::GetType() const
{
    return type_;
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
    throw std::runtime_error( __FUNCTION__ " not implemented." );
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
            << xml::attribute( "type", type_.ToString() )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Constant::Clone
// Created: SBO 2009-04-24
// -----------------------------------------------------------------------------
template< typename T >
Element_ABC& Constant< T >::Clone() const
{
    throw std::runtime_error( __FUNCTION__ " not implemented." );
}

}

#endif // __Constant_h_
