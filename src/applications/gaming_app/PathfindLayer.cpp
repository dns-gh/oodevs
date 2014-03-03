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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
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
{
    controllers_.Register( *this );
    std::function< void( const sword::SimToClient& ) > fun = 
        [&]( const sword::SimToClient& message ) {
        if( message.message().has_pathfind_request_ask() )
        {
            auto request = message.message().pathfind_request_ask();
            if( request.error_code() == sword::PathfindRequestAck_ErrorCode_no_error )
            {
                const auto& path = request.path();
                path_.clear();
                path_.reserve( path.location().coordinates().elem_size() );
                for( int i = 0; i < path.location().coordinates().elem_size(); ++i )
                    path_.push_back( coordinateConverter_.ConvertToXY( path.location().coordinates().elem(i) ) );
            }
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
    if( !from_ )
        return;

    glColor4f( COLOR_BLACK );
    tools_.DrawCross( *from_, GL_CROSSSIZE, gui::GlTools_ABC::pixels );
    if( !to_ )
        return;

    tools_.DrawCross( *to_, GL_CROSSSIZE, gui::GlTools_ABC::pixels );
    if( path_.empty() )
        return;

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT );
    glColor4f( COLOR_PATH );
    glLineWidth( 3 );
    glEnable( GL_LINE_STIPPLE );
    glLineStipple( 1, tools_.StipplePattern() );
    tools_.DrawLines( path_ );
    glDisable( GL_LINE_STIPPLE );
    glPopAttrib();
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
        subMenu->InsertItem( "Interface", tools::translate( "LocationEditorToolbar", "Directions from here" ), this, SLOT( SelectFromPosition() ) );
        subMenu->InsertItem( "Interface", tools::translate( "LocationEditorToolbar", "Directions to here" ), this, SLOT( SelectToPostion() ) );
        subMenu->InsertItem( "Interface", tools::translate( "LocationEditorToolbar", "Clear positions" ), this, SLOT( ClearPositions() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::ClearPositions
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::ClearPositions()
{
    from_ = boost::none;
    to_ = boost::none;
    path_.clear();
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::SelectToPostion
// -----------------------------------------------------------------------------
void PathfindLayer::SelectToPostion()
{
    to_ = point_;
    SendRequest();
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::SelectFromPosition
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::SelectFromPosition()
{
    from_ = point_;
    SendRequest();
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::SendRequest
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::SendRequest()
{
    path_.clear();
    if( element_ && to_ && from_ )
    {
        sword::ClientToSim msg;
        auto request = msg.mutable_message()->mutable_pathfind_request();
        request->mutable_unit()->set_id( element_->GetId() );
        coordinateConverter_.ConvertToGeo( *from_, *request->mutable_start() );
        coordinateConverter_.ConvertToGeo( *to_, *request->mutable_end() );
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
