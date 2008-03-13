// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamDirection.h"
#include "moc_ParamDirection.cpp"
#include "gaming/ActionParameterDirection.h"
#include "gaming/Action_ABC.h"
#include "clients_kernel/OrderParameter.h"

// -----------------------------------------------------------------------------
// Name: ParamDirection constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamDirection::ParamDirection( QObject* parent, const kernel::OrderParameter& parameter )
    : QObject( parent )
    , Param_ABC( parameter.GetName().c_str() )
    , parameter_( parameter )
    , value_( 180 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDirection destructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamDirection::~ParamDirection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamDirection::BuildInterface( QWidget* parent )
{
    QHBox* box = new QHBox( parent ); // $$$$ SBO 2007-03-16: should be removed... but need some changes in order context interface
    new QLabel( GetName(), box );
    QDial* dial = new QDial( 0, 359, 1, 0, box );
    dial->setWrapping( true );
    dial->setMaximumSize( 50, 50 );
    connect( dial, SIGNAL( valueChanged( int ) ), SLOT( OnValueChanged( int ) ) );
    dial->setValue( value_ );
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ParamDirection::CommitTo( ActionParameterContainer_ABC& action ) const
{
    action.AddParameter( *new ActionParameterDirection( parameter_, value_ ) );
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::OnValueChanged
// Created: SBO 2007-03-16
// -----------------------------------------------------------------------------
void ParamDirection::OnValueChanged( int value )
{
    value_ = value + ( value > 180 ? -180 : 180 );
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
bool ParamDirection::IsOptional() const
{
    return parameter_.IsOptional();
}
