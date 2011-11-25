// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace actions {
    namespace parameters {

// -----------------------------------------------------------------------------
// Name: Parameter constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
Parameter< T >::Parameter( const kernel::OrderParameter& parameter )
    : Parameter_ABC( parameter.GetName().c_str() )
    , parameter_   ( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Parameter constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
template< typename T >
Parameter< T >::Parameter( const kernel::OrderParameter& parameter, const T& value )
    : Parameter_ABC( parameter.GetName().c_str() )
    , parameter_   ( parameter )
{
    SetValue( value );
}

// -----------------------------------------------------------------------------
// Name: Parameter destructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
Parameter< T >::~Parameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Parameter::GetType
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
template< typename T >
std::string Parameter< T >::GetType() const
{
    return parameter_.GetType();
}

// -----------------------------------------------------------------------------
// Name: Parameter::SetValue
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
void Parameter< T >::SetValue( const T& value )
{
    value_ = value;
    Set( true );
    NotifyValueSet();
}

// -----------------------------------------------------------------------------
// Name: Parameter::GetValue
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
const T& Parameter< T >::GetValue() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: Parameter::Display
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
void Parameter< T >::Display( kernel::Displayer_ABC& displayer ) const
{
    if( IsSet() )
        displayer.Item( tools::translate( "Parameter", "Parameter" ) ).Display( GetName() )
                 .Item( tools::translate( "Parameter", "Value" ) ).Display( value_ );
    else
        displayer.Item( tools::translate( "Parameter", "Parameter" ) ).Display( GetName() )
                 .Item( tools::translate( "Parameter", "Value" ) ).Display( tools::translate( "ParameterList", "Not set" ) );
}

// -----------------------------------------------------------------------------
// Name: Parameter::Serialize
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
template< typename T >
void Parameter< T >::Serialize( xml::xostream& xos ) const
{
    Parameter_ABC::Serialize( xos );
    xos << xml::attribute( "type", SerializeType() );
}

// -----------------------------------------------------------------------------
// Name: Parameter::IsInRange
// Created: LDC 2010-09-21
// -----------------------------------------------------------------------------
template< typename T >
bool Parameter< T >::IsInRange() const
{
    std::size_t nElements = elements_.size();
    return parameter_.MinOccurs() <= nElements && parameter_.MaxOccurs() >= nElements;
}
    }
}
