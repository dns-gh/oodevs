// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FloodAttribute.h"
#include "Tools.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Positions.h"
#include "flood/FloodDrawer.h"
#include "flood/FloodModel.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( kernel::Controller& controller, const kernel::DetectionMap& detection, const kernel::Positions& positions )
    : controller_ ( controller )
    , detection_  ( detection )
    , positions_  ( positions )
    , floodModel_ ( new flood::FloodModel( *this ) )
    , floodDrawer_( new flood::FloodDrawer( *floodModel_ ) )
    , depth_      ( 0 )
    , refDist_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute destructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::~FloodAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Display
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Flood" ) )
             .Display( tools::translate( "Object", "Depth:" ), depth_ )
             .Display( tools::translate( "Object", "Reference distance:" ), refDist_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::DisplayInSummary
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::DisplayInTooltip
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::Draw
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& /*tools*/ ) const
{
    floodDrawer_->Draw();
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::GetElevationAt
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
short FloodAttribute::GetElevationAt( const geometry::Point2f& point ) const
{
    // TODO rajouter les objets digue
    if( detection_.Extent().IsOutside( point ) )
        return std::numeric_limits< short >::max();
    return detection_.ElevationAt( point );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::DoUpdate
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::DoUpdate
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::UpdateData
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
template< typename T >
void FloodAttribute::UpdateData( const T& message )
{
    if( message.has_flood() )
    {
        depth_ = message.flood().depth();
        refDist_ = message.flood().reference_distance();
        controller_.Update( *static_cast< FloodAttribute_ABC* >( this ) );
        floodModel_->GenerateFlood( positions_.GetPosition(), depth_, refDist_ );
        floodDrawer_->ResetTexture();
    }
}
