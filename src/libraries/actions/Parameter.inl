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
// Name: Parameter::SetKeyName
// Created: LDC 2012-09-05
// -----------------------------------------------------------------------------
template< typename T >
void Parameter< T >::SetKeyName( const std::string& identifier )
{
    parameter_.SetKeyName( identifier );
}

// -----------------------------------------------------------------------------
// Name: Parameter::IsOptional
// Created: LDC 2012-09-05
// -----------------------------------------------------------------------------
template< typename T >
bool Parameter< T >::IsOptional() const
{
    return parameter_.IsOptional();
}

// -----------------------------------------------------------------------------
// Name: Parameter::IsContext
// Created: LDC 2014-04-04
// -----------------------------------------------------------------------------
template< typename T >
bool Parameter< T >::IsContext() const
{
    return parameter_.IsContext();
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
// Name: Parameter::GetDisplayName
// Created: JSR 2012-10-17
// -----------------------------------------------------------------------------
template< typename T >
QString Parameter< T >::GetDisplayName( kernel::DisplayExtractor_ABC& extractor ) const
{
    if( IsSet() )
        return extractor.GetDisplayName( value_ );
    return tools::translate( "ParameterList", "Not set" );
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

template< typename T >
void Parameter< T >::Serialize( xml::xostream& xos, const protocol::Writer_ABC& writer ) const
{
    Parameter< T >::Serialize( xos );
    sword::MissionParameter message;
    CommitTo( message );
    protocol::Write( xos, writer, message );
}

// -----------------------------------------------------------------------------
// Name: Parameter::GetKeyName
// Created: MMC 2012-05-16
// -----------------------------------------------------------------------------
template< typename T >
std::string Parameter< T >::GetKeyName() const
{
    return parameter_.GetKeyName();
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

// -----------------------------------------------------------------------------
// Name: Parameter::GetOrderParameter
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
template< typename T >
const kernel::OrderParameter& Parameter< T >::GetOrderParameter() const
{
    return parameter_;
}

    }
}
