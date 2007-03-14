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
ParamDirection::ParamDirection( ASN1T_Direction& direction, const QString& name )
    : Param_ABC( name )
    , direction_( direction )
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
// Name: ParamDirection::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamDirection::Commit()
{
    if( !dial_ )
        InterfaceNotInitialized();
    direction_ = dial_->value();
    direction_ += (direction_ > 180 ) ? -180 : 180;
}
