// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentPositions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace geometry;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentPositions constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::AgentPositions( kernel::Controller& controller, const Agent_ABC& agent, const CoordinateConverter_ABC& converter )
    : controller_( controller )
    , agent_( agent )
    , converter_( converter )
    , height_( 0 )
    , dead_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentPositions destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
AgentPositions::~AgentPositions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetPosition
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
Point2f AgentPositions::GetPosition( bool aggregated ) const
{
    if( !aggregated || !agent_.IsAggregated() )
        return position_;
    return agent_.Get< TacticalHierarchies >().GetUp().Get< Positions >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetHeight
// Created: AGE 2006-04-18
// -----------------------------------------------------------------------------
float AgentPositions::GetHeight( bool aggregated ) const
{
    if( !aggregated || !agent_.IsAggregated() )
        return height_;
    return agent_.Get< TacticalHierarchies >().GetUp().Get< Positions >().GetHeight();
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::DoUpdate
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
void AgentPositions::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_position()  )
        position_ = converter_.ConvertToXY( message.position() );
    if( message.has_height()  )
        height_ = float( message.height() );
    if( message.has_dead()  )
        dead_ = message.dead() != 0;

    controller_.Update( *static_cast< kernel::Positions* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsIn
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool AgentPositions::IsIn( const Rectangle2f& rectangle ) const
{
    return rectangle.IsInside( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::GetBoundingBox
// Created: SBO 2006-07-05
// -----------------------------------------------------------------------------
geometry::Rectangle2f AgentPositions::GetBoundingBox() const
{
    const geometry::Point2f center = GetPosition( true );
    return geometry::Rectangle2f( center.X() - 250, center.Y(), center.X() + 250, center.Y() + 400 );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Accept
// Created: SBO 2009-05-25
// -----------------------------------------------------------------------------
void AgentPositions::Accept( kernel::LocationVisitor_ABC& visitor ) const
{
    visitor.VisitPoint( GetPosition( true ) );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void AgentPositions::Draw( const Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() && tools.ShouldDisplay( "UnitDetails" ) && !agent_.IsAggregated() )
        tools.DrawCross( where, GL_CROSSSIZE, gui::GLView_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::DisplayInSummary
// Created: AME 2010-03-19
// -----------------------------------------------------------------------------
void AgentPositions::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "AgentPositions", "Position:" ) , converter_.GetStringPosition( GetPosition( true ) ) ) ;
}

// -----------------------------------------------------------------------------
// Name: AgentPositions::IsDead
// Created: LDC 2010-10-07
// -----------------------------------------------------------------------------
bool AgentPositions::CanAggregate() const
{
    return !dead_;
}
