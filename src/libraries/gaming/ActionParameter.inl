// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ActionParameter constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
ActionParameter< T >::ActionParameter( const kernel::OrderParameter& parameter )
    : ActionParameter_ABC( parameter.GetName() )
    , parameter_( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
template< typename T >
ActionParameter< T >::ActionParameter( const kernel::OrderParameter& parameter, const T& value )
    : ActionParameter_ABC( parameter.GetName() )
    , parameter_( parameter )
{
    SetValue( value );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter destructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
ActionParameter< T >::~ActionParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter::GetType
// Created: SBO 2007-05-25
// -----------------------------------------------------------------------------
template< typename T >
std::string ActionParameter< T >::GetType() const
{
    return parameter_.GetType();
}

// -----------------------------------------------------------------------------
// Name: ActionParameter::IsContext
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
template< typename T >
bool ActionParameter< T >::IsContext() const
{
    return parameter_.IsContext();
}

// -----------------------------------------------------------------------------
// Name: ActionParameter::SetValue
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
void ActionParameter< T >::SetValue( const T& value )
{
    value_ = value;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter::GetValue
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
const T& ActionParameter< T >::GetValue() const
{
    return value_;
}

// -----------------------------------------------------------------------------
// Name: ActionParameter::Display
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
void ActionParameter< T >::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "ActionParameter", "Action" ) ).Display( GetName() )
             .Item( tools::translate( "ActionParameter", "Value" ) ).Display( value_ );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter::Serialize
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
template< typename T >
void ActionParameter< T >::Serialize( xml::xostream& xos ) const
{
    ActionParameter_ABC::Serialize( xos );
    xos << xml::attribute( "type", parameter_.GetType() );
}
