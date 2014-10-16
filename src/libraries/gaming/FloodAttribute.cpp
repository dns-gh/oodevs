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
#include "FloodProxy.h"
#include "clients_kernel/AltitudeModified.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

// -----------------------------------------------------------------------------
// Name: FloodAttribute constructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::FloodAttribute( kernel::Controller& controller, FloodProxy& proxy, const kernel::Positions& positions )
    : controller_ ( controller )
    , positions_  ( positions )
    , proxy_      ( proxy )
    , floodId_    ( 0 )
    , isReal_     ( false )
    , depth_      ( 0 )
    , refDist_    ( 0 )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute destructor
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
FloodAttribute::~FloodAttribute()
{
    if( isReal_ )
        proxy_.Remove( floodId_ );
    controller_.Unregister( *this );
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
void FloodAttribute::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GLView_ABC& /*tools*/ ) const
{
    proxy_.Draw( floodId_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::NotifyUpdated
// Created: JSR 2011-10-04
// -----------------------------------------------------------------------------
void FloodAttribute::NotifyUpdated( const kernel::AltitudeModified& attribute )
{
    if( isReal_ && attribute.polygon_.Intersect( positions_.GetPosition(), static_cast< float >( refDist_ ) ) )
        proxy_.GenerateFlood( floodId_, positions_.GetPosition(), depth_, refDist_ );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::DoUpdate
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes(), false );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::DoUpdate
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
void FloodAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes(), true );
}

// -----------------------------------------------------------------------------
// Name: FloodAttribute::UpdateData
// Created: JSR 2010-12-15
// -----------------------------------------------------------------------------
template< typename T >
void FloodAttribute::UpdateData( const T& message, bool isReal )
{
    if( message.has_flood() )
    {
        isReal_ = isReal;
        depth_ = message.flood().depth();
        refDist_ = message.flood().reference_distance();
        controller_.Update( *static_cast< FloodAttribute_ABC* >( this ) );
        if( isReal )
            floodId_ = proxy_.GenerateFlood( floodId_, positions_.GetPosition(), depth_, refDist_ );
        else
            floodId_ = proxy_.FindFlood( positions_.GetPosition(), depth_, refDist_ );
    }
}
