// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentsLayer.h"
#include "ModelBuilder.h"
#include "preparation/Model.h"
#include "preparation/AgentsModel.h"
#include "preparation/AgentPositions.h"
#include "preparation/AutomatPositions.h"
#include "preparation/GhostModel.h"
#include "preparation/HierarchyTemplate.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_gui/ValuedDragObject.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( Controllers& controllers, const GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, ModelBuilder& modelBuilder, const Profile_ABC& profile, QWidget* parent )
    : gui::AgentsLayer( controllers, tools, strategy, view, profile )
    , model_            ( model )
    , modelBuilder_     ( modelBuilder )
    , selectedAgent_    ( controllers )
    , selectedAutomat_  ( controllers )
    , selectedFormation_( controllers )
    , selectedTeam_     ( controllers )
{
    setParent( parent );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer destructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::~AgentsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::BeforeSelection
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsLayer::BeforeSelection()
{
    selectedAgent_ = 0;
    selectedAutomat_ = 0;
    selectedFormation_ = 0;
    selectedTeam_ = 0;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void AgentsLayer::MultipleSelect( const std::vector< const kernel::Agent_ABC* >& elements )
{
    selectedAgent_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void AgentsLayer::MultipleSelect( const std::vector< const kernel::Automat_ABC* >& elements )
{
    selectedAutomat_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void AgentsLayer::MultipleSelect( const std::vector< const kernel::Formation_ABC* >& elements )
{
    selectedFormation_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::MultipleSelect
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void AgentsLayer::MultipleSelect( const std::vector< const kernel::Team_ABC* >& elements )
{
    selectedTeam_ = elements.size() == 1 ? elements.front() : 0;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::AfterSelection
// Created: JSR 2012-05-31
// -----------------------------------------------------------------------------
void AgentsLayer::AfterSelection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::CanDrop
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::CanDrop( QDragMoveEvent* event, const geometry::Point2f& ) const
{
    return ( gui::ValuedDragObject::Provides< const AgentPositions >   ( event ) && selectedAgent_ )
        || ( gui::ValuedDragObject::Provides< const AgentType >        ( event ) && selectedAutomat_ )
        || ( gui::ValuedDragObject::Provides< const AutomatType >      ( event ) && selectedFormation_ )
        || ( gui::ValuedDragObject::Provides< const HierarchyTemplate >( event ) && IsValidTemplate( event ) )
        || ( gui::ValuedDragObject::Provides< const Entity_ABC >       ( event ) && ( selectedAutomat_ || selectedAgent_ || selectedFormation_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsValidTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AgentsLayer::IsValidTemplate( QDragMoveEvent* event ) const
{
    if( !selectedFormation_ && !selectedTeam_ && !selectedAutomat_ )
        return false;
    const HierarchyTemplate* droppedItem = gui::ValuedDragObject::GetValue< const HierarchyTemplate >( event );
    if( ! droppedItem )
        return false;
    if( selectedTeam_ )
        return droppedItem->IsCompatible( *selectedTeam_ );
    else if( selectedFormation_ )
        return droppedItem->IsCompatible( *selectedFormation_ );
    else
        return droppedItem->IsCompatible( *selectedAutomat_ );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleEnterDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    oldPosition_ = geometry::Point2f();
    kernel::Entity_ABC* entity = gui::ValuedDragObject::GetValue< kernel::Entity_ABC >( event );
    if( dynamic_cast< kernel::Agent_ABC* >( entity) || dynamic_cast< kernel::Automat_ABC* >( entity) || dynamic_cast< kernel::Formation_ABC* >( entity) )
        oldPosition_ = entity->Retrieve< kernel::Positions >()->GetPosition();
    return gui::AgentsLayer::HandleEnterDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleMoveDragEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( AgentPositions* position = gui::ValuedDragObject::GetValue< AgentPositions >( event ) )
    {
        if( !selectedAgent_ )
            return false;
        if( world_.IsInside( point ) && draggingPoint_.Distance( point ) >= 5.f * tools_.Pixels( point ) )
        {
            position->Move( point + draggingOffset_.ToVector() );
            draggingPoint_ = point;
        }
        return true;
    }
    else if( kernel::Entity_ABC* entity = gui::ValuedDragObject::GetValue< kernel::Entity_ABC >( event ) )
    {
        if( ( dynamic_cast< kernel::Agent_ABC* >( entity) || dynamic_cast< kernel::Automat_ABC* >( entity) || dynamic_cast< kernel::Formation_ABC* >( entity) ) && world_.IsInside( point ) )
        {
            kernel::Moveable_ABC* position = dynamic_cast< kernel::Moveable_ABC* >( entity->Retrieve< kernel::Positions >() );
            if( position )
            {
                position->Move( point );
                draggingPoint_ = point;
            }
            return true;
        }
    }
    return gui::AgentsLayer::HandleMoveDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleDropEvent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( AgentPositions* position = gui::ValuedDragObject::GetValue< AgentPositions >( event ) )
    {
        if( !selectedAgent_ )
            return false;
        draggingPoint_.Set( 0, 0 );
        draggingOffset_.Set( 0, 0 );
        return true;
    }
    if( const AgentType* droppedItem = gui::ValuedDragObject::GetValue< const AgentType >( event ) )
    {
        if( !selectedAutomat_ )
            return false;
        model_.agents_.CreateAgent( *selectedAutomat_.ConstCast(), *droppedItem, point );
        return true;
    }
    if( const AutomatType* droppedItem = gui::ValuedDragObject::GetValue< const AutomatType >( event ) )
    {
        if( !selectedFormation_ && !selectedAutomat_ )
            return false;
        Entity_ABC* selectedEntity = selectedFormation_.ConstCast();
        if( !selectedEntity )
            selectedEntity = selectedAutomat_.ConstCast();
        model_.agents_.CreateAutomat( *selectedEntity, *droppedItem, point );
        return true;
    }
    if( const HierarchyTemplate* droppedItem = gui::ValuedDragObject::GetValue< const HierarchyTemplate >( event ) )
    {
        if( !selectedFormation_ && !selectedTeam_ && !selectedAutomat_ )
            return false;
        if( selectedTeam_ )
            droppedItem->Instanciate( *selectedTeam_.ConstCast(), point );
        else if( selectedFormation_ )
            droppedItem->Instanciate( *selectedFormation_.ConstCast(), point );
        else
            droppedItem->Instanciate( *selectedAutomat_.ConstCast(), point );
        return true;
    }
    else if( kernel::Entity_ABC* entity = gui::ValuedDragObject::GetValue< kernel::Entity_ABC >( event ) )
        if( ( dynamic_cast< kernel::Agent_ABC* >( entity) || dynamic_cast< kernel::Automat_ABC* >( entity) || dynamic_cast< kernel::Formation_ABC* >( entity) ) && world_.IsInside( point ) )
            oldPosition_ = geometry::Point2f();
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleLeaveDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleLeaveDragEvent( QDragLeaveEvent* /*event*/ )
{
    if( oldPosition_.IsZero() )
        return false;
    kernel::Entity_ABC* entity = 0;
    if( selectedAgent_ )
        entity = selectedAgent_.ConstCast();
    else if( selectedAutomat_ )
        entity = selectedAutomat_.ConstCast();
    else if( selectedFormation_ )
        entity = selectedFormation_.ConstCast();
    else if( selectedTeam_ )
        entity = selectedTeam_.ConstCast();
    if( entity )
    {
        kernel::Moveable_ABC* position = dynamic_cast< kernel::Moveable_ABC* >( entity->Retrieve< kernel::Positions >() );
        if( position )
        {
            position->Move( oldPosition_ );
            oldPosition_ = geometry::Point2f();
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleKeyPress
// Created: JSR 2012-06-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleKeyPress( QKeyEvent* key )
{
    if( key->key() == Qt::Key_Delete && ( selectedAgent_ || selectedAutomat_ || selectedFormation_ || selectedTeam_ ) )
    {
        if( selectedTeam_ )
            modelBuilder_.DeleteEntity( *selectedTeam_ );
        else if( selectedFormation_ )
            modelBuilder_.DeleteEntity( *selectedFormation_ );
        else if( selectedAutomat_ )
            modelBuilder_.DeleteEntity( *selectedAutomat_ );
        else if( selectedAgent_ )
            modelBuilder_.DeleteEntity( *selectedAgent_ );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleMousePress
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    bool result = gui::AgentsLayer::HandleMousePress( event, point );
    if( ( event->button() & Qt::LeftButton ) != 0 && event->state() == Qt::NoButton && IsEligibleForDrag( point ) )
    {
        if( const AgentPositions* pos = static_cast< const AgentPositions* >( selectedAgent_->Retrieve< Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            Q3DragObject* drag = new gui::ValuedDragObject( pos, dynamic_cast< QWidget* >( parent() ) );
            drag->dragMove();
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsEligibleForDrag
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
bool AgentsLayer::IsEligibleForDrag( const geometry::Point2f& point ) const
{
    return selectedAgent_ && IsInSelection( *selectedAgent_, point );
}
