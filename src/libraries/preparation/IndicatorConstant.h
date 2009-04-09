// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorConstant_h_
#define __IndicatorConstant_h_

#include "IndicatorElement_ABC.h"
#include "IndicatorType.h"
#include <xeumeuleu/xml.h>

// =============================================================================
/** @class  IndicatorConstant
    @brief  IndicatorConstant
*/
// Created: SBO 2009-03-17
// =============================================================================
template< typename T >
class IndicatorConstant : public IndicatorElement_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorConstant( unsigned long id, const T& value, const IndicatorType& type )
                 : IndicatorElement_ABC( id ), value_( value ), type_( type ) {}
    virtual ~IndicatorConstant() {}
    //@}

    //! @name Accessors
    //@{
    const IndicatorType& GetType() const;
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( boost::shared_ptr< IndicatorElement_ABC > element );
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void SerializeDeclaration( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorConstant( const IndicatorConstant& );            //!< Copy constructor
    IndicatorConstant& operator=( const IndicatorConstant& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const T value_;
    const IndicatorType& type_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: IndicatorConstant::GetType
// Created: SBO 2009-04-09
// -----------------------------------------------------------------------------
template< typename T >
const IndicatorType& IndicatorConstant< T >::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: IndicatorConstant::AddParameter
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
template< typename T >
void IndicatorConstant< T >::AddParameter( boost::shared_ptr< IndicatorElement_ABC > element )
{
    throw std::runtime_error( "Not implemented" );
}

// -----------------------------------------------------------------------------
// Name: IndicatorConstant::Serialize
// Created: SBO 2009-03-17
// -----------------------------------------------------------------------------
template< typename T >
void IndicatorConstant< T >::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "constant" )
            << xml::attribute( "id", GetId() )
            << xml::attribute< T >( "value", value_ )
            << xml::attribute( "type", type_.ToString() )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: IndicatorConstant::SerializeDeclaration
// Created: SBO 2009-04-03
// -----------------------------------------------------------------------------
template< typename T >
void IndicatorConstant< T >::SerializeDeclaration( xml::xostream& xos ) const
{
    Serialize( xos );
}

#endif // __IndicatorConstant_h_
