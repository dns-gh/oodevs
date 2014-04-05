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
{
    controllers_.Register( *this );
    std::function< void( const sword::SimToClient& ) > fun =
        [&]( const sword::SimToClient& message )
        {
            if( !message.message().has_pathfind_request_ack() )
                return;
            const auto& request = message.message().pathfind_request_ack();
            if( request.error_code() != sword::PathfindRequestAck_ErrorCode_no_error )
                return;
            const auto& path = request.path();
            path_.clear();
            const auto& elements = path.points();
            for( auto it = elements.begin(); it != elements.end(); ++it )
                path_.push_back( coordinateConverter_.ConvertToXY( it->coordinate() ) );
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
    glPopAttrib();
}

void PathfindLayer::DrawLines( float width, uint8_t r, uint8_t g, uint8_t b ) const
{
    glLineWidth( width );
    glColor3ub( r, g, b );
    tools_.DrawLines( path_ );
    for( auto it = path_.begin(); it != path_.end(); ++it )
        tools_.DrawDisc( *it, width / 2, gui::GlTools_ABC::pixels );
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
    if( element_ )
    {
        point_ = point;
        kernel::ContextMenu* subMenu = menu.SubMenu( "Interface", tools::translate( "LocationEditorToolbar", "Pathfind" ) );
        subMenu->InsertItem( "Interface", tools::translate( "LocationEditorToolbar", "Add position" ), this, SLOT( AddPosition() ) );
        subMenu->InsertItem( "Interface", tools::translate( "LocationEditorToolbar", "Clear positions" ), this, SLOT( ClearPositions() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::ClearPositions
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::ClearPositions()
{
    positions_.clear();
    path_.clear();
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::AddPosition
// -----------------------------------------------------------------------------
void PathfindLayer::AddPosition()
{
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
