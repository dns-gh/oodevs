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
#include "clients_kernel/Displayer_ABC.h"

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
    virtual ~ActionParameter();
    //@}

    //! @name Operations
    //@{
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

private:
    //! @name Member data
    //@{
    T value_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionParameter constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename T >
ActionParameter< T >::ActionParameter( const QString& name )
    : ActionParameter_ABC( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameter constructor
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
template< typename T >
ActionParameter< T >::ActionParameter( const QString& name, const T& value )
    : ActionParameter_ABC( name )
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

#endif // __ActionParameter_h_
