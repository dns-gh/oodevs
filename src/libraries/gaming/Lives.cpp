// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Lives.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"

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
void Lives::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( ! viewport.IsHotpointVisible() )
        return;
    tools.DrawLife( where, life_ );
}

// -----------------------------------------------------------------------------
// Name: Lives::DoUpdate
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Lives::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_raw_operational_state()  )
        life_ = message.raw_operational_state() * 0.01f;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Lives::DoUpdate
// Created: SBO 2009-11-23
// -----------------------------------------------------------------------------
void Lives::DoUpdate( const sword::UnitKnowledgeUpdate& message )
{
    if( message.has_operational_state() )
        life_ = message.operational_state() * 0.01f;
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
