// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Speeds.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Speeds constructor
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
Speeds::Speeds( const Entity_ABC& entity )
    : entity_( entity )
    , speed_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Speeds destructor
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
Speeds::~Speeds()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Speeds::DoUpdate
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
void Speeds::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_speed()  )
        speed_ = float( message.speed() );
    if( message.has_direction()  )
    {
        const float angle = message.direction().heading() * 3.14f / 180.f;
        direction_ = geometry::Vector2f( std::sin( angle ), std::cos( angle ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Speeds::DoUpdate
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
void Speeds::DoUpdate( const sword::UnitKnowledgeUpdate& message )
{
    // $$$$ AGE 2007-12-17: *soupir*
    if( message.has_speed()  )
        speed_ = float( message.speed() );
    if( message.has_direction()  )
    {
        const float angle = message.direction().heading() * 3.14f / 180.f;
        direction_ = geometry::Vector2f( std::sin( angle ), std::cos( angle ) );
    }
}

// -----------------------------------------------------------------------------
// Name: Speeds::Draw
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
void Speeds::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    // $$$$ AGE 2007-12-17: borner la speed_ ou mettre un log
    const float factor = 10.f;
    if( viewport.IsHotpointVisible() && tools.ShouldDisplay( "Direction" ) && !entity_.IsAggregated() )
        tools.DrawArrow( where, where + direction_ * speed_ * factor, -1.f, gui::GlTools_ABC::pixels );
}
