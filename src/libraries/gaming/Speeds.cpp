// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Speeds.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Speeds constructor
// Created: AGE 2007-12-17
// -----------------------------------------------------------------------------
Speeds::Speeds( const CoordinateConverter_ABC& converter )
    : converter_( converter )
    , speed_    ( 0 )
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
void Speeds::DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message )
{
    if( message.has_vitesse()  )
        speed_ = float( message.vitesse() );
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
void Speeds::DoUpdate( const MsgsSimToClient::MsgUnitKnowledgeUpdate& message )
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
void Speeds::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2007-12-17: borner la speed_ ou mettre un log
    const float factor = 10.f;
    if( viewport.IsHotpointVisible() && tools.ShouldDisplay( "Direction" ) )
        tools.DrawArrow( where, where + direction_ * speed_ * factor );
}
