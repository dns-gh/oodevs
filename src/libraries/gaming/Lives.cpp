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
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Lives constructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Lives::Lives( const kernel::Entity_ABC& entity, kernel::Controller& controller )
    : entity_( entity )
    , controller_( controller )
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
void Lives::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() && !entity_.IsAnAggregatedSubordinate() )
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
