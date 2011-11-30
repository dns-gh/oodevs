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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Tools.h"
#include "statusicons.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute constructor
// Created: SBO 2007-02-08
// -----------------------------------------------------------------------------
ObstacleAttribute::ObstacleAttribute( kernel::Controller& controller )
    : controller_( controller )
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
             .Display( tools::translate( "Object", "Obstacle type:" ), obstacleType_ )
             .Display( tools::translate( "Object", "Reserved obstacle activated:" ), reservedObstacleActivated_ )
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
        obstacleType_ = (E_DemolitionTargetType)message.obstacle().type();
        reservedObstacleActivated_= (message.obstacle().activated() != 0);
        if( message.obstacle().has_activation_time() )
            activationTime_ = message.obstacle().activation_time();
        if( message.obstacle().has_activity_time() )
            activityTime_ = message.obstacle().activity_time();
        controller_.Update( *(ObstacleAttribute_ABC*)this );
     }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::Draw
// Created: JCR 2008-06-10
// -----------------------------------------------------------------------------
void ObstacleAttribute::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( reservedObstacleActivated_.IsSet() && viewport.IsVisible( where ) )
    {
        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 1, 1, 1 );
            tools.DrawIcon( reservedObstacleActivated_ ? xpm_activated : xpm_not_activated, where + geometry::Vector2f( 250.f, 150.f ), 150.f );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsReservedObstacle
// Created: LDC 2010-12-27
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsReservedObstacle() const
{
    return obstacleType_.IsSet() && obstacleType_ == eDemolitionTargetType_Reserved;
}

// -----------------------------------------------------------------------------
// Name: ObstacleAttribute::IsReservedObstacleActivated
// Created: LDC 2010-12-27
// -----------------------------------------------------------------------------
bool ObstacleAttribute::IsReservedObstacleActivated() const
{
    return reservedObstacleActivated_.IsSet() && reservedObstacleActivated_;
}
