// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameter_h_
#define __ActionParameter_h_

#include "ActionParameter_ABC.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Displayer_ABC.h"
#include "xeumeuleu/xml.h"

// =============================================================================
/** @class  ActionParameter
    @brief  ActionParameter
*/
// Created: SBO 2007-03-19
// =============================================================================
template< typename T >
class ActionParameter : public ActionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameter( const QString& name );
             ActionParameter( const QString& name, const T& value );
    explicit ActionParameter( const kernel::OrderParameter& parameter );
             ActionParameter( const kernel::OrderParameter& parameter, const T& value );
    virtual ~ActionParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool IsContext() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    void SetValue( const T& value );
    const T& GetValue() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameter( const ActionParameter& );            //!< Copy constructor
    ActionParameter& operator=( const ActionParameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter* parameter_;
    T value_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionParameter constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename T >
ActionParameter< T >::ActionParameter( const QString& name )
    : ActionParameter_ABC( name )
    , parameter_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter constructor
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename T >
ActionParameter< T >::ActionParameter( const QString& name, const T& value )
    : ActionParameter_ABC( name )
    , parameter_( 0 )
{
    SetValue( value );
}

// -----------------------------------------------------------------------------
// Name: ActionParameter constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
ActionParameter< T >::ActionParameter( const kernel::OrderParameter& parameter )
    : ActionParameter_ABC( parameter.GetName() )
    , parameter_( &parameter )
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
    , parameter_( &parameter )
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
// Name: ActionParameter::IsContext
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
template< typename T >
bool ActionParameter< T >::IsContext() const
{
    return parameter_ ? parameter_->IsContext() : false;
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
    const QString name = GetName() + ( parameter_ ? QString( " [%1] " ).arg( parameter_->GetType() ) : "" );
    displayer.Item( tools::translate( "ActionParameter", "Action" ) ).Display( name )
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
    if( parameter_ )
        xos << xml::attribute( "type", parameter_->GetType() );
}

#endif // __ActionParameter_h_
