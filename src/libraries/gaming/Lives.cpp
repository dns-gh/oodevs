// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Lives.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Lives constructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Lives::Lives( kernel::Controller& controller )
    : controller_( controller )
    , life_( 1.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lives destructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Lives::~Lives()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lives::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Lives::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( ! viewport.IsInside( where ) )
        return;
    tools.DrawLife( where, life_ );
}

// -----------------------------------------------------------------------------
// Name: Lives::DoUpdate
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Lives::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.etat_operationnel_brutPresent )
        life_ = message.etat_operationnel_brut * 0.01f;
    controller_.Update( *this );
}


// -----------------------------------------------------------------------------
// Name: Lives::GetLife
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
float Lives::GetLife() const
{
    return life_;
}
