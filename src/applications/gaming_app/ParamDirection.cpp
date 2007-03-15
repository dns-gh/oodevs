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

// -----------------------------------------------------------------------------
// Name: ParamDirection constructor
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
ParamDirection::ParamDirection( const QString& name )
    : Param_ABC( name )
    , dial_( 0 )
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
    QHBox* box = new QHBox( parent );
    new QLabel( GetName(), box );
    dial_ = new QDial( 0, 359, 1, 0, box );
    dial_->setWrapping( true );
    dial_->setMaximumSize( 50, 50 );
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamDirection::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( !dial_ )
        InterfaceNotInitialized();
    asn.null_value = 0;
    asn.value.t = T_MissionParameter_value_direction;
    asn.value.u.direction = dial_->value() + ( dial_->value() > 180 ? -180 : 180 );
}

// -----------------------------------------------------------------------------
// Name: ParamDirection::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamDirection::CommitTo( ASN1T_OrderContext& asn ) const
{
    if( !dial_ )
        InterfaceNotInitialized();
    asn.direction_dangereuse = dial_->value() + ( dial_->value() > 180 ? -180 : 180 );
}
