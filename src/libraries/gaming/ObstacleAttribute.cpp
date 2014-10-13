// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObstacleAttribute.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Tools.h"
#include "statusicons.h"
#include "protocol/SimulationSenders.h"

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( kernel::Controller& controller, bool singlePointPos )
    : controller_( controller )
    , hasSinglePointPos_( singlePointPos )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute destructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::~ObstacleAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Display
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Obstacle activated:" ), obstacleActivated_ )
             .Display( tools::translate( "Object", "Activation time:" ), activationTime_ )
             .Display( tools::translate( "Object", "Activity time:" ), activityTime_ );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ObstacleAttribute::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::DisplayInTooltip
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObstacleAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::DoUpdate
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
void ObstacleAttribute::DoUpdate( const sword::ObjectUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::UpdateData
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
template< typename T >
void ObstacleAttribute::UpdateData( const T& message )
{
     if( message.has_obstacle() )
     {
        obstacleActivated_= message.obstacle().activated();
        if( message.obstacle().has_activation_time() )
            activationTime_ = message.obstacle().activation_time();
        if( message.obstacle().has_activity_time() )
            activityTime_ = message.obstacle().activity_time();
        controller_.Update( *static_cast< ObstacleAttribute_ABC* >( this ) );
     }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Draw
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ObstacleAttribute::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( obstacleActivated_.IsSet() && viewport.IsVisible( where ) )
    {
        const float zoomFactor = tools.GetAdaptiveZoomFactor( !hasSinglePointPos_ );
        const geometry::Point2f offsetPoint = where + geometry::Vector2f( zoomFactor * 250.f, zoomFactor * 150.f );

        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 1, 1, 1 );
            tools.DrawIcon( obstacleActivated_ ? xpm_activated : xpm_not_activated
                          , offsetPoint, 150.f, hasSinglePointPos_? gui::GLView_ABC::pixels : gui::GLView_ABC::meters );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsObstacleActivated
// Created: MMC 2012-07-25
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsObstacleActivated() const
{
    return obstacleActivated_.IsSet() && obstacleActivated_;
}
