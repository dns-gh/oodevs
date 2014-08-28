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

#include "actions/ActionsModel.h"
#include "actions/Helpers.h"
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/ModelObserver_ABC.h"
#include "clients_gui/RichDockWidget.h"
#include "clients_gui/View_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "gaming/Pathfind.h"
#include "protocol/Protocol.h"
#include "protocol/ServerPublisher_ABC.h"

#include <boost/assign.hpp>

// -----------------------------------------------------------------------------
// Name: PathfindLayer constructor
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
PathfindLayer::PathfindLayer( kernel::Controllers& controllers,
                              gui::GlTools_ABC& tools,
                              gui::ColorStrategy_ABC& strategy,
                              gui::View_ABC& view,
                              const kernel::Profile_ABC& profile,
                              Publisher_ABC& publisher,
                              const kernel::CoordinateConverter_ABC& converter,
                              const tools::Resolver_ABC< kernel::Agent_ABC >& agents,
                              const tools::Resolver_ABC< kernel::Population_ABC >& populations,
                              gui::ModelObserver_ABC& model,
                              actions::ActionsModel& actions )
    : EntityLayer< kernel::Pathfind_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Pathfinds )
    , controllers_( controllers )
    , tools_( tools )
    , view_( view )
    , target_( controllers )
    , selectedEntity_( controllers )
    , selectedPathfind_( controllers )
    , publisher_( publisher )
    , converter_( converter )
    , model_( model )
    , actions_( actions )
    , agents_( agents )
    , populations_( populations )
    , replaceable_( false )
    , lock_( false )
{
    publisher_.Register( Publisher_ABC::T_SimHandler( [&]( const sword::SimToClient& message )
        {
            if( !message.message().has_compute_pathfind_ack() || !edited_ )
                return;
            const auto& request = message.message().compute_pathfind_ack();
            if( request.error_code() == sword::ComputePathfindAck_ErrorCode_no_error )
                edited_->LoadPoints( request.path() );
            ProcessEvents();
        } ) );
    publisher_.Register( Publisher_ABC::T_SimHandler( [&]( const sword::SimToClient& message )
        {
            if( !message.message().has_segment_request_ack() || !hovered_ )
                return;
            const auto& request = message.message().segment_request_ack();
            if( request.error_code() == sword::SegmentRequestAck_ErrorCode_no_error )
            {
                const auto& segments = request.segments();
                for( auto it = segments.begin(); it != segments.end(); ++it )
                    hovered_->coordinate_ = geometry::Segment2f(
                        converter_.ConvertToXY( it->from() ),
                        converter_.ConvertToXY( it->to() ) ).Project( point_ );
            }
            ProcessEvents();
        } ) );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer destructor
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
PathfindLayer::~PathfindLayer()
{
    // NOTHING
}

void PathfindLayer::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    lock_ = false;
    ClearPositions();
}

