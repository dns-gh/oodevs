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
#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"
#include <boost/assign.hpp>

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
            if( !message.message().has_compute_pathfind_ack() )
                return;
            lock_ = false;
            const auto& request = message.message().compute_pathfind_ack();
            if( request.error_code() != sword::ComputePathfindAck_ErrorCode_no_error )
                return;
            path_.clear();
            const auto& points = request.path().points();
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
    std::function< void( const sword::SimToClient& ) > fun2 =
        [&]( const sword::SimToClient& message )
        {
            if( !message.message().has_segment_request_ack() )
                return;
            lock_ = false;
            if( !hovered_ )
                return;
            const auto& request = message.message().segment_request_ack();
            if( request.error_code() != sword::PathfindRequestAck_ErrorCode_no_error )
                return;
            const auto& segments = request.segments();
            for( auto it = segments.begin(); it != segments.end(); ++it )
                hovered_->coordinate_ = geometry::Segment2f(
                    coordinateConverter_.ConvertToXY( it->from() ),
                    coordinateConverter_.ConvertToXY( it->to() ) ).Project( point_ );
        };
    publisher_.Register( fun2 );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer destructor
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
PathfindLayer::~PathfindLayer()
{
    controllers_.Unregister( *this );
}

namespace
{
    void SetColor( QColor color )
    {
        glColor3d( color.redF(), color.greenF(), color.blueF() );
    }
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::Paint
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::Paint( gui::Viewport_ABC& )
{
    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
    SetColor( "#2269BB" );
    DrawLines( 5 );
    SetColor( "#00B3FD" );
    DrawLines( 3 );
    DrawPoints();
    glPopAttrib();
}

void PathfindLayer::DrawLines( float width ) const
{
    if( path_.empty() )
        return;
    tools_.DrawDisc( path_.front().coordinate_, width / 2, gui::GlTools_ABC::pixels );
    for( auto it = path_.begin(); it != path_.end() - 1; ++it )
    {
        tools_.DrawLine( it->coordinate_, (it + 1)->coordinate_, width );
        tools_.DrawDisc( (it + 1)->coordinate_, width / 2, gui::GlTools_ABC::pixels );
    }
}

void PathfindLayer::DrawPoints() const
{
    for( std::size_t i = 0; i < positions_.size(); ++i )
        DrawPoint( positions_[i],
            hovered_ && hovered_->onWaypoint_ && hovered_->lastWaypoint_ == i );
    if( hovered_ && !hovered_->onWaypoint_ )
        DrawPoint( hovered_->coordinate_, false );
}

void PathfindLayer::DrawPoint( geometry::Point2f p, bool invert ) const
{
    const auto bottom = invert ? Qt::black : Qt::white;
    SetColor( bottom );
    tools_.DrawDisc( p, 6, gui::GlTools_ABC::pixels );
    SetColor( invert ? Qt::white : Qt::black );
    tools_.DrawDisc( p, 5, gui::GlTools_ABC::pixels );
    SetColor( bottom );
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
    if( controllers_.GetCurrentMode() == eModes_Itinerary )
    {
        menu.InsertItem( "Itinerary", tools::translate( "LocationEditorToolbar", "Itinerary from here" ), this, SLOT( SetStartPosition() ) );
        menu.InsertItem( "Itinerary", tools::translate( "LocationEditorToolbar", "Itinerary to here" ), this, SLOT( SetEndPosition() ) );
        menu.InsertItem( "Itinerary", tools::translate( "LocationEditorToolbar", "Clear waypoints" ), this, SLOT( ClearPositions() ) );
}
    else
        menu.InsertItem( "Itinerary", tools::translate( "LocationEditorToolbar", "Create itinerary" ), this, SLOT( OpenEditingMode() ) );
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
// Name: PathfindLayer::SetStartPosition
// -----------------------------------------------------------------------------
void PathfindLayer::SetStartPosition()
{
    if( lock_ )
        return;
    if( positions_.size() > 1 )
        positions_.front() = point_;
    else
        positions_.push_front( point_ );
    SendRequest();
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::SetEndPosition
// -----------------------------------------------------------------------------
void PathfindLayer::SetEndPosition()
{
    if( lock_ )
        return;
    if( positions_.size() > 1 )
        positions_.back() = point_;
    else
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
        auto request = msg.mutable_message()->mutable_compute_pathfind()->mutable_request();
        request->mutable_unit()->set_id( element_->GetId() );
        for( auto it = positions_.begin(); it != positions_.end(); ++it )
            coordinateConverter_.ConvertToGeo( *it, *request->add_positions() );
        for( auto it = element_->Get< Equipments >().CreateIterator(); it.HasMoreElements(); )
            request->add_equipment_types()->set_id( it.NextElement().type_.GetId() );
        request->set_ignore_dynamic_objects( true );
        publisher_.Send( msg );
        lock_ = true;
        hovered_ = boost::none;
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

bool PathfindLayer::IsNear( float squareDistance, geometry::Point2f point ) const
{
    const auto pixels = tools_.Pixels( point );
    return squareDistance < threshold * pixels * pixels;
}

bool PathfindLayer::PickWaypoint( geometry::Point2f point )
{
    // path iterated backwards to select the waypoint on top
    // when several of them overlap
    for( auto it = path_.rbegin(); it != path_.rend(); ++it )
    {
        if( it->waypoint_
            && IsNear( point.SquareDistance( it->coordinate_ ), point ) )
        {
            const Hover hover = {
                point,
                *it->waypoint_,
                true
            };
            hovered_ = hover;
            return true;
        }
    }
    return false;
}

void PathfindLayer::PickSegment( geometry::Point2f point )
{
    std::size_t waypoint = 0;
    float distance = std::numeric_limits< float >::infinity();
    for( auto it = path_.begin(); it != path_.end() - 1; ++it )
    {
        if( it->waypoint_ )
            waypoint = *it->waypoint_;
        const auto projection =
            geometry::Segment2f( (it + 1)->coordinate_, it->coordinate_ )
                .Project( point );
        const auto d = point.SquareDistance( projection );
        if( d < distance && IsNear( d, point ) )
        {
            distance = d;
            const Hover hover = {
                projection,
                waypoint,
                false
            };
            hovered_ = hover;
        }
    }
}

bool PathfindLayer::HandleMouseMove( QMouseEvent* /*mouse*/, const geometry::Point2f& point )
{
    hovered_ = boost::none;
    if( path_.empty() || lock_ || !element_ )
        return false;
    if( PickWaypoint( point ) )
        return false;
    PickSegment( point );
    return false;
}

bool PathfindLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( lock_ || !hovered_ )
        return false;
    if( event->button() == Qt::LeftButton )
    {
        hovered_->coordinate_ = point;
        if( hovered_->onWaypoint_ )
            positions_.erase( positions_.begin() + hovered_->lastWaypoint_ );
        else
            ++hovered_->lastWaypoint_;
        hovered_->onWaypoint_ = false;
        QWidget w;
        dnd::CreateDragObject( this, &w );
    }
    else if( event->button() == Qt::RightButton )
    {
        if( hovered_->onWaypoint_ )
            positions_.erase( positions_.begin() + hovered_->lastWaypoint_ );
        if( positions_.size() < 2 )
            ClearPositions();
        else
            SendRequest();
    }
    return true;
}

namespace
{
    sword::ClientToSim MakeMessage()
    {
        sword::ClientToSim msg;
        auto request = msg.mutable_message()->mutable_segment_request();
        request->add_terrains( sword::highway );
        request->add_terrains( sword::large_road );
        request->add_terrains( sword::medium_road );
        request->add_terrains( sword::small_road );
        request->add_terrains( sword::crossroad );
        request->add_terrains( sword::street );
        request->add_terrains( sword::avenue );
        request->add_terrains( sword::underpass );
        request->set_radius( 500 );
        request->set_count( 1 );
        return msg;
    }
}

bool PathfindLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( !dnd::HasData< PathfindLayer >( event ) )
        return false;
    if( event->keyboardModifiers() == Qt::ControlModifier )
        hovered_->coordinate_ = point;
    else
    {
        static sword::ClientToSim msg = MakeMessage();
        auto request = msg.mutable_message()->mutable_segment_request();
        coordinateConverter_.ConvertToGeo( point, *request->mutable_position() );
        publisher_.Send( msg );
        point_ = point;
    }
    lock_ = true;
    return true;
}

bool PathfindLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( !dnd::HasData< PathfindLayer >( event ) || !hovered_ )
        return false;
    hovered_ = boost::none;
    positions_.insert( positions_.begin() + hovered_->lastWaypoint_, point );
    SendRequest();
    return true;
}

bool PathfindLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& /*point*/ ) const
{
    return dnd::HasData< PathfindLayer >( event );
}

void PathfindLayer::OpenEditingMode()
{
    const kernel::Entity_ABC* element = element_;
    if( !element )
        return;
    controllers_.ChangeMode( eModes_Itinerary );
    element->Select( controllers_.actions_ );
    element->MultipleSelect( controllers_.actions_, boost::assign::list_of( element ) );
    ClearPositions();
}
