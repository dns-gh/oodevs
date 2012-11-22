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
#include "clients_gui/DragAndDropHelpers.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Moveable_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/StandardModel.h"

// -----------------------------------------------------------------------------
// Name: AgentsLayer constructor
// Created: SBO 2006-08-31
// -----------------------------------------------------------------------------
AgentsLayer::AgentsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, Model& model, ModelBuilder& modelBuilder, const kernel::Profile_ABC& profile )
    : gui::AgentsLayer( controllers, tools, strategy, view, profile )
    , model_            ( model )
    , modelBuilder_     ( modelBuilder )
    , selectedAgent_    ( controllers )
    , selectedAutomat_  ( controllers )
    , selectedFormation_( controllers )
    , selectedTeam_     ( controllers )
    , dummy_            ( new QWidget() )
{
    // NOTHING
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
    return ( dnd::HasData< kernel::Agent_ABC>( event ) && selectedAgent_ ) ||
           ( dnd::HasData< kernel::Automat_ABC >( event ) && selectedAutomat_ ) ||
           ( dnd::HasData< kernel::Formation_ABC >( event ) && selectedFormation_ ) ||
           ( dnd::HasData< AgentPositions >( event ) && selectedAgent_ ) ||
           ( dnd::HasData< kernel::AgentType >( event ) && selectedAutomat_ ) ||
           ( dnd::HasData<  kernel::AutomatType >( event ) && ( selectedFormation_ ) ) ||
           IsValidTemplate( event );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::IsValidTemplate
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AgentsLayer::IsValidTemplate( QDropEvent* event ) const
{
    if( !selectedFormation_ && !selectedTeam_ && !selectedAutomat_ )
        return false;
    const HierarchyTemplate* droppedItem = dnd::FindData< HierarchyTemplate >( event );
    if( ! droppedItem )
        return false;
    if( selectedTeam_ )
        return droppedItem->IsCompatible( *selectedTeam_ );
    else if( selectedFormation_ )
        return droppedItem->IsCompatible( *selectedFormation_ );
    else
        return droppedItem->IsCompatible( *selectedAutomat_ );
}

namespace
{
    bool IsValid( const kernel::Automat_ABC& automat, const kernel::AgentType& type )
    {
        if( ( type.IsTC2() || type.IsLogisticSupply() || type.IsLogisticMaintenance() ||  type.IsLogisticMedical() )
            && automat.GetLogisticLevel() == kernel::LogisticLevel::none_ )
             return false;
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleEnterDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    oldPosition_ = geometry::Point2f();
    const kernel::Entity_ABC* entity = dnd::FindSafeEntityData< kernel::Agent_ABC, kernel::Automat_ABC, kernel::Formation_ABC >( event );
    if( entity )
        oldPosition_ = entity->Retrieve< kernel::Positions >()->GetPosition();
    return gui::AgentsLayer::HandleEnterDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleMoveDragEvent
// Created: JSR 2011-12-22
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    AgentPositions* positions = dnd::FindData< AgentPositions >( event );
    if( positions )
    {
        if( selectedAgent_ && world_.IsInside( point ) && draggingPoint_.Distance( point ) >= 5.f * tools_.Pixels( point ) )
        {
            positions->Move( point + draggingOffset_.ToVector() );
            draggingPoint_ = point;
        }
        return true;
    }
    kernel::Entity_ABC* entity = dnd::FindSafeEntityData< kernel::Agent_ABC, kernel::Automat_ABC, kernel::Formation_ABC >( event );
    if( entity )
    {
        if( kernel::Moveable_ABC* position = dynamic_cast< kernel::Moveable_ABC* >( entity->Retrieve< kernel::Positions >() ) )
        {
            position->Move( point );
            draggingPoint_ = point;
        }
        return true;
    }
    return gui::AgentsLayer::HandleMoveDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: AgentsLayer::HandleDropEvent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
bool AgentsLayer::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    AgentPositions* positions = dnd::FindData< AgentPositions >( event );
    if( positions && selectedAgent_)
    {
        draggingPoint_.Set( 0, 0 );
        draggingOffset_.Set( 0, 0 );
        return true;
    }
    const kernel::AgentType* agentType = dnd::FindData< kernel::AgentType >( event );
    if( agentType && selectedAutomat_ )
    {
        if( !IsValid( *selectedAutomat_, *agentType ) )
        {
            QMessageBox::warning( 0, tools::translate( "Application", "SWORD" ), tools::translate( "AgentsLayer", "Logistic units can not be placed under a non logistic automat" ) );
            return false;
        }
        model_.agents_.CreateAgent( *selectedAutomat_.ConstCast(), *agentType, point );
        return true;
    }
    const kernel::AutomatType* automatType = dnd::FindData< kernel::AutomatType >( event );
    if( automatType && ( selectedFormation_ || selectedAutomat_ ) )
    {
        kernel::Entity_ABC* selectedEntity = selectedFormation_.ConstCast();
        if( !selectedEntity )
            selectedEntity = selectedAutomat_.ConstCast();
        model_.agents_.CreateAutomat( *selectedEntity, *automatType, point );
        return true;
    }
    const HierarchyTemplate* model = dnd::FindData< HierarchyTemplate >( event );
    if( model && ( selectedFormation_ || selectedTeam_ || selectedAutomat_ ) )
    {
        if( selectedTeam_ )
            model->Instanciate( *selectedTeam_.ConstCast(), point );
        else if( selectedFormation_ )
            model->Instanciate( *selectedFormation_.ConstCast(), point );
        else
            model->Instanciate( *selectedAutomat_.ConstCast(), point );
        return true;
    }
    if( dnd::FindSafeEntityData< kernel::Agent_ABC, kernel::Automat_ABC, kernel::Formation_ABC >( event ) )
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
        if( const AgentPositions* pos = static_cast< const AgentPositions* >( selectedAgent_->Retrieve< kernel::Positions >() ) )
        {
            draggingPoint_ = point;
            draggingOffset_ = pos->GetPosition( true ) - point.ToVector();
            dnd::CreateDragObject( pos, dummy_.get() );
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