void PathfindLayer::Initialize( const geometry::Rectangle2f& extent )
{
    world_ = extent;
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::Paint
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::Paint( gui::Viewport_ABC& view )
{
    EntityLayer< kernel::Pathfind_ABC >::Paint( view );
    if( !edited_ )
        return;
    edited_->SetHover( hovered_ );
    if( controllers_.GetCurrentMode() == eModes_Itinerary )
        EntityLayer< kernel::Pathfind_ABC >::Draw( *edited_, view, false );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::NotifyContextMenu
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
{
    if( !selectedEntity_ || lock_ )
        return;
    point_ = point;
    if( controllers_.GetCurrentMode() == eModes_Itinerary )
    {
        if( world_.IsInside( point ) )
        {
            menu.InsertItem( "Itinerary", tools::translate( "LocationEditorToolbar", "Itinerary from here" ), this, SLOT( SetStartPosition() ) );
            menu.InsertItem( "Itinerary", tools::translate( "LocationEditorToolbar", "Itinerary to here" ), this, SLOT( SetEndPosition() ) );
        }
        menu.InsertItem( "Itinerary", tools::translate( "LocationEditorToolbar", "Clear waypoints" ), this, SLOT( ClearPositions() ) );
    }
    else if( selectedEntity_ == target_ && profile_.CanBeOrdered( *selectedEntity_ ) )
        menu.InsertItem( "Itinerary", tools::translate( "LocationEditorToolbar", "Create itinerary" ), this, SLOT( OnOpenEditingMode() ) );
}

void PathfindLayer::NotifyContextMenu( const kernel::Pathfind_ABC& pathfind, kernel::ContextMenu& menu )
{
    if( controllers_.GetCurrentMode() == eModes_Itinerary )
        return;
    if( !profile_.CanBeOrdered( pathfind ) )
        return;
    target_ = &pathfind;
    selectedEntity_ = nullptr;
    selectedPathfind_ = &pathfind;
    menu.InsertItem( "Itinerary", tr( "Edit" ), this, SLOT( OnEditPathfind() ) );
    menu.InsertItem( "Itinerary", tr( "Delete" ), this, SLOT( OnDeletePathfind() ) );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::ClearPositions
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::ClearPositions()
{
    HandleEvent( [&]()
        {
            if( edited_ )
                edited_->Clear();
            hovered_ = boost::none;
            events_.clear();
            lock_ = false;
        } );
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::SetStartPosition
// -----------------------------------------------------------------------------
void PathfindLayer::SetStartPosition()
{
    if( lock_ )
        return;
    edited_->StartOn( point_ );
    SendRequest();
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::SetEndPosition
// -----------------------------------------------------------------------------
void PathfindLayer::SetEndPosition()
{
    if( lock_ )
        return;
    edited_->StopOn( point_ );
    SendRequest();
}

bool PathfindLayer::HasValidPathfind() const
{
    return target_ && edited_->HasWaypoints();
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::SendRequest
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::SendRequest()
{
    if( !HasValidPathfind() )
        return;
    sword::ClientToSim msg;
    auto request = msg.mutable_message()->mutable_compute_pathfind()->mutable_request();
    actions::parameters::FillPathfindRequest( *request, GetUnitId(),
         converter_, *target_, edited_->GetDots(), edited_->GetName().toStdString() );
    publisher_.Send( msg );
    lock_ = true;
    hovered_ = boost::none;
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::BeforeSelection
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::BeforeSelection()
{
    if( controllers_.GetCurrentMode() != eModes_Itinerary )
        target_ = selectedEntity_ = selectedPathfind_ = 0;
}

void PathfindLayer::Select( const kernel::Agent_ABC& element )
{
    if( controllers_.GetCurrentMode() != eModes_Itinerary )
        target_ = selectedEntity_ = &element;
}

void PathfindLayer::Select( const kernel::Automat_ABC& element )
{
    if( controllers_.GetCurrentMode() != eModes_Itinerary )
        selectedEntity_ = &element;
}

void PathfindLayer::Select( const kernel::Formation_ABC& element )
{
    if( controllers_.GetCurrentMode() != eModes_Itinerary )
        selectedEntity_ = &element;
}

void PathfindLayer::Select( const kernel::Pathfind_ABC& element )
{
    if( controllers_.GetCurrentMode() != eModes_Itinerary )
        target_ = selectedPathfind_ = &element;
}

void PathfindLayer::Select( const kernel::Population_ABC& element )
{
    if( controllers_.GetCurrentMode() != eModes_Itinerary )
        target_ = selectedEntity_ = &element;
}

void PathfindLayer::Select( const kernel::TacticalLine_ABC& element )
{
    if( controllers_.GetCurrentMode() != eModes_Itinerary )
        if( auto hierarchies = element.Retrieve< kernel::TacticalHierarchies >() )
            if( auto automat = hierarchies->GetSuperior() )
                selectedEntity_ = automat;
}

// -----------------------------------------------------------------------------
// Name: PathfindLayer::AfterSelection
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void PathfindLayer::AfterSelection()
{
    // NOTHING
}

bool PathfindLayer::PickWaypoint( geometry::Point2f point )
{
    if( auto hover = edited_->PickWaypoint( tools_, point ) )
        hovered_ = *hover;
    return hovered_;
}

void PathfindLayer::PickSegment( geometry::Point2f point )
{
    if( auto hover = edited_->PickSegment( tools_, point ) )
        hovered_ = *hover;
}

bool PathfindLayer::HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point )
{
    if( event->buttons() == Qt::NoButton )
        HandleEvent( [this,point]()
            {
                hovered_ = boost::none;
                if( !edited_ || !edited_->HasPath() || !target_ )
                    return;
                if( PickWaypoint( point ) )
                    return;
                PickSegment( point );
            }, true );
    return false;
}

bool PathfindLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& /*point*/ )
{
    if( !hovered_ )
        return false;
    const auto button = event->button();
    HandleEvent( [this,button]()
        {
            if( button == Qt::LeftButton )
            {
                if( !hovered_->onWaypoint_ )
                    ++hovered_->lastWaypoint_;
                hovered_->onWaypoint_ = false;
                QWidget w;
                dnd::CreateDragObject( this, &w );
            }
            else if( button == Qt::RightButton )
            {
                if( hovered_->onWaypoint_ )
                    edited_->EraseWaypoint( hovered_->lastWaypoint_ );
                if( !edited_->HasWaypoints() )
                    ClearPositions();
                else
                    SendRequest();
            }
        } );
    return true;
}

namespace
{
    geometry::Point2f Snap( const geometry::Point2f& p, const geometry::Rectangle2f& r )
    {
        return geometry::Point2f(
            std::min( r.Right(), std::max( r.Left(), p.X() ) ),
            std::min( r.Top(), std::max( r.Bottom(), p.Y() ) ) );
    }

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

void PathfindLayer::UpdateHovered( bool snap, const geometry::Point2f& point )
{
    const geometry::Point2f snapped = Snap( point, world_ );
    if( snap )
    {
        auto message = MakeMessage();
        auto request = message.mutable_message()->mutable_segment_request();
        converter_.ConvertToGeo( snapped, *request->mutable_position() );
        publisher_.Send( message );
        point_ = snapped;
        lock_ = true;
    }
    else
        hovered_->coordinate_ = snapped;
}

bool PathfindLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( !dnd::HasData< PathfindLayer >( event ) )
        return false;
    const bool snap = !event->keyboardModifiers().testFlag( Qt::ControlModifier );
    HandleEvent( [this, snap, point]()
        {
            UpdateHovered( snap, point );
        }, true );
    return true;
}

bool PathfindLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& /*point*/ )
{
    if( !dnd::HasData< PathfindLayer >( event ) )
        return false;
    HandleEvent( [this]()
        {
            edited_->InsertWaypoint( hovered_->lastWaypoint_, *hovered_->coordinate_ );
            SendRequest();
        } );
    return true;
}

bool PathfindLayer::HandleLeaveDragEvent( QDragLeaveEvent* /*event*/ )
{
    if( !hovered_ )
        return false;
    HandleEvent( [this]()
        {
            if( hovered_->origin_ )
                edited_->InsertWaypoint( hovered_->lastWaypoint_, *hovered_->origin_ );
            hovered_->coordinate_ = boost::none;
        } );
    return true;
}

bool PathfindLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    if( !dnd::HasData< PathfindLayer >( event ) )
        return false;
    const bool snap = !event->keyboardModifiers().testFlag( Qt::ControlModifier );
    HandleEvent( [this, snap, point]()
        {
            if( hovered_->origin_ )
                edited_->EraseWaypoint( hovered_->lastWaypoint_ );
            UpdateHovered( snap, point );
        } );
    return true;
}

namespace
{
    void Select( kernel::Controllers& controllers, const kernel::Entity_ABC& entity )
    {
        entity.Select( controllers.actions_ );
        entity.MultipleSelect( controllers.actions_, boost::assign::list_of( &entity ) );
    }
}

void PathfindLayer::OpenEditingMode( kernel::Entity_ABC* entity,
                                     const sword::Pathfind& pathfind )
{
    if( !entity )
        return;
    edited_.reset( new Pathfind( controllers_.controller_, actions_, converter_, *entity, pathfind, true, []( const kernel::Pathfind_ABC& ){ return true; } ) );
    target_ = entity;
    controllers_.ChangeMode( eModes_Itinerary );
    ::Select( controllers_, *entity );
}

void PathfindLayer::OnOpenEditingMode()
{
    sword::Pathfind pathfind;
    if( target_ )
        pathfind.mutable_request()->mutable_unit()->set_id( target_->GetId() );
    OpenEditingMode( target_.ConstCast(), pathfind );
    ClearPositions();
}

bool PathfindLayer::HandleKeyPress( QKeyEvent* keyEvent )
{
    const auto mode = controllers_.GetCurrentMode();
    const auto key = keyEvent->key();
    if( key == Qt::Key_Escape && mode == eModes_Itinerary )
    {
        OnRejectEdit();
        return false;
    }
    if( key == Qt::Key_Delete && mode == eModes_Gaming )
    {
        OnDeletePathfind();
        return false;
    }
    return true;
}

bool PathfindLayer::HandleEvent( const std::function< void() >& event, bool replaceable )
{
    if( lock_ )
    {
        if( events_.empty() || !replaceable_ )
            events_.push_back( event );
        else
            events_.back() = event;
        replaceable_ = replaceable;
        return false;
    }
    event();
    return true;
}

void PathfindLayer::ProcessEvents()
{
    lock_ = false;
    while( !lock_ && !events_.empty() )
    {
        const auto event = events_.front();
        events_.pop_front();
        event();
    }
}

void PathfindLayer::StopEdition() const
{
    const kernel::SafePointer< kernel::Entity_ABC > selected( controllers_, selectedPathfind_ ? selectedPathfind_.ConstCast() : selectedEntity_.ConstCast() );
    controllers_.ChangeMode( eModes_Gaming );
    if( selected )
        ::Select( controllers_, *selected );
}

void PathfindLayer::OnAcceptEdit()
{
    if( selectedPathfind_ )
        actions_.PublishDestroyPathfind( selectedPathfind_->GetId() );
    if( HasValidPathfind() )
        actions_.PublishCreatePathfind( GetUnitId(), *target_, edited_->GetDots(), edited_->GetName().toStdString() );
    ClearPositions();
    edited_.reset();
    StopEdition();
}

void PathfindLayer::OnRejectEdit()
{
    ClearPositions();
    if( selectedPathfind_ )
        selectedPathfind_.ConstCast()->SetVisible( true );
    edited_.reset();
    StopEdition();
}

void PathfindLayer::OnDeletePathfind()
{
    if( !selectedPathfind_ )
        return;
    if( !profile_.CanBeOrdered( *selectedPathfind_ ) )
        return;
    model_.DeleteEntity( *selectedPathfind_ );
}

void PathfindLayer::OnEditPathfind()
{
    if( !selectedPathfind_ )
        return;
    selectedPathfind_.ConstCast()->SetVisible( false );
    sword::Pathfind pathfind;
    pathfind.mutable_request()->mutable_unit()->set_id( selectedPathfind_->GetUnit().GetId() );
    pathfind.mutable_request()->set_name( selectedPathfind_->GetName() );
    *pathfind.mutable_result() = selectedPathfind_->GetPathfind();
    OpenEditingMode( selectedPathfind_.ConstCast(), pathfind );
}

uint32_t PathfindLayer::GetUnitId() const
{
    return target_ == selectedPathfind_ ? selectedPathfind_->GetUnit().GetId() : target_->GetId();
}

void PathfindLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "TacticalLines" )
        filter_ = value.To< kernel::FourStateOption >();
}

 namespace
 {
    template< typename T >
    bool IsSubordinate( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& candidate )
    {
        if( &entity == &candidate )
            return true;
        if( const auto hierarchy = entity.Retrieve< T >() )
            return hierarchy->IsSubordinateOf( candidate );
        return false;
    }

    bool IsSuperior( const kernel::Entity_ABC* superior, const kernel::Entity_ABC& unit )
    {
        return superior &&
            ( IsSubordinate< kernel::TacticalHierarchies >( unit, *superior ) ||
              IsSubordinate< kernel::CommunicationHierarchies >( unit, *superior ) );
    }
 }

bool PathfindLayer::ShouldDisplay( const kernel::Entity_ABC& entity )
{
    if( controllers_.GetCurrentMode() == eModes_Itinerary )
        if( edited_.get() == &entity )
            return true;
    if( !filter_.IsSet( true, true, true ) )
        return false;
    const auto& pathfind = static_cast< const kernel::Pathfind_ABC& >( entity );
    const auto& element = pathfind.GetUnit();
    const auto* selection = selectedPathfind_ ? &selectedPathfind_->GetUnit() : selectedEntity_;
    const bool selected = selection && IsSuperior( &element, *selection );
    const bool superior = IsSuperior( selection, element );
    if( !filter_.IsSet( selected, selected || superior, profile_.CanBeOrdered( element ) ) )
        return false;
    return gui::EntityLayer< kernel::Pathfind_ABC >::ShouldDisplay( entity );
}

void PathfindLayer::ActivateEntity( const kernel::Entity_ABC& entity )
{
    view_.CenterOn( static_cast< const kernel::Pathfind_ABC& >( entity ).GetPosition() );
}

void PathfindLayer::NotifyDeleted( const kernel::Agent_ABC& unit )
{
    if( controllers_.GetCurrentMode() == eModes_Itinerary )
        if( !target_ || target_ == &unit )
            OnRejectEdit();
}
