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
ParamDirection::ParamDirection( QWidget* parent, ASN1T_Direction& direction, const QString& label )
    : QHBox( parent )
    , direction_( direction )
{
    new QLabel( label, this );
    pDial_ = new QDial( 0, 359, 1, 0, this );
    pDial_->setWrapping( true );
    pDial_->setMaximumSize( 50, 50 );
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
// Name: ParamDirection::Commit
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void ParamDirection::Commit()
{
    direction_ = pDial_->value();
    direction_ += (direction_ > 180 ) ? -180 : 180;
}
