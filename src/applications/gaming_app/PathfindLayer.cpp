// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PathfindLayer.h"
#include "moc_PathfindLayer.cpp"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"

// -----------------------------------------------------------------------------
// Name: PathfindLayer constructor
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
PathfindLayer::PathfindLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools,
                              Publisher_ABC& publisher, const kernel::CoordinateConverter_ABC& coordinateConverter )
    : controllers_( controllers )
    , tools_( tools )
    , element_( controllers )
    , publisher_( publisher )
    , coordinateConverter_( coordinateConverter )
    , lock_( false )
{
    controllers_.Register( *this );
    std::function< void( const sword::SimToClient& ) > fun =
        [&]( const sword::SimToClient& message )
        {
            if( !message.message().has_pathfind_request_ack() )
                return;
            lock_ = false;
            const auto& request = message.message().pathfind_request_ack();
            if( request.error_code() != sword::PathfindRequestAck_ErrorCode_no_error )
                return;
            path_.clear();
            const auto& path = request.path();
            const auto& points = path.points();
            for( auto it = points.begin(); it != points.end(); ++it )
            {
                boost::optional< uint32_t > waypoint;
                if( it->has_waypoint() )
                    waypoint = it->waypoint();
                const Point p = { coordinateConverter_.ConvertToXY( it->coordinate() ), waypoint };
                path_.push_back( p );
            }
        };
    publisher_.Register( fun );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer destructor
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
PathfindLayer::~PathfindLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::Paint
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::Paint( gui::Viewport_ABC& )
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
    DrawLines( 5, 34, 105, 187 );
    DrawLines( 3, 0, 179, 253 );
    for( auto it = positions_.begin(); it != positions_.end(); ++it )
        DrawPoint( *it );
    if( hovered_ )
        DrawPoint( hovered_->first );
    glPopAttrib();
}

void PathfindLayer::DrawLines( float width, uint8_t r, uint8_t g, uint8_t b ) const
{
    if( path_.empty() )
        return;
    glColor3ub( r, g, b );
    for( auto it = path_.begin(); it != path_.end() - 1; ++it )
    {
        tools_.DrawLine( it->coordinate_, (it + 1)->coordinate_, width );
        tools_.DrawDisc( (it + 1)->coordinate_, width / 2, gui::GlTools_ABC::pixels );
    }
}

void PathfindLayer::DrawPoint( geometry::Point2f p ) const
{
    glColor4f( COLOR_WHITE );
    tools_.DrawDisc( p, 6, gui::GlTools_ABC::pixels );
    glColor4f( COLOR_BLACK );
    tools_.DrawDisc( p, 5, gui::GlTools_ABC::pixels );
    glColor4f( COLOR_WHITE );
    tools_.DrawDisc( p, 3, gui::GlTools_ABC::pixels );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::Select
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::Select( const kernel::Agent_ABC& element )
{
    element_ = &element;
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::Select
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::Select( const kernel::Population_ABC& element )
{
    element_ = &element;
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::NotifyContextMenu
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
{
    if( !element_ || lock_ )
        return;
    point_ = point;
    kernel::ContextMenu* subMenu = menu.SubMenu( "Interface", tools::translate( "LocationEditorToolbar", "Pathfind" ) );
    subMenu->InsertItem( "Interface", tools::translate( "LocationEditorToolbar", "Add position" ), this, SLOT( AddPosition() ) );
    subMenu->InsertItem( "Interface", tools::translate( "LocationEditorToolbar", "Clear positions" ), this, SLOT( ClearPositions() ) );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::ClearPositions
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::ClearPositions()
{
    if( lock_ )
        return;
    positions_.clear();
    path_.clear();
    hovered_ = boost::none;
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::AddPosition
// -----------------------------------------------------------------------------
void PathfindLayer::AddPosition()
{
    if( lock_ )
        return;
    positions_.push_back( point_ );
    SendRequest();
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::SendRequest
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::SendRequest()
{
    if( element_ && positions_.size() > 1 )
    {
        sword::ClientToSim msg;
        auto request = msg.mutable_message()->mutable_pathfind_request();
        request->mutable_unit()->set_id( element_->GetId() );
        auto* positions = request->mutable_positions();
        for( auto it = positions_.begin(); it != positions_.end(); ++it )
            coordinateConverter_.ConvertToGeo( *it, *positions->Add() );
        publisher_.Send( msg );
        lock_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::BeforeSelection
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::BeforeSelection()
{
    element_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::AfterSelection
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::AfterSelection()
{
    // NOTHING
}

namespace
{
    const auto threshold = 200; // pixels
}

bool PathfindLayer::HandleMouseMove( QMouseEvent* /*mouse*/, const geometry::Point2f& point )
{
    hovered_ = boost::none;
    if( path_.empty() || lock_ )
        return false;
    std::size_t waypoint = 0;
    float distance = std::numeric_limits< float >::infinity();
    for( auto it = path_.begin(); it != path_.end() - 1; ++it )
    {
        if( it->waypoint_ )
            waypoint = *it->waypoint_;
        const auto projection =
            geometry::Segment2f( (it + 1)->coordinate_, it->coordinate_ )
                .Project( point );
        const auto d = projection.SquareDistance( point );
        const auto pixels = tools_.Pixels( projection );
        if( d < distance && d < threshold * pixels * pixels )
        {
            distance = d;
            hovered_ = std::make_pair( projection, waypoint );
        }
    }
    return false;
}

bool PathfindLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( event->button() != Qt::LeftButton || !hovered_ )
        return false;
    const auto pixels = tools_.Pixels( point );
    if( point.SquareDistance( hovered_->first ) >= threshold * pixels * pixels )
        return false;
    hovered_->first = point;
    QWidget w;
    dnd::CreateDragObject( this, &w );
    return true;
}

bool PathfindLayer::HandleMoveDragEvent( QDragMoveEvent* /*event*/, const geometry::Point2f& point )
{
    hovered_->first = point;
    return true;
}

bool PathfindLayer::HandleDropEvent( QDropEvent* /*event*/, const geometry::Point2f& point )
{
    if( hovered_->second < positions_.size() )
    {
        const auto it = positions_.begin() + hovered_->second + 1;
        positions_.insert( it, point );
        SendRequest();
    }
    return true;
}

bool PathfindLayer::HandleLeaveDragEvent( QDragLeaveEvent* /*event*/ )
{
    hovered_ = boost::none;
    return true;
}

bool PathfindLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& /*point*/ ) const
{
    return dnd::HasData< PathfindLayer >( event );
}
